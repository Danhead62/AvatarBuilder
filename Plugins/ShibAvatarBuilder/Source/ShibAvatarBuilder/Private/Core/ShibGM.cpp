// Fill out your copyright notice in the Description page of Project Settings.

#include "ShibGM.h"
#include "Kismet/GameplayStatics.h"
#include "ShibController.h"
#include "ShibSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "BuilderUI.h"
#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ShibGM)

AShibGM::AShibGM()
{
	AnimateWidgetTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("AnimateWidgetTimelineComponent"));
}

void AShibGM::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AShibGM::CheckGameInstanceLoaded);
}

void AShibGM::CheckGameInstanceLoaded()
{
	if (GetGameInstance())
	{
		UShibAvatarBuilderSubsystem* SubSystem = GetGameInstance()->GetSubsystem<UShibAvatarBuilderSubsystem>();
		FadeEffect(0);
		PickEventBasedOnLevelName();
		// Animate Widget Timeline
		if (AnimateWidgetCurve)
		{
			AnimateWidgetInterpFunc.BindUFunction(this, FName("AnimateWidgetTimelineReturn"));
			AnimateWidgetTimelineFinished.BindUFunction(this, FName("OnAnimateWidgetTimelineFinished"));
			AnimateWidgetTimelineComponent->AddInterpFloat(AnimateWidgetCurve, AnimateWidgetInterpFunc, FName("Alpha"));
			AnimateWidgetTimelineComponent->SetLooping(false);
			AnimateWidgetTimelineComponent->SetIgnoreTimeDilation(true);
		}
		SubSystem->OnGameReady.Broadcast();
	}
	else
	{
		// Retry next frame if not loaded
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AShibGM::CheckGameInstanceLoaded);
	}
}

void AShibGM::FadeEffect(int32 CameraFadeType)
{
	switch (CameraFadeType)
	{
	case 0:
		UGameplayStatics::GetPlayerCameraManager(this, 0)->SetManualCameraFade(1.0f, FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), false);
		break;
	case 1:
		UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(1.0f, 0.0f, 2.0f, FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), true, true);
		break;
	case 2:
		UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(0.0f, 1.0f, 2.0f, FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), true, true);
		break;
	default:
		break;
	}
}

void AShibGM::CastingCall()
{
	Cast<AShibController>(UGameplayStatics::GetPlayerPawn(this, 0));
}

/*void AShibGM::SaveGame(FString SlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		SaveReference = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
	}
	else
	{
		SaveReference = UGameplayStatics::CreateSaveGameObject(UShibSaveGame::StaticClass());
	}
	ShibSaveGame = Cast<UShibSaveGame>(SaveReference);
	UGameplayStatics::SaveGameToSlot(ShibSaveGame, SlotName, 0);
}*/

/*void AShibGM::LoadGame(FString SlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		ShibSaveGame = Cast<UShibSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		UE_LOG(LogTemp, Warning, TEXT("ShibSaveGame: [%s]"), *ShibSaveGame->GetAvatarSaveData().AvatarName);
	}
}*/

/*UMaterial* AShibGM::CheckIfAvatarIsBuild(FString SlotName, bool& bNewUser)
{
	ShibSaveGame = Cast<UShibSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (ShibSaveGame->GetAvatarSaveData().bNewUser)
	{
		bNewUser = ShibSaveGame->GetAvatarSaveData().bNewUser;
		return ShibSaveGame->GetAvatarSaveData().AvatarThumbnail.Get();
	}
	else
	{
		bNewUser = ShibSaveGame->GetAvatarSaveData().bNewUser;
		return ShibSaveGame->GetAvatarSaveData().AvatarThumbnail.Get();
	}
}*/

/*bool AShibGM::CheckIfSaveGameExists(FString SlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		ShibSaveGame = Cast<UShibSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		return true;
	}
	else
	{
		CreateSaveGame(SlotName);
		return false;
	}
}*/

