// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "JsonObjectConverter.h"
#include "GameFramework/SaveGame.h"
#include "ShibLibrary.h"
#include "ShibSaveGame.generated.h"

/**
* 
*/

class UShibAvatarBuilderSubsystem;

UENUM(BlueprintType)
enum class EShibColorSaveType : uint8
{
	AccessoryColor,
	BottomColor,
	EyebrowColor,
	EyeColor,
	FullBodyColor,
	HairColor,
	ShoeColor,
	TattooColor,
	TopColor,
	
};



UENUM(BlueprintType)
enum class ELoadAvatarType : uint8
{
	JsonFile,
	JsonString,
};




USTRUCT(BlueprintType)
struct FAvatarSave
{
	GENERATED_BODY()

	// For gender
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AvatarBaseRow{"NewRow"};	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMale{true};
	
	// For Mesh Data rows
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EShibMesh,FName> MeshAssetRowNameData;

	// For Texture Data rows 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EShibTexture,FName> ShibTextureAssetRowNameData;

	// For Accessory Data rows
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EShibAccessory,FName> AccessoryRowNameData;

	// For Tattoo Data rows
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EShibTattoo,FName> TattooRowNameData;

	// For Morph values
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EShibBodyMorphs,double> MorphData;

	// For color values
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EShibMesh,FLinearColor> ShibMeshColorData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EShibTexture,FLinearColor> ShibTextureColorData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EShibAccessory,FLinearColor> AccessoryShibColorData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EShibTattoo,FLinearColor> TattooShibColorData;

	bool operator==(const FAvatarSave& Other) const
	{
		return AvatarBaseRow == Other.AvatarBaseRow &&
			   bIsMale == Other.bIsMale &&
			   MeshAssetRowNameData.OrderIndependentCompareEqual(Other.MeshAssetRowNameData) &&
			   ShibTextureAssetRowNameData.OrderIndependentCompareEqual(Other.ShibTextureAssetRowNameData) &&
			   AccessoryRowNameData.OrderIndependentCompareEqual(Other.AccessoryRowNameData) &&
			   TattooRowNameData.OrderIndependentCompareEqual(Other.TattooRowNameData) &&
			   MorphData.OrderIndependentCompareEqual(Other.MorphData) &&
			   ShibMeshColorData.OrderIndependentCompareEqual(Other.ShibMeshColorData) &&
			   ShibTextureColorData.OrderIndependentCompareEqual(Other.ShibTextureColorData) &&
			   AccessoryShibColorData.OrderIndependentCompareEqual(Other.AccessoryShibColorData) &&
			   TattooShibColorData.OrderIndependentCompareEqual(Other.TattooShibColorData);
	}

	FAvatarSave()
	{
		// Set default values for body morphs
		for (auto Morphs : TEnumRange<EShibBodyMorphs>())
		{
			MorphData.Add(Morphs,0.5);
		}
	}
	
	void PrintData() const
	{
		// Convert Saved Struct to Json String
		FString JsonString;
		if(!FJsonObjectConverter::UStructToJsonObjectString(*this, JsonString))
		{
			UE_LOG(LogShib, Warning, TEXT("%hs - Could not convert data to string"), __FUNCTION__);
			return;
		}

		UE_LOG(LogShib, Log, TEXT("%hs - Save Data = %s"), __FUNCTION__,*JsonString);
	}
	
};

