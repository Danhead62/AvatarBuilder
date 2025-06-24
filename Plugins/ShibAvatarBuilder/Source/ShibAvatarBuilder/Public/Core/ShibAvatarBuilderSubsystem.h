// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShibLibrary.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "I_PlayerInterractions.h"
#include "AvatarCommand.h"
#include <type_traits>
#include "ShibAnimInstance.h"
#include "ShibSaveGame.h"

#include "WorldPartition/ContentBundle/ContentBundleLog.h"
#include "ShibAvatarBuilderSubsystem.generated.h"

#define COPY_DYN_MATERIAL_PARAMETER(OldMaterial, NewMaterial, Param, ParamType) \
	NewMaterial->Set##ParamType##ParameterValue(FName(Param), OldMaterial->K2_Get##ParamType##ParameterValue(FName(Param)));


struct FAvatarSave;
enum class ELoadAvatarType : uint8;
class UAvatarHistory;
class USkeletalMeshComponent;
class AShibController;
class AShibAvatarCharacter;
class IAvatarCommand;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAvatarLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGenderSwapped);

/**
* 
*/
UCLASS(MinimalAPI)
class UShibAvatarBuilderSubsystem : public UGameInstanceSubsystem, public II_PlayerInterractions
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void Init();

	// Shib Controller //
	UFUNCTION(BlueprintCallable)
	void SetShibController(AShibController* InShibController) { ShibController = InShibController; }
	UFUNCTION(BlueprintCallable)
	const AShibController* GetShibController() const { return ShibController; }

	

	// Shib Avatar
	UFUNCTION(BlueprintPure)
	AShibAvatarCharacter* GetAvatarCharacter();
	
	// Save Game //

	UFUNCTION(BlueprintCallable)
	void SaveAvatar();

	// Tries to Load Avatar from Json File in SaveDir 
	UFUNCTION(BlueprintCallable)
	void LoadAvatarFromJsonFile();

	// Tries to Load avatar from json string passed
	UFUNCTION(BlueprintCallable)
	void LoadAvatarFromJsonString(const FString& JsonString);

	UFUNCTION(BlueprintCallable)
	void LoadAvatarData(FAvatarSave AvatarData);
	
	UFUNCTION(BlueprintCallable)
	void SetShibSaveGame(UShibSaveGame* InShibSaveGame) { ShibSaveGame = InShibSaveGame; }
	
	UFUNCTION(BlueprintCallable)
	const UShibSaveGame* GetShibSaveGame() const { return ShibSaveGame; }

	// Gender // 
	UFUNCTION(BlueprintCallable)
	const EGender GetGender() const { return Gender; }

	UFUNCTION(BlueprintCallable)
	void SetGender(EGender NewGender) { Gender = NewGender; }

	UFUNCTION(BlueprintCallable)
	void SwapGender(const FBase& Base = FBase(), EGender InGender = EGender::Male);
	
	// Thumbnails //
	// Gets thumbnails from FMesh and FShibTexture Datatables, (except for Accessories and Tattoos since you need to pass in the type)  
	UFUNCTION(BlueprintCallable)
	const FCustomButtonUIData  GetThumbnails(const UDataTable* Table, EGender MyGender = EGender::Male);

	// Changes the bodies skeletal mesh based on the applied meshes
	UFUNCTION(BlueprintCallable)
	void UpdateSkmBasedOnMesh();
	
	UFUNCTION(BlueprintCallable)
	const FCustomButtonUIData  GetPresetThumbnails(EGender MyGender = EGender::Male);
	
	UFUNCTION(BlueprintCallable)
	const FCustomButtonUIData GetAccessoryThumbnails(const UDataTable* Table, EShibAccessory AccessoryType = EShibAccessory::NONE, EGender MyGender = EGender::Male);

	UFUNCTION(BlueprintCallable)
	const FCustomButtonUIData GetTattooThumbnails(const UDataTable* Table, EShibTattoo TattooType = EShibTattoo::NONE, EGender MyGender = EGender::Male);

	// Applies Facial Morph Data (Nose/Jaw/Mouth - Height/Width/Length based on type passed)
	UFUNCTION(BlueprintCallable)
	void ApplyBodyMorphData(EShibBodyMorphs Type, const FBase& Base = FBase(), const float& Value = 0.5f, const bool& bFinal = false);

	

	// Body Meshes
	UFUNCTION(BlueprintCallable)
	void ApplyNewBodyMesh(EShibMesh Type, const FMesh& Mesh = FMesh());
	
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetMeshHolders(EShibMesh Type);

	// Accessories
	UFUNCTION(BlueprintCallable)
	void ApplyNewAccessory(const FMesh& Mesh = FMesh());
	
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetAccessoryHolder(EShibAccessory Type);

	
	// Applies ShibTexture
	UFUNCTION(BlueprintCallable)
	void ApplyNewMaterialValue(const FShibTexture& Texture = FShibTexture(), bool bFinal = false);

	// Applies Shib Color
	UFUNCTION(BlueprintCallable)
	void ApplyNewColor(USkeletalMeshComponent* Holder, const FShibColor& Texture, const FLinearColor& Color, bool bFinal);

	// Randomization
	UFUNCTION(BlueprintCallable)
	void BuildRandomAvatar();


	// Undo/Redo //
	
	UFUNCTION(BlueprintCallable)
	void Undo();
	
	UFUNCTION(BlueprintCallable)
	void Redo();



	float PrevHeightValue{};
	

	// Delegates // 
	UPROPERTY(BlueprintAssignable)
	FOnTextureSelect OnTextureSelect;

	UPROPERTY(BlueprintAssignable)
	FOnMeshSelect OnMeshSelect;

	UPROPERTY(BlueprintAssignable)
	FOnGameReady OnGameReady;

	UPROPERTY(BlueprintAssignable)
	FOnAvatarLoaded OnAvatarLoaded;
	UPROPERTY(BlueprintAssignable)
	FOnAvatarLoaded OnGenderSwapped;

	
	UPROPERTY(BlueprintReadOnly)
	UShibAvatarDataAsset* AvatarDataAsset;

	UFUNCTION(BlueprintCallable)
	void SetupUndergarments();

	UFUNCTION(BlueprintPure)
	bool CheckAvatarChanged();