/*void AShibGM::NewUserResetSaveGame(FString SlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, 0);
		CreateSaveGame(SlotName);
	}
}

void AShibGM::CreateSaveGame(FString SlotName)
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		SaveReference = UGameplayStatics::CreateSaveGameObject(UShibSaveGame::StaticClass());
		ShibSaveGame = Cast<UShibSaveGame>(SaveReference);
		UGameplayStatics::SaveGameToSlot(SaveReference, SlotName, 0);
	}
}*/

/*void AShibGM::SaveGameAvatarName(FString SlotName, FText AvatarName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		SaveReference = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
		ShibSaveGame = Cast<UShibSaveGame>(SaveReference);
		FAvatarSaveData AvatarSaveData;
		AvatarSaveData = ShibSaveGame->GetCurrentAvatarData();
		AvatarSaveData.AvatarName = AvatarName.ToString();
		ShibSaveGame->SetAvatarSaveData(AvatarSaveData);
		UGameplayStatics::SaveGameToSlot(SaveReference, SlotName, 0);
	}
}*/

void AShibGM::PickEventBasedOnLevelName()
{
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	if (LevelName == "Lvl_BootMenu")
	{
		CreateBootMenuWidget();
	}
	if (LevelName == "Lvl_MainMenu" || LevelName == "Lvl_MainMenuRyo" || LevelName == "Lvl_MainMenuEmpty")
	{
		CreateMainMenuWidget();
	}
	if (LevelName == "Lvl_AvatarBuilder04")
	{
		CreateAvatarBuilderWidget();
	}
}

void AShibGM::SetInputModeUIEmptyAndDisbaleMouse()
{
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = false;
	FInputModeUIOnly InputModeUI;
	InputModeUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(InputModeUI);
}

void AShibGM::SetInputModeForceHideMouse()
{
	FInputModeGameOnly InputModeGame;
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(InputModeGame);
	FInputModeUIOnly InputModeUI;
	InputModeUI.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(InputModeUI);
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = false;
}

void AShibGM::QuitAvatarLevel()
{
	FLatentActionInfo LatentActionInfo;
	UKismetSystemLibrary::Delay(this, 1.5f, LatentActionInfo);
	FadeEffect(2);
	//@TODO: Widget Animation
	SetInputModeUIEmptyAndDisbaleMouse();
	FLatentActionInfo LatentInfo;
	UKismetSystemLibrary::Delay(this, 2.2f, LatentInfo);
	UGameplayStatics::OpenLevel(this, FName("Lvl_MainMenuRyo"));
}

void AShibGM::QuitMainMenuLevel(bool bQuit)
{
	//@TODO: Widget Animation
	FadeEffect(2);
	SetInputModeUIEmptyAndDisbaleMouse();
	UKismetSystemLibrary::Delay(this, 2.0f, FLatentActionInfo::FLatentActionInfo());
	if (bQuit)
	{
		UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, true);
	}
	else
	{
		UGameplayStatics::OpenLevel(this, FName("WorldMap02"));
	}
}

void AShibGM::AnimateWidget(bool bFadeIn, UWidget* Target)
{
	if (bFadeIn)
	{
		CurrentWidget = Target;
		Target->SetRenderOpacity(0.0f);
		PlayAnimateWidgetTimeline();
	}
	else
	{
		AnimateWidgetReverseFromEnd();
	}
}

void AShibGM::CreateBootMenuWidget()
{
	FInputModeGameOnly InputModeGame;
	//UE_LOG(LogTemp, Warning, TEXT("[%s]"), *InputModeGame.GetDebugDisplayName());
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(InputModeGame);
	//const bool DoesSaveGame = CheckIfSaveGameExists("Slot01");
	UKismetSystemLibrary::Delay(this, 1.0f, FLatentActionInfo::FLatentActionInfo());
	FadeEffect(1);
	//@TODO:: Setup Widget with all the configurations
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;
}

