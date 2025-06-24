// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "I_PlayerInterractions.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UI_PlayerInterractions : public UInterface
{
	GENERATED_BODY()
};

/**
* 
*/
class II_PlayerInterractions
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ForceStopTutorial(bool bForceStop);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MainMenu();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AllowMovement(bool bAllowMovement = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SpawnIntoAvatarEvent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void QuitWidgetLevel(bool bQuitWidgetLevle = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OpenAvatarBuilderLevel(bool bOpenAvatarBuilderLevel = false);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void QuitAvatarBuildLevel();
};