UCLASS()
class UShibSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	static inline const FString& AvatarCustomizationSlot{"AvatarCustomizationSlot"};

	// Dont save anything before the avatar has tried to load to prevent over writting values on subsystem initialization
	UPROPERTY(BlueprintReadOnly,Transient)
	bool bTriedToLoad{false};

	// Save to disk 
	void SaveCurrentAvatarData();
	
	// Json Save/Load //
	void SaveCurrentAvatarDataToJson() const;

	// Loads Data from the JSON saved in SavedDir (This is done by calling the "SaveCurrentAvatarToJson")
	bool LoadCurrentAvatarDataFromJson();

	// Loads Data from the Json Object supplied
	bool LoadCurrentAvatarDataFromJson(TSharedRef<FJsonObject> JsonObject);

	// Loads Data from the Json String supplied
	bool LoadCurrentAvatarDataFromJson(const FString& JsonString);
	
	static bool CheckJsonSaveGameExists();

	static FString GetSaveJsonFilePath();

	
	
	UFUNCTION(BlueprintPure)
	FAvatarSave& GetCurrentAvatarData();

	void SetCurrentAvatarData(const FAvatarSave Data);
	
	// Set Save game cache //
	UFUNCTION()
	void SetBaseDataRowName(FName RowName,EGender inGender);
	
	UFUNCTION()
	void SetSavedMeshAssetRowName(EShibMesh Type,FName RowName);

	UFUNCTION(BlueprintCallable)
	void SetSavedShibTextureAssetRowName(EShibTexture Type, FName RowName);

	UFUNCTION()
	void SetSavedAccessoryRowName(EShibAccessory Type,FName RowName);
	
	UFUNCTION()
	void SetSavedTattooRowName(EShibTattoo Type, FName RowName);

	UFUNCTION()
	void SetMorphData(EShibBodyMorphs Type,double val);

	UFUNCTION(BlueprintCallable)
	void SetShibMeshColorData(EShibMesh Type,FLinearColor val);

	UFUNCTION(BlueprintCallable)
	void SetShibTextureColorData(EShibTexture Type,FLinearColor val);
	
	UFUNCTION(BlueprintCallable)
	void SetAccessoryShibColorData(EShibAccessory Type, FLinearColor val);

	UFUNCTION(BlueprintCallable)
	void SetTattooShibColorData(EShibTattoo Type, FLinearColor val);

	// Removers // 
	UFUNCTION(BlueprintCallable)
	void RemoveSavedMeshData(EShibMesh Type);

	UFUNCTION(BlueprintCallable)
	void RemoveSavedShibTextureData(EShibTexture Type);
	
	UFUNCTION(BlueprintCallable)
	void RemoveSavedAccessoryData(EShibAccessory Type);
	
	UFUNCTION(BlueprintCallable)
	void RemoveSavedTattooData(EShibTattoo Type);
	
	// Get Loaded Save Game data //
	

	UFUNCTION(BlueprintCallable)
	bool GetSavedBaseAssetData(FBase& OutData) const;
	bool GetSavedBaseAssetData( const FAvatarSave& SaveData,FBase& OutData) const;

	UFUNCTION(BlueprintCallable)
	bool GetSavedMeshAssetData(EShibMesh Type, FMesh& OutMesh) const;
	bool GetSavedMeshAssetData(const FAvatarSave& SaveData,EShibMesh Type, FMesh& OutMesh) const;

	UFUNCTION(BlueprintCallable)
	bool GetSavedShibTextureAssetData(EShibTexture Type, FShibTexture& OutShibTex) const;
	bool GetSavedShibTextureAssetData(const FAvatarSave& SaveData,EShibTexture Type, FShibTexture& OutShibTex) const;

	UFUNCTION(BlueprintCallable)
	bool GetSavedAccessoryMeshData(EShibAccessory Type, FMesh& OutMesh) const;
	bool GetSavedAccessoryMeshData(const FAvatarSave& SaveData,EShibAccessory Type, FMesh& OutMesh) const;


	UFUNCTION(BlueprintCallable)
	bool GetSavedTattooTextureData(EShibTattoo Type, FShibTexture& OutShibTex) const;
	bool GetSavedTattooTextureData(const FAvatarSave& SaveData,EShibTattoo Type, FShibTexture& OutShibTex) const;
	


	UFUNCTION(BlueprintCallable)
	double GetSavedMorphData(EShibBodyMorphs Type) const;
	double GetSavedMorphData(const FAvatarSave& SaveData,EShibBodyMorphs Type) const;

	
	UFUNCTION(BlueprintCallable)
	bool GetSavedShibMeshColorData(EShibMesh Type, FLinearColor& OutColor) const;
	bool GetSavedShibMeshColorData(const FAvatarSave& SaveData,EShibMesh Type, FLinearColor& OutColor) const;

	
	UFUNCTION(BlueprintCallable)
	bool GetSavedShibTextureColorData(EShibTexture Type, FLinearColor& OutColor) const;
	bool GetSavedShibTextureColorData(const FAvatarSave& SaveData,EShibTexture Type, FLinearColor& OutColor) const;

	
	UFUNCTION(BlueprintCallable)
	bool GetSavedAccessoryColorData(EShibAccessory Type, FLinearColor& OutColor) const;
	bool GetSavedAccessoryColorData(const FAvatarSave& SaveData,EShibAccessory Type, FLinearColor& OutColor) const;

	UFUNCTION(BlueprintCallable)
	bool GetSavedTattooColorData(EShibTattoo Type, FLinearColor& OutColor) const;
	bool GetSavedTattooColorData(const FAvatarSave& SaveData,EShibTattoo Type, FLinearColor& OutColor) const;
	
	void PrintCurrentSaveGame();

	
	UFUNCTION()
	void SetShibBuilder(const UShibAvatarBuilderSubsystem* Builder);


	
private:

	
	
	// Save Data 
	UPROPERTY()
	FAvatarSave AvatarSaveData;


	// Dont need to save 
	UPROPERTY(Transient)
	const UShibAvatarBuilderSubsystem* ShibBuilder;
	
	
	// Helper //
	UDataTable* LoadDataTable( const FString& Path);
	
	
};