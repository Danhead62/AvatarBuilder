// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "ShibLibrary.h"
#include "Components/TimelineComponent.h"
#include "I_PlayerInterractions.h"

#include "ShibController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;

UCLASS(MinimalAPI)
class AShibController : public APawn, public II_PlayerInterractions
{
	GENERATED_BODY()
public:
	AShibController();
	
	UPROPERTY(BlueprintReadWrite, Category="Config")
	float MoveLimit = 30.f;
	
	bool bInitialZoomCompleted{false};
	
	UFUNCTION(BlueprintCallable)
	const AActor* GetAvatar() const { return Avatar; }
	UFUNCTION(BlueprintCallable)
	const FAvatarControllerData GetControllerData() const { return ControllerData; }
	UFUNCTION(BlueprintCallable)
	void SetControllerData(FAvatarControllerData AvatarControllerData) { ControllerData = AvatarControllerData; }
	UFUNCTION(BlueprintCallable)
	const UCameraComponent* GetControllerCamera() const { return ControllerCamera; }
	UFUNCTION(BlueprintCallable)
	const bool GetIsAvatarSelected() const { return bAvatarSelected; }

	virtual void SpawnIntoAvatarEvent_Implementation() override;

	/// Reset Rotation Timeline
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UCurveFloat> ResetRotationCurve;

	/// Zoom Double Click Timeline
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UCurveFloat> ZoomDoubleClickCurve;

	// Define minimum and maximum radius values for the offset
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shib Camera Setting | Zoom")
	float MinOffsetRadius = 5.0f; // Minimum radius at maximum zoom-in
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shib Camera Setting | Zoom")
	float MaxOffsetRadius = 50.0f; // Maximum radius at maximum zoom-out
	

	//Controls how zoomed in or out you are, ranging from 0.0 (zoomed in) to 1.0 (zoomed out).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shib Camera Setting | Zoom",meta= (ClampMin = "0.0", ClampMax = "1.0"))
	float CurrentZoomLevel = 0.1f;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shib Camera Setting | Zoom")
	float ZoomSensitivity = 0.04f; // Sensitivity to control how much zoom changes based on input

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shib Camera Setting | Zoom")
	bool bZoomDebuger = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Shib Camera Setting | Panning")
	float PanningSpeed = 1.f;

	/// Zoom In Timeline
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UCurveFloat> ZoomInCurve;
	UFUNCTION(BlueprintCallable)
	bool FindFocusPoint();
	UFUNCTION(BlueprintCallable)
	void FindMouseTarget();
	UFUNCTION(BlueprintCallable)
	void ScaleAvatarHeight(float HeightScale);
	UFUNCTION(BlueprintCallable)
	void SetCameraHeightBasedOnGender();
	UFUNCTION(BlueprintCallable)
	void DisableMovementInLevel();
	UFUNCTION(BlueprintCallable)
	void ClickAutoRotate(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void TriggeredRightClickButton(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void TriggeredLeftClickButton(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void TriggeredMiddleMouseClickButton(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void ResetRotationOverride();
	UFUNCTION(BlueprintCallable)
	void ResetAvatarRotation();
	UFUNCTION(BlueprintCallable)
	void RotateActor();
	UFUNCTION(BlueprintCallable)
	void DoubleClickFocus();
	UFUNCTION(BlueprintCallable)
	void ZoomIntoBodyPart(bool bExit);
	UFUNCTION(BlueprintCallable)
	void ZoomDoubleClick();
	UFUNCTION(BlueprintCallable)
	void DeselectAvatar();
	UFUNCTION(BlueprintCallable)
	const AShibAvatarCharacter* GetShibAvatarCharacter() const { return ShibAvatarCharacter; }
	UFUNCTION(BlueprintCallable)
	void SetShibAvatarCharacter(AShibAvatarCharacter* InShibAvatarCharacter) { ShibAvatarCharacter = InShibAvatarCharacter; }

	UFUNCTION(BlueprintCallable)
	void HandleSpringArmLag(bool isLag);
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	AActor* TraceAvatar(bool& bTraced, FHitResult& Hit);
	
	void Rotate(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
	void ZoomTowardsMouseCursor(const FInputActionValue& Value);

	void MoveVertical(const FInputActionValue& Value);
	void MoveHorizontal(const FInputActionValue& Value);
	
	FDoOnlyOnce DoOnce;
private:
	bool bAvatarSelected;
	TObjectPtr<AActor> Avatar;

	float ZoomFactor;
	float MinZoomFOV;
	float MaxZoomFOV;

	FVector InitialCursorLocation;
	bool bIsInitialCursorSet = false;
	float OffsetRadius;
	


	bool bLeftClick;
	bool bMiddleMouseClick;
	bool bRightClick;
	float ButtonPressStartTime;
	
	FVector InitialSpringArmLocation;

	FAvatarControllerData ControllerData;

	FTimerHandle ClickRotateHandle;

	/// Reset Rotation Timeline
	UPROPERTY()
	TObjectPtr<UTimelineComponent> ResetRotationTimelineComponent;
	FOnTimelineFloat ResetRotationInterpFunc{};
	FOnTimelineEvent ResetRotationTimelineFinished{};
	UFUNCTION()
	void ResetRotationTimelineReturn(float Value);
	UFUNCTION()
	void OnResetRotationTimelineFinished();
	void PlayResetRotationTimeline();
	/// End Reset Rotation Timeline

	/// Zoom Double Click Timeline
	UPROPERTY()
	TObjectPtr<UTimelineComponent> ZoomDoubleClickTimelineComponent;
	FOnTimelineFloat ZoomDoubleClickInterpFunc{};
	FOnTimelineEvent ZoomDoubleClickTimelineFinished{};
	UFUNCTION()
	void ZoomDoubleClickTimelineReturn(float Value);
	void PlayZoomDoubleClickTimeline();
	/// End Zoom Double Click Timeline

	/// Zoom In Timeline
	UPROPERTY()
	TObjectPtr<UTimelineComponent> ZoomInTimelineComponent;
	FOnTimelineFloat ZoomInInterpFunc{};
	FOnTimelineEvent ZoomInTimelineFinished{};
	UFUNCTION()
	void ZoomInTimelineReturn(float Value);
	void PlayZoomInTimeline();
	UFUNCTION()
	void OnZoomInTimelineFinished();
	/// End Zoom In Timeline

	/// Spawn into avatar Timeline
	// UPROPERTY()
	// TObjectPtr<UTimelineComponent> SpawnIntoAvatarTimelineComponent;
	// FOnTimelineFloat SpawnIntoAvatarInterpFunc{};
	// FOnTimelineEvent SpawnIntoAvatarTimelineFinished{};
	// UFUNCTION()
	// void SpawnIntoAvatarTimelineReturn(float Value);
	// void PlaySpawnIntoAvatarTimeline();
	// UFUNCTION()
	// void OnSpawnIntoAvatarTimelineFinished();
	/// End Spawn into avatar Timeline

	UPROPERTY()
	TObjectPtr<AShibAvatarCharacter> ShibAvatarCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> ControllerCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MappingContext;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RotateAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeftClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MiddleMouseClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RightClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeftDoubleClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RightDoubleClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ZoomActionForKeyBoard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ZoomActionForMouse;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveVerticalAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveHorizontalAction;
};