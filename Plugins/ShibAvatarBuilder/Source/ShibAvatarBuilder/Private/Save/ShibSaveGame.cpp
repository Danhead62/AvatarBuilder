// Fill out your copyright notice in the Description page of Project Settings.

#include "Save/ShibSaveGame.h"
#include "ShibAvatarBuilderSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"



#include UE_INLINE_GENERATED_CPP_BY_NAME(ShibSaveGame)






void UShibSaveGame::SetBaseDataRowName(FName RowName,EGender inGender)
{
	if(!bTriedToLoad)return;
	AvatarSaveData.AvatarBaseRow=RowName;
	AvatarSaveData.bIsMale = (inGender == EGender::Male);
	UE_LOG(LogShib, Log, TEXT("%hs - Updated Base Row name to [%s]"), __FUNCTION__, *RowName.ToString());

}

void UShibSaveGame::SetSavedMeshAssetRowName(EShibMesh Type ,FName RowName)
{
	if(!bTriedToLoad || Type == EShibMesh::NONE)return;
	
	if (AvatarSaveData.MeshAssetRowNameData.Contains(Type))
	{
		if(AvatarSaveData.MeshAssetRowNameData[Type] != RowName)
		{
			UE_LOG(LogShib, Log, TEXT("%hs - Changed Asset Row data [%s] to value [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString(), *RowName.ToString());
			AvatarSaveData.MeshAssetRowNameData[Type] = RowName;
		}
	}
	else
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Added Asset Row data [%s] to value [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString(), *RowName.ToString());
		AvatarSaveData.MeshAssetRowNameData.Add(Type,RowName);
	}
	
}

void UShibSaveGame::SetSavedShibTextureAssetRowName(EShibTexture Type ,FName RowName)
{
	if(!bTriedToLoad || Type == EShibTexture::NONE)return;
	
	if (AvatarSaveData.ShibTextureAssetRowNameData.Contains(Type))
	{
		if(AvatarSaveData.ShibTextureAssetRowNameData[Type] != RowName)
		{
			UE_LOG(LogShib, Log, TEXT("%hs - Changed Asset Row data [%s] to value [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString(), *RowName.ToString());
			AvatarSaveData.ShibTextureAssetRowNameData[Type] = RowName;
		}

	}
	else
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Added Asset Row data [%s] to value [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString(), *RowName.ToString());
		AvatarSaveData.ShibTextureAssetRowNameData.Add(Type,RowName);
	}
	
}

void UShibSaveGame::SetSavedAccessoryRowName(EShibAccessory Type, FName RowName)
{
	if(!bTriedToLoad || Type == EShibAccessory::NONE)return;
	
	if (AvatarSaveData.AccessoryRowNameData.Contains(Type))
	{
		if( AvatarSaveData.AccessoryRowNameData[Type] != RowName)
		{
			UE_LOG(LogShib, Log, TEXT("%hs - Changed Asset Row data [%s] to value [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString(), *RowName.ToString());
			AvatarSaveData.AccessoryRowNameData[Type] = RowName;
		}
	}
	else
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Added Asset Row data [%s] to value [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString(), *RowName.ToString());
		AvatarSaveData.AccessoryRowNameData.Add(Type,RowName);
	}
}

void UShibSaveGame::SetSavedTattooRowName(EShibTattoo Type, FName RowName)
{
	if(!bTriedToLoad || Type == EShibTattoo::NONE)return;
	
	if (AvatarSaveData.TattooRowNameData.Contains(Type))
	{
		if(AvatarSaveData.TattooRowNameData[Type] != RowName)
		{
			UE_LOG(LogShib, Log, TEXT("%hs - Changed Asset Row data [%s] to value [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString(), *RowName.ToString());
			AvatarSaveData.TattooRowNameData[Type] = RowName;
		}
		
	}
	else
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Added Asset Row data [%s] to value [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString(), *RowName.ToString());
		AvatarSaveData.TattooRowNameData.Add(Type,RowName);
	}
}

void UShibSaveGame::SetMorphData(EShibBodyMorphs Type, double val)
{

	if(!bTriedToLoad || Type == EShibBodyMorphs::NONE)return;
	
	if (AvatarSaveData.MorphData.Contains(Type))
	{
		if( AvatarSaveData.MorphData[Type] != val)
		{
			UE_LOG(LogShib, Log, TEXT("%hs - Changed Morph data [%s] to value [%f]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString(), val);
			AvatarSaveData.MorphData[Type] = val;
		}
		
	}
	else
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Added Morph data [%s] to value [%f]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString(), val);
		AvatarSaveData.MorphData.Add(Type,val);
	}

}

void UShibSaveGame::SetShibMeshColorData(EShibMesh Type, FLinearColor val)
{
	
	
	if(!bTriedToLoad || Type == EShibMesh::NONE)return;
	
	// Skip if Accessory Type, use the respective functions
	if(Type == EShibMesh::Accessory)
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Use the Set Accessory ColorData functions for corresponding type. Returning. "), __FUNCTION__);
		return;
	}

	// Default color doesnt need to be saved
	if(val == FLinearColor(1,1,1,1))
	{
		AvatarSaveData.ShibMeshColorData.Remove(Type);
		return;
	}

	
	if (AvatarSaveData.ShibMeshColorData.Contains(Type))
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Changed Color data [%s] to value [%s]"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString(), *val.ToString());
		AvatarSaveData.ShibMeshColorData[Type] = val;
	}
	else
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Added Color data [%s] to value [%s]"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString(), *val.ToString());
		AvatarSaveData.ShibMeshColorData.Add(Type, val);
	}
	
}

void UShibSaveGame::SetShibTextureColorData(EShibTexture Type, FLinearColor val)
{
	if(!bTriedToLoad || Type == EShibTexture::NONE)return;

	// Default color doesnt need to be saved (Black for eyebrows and eyes)
	FLinearColor DefaultColor = (Type == EShibTexture::EyebrowShape || Type == EShibTexture::EyeShape) ? FLinearColor(0,0,0,1):  FLinearColor(1,1,1,1);
	if(val == DefaultColor)
	{
		AvatarSaveData.ShibTextureColorData.Remove(Type);
		return;
	}
	
	if (AvatarSaveData.ShibTextureColorData.Contains(Type))
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Changed Color data [%s] to value [%s]"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString(), *val.ToString());
		AvatarSaveData.ShibTextureColorData[Type] = val;
	}
	else
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Added Color data [%s] to value [%s]"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString(), *val.ToString());
		AvatarSaveData.ShibTextureColorData.Add(Type, val);
	}
	
}

