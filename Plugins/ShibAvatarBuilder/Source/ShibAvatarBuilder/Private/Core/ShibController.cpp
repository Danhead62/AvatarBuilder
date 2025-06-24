// Fill out your copyright notice in the Description page of Project Settings.

#include "ShibController.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "ShibSaveGame.h"
#include "ShibAvatarCharacter.h"
#include "UISubsystem.h"
#include "ShibAvatarBuilderSubsystem.h"
#include "DrawDebugHelpers.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(ShibController)

AShibController::AShibController()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bUsePawnControlRotation = false;

	ControllerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ControllerCamera"));
	ControllerCamera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);

	ResetRotationTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("ResetRotationTimelineComponent"));
	ZoomDoubleClickTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("ZoomDoubleClickTimelineComponent"));
	ZoomInTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("ZoomInTimelineComponent"));

	bAvatarSelected = false;

	ZoomFactor = 10.0f;
	MinZoomFOV = 30.0f;
	MaxZoomFOV = 90.0f;
}

void AShibController::SpawnIntoAvatarEvent_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Interface function called!"));
	SpringArm->TargetArmLength = 1500.0f;
	if (ZoomInTimelineComponent)
	{
		ControllerData.TargetBodyPartIndex = 7;
		ResetRotationOverride();
		ZoomInTimelineComponent->SetPlayRate(0.05f);
		ZoomInTimelineComponent->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
		ZoomInTimelineComponent->SetTimelineLength(0.1);
		PlayZoomInTimeline();
		HandleSpringArmLag(true);
	}
}



void AShibController::BeginPlay()
{
	Super::BeginPlay();
	const TObjectPtr<UUISubsystem> ShibUISubsystem = GetGameInstance()->GetSubsystem<UUISubsystem>();
	ShibUISubsystem->SetShibController(this);
	const TObjectPtr<UShibAvatarBuilderSubsystem> ShibAvatarBuilderSubsystem = GetGameInstance()->GetSubsystem<UShibAvatarBuilderSubsystem>();
	ShibAvatarBuilderSubsystem->SetShibController(this);
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
	DoOnce.Reset();
	/*FHitResult Hit;
	bool bTraced;
	TraceAvatar(bTraced, Hit);*/
	ControllerData.AvatarActor = UGameplayStatics::GetActorOfClass(this, AShibAvatarCharacter::StaticClass());
	UE_LOG(LogTemp, Warning, TEXT("Avatar Character [%s]"), *GetControllerData().AvatarActor.GetName());
	SetShibAvatarCharacter(Cast<AShibAvatarCharacter>(GetControllerData().AvatarActor));
	DisableMovementInLevel();
	SetCameraHeightBasedOnGender();
	UKismetSystemLibrary::Delay(this, 3.0f, FLatentActionInfo::FLatentActionInfo());
	ControllerData.bEnableRightClick = true;

	// Reset Rotation Timeline
	if (ResetRotationCurve)
	{
		ResetRotationInterpFunc.BindUFunction(this, FName("ResetRotationTimelineReturn"));
		ResetRotationTimelineFinished.BindUFunction(this, FName("OnResetRotationTimelineFinished"));
		ResetRotationTimelineComponent->AddInterpFloat(ResetRotationCurve, ResetRotationInterpFunc, FName("Alpha"));
		ResetRotationTimelineComponent->SetTimelineFinishedFunc(ResetRotationTimelineFinished);
		ResetRotationTimelineComponent->SetLooping(false);
		ResetRotationTimelineComponent->SetIgnoreTimeDilation(true);
	}
	// Zoom Double Click Timeline
	if (ZoomDoubleClickCurve)
	{
		ZoomDoubleClickInterpFunc.BindUFunction(this, FName("ZoomDoubleClickTimelineReturn"));
		ResetRotationTimelineFinished.BindUFunction(this, FName("OnZoomDoubleClickTimelineFinished"));
		ZoomDoubleClickTimelineComponent->AddInterpFloat(ZoomDoubleClickCurve, ZoomDoubleClickInterpFunc, FName("Alpha"));
		ZoomDoubleClickTimelineComponent->SetLooping(false);
		ZoomDoubleClickTimelineComponent->SetIgnoreTimeDilation(true);
	}
	// Zoom In Timeline
	if (ZoomInCurve)
	{
		ZoomInInterpFunc.BindUFunction(this, FName("ZoomInTimelineReturn"));
		ZoomInTimelineFinished.BindUFunction(this, FName("OnZoomInTimelineFinished"));
		ZoomInTimelineComponent->SetTimelineFinishedFunc(ZoomInTimelineFinished);
		ZoomInTimelineComponent->AddInterpFloat(ZoomInCurve, ZoomInInterpFunc, FName("Alpha"));
		ZoomInTimelineComponent->SetLooping(false);
		ZoomInTimelineComponent->SetIgnoreTimeDilation(true);
	}
	InitialSpringArmLocation = SpringArm->GetComponentLocation();
}