private:
	
	UPROPERTY()
	UAvatarHistory* AvatarHistory;

	
	UPROPERTY()
	TObjectPtr<AShibController> ShibController;


	UPROPERTY()
	AShibAvatarCharacter* AvatarCharacter;
	
	UPROPERTY()
	TObjectPtr<UShibSaveGame> ShibSaveGame;

	// Value should only be set once when avatar is first loaded in 
	UPROPERTY()
	FAvatarSave InitialAvatarSaveData;
	
	UPROPERTY()
	EGender Gender{EGender::Male};
	
	UPROPERTY()
	TObjectPtr<UTexture> TattooForColorChange;

	// Body Morphs
	void UpdateMeshesBasedOnMorphWeight(const float Weight);
	void ApplyNewWeight(const FBase& Base = FBase(), const float& Weight = 0.5f, const bool& bFinal = false);
	

	
	void ApplyNewHeight(float Value, bool bFinal);
	
	TArray<FName> GetFacialMorphNames(const FBase& Base = FBase(), EShibBodyMorphs Type = EShibBodyMorphs::NONE);
	
	// Body Meshes
	void RemoveBodyMesh(EShibMesh Type);
	void RemoveConflicts(const FMesh& Mesh);
	bool CheckConflicts(const FMesh& Mesh);
	
	// Accessory
	void RemoveAccessoryMesh(EShibAccessory Type);

	void ApplyPresetMI(const FShibTexture& Texture);

	
	void GetFirstMeshVariation(FMesh& Mesh);

	void ManageLowerBodySense(const FMesh& Mesh);

	void ApplyRandomMeshWithColor(EShibMesh MeshType);

	void ApplyRandomAccessory(const TArray<FName>& Accessories);
	void ApplyRandomBaseMorphs(const TArray<FName>& Bases);
	void ApplyRandomTattoo(const TArray<FName>& Tattoos);
	void ApplyRandomTextures();

	FLinearColor GenerateRandomColor(float MinColorRange = 0.1f, float MaxColorRange = 1.0f)
	{
		return FLinearColor(
			FMath::RandRange(MinColorRange, MaxColorRange), // Red
			FMath::RandRange(MinColorRange, MaxColorRange), // Green
			FMath::RandRange(MinColorRange, MaxColorRange), // Blue
			1.0f // Alpha (fully opaque)
		);
	}

