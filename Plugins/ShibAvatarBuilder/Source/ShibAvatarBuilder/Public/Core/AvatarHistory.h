// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShibSaveGame.h"
#include "AvatarHistory.generated.h"

/**
 * TESTING - Possible history refactor
 * It takes snapshots of the current avatar appearance data
 * `Undo`/`Redo` move the position down/up the history and returns the data at that position
 * `Add` adds the struct to history preserving the max length
 *
 * I had to remove all implementation of this class in the subsystem
 * because the way we currently load the avatar is not compatible with this implementation
 */
UCLASS()
class SHIBAVATARBUILDER_API UAvatarHistory : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	int32 MaxHistoryLength = 10;
	
	UPROPERTY(BlueprintReadOnly)
	int32 HistoryPosition =0 ;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FAvatarSave> History;

	UFUNCTION(BlueprintCallable, Category="Avatar History")
	bool Undo(FAvatarSave& Data);
	
	UFUNCTION(BlueprintCallable, Category="Avatar History")
	bool Redo(FAvatarSave& Data);

	UFUNCTION(BlueprintCallable, Category="Avatar History")
	void Add(FAvatarSave Data);

	bool GetCurrentAvatarHistory(FAvatarSave& Outdata);

	// If locked multiple times, will have to unlock the same amout of times to be able to save datq
	// This is so that if an outer function has locked the stack and an inner function already locks the stack temporarily (That is locks at the start of the function and unlocks at the end) the stack remains locked until the outer function unlocks it 
	void LockHistory();
	void UnlockHistory();
	bool IsLocked() const;

	
	void ResetHistory();

	

	// For debugging save game whenever it is added to stack (see "Add" function)
	int printOnAdd = false;
private:
	bool StackLocked = false;
	int MultiLockCounter = 0;
	

};