void AShibController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(LeftClickAction, ETriggerEvent::Started, this, &AShibController::TriggeredLeftClickButton);
		EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AShibController::TriggeredLeftClickButton);


		EIC->BindAction(MiddleMouseClickAction, ETriggerEvent::Started, this, &AShibController::TriggeredMiddleMouseClickButton);
		EIC->BindAction(MiddleMouseClickAction, ETriggerEvent::Completed, this, &AShibController::TriggeredMiddleMouseClickButton);

		

		EIC->BindAction(RightClickAction, ETriggerEvent::Triggered, this, &AShibController::TriggeredRightClickButton);
		EIC->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AShibController::TriggeredRightClickButton);

		EIC->BindAction(LeftDoubleClickAction, ETriggerEvent::Completed, this, &AShibController::DoubleClickFocus);

		EIC->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AShibController::Rotate);

		EIC->BindAction(RightDoubleClickAction, ETriggerEvent::Completed, this, &AShibController::ResetRotationOverride);

		EIC->BindAction(ZoomActionForKeyBoard, ETriggerEvent::Triggered, this, &AShibController::Zoom);

		EIC->BindAction(ZoomActionForMouse, ETriggerEvent::Triggered, this, &AShibController::ZoomTowardsMouseCursor);
		

		if(MoveHorizontalAction)
		{
			EIC->BindAction(MoveHorizontalAction, ETriggerEvent::Triggered, this, &AShibController::MoveHorizontal);
		}
		if(MoveVerticalAction)
		{
			EIC->BindAction(MoveVerticalAction, ETriggerEvent::Triggered, this, &AShibController::MoveVertical);
		}
	}
}

AActor* AShibController::TraceAvatar(bool& bTraced, FHitResult& Hit)
{
	FHitResult HitResult;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	GetPlayerState()->GetPlayerController()->GetHitResultUnderCursorByChannel(TraceType, true, HitResult);
	Hit = HitResult;
	bTraced = HitResult.bBlockingHit;
	return HitResult.GetActor();
}

bool AShibController::FindFocusPoint()
{
	FHitResult Hit;
	/*FVector MouseLocation;
	FVector MouseDirection;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	const TArray<AActor*> IgnoreActors;
	GetPlayerState()->GetPlayerController()->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);*/
	//UKismetSystemLibrary::LineTraceSingle(this, MouseLocation, (MouseLocation + (MouseDirection * GetControllerData().MaxZoomOutPoint)), TraceType, false, IgnoreActors, EDrawDebugTrace::Persistent, Hit, true);
	UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, true, Hit);
	if (Hit.bBlockingHit == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Name [%s]"), *Hit.GetActor()->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Hit Found on focus Target!"));
		ControllerData.bFocusPoint = true;
		ControllerData.TargetHeight = Hit.Location.Z;
		UE_LOG(LogTemp, Warning, TEXT("Focus Point [%d]"), GetControllerData().bFocusPoint);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Hit Found on focus Target!"));
		ControllerData.bFocusPoint = false;
		return false;
	}
}