public:
	// For Commands
	UFUNCTION()
	static void SetHeight(AShibAvatarCharacter* Character, float Value);

	UFUNCTION()
	static void ApplyNewTexture(UMaterialInstanceDynamic* Material, const TArray<FName> Parameters, const TArray<UTexture*>& Textures);

	UFUNCTION()
	static void ApplyNewMorphs(USkeletalMeshComponent* Component, USkeletalMesh* Mesh, const TArray<FName> Morph, const float& Value);

	UFUNCTION()
	static void ApplyNewMesh(USkeletalMeshComponent* Holder, USkeletalMesh* NewMesh);

	// make sure the body DMI is already set before calling this function so we can copy from it
	UFUNCTION()
	static void SetBodyDynMatInstAndCopyParams(const AShibAvatarCharacter* InAvatarCharacter, UMaterialInterface* NewMaterial);
};


template<typename THolder>
inline void SetHolderVisibilityDependOnType(EShibMesh Type, THolder* Holder, const bool& bIsVisible = true)
{
}

template<typename TStruct>
inline bool IsMatchingID(UDataTable* DataTable, const int32& CurrentMeshID, EGender InGender, TStruct& OutData)
{
	if (!DataTable) return false;
	LOG_SHIB(Warning, "[%s] is valid!", *DataTable->GetName());
	const TArray<FName> Rows = DataTable->GetRowNames();
	for (const auto& Row : Rows)
	{
		const TStruct* Data = DataTable->FindRow<TStruct>(Row, TEXT(""));
		if (Data && Data->Gender == InGender && Data->MeshID == CurrentMeshID)
		{
			OutData = *Data;
			LOG_SHIB(Warning, "Mesh [%s]", *Data->Mesh->GetName());
			return true;
		}
	}

	return false;
}

template<typename TStruct, typename TEnum>
inline bool IsMatchingIDForAllType(UDataTable* DataTable, const int32& CurrentMeshID, EGender InGender, TStruct& OutData = TStruct(), TEnum Type = TEnum())
{
	if (!DataTable) return false;
	LOG_SHIB(Log, "[%s] is valid!", *DataTable->GetName());
	const TArray<FName> Rows = DataTable->GetRowNames();
	for (const auto& Row : Rows)
	{
		const TStruct* Data = DataTable->FindRow<TStruct>(Row, TEXT(""));
		if(Data && Data->Mesh)
		{
			if (Data->bAccessory)
			{
				if ( Data->Gender == InGender && Data->MeshID == CurrentMeshID && Data->AccessoryType == Type)
				{
					OutData = *Data;
					LOG_SHIB(Log, "Mesh [%s]", *Data->Mesh->GetName());
					return true;
				}
			}
			else
			{
			
				if ( Data->Gender == InGender && Data->MeshID == CurrentMeshID)
				{
					OutData = *Data;
					LOG_SHIB(Log, "Mesh [%s]", *Data->Mesh->GetName());
					return true;
				}
			}
		}
		
	}

	return false;
}

template<typename T>
inline UMaterialInstanceDynamic* GetMaterialParameterValue(const T& Parameter, FLinearColor& OutColor, const int32 Index, USkeletalMeshComponent* Holder)
{
	if (!Holder)return nullptr;
	UMaterialInstanceDynamic* Matrial = Holder->CreateDynamicMaterialInstance(Index, Holder->GetMaterial(Index));
	if (Matrial)
	{
		FHashedMaterialParameterInfo Info(Parameter, EMaterialParameterAssociation::GlobalParameter);
		if (Matrial->GetVectorParameterValue(Info, OutColor))
		{
			LOG_SHIB(Warning, "Material [%s] for [%s]", *Matrial->GetName(), *Holder->GetName());
			return Matrial;
		}
		else
		{
			OutColor = FLinearColor::Black;
			return nullptr;
		}
	}
	return nullptr;
}