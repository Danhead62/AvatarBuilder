// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShibAvatarCharacter.h"

//#include "AvatarCommand.generated.h"

/**
* 
*/
class IAvatarCommand
{
public:
	virtual ~IAvatarCommand() = default;

	virtual void Execute(const AShibAvatarCharacter* Character) = 0;
	virtual void Undo(const AShibAvatarCharacter* Character) = 0;
};