void AShibGM::LoopWidgetA(bool bSaveName, FText AvatarName)
{
	if (bSaveName)
	{
	//	SaveGameAvatarName("Slot01", AvatarName);
	}
	AnimateWidget(/*@TODO: Setup Proper inputs for widget*/);
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = false;
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeGameOnly::FInputModeGameOnly());
	FadeEffect(2);
	UKismetSystemLibrary::Delay(this, 2.0f, FLatentActionInfo::FLatentActionInfo());
	if (bSaveName)
	{
		UGameplayStatics::OpenLevel(this, FName("Lvl_MainMenuEmpty"));
	}
	else
	{
		UGameplayStatics::OpenLevel(this, FName("Lvl_MainMenuRyo"));
	}
}

void AShibGM::CreateMainMenuWidget()
{
	SetInputModeForceHideMouse();
	UKismetSystemLibrary::Delay(this, 1.0f, FLatentActionInfo::FLatentActionInfo());
	FadeEffect(1);
	//@TODO: Setup Widget According to the requrements.
}

void AShibGM::OpenAvatarBuilderLevel_Implementation(bool bOpenAvatarBuilderLevel)
{
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeGameOnly::FInputModeGameOnly());
	UKismetSystemLibrary::Delay(this, 0.5f, FLatentActionInfo::FLatentActionInfo());
	FadeEffect(2);
	AnimateWidget(false/*@TODO: Setup Proper inputs for widget*/);
	UKismetSystemLibrary::Delay(this, 2.2f, FLatentActionInfo::FLatentActionInfo());
	if (bOpenAvatarBuilderLevel)
	{
		UGameplayStatics::OpenLevel(this, FName("Lvl_AvatarBuilder04"));
	}
	else
	{
		UGameplayStatics::OpenLevel(this, FName("Lvl_MainMenuRyo"));
	}
}

void AShibGM::CreateAvatarBuilderWidget()
{
	SetInputModeForceHideMouse();
	FadeEffect(0);
	BuilderUI = CreateWidget<UBuilderUI>(UGameplayStatics::GetPlayerController(this, 0), BuilderUIRef);
	UE_LOG(LogTemp, Warning, TEXT("Widget Added [%s]"), *BuilderUI.Get()->GetName());
	BuilderUI->AddToViewport(5);
	BuilderUI->SetRenderOpacity(0.0f);
	UKismetSystemLibrary::Delay(this, 1.0f, FLatentActionInfo::FLatentActionInfo());
	TArray<AActor*> OutInterfacedActors;
	UGameplayStatics::GetAllActorsWithInterface(this, UI_PlayerInterractions::StaticClass(), OutInterfacedActors);
	for (auto& Actor : OutInterfacedActors)
	{
		if (Actor)
		{
			II_PlayerInterractions* Interface = Cast<II_PlayerInterractions>(Actor);
			if (Interface)
			{
				Interface->Execute_SpawnIntoAvatarEvent(Actor);
			}
		}
	}
	FadeEffect(1);
	UKismetSystemLibrary::Delay(this, 1.0f, FLatentActionInfo::FLatentActionInfo());
	AnimateWidget(true, BuilderUI);
	UKismetSystemLibrary::Delay(this, 2.0f, FLatentActionInfo::FLatentActionInfo());
	FInputModeGameAndUI InputModeGameAndUI;
	InputModeGameAndUI.SetHideCursorDuringCapture(true);
	InputModeGameAndUI.SetWidgetToFocus(BuilderUI.Get()->GetCachedWidget());
	InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(InputModeGameAndUI);
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;
}

void AShibGM::QuitAvatarBuildLevel_Implementation()
{
	QuitAvatarLevel();
}

void AShibGM::QuitWidgetLevel_Implementation(bool bQuitWidgetLevle)
{
	QuitMainMenuLevel(bQuitWidgetLevle);
}

void AShibGM::AnimateWidgetTimelineReturn(float Value)
{
	CurrentWidget->SetRenderOpacity(Value);
}

void AShibGM::AnimateWidgetReverseFromEnd()
{
	if (AnimateWidgetTimelineComponent)
	{
		AnimateWidgetTimelineComponent->ReverseFromEnd();
	}
}

void AShibGM::PlayAnimateWidgetTimeline()
{
	if (AnimateWidgetTimelineComponent)
	{
		AnimateWidgetTimelineComponent->PlayFromStart();
	}
}