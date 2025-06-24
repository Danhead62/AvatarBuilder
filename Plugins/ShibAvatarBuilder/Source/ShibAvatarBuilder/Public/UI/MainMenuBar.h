// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"

#include "MainMenuBar.generated.h"

class UBuilderCustomButton;
class UTextBlock;

/**
* 
*/
UCLASS()
class SHIBAVATARBUILDER_API UMainMenuBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
};