void AShibController::FindMouseTarget()
{
	FHitResult Hit;
	FVector MouseLocation;
	FVector MouseDirection;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
	GetPlayerState()->GetPlayerController()->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	const TArray<AActor*> IgnoreActors;
	UE_LOG(LogTemp, Warning, TEXT("Mouse Loc/Dir: [(%f, %f, %f)/(%f, %f, %f)]"), MouseLocation.X, MouseLocation.Y, MouseLocation.Z, MouseDirection.X, MouseLocation.Y, MouseLocation.Z);
	UKismetSystemLibrary::LineTraceSingle(this, MouseLocation, (MouseLocation + (MouseDirection * GetControllerData().MaxZoomOutPoint)), TraceType, false, IgnoreActors, EDrawDebugTrace::None, Hit, true);
	if (Hit.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mouse Target [%s]"), *Hit.GetActor()->GetName());
		ControllerData.ZoomTarget = Hit.Location.Z;
		if (Hit.Distance > 200.0f)
		{
			ControllerData.ZoomInReductionValue = 4.0f;
		}
		else
		{
			ControllerData.ZoomInReductionValue = 2.0f;
		}
		if (SpringArm->GetComponentLocation().Z > ControllerData.ZoomTarget)
		{
			SpringArm->SetWorldLocation(FVector(SpringArm->GetComponentLocation().X, SpringArm->GetComponentLocation().Y,
				(SpringArm->GetComponentLocation().Z - (
					(SpringArm->GetComponentLocation().Z - ControllerData.ZoomTarget) / ControllerData.ZoomInReductionValue))));
			InitialSpringArmLocation = SpringArm->GetComponentLocation();
		}
		else
		{
			SpringArm->SetWorldLocation(FVector(SpringArm->GetComponentLocation().X, SpringArm->GetComponentLocation().Y,
				(SpringArm->GetComponentLocation().Z + (
					(UKismetMathLibrary::Abs(SpringArm->GetComponentLocation().Z - ControllerData.ZoomTarget)) / ControllerData.ZoomInReductionValue))));
			InitialSpringArmLocation = SpringArm->GetComponentLocation();
		}
	}
}

void AShibController::ScaleAvatarHeight(float HeightScale)
{
	ControllerData.TargetHeightMultiplier = UKismetMathLibrary::MapRangeClamped(HeightScale, 0.0f, 1.0f, 0.75f, 1.1f);
	UE_LOG(LogTemp, Warning, TEXT("Height Multiplier [%f]"), GetControllerData().TargetHeightMultiplier);
}

void AShibController::SetCameraHeightBasedOnGender()
{
	// if (USaveGame* Ref = UGameplayStatics::LoadGameFromSlot(FString("Slot01"), 0))
	// {
	// 	if (UShibSaveGame* CastedRef = Cast<UShibSaveGame>(Ref))
	// 	{
	// 		if (!CastedRef->GetCurrentAvatarData().bIsMale)
	// 		{
	// 			ControllerData.TargetHeights.Empty();
	// 			ControllerData.FemaleTargetHeights.Append(GetControllerData().TargetHeights);
	// 		}
	// 		ControllerData.TargetHeightMultiplier = CastedRef->GetSavedMorphData(EShibBodyMorphs::AvatarHeight);
	// 	}
	// }
}

void AShibController::DisableMovementInLevel()
{
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	if (LevelName == TEXT("Lvl_MainMenu"))
	{
		ControllerData.bAllowCameraMovement = false;
		return;
	}
	if (LevelName == TEXT("Lvl_AvatarBuilder") || LevelName == TEXT("Lvl_AvatarBuilder02") || LevelName == TEXT("Lvl_AvatarBuilder03"))
	{
		ControllerData.bAllowCameraMovement = false;
		FLatentActionInfo LatentActionInfo;
		UKismetSystemLibrary::Delay(this, 3.0f, LatentActionInfo);
		ControllerData.bAllowCameraMovement = true;
		return;
	}
}

void AShibController::ClickAutoRotate(const FInputActionValue& Value)
{
	bRightClick = Value.Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("Right Click [%d]"), GetControllerData().bEnableRightClick);
	if (GetControllerData().bEnableRightClick)
	{
		UE_LOG(LogTemp, Warning, TEXT("Is Rotating [%d]"), GetControllerData().bSetIsRotating);
		if (!GetControllerData().bSetIsRotating)
		{
			ControllerData.bSetIsRotating = true;
			ControllerData.bEnableRightClick = true;
			GetWorld()->GetTimerManager().SetTimer(ClickRotateHandle, this, &AShibController::RotateActor, 0.02f, true);
		}
		else
		{
			ControllerData.bSetIsRotating = false;
			GetWorld()->GetTimerManager().PauseTimer(ClickRotateHandle);
			ControllerData.bEnableRightClick = true;
		}
	}
}

