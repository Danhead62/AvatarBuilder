// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AvatarHistory.h"
#include <string>
#include "JsonObjectConverter.h"


bool UAvatarHistory::Undo(FAvatarSave& Data)
{

	if (!History.IsValidIndex(HistoryPosition - 1)) return false;
	Data = History[--HistoryPosition];
	UE_LOG(LogShib, Log, TEXT("%hs - Undo change [%d] "), __FUNCTION__,HistoryPosition);
	return true;
}

bool UAvatarHistory::Redo(FAvatarSave& Data)
{
	if (!History.IsValidIndex(HistoryPosition + 1)) return false;
	Data = History[++HistoryPosition];
	UE_LOG(LogShib, Log, TEXT("%hs - Redo change [%d] "), __FUNCTION__,HistoryPosition);
	return true;
}

void UAvatarHistory::Add(const FAvatarSave Data)
{
	if(StackLocked) return;
	
	if (!History.IsEmpty())
	{
		if(Data == History[HistoryPosition])
		{
			UE_LOG(LogShib, Log, TEXT("%hs - Attempting to add duplicate data, returning.."), __FUNCTION__);
			return;
		}
		
		if (History.Num() - 1 == HistoryPosition) // if top of the stack
		{
			if (History.Num() == MaxHistoryLength) History.RemoveAt(0);
			else HistoryPosition++;
		}
		else
		{
			// remove everything after the current position
			History.RemoveAtSwap(HistoryPosition + 1, History.Num() - HistoryPosition - 1);
			HistoryPosition++;
		}
	}
	History.Add(Data);
	UE_LOG(LogShib, Log, TEXT("%hs - Added change [%d], Size of Stack [%d] "), __FUNCTION__,HistoryPosition, History.Num());

	if(printOnAdd)
	{
		Data.PrintData();
	}
	
}

bool UAvatarHistory::GetCurrentAvatarHistory(FAvatarSave& OutData)
{
	if(History.IsValidIndex(HistoryPosition))
	{
		OutData = History[HistoryPosition];
		return true;
	}
	return false;
}

void UAvatarHistory::LockHistory()
{
	if(!StackLocked)
	{
		StackLocked = true;
	}
	MultiLockCounter++;
}

void UAvatarHistory::UnlockHistory()
{
	MultiLockCounter--;
	if(MultiLockCounter <= 0)
	{
		MultiLockCounter = 0;
		StackLocked = false;
	} 
		
}

void UAvatarHistory::ResetHistory()
{
	History.Empty();
	HistoryPosition = 0;
	IFileManager& FileManager = IFileManager::Get();
	FileManager.DeleteDirectory(*FPaths::Combine(FPaths::ProjectSavedDir(),"SaveGames","DebugHistory"));

}

bool UAvatarHistory::IsLocked() const
{
	return StackLocked;
}


