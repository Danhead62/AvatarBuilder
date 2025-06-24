// Fill out your copyright notice in the Description page of Project Settings.

#include "UISubsystem.h"

#include "ParticleHelper.h"
#include "ShibController.h"
#include "ShibLibrary.h"
#include "Engine/DataTable.h"
#include "ShibSaveGame.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(UISubsystem)

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UUISubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UUISubsystem::ChangeAvatarFocus(int32 FocusIndex)
{
	if(GetShibController() && GetShibController()->bInitialZoomCompleted)
	{
		UE_LOG(LogTemp, Log, TEXT("Focusing on body part with index: %i"), FocusIndex);
		FocusIndexHistory = FocusIndex;
		FAvatarControllerData AvatarControllerData = GetShibController()->GetControllerData();
		AvatarControllerData.TargetBodyPartIndex = FocusIndex;
		ShibController->SetControllerData(AvatarControllerData);
		ShibController->ZoomIntoBodyPart(false);
		ShibController->ResetRotationOverride();
		ShibController->HandleSpringArmLag(true);
	}
	else
	{
		UE_LOG(LogShib, Warning, TEXT("Spawn into avatar not finished yet"));
	}

}

void UUISubsystem::ChangeAvatarFocusExit(int32 FocusIndex)
{
	UE_LOG(LogTemp, Log, TEXT("Focus Exit on body part with index: %i"), FocusIndex);
	FAvatarControllerData AvatarControllerData = GetShibController()->GetControllerData();
	AvatarControllerData.TargetBodyPartIndex = FocusIndex;
	ShibController->SetControllerData(AvatarControllerData);
	//ShibController->ResetRotationOverride();
}