void AShibController::TriggeredRightClickButton(const FInputActionValue& Value)
{
	 bRightClick = Value.Get<bool>(); 

	 if (bRightClick)
	 {
		 ButtonPressStartTime = ButtonPressStartTime + 0.1;
	 }
	 if (!bRightClick)
	 {
		 
		 if(ButtonPressStartTime < 1)
		 {
			 ClickAutoRotate(bRightClick);
		 }
		 ButtonPressStartTime = 0;
	 }

}

void AShibController::TriggeredLeftClickButton(const FInputActionValue& Value)
{
	bLeftClick = Value.Get<bool>();
}

void AShibController::TriggeredMiddleMouseClickButton(const FInputActionValue& Value)
{
	bMiddleMouseClick = Value.Get<bool>();
}

void AShibController::ResetRotationOverride()
{
	GetWorld()->GetTimerManager().PauseTimer(ClickRotateHandle);
	ResetAvatarRotation();
	ControllerData.bEnableRightClick = false;
}

void AShibController::ResetAvatarRotation()
{
	PlayResetRotationTimeline();
}

void AShibController::RotateActor()
{
	GetControllerData().AvatarActor->AddActorWorldRotation(FRotator(0.0f, GetControllerData().AvatarRotationSpeed, 0.0f));
	ControllerData.CurrentAvatarRotation = GetControllerData().AvatarActor->GetActorRotation().Yaw + 180.0f;
}

void AShibController::DoubleClickFocus()
{
	UE_LOG(LogTemp, Warning, TEXT("Double Click Found."));
	
	if (FindFocusPoint())
	{
		HandleSpringArmLag(false);
		UE_LOG(LogTemp, Warning, TEXT("Focus Point Found (Double Click Zoom) [%d]"), GetControllerData().bFocusPoint);
		ZoomDoubleClick();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Focus Point not Found (Double Click Zoom) [%d]"), GetControllerData().bFocusPoint);
	}
}

