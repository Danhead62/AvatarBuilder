// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "I_PlayerInterractions.h"
#include "Components/TimelineComponent.h"

#include "ShibGM.generated.h"

class UShibSaveGame;
class USaveGame;
class UBuilderUI;

/**
* 
*/
UCLASS()
class SHIBAVATARBUILDER_API AShibGM : public AGameModeBase, public II_PlayerInterractions
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UBuilderUI> BuilderUIRef;
	
	/// Animate Widget Timeline
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UCurveFloat> AnimateWidgetCurve;
protected:
	AShibGM();
	virtual void BeginPlay() override;
	void CheckGameInstanceLoaded();
	void FadeEffect(int32 CameraFadeType);
	void CastingCall();
	//void SaveGame(FString SlotName);
	//void LoadGame(FString SlotName);
	//UMaterial* CheckIfAvatarIsBuild(FString SlotName, bool& bNewUser);
	///bool CheckIfSaveGameExists(FString SlotName);
	//void NewUserResetSaveGame(FString SlotName);
	//void CreateSaveGame(FString SlotName);
	//void SaveGameAvatarName(FString SlotName, FText AvatarName);
	void PickEventBasedOnLevelName();
	void SetInputModeUIEmptyAndDisbaleMouse();
	void SetInputModeForceHideMouse();
	void QuitAvatarLevel();
	void QuitMainMenuLevel(bool bQuit);
	void AnimateWidget(bool bFadeIn = false, class UWidget* Target = nullptr);
	void CreateBootMenuWidget();
	void LoopWidgetA(bool bSaveName, FText AvatarName);
	void CreateMainMenuWidget();
	void OpenAvatarBuilderLevel_Implementation(bool bOpenAvatarBuilderLevel) override;
	void CreateAvatarBuilderWidget();
	void QuitAvatarBuildLevel_Implementation() override;
	void QuitWidgetLevel_Implementation(bool bQuitWidgetLevle) override;
	UPROPERTY()
	bool bInit{ false };
private:
	TObjectPtr<USaveGame> SaveReference;
	TObjectPtr<UShibSaveGame> ShibSaveGame;
	TObjectPtr<UBuilderUI> BuilderUI;
	TObjectPtr<UWidget> CurrentWidget;

	/// Animate Widget Timeline
	UPROPERTY()
	TObjectPtr<UTimelineComponent> AnimateWidgetTimelineComponent;
	FOnTimelineFloat AnimateWidgetInterpFunc{};
	FOnTimelineEvent AnimateWidgetTimelineFinished{};
	UFUNCTION()
	void AnimateWidgetTimelineReturn(float Value);
	void AnimateWidgetReverseFromEnd();
	void PlayAnimateWidgetTimeline();
	/// End Animate Widget Timeline
};