// Fill out your copyright notice in the Description page of Project Settings.

#include "BuilderCustomButton.h"
#include "Components/Button.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BuilderCustomButton)

void UBuilderCustomButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	SetButtonStyle();

}

void UBuilderCustomButton::SetButtonStyle()
{
	FButtonStyle ButtonStyle = FButtonStyle();
	FSlateBrush NormalSlateBrush = FSlateBrush();
	NormalSlateBrush.SetResourceObject(NormalImage);
	ButtonStyle.SetNormal(NormalSlateBrush);
	ButtonStyle.SetDisabled(NormalSlateBrush);
	FSlateBrush HoverSlateBrush = FSlateBrush();
	HoverSlateBrush.SetResourceObject(HoverImage);
	ButtonStyle.SetHovered(HoverSlateBrush);
	FSlateBrush PressedSlateBrush = FSlateBrush();
	PressedSlateBrush.SetResourceObject(ClickedImage);
	ButtonStyle.SetPressed(PressedSlateBrush);

	FSlateSound HoveredSlateSound = FSlateSound();
	HoveredSlateSound.SetResourceObject(HoveredSound);

	FSlateSound PressedSlateSound = FSlateSound();
	PressedSlateSound.SetResourceObject(PressedSound);

	ButtonStyle.SetPressedSound(PressedSlateSound);
	ButtonStyle.SetHoveredSound(HoveredSlateSound);

	this->SetStyle(ButtonStyle);

}