void AShibController::ZoomIntoBodyPart(bool bExit)
{
	if (!bExit)
	{
		if (ZoomInTimelineComponent)
		{
			ZoomInTimelineComponent->SetPlayRate(2.0f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Zoom In Timeline Invalid!"));
		}
	}
	else
	{
		ResetRotationOverride();
		if (ZoomInTimelineComponent)
		{
			ZoomInTimelineComponent->SetPlayRate(0.25f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Zoom In Timeline Invalid!"));
		}
	}
	PlayZoomInTimeline();
}

void AShibController::ZoomDoubleClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Zoom On Double Click!"));
	if (ZoomDoubleClickTimelineComponent)
	{
		ZoomDoubleClickTimelineComponent->SetPlayRate(2.0f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Zoom DoubleClick Timeline Invalid!"));
	}
	PlayZoomDoubleClickTimeline();
}

void AShibController::DeselectAvatar()
{
	if (bAvatarSelected && Avatar)
	{
		bAvatarSelected = false;
		UE_LOG(LogTemp, Warning, TEXT("Avatar Selected {%d}"), bAvatarSelected);
		Avatar = nullptr;
	}
}

void AShibController::Rotate(const FInputActionValue& Value)
{
	
	float RotationVector = Value.Get<float>();
	

	if (GetControllerData().bAllowCameraMovement && GetControllerData().bEnableRightClick &&  GetControllerData().AvatarActor && !bLeftClick && !bMiddleMouseClick)
	{
		// Get the current actor rotation (only affecting yaw)
		FRotator CurrentRotation = ControllerData.AvatarActor->GetActorRotation();
		// Update the yaw based on controller input
		float NewYaw = CurrentRotation.Yaw + (RotationVector * GetControllerData().CameraRotationSpeed) * -1.0f;
		// Set the new rotation for the character (keeping the pitch and roll unchanged)
		ControllerData.AvatarActor->SetActorRotation(FRotator(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll));
		//SpringArm->AddRelativeRotation(FRotator((RotationVector.Y * GetControllerData().CameraRotationSpeed * -1.0f), 0.0f, 0.0f));
	}
}

void AShibController::Zoom(const FInputActionValue& Value)
{
	float ZoomValue = Value.Get<float>();
	UE_LOG(LogShib, Log, TEXT("%hs - Zooming in/out "), __FUNCTION__);
	if (GetControllerData().bAllowCameraMovement)
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Zoom value is %f"), __FUNCTION__,ZoomValue);

		if (ZoomValue > 0)
		{
			if(SpringArm->TargetArmLength > 40.0f)
			{
				
				ControllerData.CameraZoomSpeed = -(SpringArm->TargetArmLength / 100) * 2;
				SpringArm->TargetArmLength = SpringArm->TargetArmLength + GetControllerData().CameraZoomSpeed;
				UE_LOG(LogShib, Log, TEXT("%hs - Setting SpringArm Len to %f "), __FUNCTION__, SpringArm->TargetArmLength);

			}
		}
		else
		{
			if(SpringArm->TargetArmLength < 300.f)
			{
				ControllerData.CameraZoomSpeed = (SpringArm->TargetArmLength / 100) * 2;
				SpringArm->TargetArmLength = SpringArm->TargetArmLength + GetControllerData().CameraZoomSpeed;
				UE_LOG(LogShib, Log, TEXT("%hs - Setting SpringArm Len to %f "), __FUNCTION__, SpringArm->TargetArmLength);
			}
		}
	}
}

//void AShibController::ZoomTowardsMouseCursor(const FInputActionValue& Value)
//{
//	float ZoomValue = Value.Get<float>();
//	
//	FHitResult AvatarHit;
//	UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, true, AvatarHit);
//
//	
//	if (AvatarHit.bBlockingHit)
//	{
//		HandleSpringArmLag(true);
//		if (SpringArm)
//		{
//			SpringArm->SetRelativeLocation(AvatarHit.Location);
//		}
//
//		Zoom(ZoomValue);
//
//	}
//}

//void AShibController::ZoomTowardsMouseCursor(const FInputActionValue& Value)
//{
//	float ZoomValue = Value.Get<float>();
//
//	// Get the current mouse cursor location
//	FHitResult AvatarHit;
//	UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, true, AvatarHit);
//
//	if (AvatarHit.bBlockingHit)
//	{
//		HandleSpringArmLag(true);
//
//		if (SpringArm)
//		{
//			// If the initial cursor position hasn't been set, set it
//			if (!bIsInitialCursorSet)
//			{
//				InitialCursorLocation = AvatarHit.Location;
//				bIsInitialCursorSet = true;
//			}
//
//			// Draw a debug sphere around the initial cursor location with OffsetRadius
//			//DrawDebugSphere(GetWorld(), InitialCursorLocation, OffsetRadius, 24, FColor::Green, false, -1, 0, 2.0f);
//
//			DrawDebugCircle(GetWorld(), InitialCursorLocation, OffsetRadius, 24, FColor::Green, false, 50, 0, 0.2f);
//			// Calculate the distance from the initial location to the current cursor location
//			float DistanceToInitialLocation = FVector::Dist(AvatarHit.Location, InitialCursorLocation);
//
//			// Check if the cursor is outside the defined radius
//			if (DistanceToInitialLocation > OffsetRadius)
//			{
//				// Update spring arm location to the new cursor position
//				SpringArm->SetRelativeLocation(AvatarHit.Location);
//
//				// Log and reset the initial location to the new position
//				LOG_SHIB(Warning, "Cursor moved outside offset radius, updating location -----------------------");
//				InitialCursorLocation = AvatarHit.Location;
//			}
//		}
//
//		// Apply zoom based on input
//		Zoom(ZoomValue);
//	}
//	else
//	{
//		// Reset the initial cursor location if the hit was invalid
//		bIsInitialCursorSet = false;
//		LOG_SHIB(Warning, "bIsInitialCursorSet = false -----------------------");
//	}
//}


void AShibController::ZoomTowardsMouseCursor(const FInputActionValue& Value)
{
	float ZoomValue = Value.Get<float>();
	Zoom(ZoomValue);
	// OffsetRadius = FMath::Lerp(MaxOffsetRadius, MinOffsetRadius, CurrentZoomLevel);
	//
	// // Get the current mouse cursor location
	// FHitResult AvatarHit;
	// UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, true, AvatarHit);
	//
	// if (AvatarHit.bBlockingHit)
	// {
	// 	HandleSpringArmLag(true);
	//
	// 	if (SpringArm)
	// 	{
	// 		// If the initial cursor position hasn't been set, set it
	// 		if (!bIsInitialCursorSet)
	// 		{
	// 			InitialCursorLocation = AvatarHit.Location;
	// 			bIsInitialCursorSet = true;
	// 		}
	//
	// 		if (bZoomDebuger)
	// 		{
	// 			// Draw a debug circle around the initial cursor location with OffsetRadius
	// 			DrawDebugCircle(GetWorld(), InitialCursorLocation, OffsetRadius, 24, FColor::Green, false, 50, 0, 0.2f);
	// 		}
	// 		
	//
	// 		// Calculate the distance from the initial location to the current cursor location
	// 		float DistanceToInitialLocation = FVector::Dist(AvatarHit.Location, InitialCursorLocation);
	//
	// 		// Check if the cursor is outside the defined radius
	// 		if (DistanceToInitialLocation > OffsetRadius)
	// 		{
	// 			SpringArm->SetRelativeLocation(AvatarHit.Location);
	// 			InitialCursorLocation = AvatarHit.Location;
	// 		}
	// 	}
	//
	// 	// Apply zoom based on input and adjust CurrentZoomLevel accordingly
	// 	Zoom(ZoomValue);
	// 	CurrentZoomLevel = FMath::Clamp(CurrentZoomLevel + ZoomValue * ZoomSensitivity, 0.0f, 1.0f);
	// }
	// else
	// {
	// 	bIsInitialCursorSet = false;
	// }
}



void AShibController::HandleSpringArmLag(bool isLag)
{
	SpringArm->bEnableCameraLag = isLag;
	SpringArm->CameraLagSpeed = 1.5f;
}

void AShibController::MoveVertical(const FInputActionValue& Value)
{
	float MoveValue = -Value.Get<float>();

	if ((bLeftClick || MoveValue != 0) && !bRightClick && !bMiddleMouseClick)
	{
		FVector CurrentLocation = SpringArm->GetComponentLocation();
		CurrentLocation.Z = CurrentLocation.Z + MoveValue / PanningSpeed;
		if (CurrentLocation.Z < InitialSpringArmLocation.Z + MoveLimit && CurrentLocation.Z > InitialSpringArmLocation.Z - MoveLimit)
		{
			SpringArm->SetWorldLocation(CurrentLocation);
		}
	}
}

void AShibController::MoveHorizontal(const FInputActionValue& Value)
{
	float MoveValue = -Value.Get<float>();

	if ((bLeftClick || MoveValue != 0) && !bRightClick && !bMiddleMouseClick)
	{
		FVector CurrentLocation = SpringArm->GetComponentLocation();
		CurrentLocation.Y = CurrentLocation.Y + MoveValue / PanningSpeed;
		if (CurrentLocation.Y < InitialSpringArmLocation.Y + MoveLimit && CurrentLocation.Y > InitialSpringArmLocation.Y - MoveLimit)
		{
			SpringArm->SetWorldLocation(CurrentLocation);
		}
	}
}

void AShibController::ResetRotationTimelineReturn(float Value)
{
	// Handle the interpolation value (Value) here
	//UE_LOG(LogTemp, Warning, TEXT("Reset Rotation Value {%f}"), Value);
	GetControllerData().AvatarActor->SetActorRotation(FRotator(UKismetMathLibrary::RLerp(GetControllerData().AvatarActor->GetActorRotation(), FRotator(0.0f, 180.0f, 0.0f), Value, true)), ETeleportType::None);
	if (DoOnce.Execute())
	{
		UE_LOG(LogTemp, Warning, TEXT("DoOnce Executed!"));
		UKismetSystemLibrary::Delay(this, 0.2f, FLatentActionInfo::FLatentActionInfo());
		ControllerData.bEnableRightClick = true;
		ControllerData.bSetIsRotating = false;
	}
}

void AShibController::OnResetRotationTimelineFinished()
{
	ControllerData.bEnableRightClick = true;
	ControllerData.bSetIsRotating = false;
}

void AShibController::PlayResetRotationTimeline()
{
	if (ResetRotationTimelineComponent)
	{
		ResetRotationTimelineComponent->PlayFromStart();
	}
}

void AShibController::ZoomDoubleClickTimelineReturn(float Value)
{
	SpringArm->TargetArmLength = UKismetMathLibrary::Lerp(SpringArm->TargetArmLength, 75.0f, Value);
	SpringArm->SetWorldLocation(FVector(SpringArm->GetComponentLocation().X, SpringArm->GetComponentLocation().Y, UKismetMathLibrary::Lerp(SpringArm->GetComponentLocation().Z, GetControllerData().TargetHeight, Value)));
	InitialSpringArmLocation = SpringArm->GetComponentLocation();
}

void AShibController::PlayZoomDoubleClickTimeline()
{
	if (ZoomDoubleClickTimelineComponent)
	{
		ZoomDoubleClickTimelineComponent->PlayFromStart();
	}
}

void AShibController::ZoomInTimelineReturn(float Value)
{
	if(ShibAvatarCharacter)
	{
		FName SocketToFocus;
		// TODO -- Instead of switch, make proper mapping - maybe make enums
		switch(GetControllerData().TargetBodyPartIndex)
		{
			case(1):
				SocketToFocus = FName("hairSocket");
				break;
			case(2):
				SocketToFocus = FName("lipsSocket");
				break;
			case(3):
				SocketToFocus = FName("noseSocket");
				break;
			case(4):
				SocketToFocus = FName("lipsSocket");
				break;
			case(7):
				SocketToFocus = FName("fullBodySocket");
				break;
			case(8):
				SocketToFocus = FName("bottomSocket");
				break;
			case(9):
				SocketToFocus = FName("shoesSocket");
				break;
			case(11):
				SocketToFocus = FName("topSocket");
				break;
			case(12):
				SocketToFocus = FName("eyesSocket"); 
				break;
			default:
				SocketToFocus = FName("noseSocket");
				break;
		};
		

		//UE_LOG(LogTemp, Warning, TEXT("Target Rotation [%f, %f, %f]"), GetControllerData().TargetRotations[GetControllerData().TargetBodyPartIndex].Pitch, GetControllerData().TargetRotations[GetControllerData().TargetBodyPartIndex].Yaw, GetControllerData().TargetRotations[GetControllerData().TargetBodyPartIndex].Roll);
		FVector LocationToMoveTo = ShibAvatarCharacter->GetBody()->GetSocketLocation(SocketToFocus);
		//SpringArm->SetWorldRotation(FRotator(UKismetMathLibrary::RLerp(SpringArm->GetComponentRotation(), GetControllerData().TargetRotations[GetControllerData().TargetBodyPartIndex], Value, true)));
		SpringArm->TargetArmLength = UKismetMathLibrary::Lerp(SpringArm->TargetArmLength, GetControllerData().TargetDistances[GetControllerData().TargetBodyPartIndex], Value);
		SpringArm->SetWorldLocation(UKismetMathLibrary::VLerp(SpringArm->GetComponentLocation(), LocationToMoveTo, Value));
		InitialSpringArmLocation = SpringArm->GetComponentLocation();
		//UE_LOG(LogShib, Warning, TEXT("%hs - Value [%f], SpringArmLength [%f]"), __FUNCTION__,Value,SpringArm->TargetArmLength);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ZoomInTimeline - ShibAvatarCharacter not set"));
	}
}

void AShibController::PlayZoomInTimeline()
{
	if (ZoomInTimelineComponent)
	{
		ZoomInTimelineComponent->PlayFromStart();
	}
}

void AShibController::OnZoomInTimelineFinished()
{
	if(!bInitialZoomCompleted)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Initial Zoom completed"));
		bInitialZoomCompleted = true;

		ZoomInTimelineComponent->SetPlayRate(1.0);
		ZoomInTimelineComponent->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
		ZoomInTimelineComponent->SetTimelineLength(1.0);
	}
}