void UShibSaveGame::SetAccessoryShibColorData(EShibAccessory Type, FLinearColor val)
{
	if(!bTriedToLoad || Type == EShibAccessory::NONE)return;

	// Default color doesnt need to be saved
	if(val == FLinearColor(1,1,1,1))
	{
		AvatarSaveData.AccessoryShibColorData.Remove(Type);
		return;
	}


	
	if (AvatarSaveData.AccessoryShibColorData.Contains(Type))
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Changed Color data [%s] to value [%s]"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString(), *val.ToString());
		AvatarSaveData.AccessoryShibColorData[Type] = val;
	}
	else
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Added Color data [%s] to value [%s]"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString(), *val.ToString());
		AvatarSaveData.AccessoryShibColorData.Add(Type, val);
	}
}

void UShibSaveGame::SetTattooShibColorData(EShibTattoo Type, FLinearColor val)
{
	if(!bTriedToLoad || Type == EShibTattoo::NONE)return;
	
	if(val == FLinearColor(1,1,1,1))
	{
		AvatarSaveData.TattooShibColorData.Remove(Type);
		return;
	}
	
	if (AvatarSaveData.TattooShibColorData.Contains(Type))
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Changed Color data [%s] to value [%s]"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString(), *val.ToString());
		AvatarSaveData.TattooShibColorData[Type] = val;
	}
	else
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Added Color data [%s] to value [%s]"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString(), *val.ToString());
		AvatarSaveData.TattooShibColorData.Add(Type, val);
	}
}


void UShibSaveGame::RemoveSavedMeshData(EShibMesh Type)
{
	if(!bTriedToLoad)return;
	//UE_LOG(LogShib, Log, TEXT("%hs - Tried to load = %s"), __FUNCTION__,bTriedToLoad? TEXT("true"):TEXT("false"));

	if(AvatarSaveData.MeshAssetRowNameData.Contains(Type))
	{
		AvatarSaveData.MeshAssetRowNameData.Remove(Type);
		UE_LOG(LogShib, Log, TEXT("%hs - Removing Saved Mesh of type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	}
	// Remove Color as well
	if(AvatarSaveData.ShibMeshColorData.Contains(Type))
	{
		AvatarSaveData.ShibMeshColorData.Remove(Type);
		UE_LOG(LogShib,Log, TEXT("%hs - Removing Saved Color for Mesh of type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString())
	}
}

void UShibSaveGame::RemoveSavedShibTextureData(EShibTexture Type)
{
	if(!bTriedToLoad)return;
	
	if(AvatarSaveData.ShibTextureAssetRowNameData.Contains(Type))
	{
		AvatarSaveData.ShibTextureAssetRowNameData.Remove(Type);
		UE_LOG(LogShib, Log, TEXT("%hs - Removing Saved Texture of type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	}

	if(AvatarSaveData.ShibTextureColorData.Contains(Type))
	{
		AvatarSaveData.ShibTextureColorData.Remove(Type);
		UE_LOG(LogShib, Log, TEXT("%hs - Removing Saved Color for Texture of type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	}
}

void UShibSaveGame::RemoveSavedAccessoryData(EShibAccessory Type)
{
	if(!bTriedToLoad)return;
	
	if(AvatarSaveData.AccessoryRowNameData.Contains(Type))
	{
		AvatarSaveData.AccessoryRowNameData.Remove(Type);
		UE_LOG(LogShib, Log, TEXT("%hs - Removing Saved Accessory of type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	}

	if(AvatarSaveData.AccessoryShibColorData.Contains(Type))
	{
		AvatarSaveData.AccessoryShibColorData.Remove(Type);
		UE_LOG(LogShib, Log, TEXT("%hs - Removing Saved Color for Accessory of type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	}
}

void UShibSaveGame::RemoveSavedTattooData(EShibTattoo Type)
{
	if(!bTriedToLoad)return;
	
	if(AvatarSaveData.TattooRowNameData.Contains(Type))
	{
		AvatarSaveData.TattooRowNameData.Remove(Type);
		UE_LOG(LogShib, Log, TEXT("%hs - Removing Saved Tattoo of type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	}

	if(AvatarSaveData.TattooShibColorData.Contains(Type))
	{
		AvatarSaveData.TattooShibColorData.Remove(Type);
		UE_LOG(LogShib, Log, TEXT("%hs - Removing Saved Color for Tattoo of type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	}
}


bool UShibSaveGame::GetSavedBaseAssetData(FBase& OutData) const
{
	return GetSavedBaseAssetData(AvatarSaveData, OutData);
}

bool UShibSaveGame::GetSavedBaseAssetData( const FAvatarSave& SaveData, FBase& OutData) const
{
	if(IsValid(ShibBuilder) && IsValid(ShibBuilder->AvatarDataAsset) && IsValid(ShibBuilder->AvatarDataAsset->Base))
	{
		if(FBase* Base = ShibBuilder->AvatarDataAsset->Base->FindRow<FBase>(SaveData.AvatarBaseRow,"LoadingAvatarBase"))
		{
			UE_LOG(LogShib, Log, TEXT("%hs - Found Base data for gender [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Base->Gender).ToString())
			OutData = *Base;
			return true;
		}
	}
//	UE_LOG(LogShib, Warning, TEXT("%hs - Failed to get saved Base data for avatar"), __FUNCTION__)
	return false;
}


double UShibSaveGame::GetSavedMorphData(EShibBodyMorphs Type) const
{
	return GetSavedMorphData(AvatarSaveData,Type);
}

double UShibSaveGame::GetSavedMorphData(const FAvatarSave& SaveData, EShibBodyMorphs Type) const 
{
	if(SaveData.MorphData.Contains(Type))
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Found Morph data [%s] of value [%f]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString(),SaveData.MorphData[Type]);
		return SaveData.MorphData[Type];
	}
	//UE_LOG(LogShib, Log, TEXT("%hs - Could not find Morph data [%s] returning 0.5"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	return 0.5;
}


bool UShibSaveGame::GetSavedMeshAssetData(EShibMesh Type, FMesh& OutMesh) const 
{
	return GetSavedMeshAssetData(AvatarSaveData,Type,OutMesh);
}

bool UShibSaveGame::GetSavedMeshAssetData(const FAvatarSave& SaveData, EShibMesh Type, FMesh& OutMesh) const 
{
	// Check if there is data to load
	if(SaveData.MeshAssetRowNameData.Contains(Type))
	{
		// Check if Target Data Table is loaded
		if(IsValid(ShibBuilder) &&IsValid(ShibBuilder->AvatarDataAsset) && ShibBuilder->AvatarDataAsset->MeshAssetDataTables.Contains(Type))
		{
			UDataTable* TargetTable = *ShibBuilder->AvatarDataAsset->MeshAssetDataTables.Find(Type);
			// Get Row from Table
			if(FMesh* mesh = TargetTable->FindRow<FMesh>(SaveData.MeshAssetRowNameData[Type],FString::Printf(TEXT("Load %s Mesh"),*UEnum::GetDisplayValueAsText(Type).ToString())))
			{
				UE_LOG(LogShib, Log, TEXT("%hs - Got Mesh for Type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
				OutMesh = *mesh;
				return true;
			}
			UE_LOG(LogShib, Warning, TEXT("%hs - Could not find row [%s] for type [%s]"), __FUNCTION__,*SaveData.MeshAssetRowNameData[Type].ToString(),*UEnum::GetDisplayValueAsText(Type).ToString());
		}
		UE_LOG(LogShib, Warning, TEXT("%hs - Target Table for Type [%s] was not loaded"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	}
	else
	{
		//UE_LOG(LogShib, Warning, TEXT("%hs - No data saved for Type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	}
	
	return false;
}


bool UShibSaveGame::GetSavedAccessoryMeshData(EShibAccessory Type, FMesh& OutMesh) const 
{
	return GetSavedAccessoryMeshData(AvatarSaveData,Type,OutMesh);
}

bool UShibSaveGame::GetSavedAccessoryMeshData(const FAvatarSave& SaveData, EShibAccessory Type, FMesh& OutMesh) const 
{
	// Check if there is a saved Accessory for Accessory Type
	if(SaveData.AccessoryRowNameData.Contains(Type))
	{
		// Check if data table for accessories is loaded
		if(IsValid(ShibBuilder) && IsValid(ShibBuilder->AvatarDataAsset) && ShibBuilder->AvatarDataAsset->Accessory)
		{
			// Get Accessory Mesh data from row
			if(FMesh* AccessoryMesh = ShibBuilder->AvatarDataAsset->Accessory->FindRow<FMesh>(SaveData.AccessoryRowNameData[Type],FString::Printf(TEXT("Load %s Accessory Mesh"),*UEnum::GetDisplayValueAsText(Type).ToString())))
			{
				UE_LOG(LogShib, Log, TEXT("%hs - Got Accessory Mesh for Type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
				OutMesh = *AccessoryMesh;
				return true;
			}
			
			UE_LOG(LogShib, Warning, TEXT("%hs - Could not find row [%s] for Accessory Mesh of Type [%s]"), __FUNCTION__,*SaveData.AccessoryRowNameData[Type].ToString(),*UEnum::GetDisplayValueAsText(Type).ToString());
		}
		
		UE_LOG(LogShib, Warning, TEXT("%hs - Accessory Table not loaded"), __FUNCTION__);
	}
	
	//UE_LOG(LogShib, Warning, TEXT("%hs - No Accessory Mesh of Type [%s] was saved"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	return false;
}


bool UShibSaveGame::GetSavedShibTextureAssetData(EShibTexture Type, FShibTexture& OutShibTex) const 
{
	return GetSavedShibTextureAssetData(AvatarSaveData,Type,OutShibTex);
}

bool UShibSaveGame::GetSavedShibTextureAssetData(const FAvatarSave& SaveData, EShibTexture Type, FShibTexture& OutShibTex) const 
{
	// Check if there is data to load
	if(SaveData.ShibTextureAssetRowNameData.Contains(Type))
	{
		// Check if Target Data Table is loaded
		if(IsValid(ShibBuilder) && IsValid(ShibBuilder->AvatarDataAsset) && ShibBuilder->AvatarDataAsset->ShibTextureAssetDataTables.Contains(Type))
		{
			UDataTable* TargetTable = *ShibBuilder->AvatarDataAsset->ShibTextureAssetDataTables.Find(Type);
			// Get Row from Table
			if(FShibTexture* Tex = TargetTable->FindRow<FShibTexture>(SaveData.ShibTextureAssetRowNameData[Type],FString::Printf(TEXT("Load %s Texture"),*UEnum::GetDisplayValueAsText(Type).ToString())))
			{
				UE_LOG(LogShib, Log, TEXT("%hs - Got Mesh for Type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
				OutShibTex =  *Tex;
				return true;
			}
			UE_LOG(LogShib, Warning, TEXT("%hs - Could not find row [%s] for type [%s]"), __FUNCTION__,*SaveData.ShibTextureAssetRowNameData[Type].ToString(),*UEnum::GetDisplayValueAsText(Type).ToString());
		}
		UE_LOG(LogShib, Warning, TEXT("%hs - Target Table for Type [%s] was not loaded"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	}
	else
	{
		//UE_LOG(LogShib, Warning, TEXT("%hs - No data saved for Type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	}

	return false;
}


bool UShibSaveGame::GetSavedTattooTextureData(EShibTattoo Type, FShibTexture& OutShibTex) const 
{
	return GetSavedTattooTextureData(AvatarSaveData,Type,OutShibTex);
}

bool UShibSaveGame::GetSavedTattooTextureData(const FAvatarSave& SaveData, EShibTattoo Type, FShibTexture& OutShibTex) const 
{
	// Check if there is a saved Accessory for Accessory Type
	if(SaveData.TattooRowNameData.Contains(Type))
	{
		// Check if data table for accessories is loaded
		if(IsValid(ShibBuilder) &&  IsValid(ShibBuilder->AvatarDataAsset) && ShibBuilder->AvatarDataAsset->Tattoo)
		{
			// Get Accessory Mesh data from row
			if(FShibTexture* TattooTex = ShibBuilder->AvatarDataAsset->Tattoo->FindRow<FShibTexture>(SaveData.TattooRowNameData[Type],FString::Printf(TEXT("Load %s Accessory Mesh"),*UEnum::GetDisplayValueAsText(Type).ToString())))
			{
				UE_LOG(LogShib, Log, TEXT("%hs - Got Tattoo Texture for Type [%s]"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
				OutShibTex =  *TattooTex;
				return true;
			}
			
			//UE_LOG(LogShib, Warning, TEXT("%hs - Could not find row [%s] for Tattoo Texture of Type [%s]"), __FUNCTION__,*SaveData.TattooRowNameData[Type].ToString(),*UEnum::GetDisplayValueAsText(Type).ToString());
		}
		
		//UE_LOG(LogShib, Warning, TEXT("%hs - Tattoo Table not loaded"), __FUNCTION__);
	}
	
	//UE_LOG(LogShib, Warning, TEXT("%hs - No Tattoo Texture of Type [%s] was saved"), __FUNCTION__,*UEnum::GetDisplayValueAsText(Type).ToString());
	return false;
}


bool UShibSaveGame::GetSavedShibMeshColorData(EShibMesh Type, FLinearColor& OutColor) const 
{
	return GetSavedShibMeshColorData(AvatarSaveData,Type,OutColor);
}

bool UShibSaveGame::GetSavedShibMeshColorData(const FAvatarSave& SaveData, EShibMesh Type, FLinearColor& OutColor) const 
{
	if(Type == EShibMesh::Accessory || Type == EShibMesh::NONE)return false;
	
	if (SaveData.ShibMeshColorData.Contains(Type))
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Found Shib Color data [%s] of value [%s]"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString(), *SaveData.ShibMeshColorData[Type].ToString());
		OutColor = SaveData.ShibMeshColorData[Type];
		return true;
	}
//	UE_LOG(LogShib, Log, TEXT("%hs - Could not find Shib Color data [%s] returning default color"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString());
	OutColor = FHSV::GetDefaultRGBValue();
	return false;
}


bool UShibSaveGame::GetSavedShibTextureColorData(EShibTexture Type, FLinearColor& OutColor) const 
{
	return GetSavedShibTextureColorData(AvatarSaveData,Type,OutColor);
}

bool UShibSaveGame::GetSavedShibTextureColorData(const FAvatarSave& SaveData, EShibTexture Type, FLinearColor& OutColor) const 
{
	if(Type == EShibTexture::NONE)return false;
	
	if (SaveData.ShibTextureColorData.Contains(Type))
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Found Shib Color data [%s] of value [%s]"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString(), *SaveData.ShibTextureColorData[Type].ToString());
		OutColor = SaveData.ShibTextureColorData[Type];
		return true;
	}
	//UE_LOG(LogShib, Log, TEXT("%hs - Could not find Shib Color data [%s] returning default color"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString());
	OutColor = FHSV::GetDefaultRGBValue();
	return false;
}


bool UShibSaveGame::GetSavedAccessoryColorData(EShibAccessory Type, FLinearColor& OutColor) const 
{
	return GetSavedAccessoryColorData(AvatarSaveData,Type,OutColor);
}

bool UShibSaveGame::GetSavedAccessoryColorData(const FAvatarSave& SaveData, EShibAccessory Type, FLinearColor& OutColor) const 
{
	// Check if there is a saved color for the accessory type
	if (SaveData.AccessoryShibColorData.Contains(Type))
	{
		
		UE_LOG(LogShib, Log, TEXT("%hs - Retrieved saved color data for Accessory Type [%s] with value, %s"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString(),*SaveData.AccessoryShibColorData[Type].ToString());
		// Return the saved color
		OutColor =  SaveData.AccessoryShibColorData[Type];
		return true;
	}

	// Log a warning if no color data was found for the accessory type
	//UE_LOG(LogShib, Warning, TEXT("%hs - No Accessory Color Data of Type [%s] was saved"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString());
	
	// Return a default color if no saved color data was found
	OutColor = FHSV::GetDefaultRGBValue();
	return false;
}


bool UShibSaveGame::GetSavedTattooColorData(EShibTattoo Type, FLinearColor& OutColor) const 
{
	return GetSavedTattooColorData(AvatarSaveData,Type,OutColor);
}

bool UShibSaveGame::GetSavedTattooColorData(const FAvatarSave& SaveData, EShibTattoo Type, FLinearColor& OutColor) const 
{
	// Check if there is a saved color for the tattoo type
	if (SaveData.TattooShibColorData.Contains(Type))
	{
		
		UE_LOG(LogShib, Log, TEXT("%hs - Retrieved saved color data for Tattoo Type [%s]"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString());
		// Return the saved color
		OutColor =  SaveData.TattooShibColorData[Type];
		return true;
	}

	// Log a warning if no color data was found for the tattoo type
	//UE_LOG(LogShib, Warning, TEXT("%hs - No Tattoo Color Data of Type [%s] was saved"), __FUNCTION__, *UEnum::GetDisplayValueAsText(Type).ToString());
	
	// Return a default color if no saved color data was found
	OutColor = FHSV::GetDefaultRGBValue();
	return false;
}

void UShibSaveGame::PrintCurrentSaveGame()
{
	AvatarSaveData.PrintData();
}


void UShibSaveGame::SetShibBuilder(const UShibAvatarBuilderSubsystem* Builder)
{
	ShibBuilder = Builder;
}



void UShibSaveGame::SaveCurrentAvatarData()
{
	if (UGameplayStatics::SaveGameToSlot(this, AvatarCustomizationSlot, 0))
	{
		UE_LOG(LogShib, Log, TEXT("Game saved successfully to slot: %s"), *AvatarCustomizationSlot);
	}
	else
	{
		UE_LOG(LogShib, Error, TEXT("Failed to save game to slot: %s"), *AvatarCustomizationSlot);
	}
}

FString UShibSaveGame::GetSaveJsonFilePath() 
{
	return  FPaths::Combine(FPaths::ProjectSavedDir(),"SaveGames",AvatarCustomizationSlot + ".json");
}


void UShibSaveGame::SaveCurrentAvatarDataToJson() const
{
	// Convert Saved Struct to Json String
	FString JsonString;
	if(!FJsonObjectConverter::UStructToJsonObjectString(AvatarSaveData, JsonString))
	{
		UE_LOG(LogShib, Warning, TEXT("%hs - Could not convert data to string"), __FUNCTION__);
		return;
	}
	
	// Save Json String to File 
	FString FilePath = GetSaveJsonFilePath();
	if(FFileHelper::SaveStringToFile(JsonString,*FilePath))
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Saved Json Data to path - %s"), __FUNCTION__,*FilePath);
	}
	else
	{
		UE_LOG(LogShib, Warning, TEXT("%hs - Failed to save Json Data to path - %s"), __FUNCTION__,*FilePath);
	}
}

bool UShibSaveGame::LoadCurrentAvatarDataFromJson()
{
	// Check if there is something to load
	if (!CheckJsonSaveGameExists())
	{
		
		UE_LOG(LogShib, Warning, TEXT("Save game does not exist in slot: %s"), *UShibSaveGame::AvatarCustomizationSlot);
		return false;
	}

	
	// Load string in json file 
	FString FileString;
	FString FilePath = GetSaveJsonFilePath();
	if(!FFileHelper::LoadFileToString(FileString,*FilePath))
	{
		UE_LOG(LogShib, Log, TEXT("%hs - Could not Load File into string with path %s"), __FUNCTION__,*FilePath);
		return false;
	}
	
	// Convert String to AvatarSave object
	LoadCurrentAvatarDataFromJson(FileString);

	UE_LOG(LogShib, Log, TEXT("%hs - Loaded file data to AvatarSaveData"), __FUNCTION__);
	return true;

}

bool UShibSaveGame::LoadCurrentAvatarDataFromJson(TSharedRef<FJsonObject> JsonObject)
{
	if(!FJsonObjectConverter::JsonObjectToUStruct<FAvatarSave>(JsonObject,&AvatarSaveData))
	{
		UE_LOG(LogShib, Warning, TEXT("%hs - Could not serialize file string to Avatar Data"), __FUNCTION__);
		return false;
	}
	UE_LOG(LogShib, Log, TEXT("%hs - Loaded Object data to AvatarSaveData"), __FUNCTION__);
	return true;
}

bool UShibSaveGame::LoadCurrentAvatarDataFromJson(const FString& JsonString)
{
	if(JsonString.IsEmpty()) return false;
	
	if(!FJsonObjectConverter::JsonObjectStringToUStruct<FAvatarSave>(JsonString,&AvatarSaveData))
	{
		UE_LOG(LogShib, Warning, TEXT("%hs - Could not serialize file string to Avatar Data"), __FUNCTION__);
		return false;
	}

	UE_LOG(LogShib, Log, TEXT("%hs - Loaded string data to AvatarSaveData"), __FUNCTION__);
	return true;
}

bool UShibSaveGame::CheckJsonSaveGameExists()
{
	return FPaths::FileExists(GetSaveJsonFilePath());
}

FAvatarSave& UShibSaveGame::GetCurrentAvatarData()
{
	return AvatarSaveData;
}

void UShibSaveGame::SetCurrentAvatarData(const FAvatarSave Data)
{
	AvatarSaveData=Data;
}






