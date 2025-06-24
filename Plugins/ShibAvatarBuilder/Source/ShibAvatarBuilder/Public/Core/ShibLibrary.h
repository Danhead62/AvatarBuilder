// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "ShibLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogShib, Log, All);

#define LOG_SHIB(Verbosity, Format, ...) \
{ \
	UE_LOG(LogShib, Verbosity, TEXT("[%s(%d)]: " Format), TEXT(__FUNCTION__), __LINE__, ##__VA_ARGS__) \
}

class UButton;
class UCanvasPanel;
class UVerticalBox;
class UWidgetAnimation;
class UShibAnimInstance;

// Used to create checks where pieces of code will execute once unless the struct object has explicitly been reset.
struct FDoOnlyOnce
{
public:
	FORCEINLINE FDoOnlyOnce() : bDoOnce(false) { }
	explicit FORCEINLINE FDoOnlyOnce(const bool& bStartClosed) : bDoOnce(bStartClosed) { }

	// Reset to allow the function to run again
	FORCEINLINE void Reset()
	{
		bDoOnce = false;
		LOG_SHIB(Log, "DoOnce resets!");
	}

	// Execute the function only once
	FORCEINLINE bool Execute()
	{
		if (!bDoOnce)
		{
			bDoOnce = true; // Set to true after first execution
			return true;
		}
		return false;
	}

private:
	bool bDoOnce;
};


// Enum classes //


UENUM(Blueprintable)
enum class EShibMesh : uint8
{
	NONE,
	Top,
	Bottom,
	Shoe,
	Hair,
	FullBody,
	Accessory,
	UpperUndergarment,
	LowerUndergarment,
};
ENUM_RANGE_BY_FIRST_AND_LAST(EShibMesh, EShibMesh::NONE, EShibMesh::LowerUndergarment);

UENUM(Blueprintable)
enum class EShibTexture : uint8
{
	NONE,
	Skin UMETA(DisplayName = "Skin"),
	Preset UMETA(DisplayName = "Preset"),
	EyebrowShape UMETA(DisplayName = "Eyebrow Shape"),
	EyeShape UMETA(DisplayName = "Eye Shape"),
};
ENUM_RANGE_BY_FIRST_AND_LAST(EShibTexture, EShibTexture::NONE, EShibTexture::EyebrowShape);

UENUM(BlueprintType)
enum class EShibBodyMorphs : uint8
{
	NONE,
	AvatarHeight UMETA(DisplayName = "Avatar Height"),
	AvatarWeight UMETA(DisplayName = "Avatar Weight"),
	
	JawLength UMETA(DisplayName = "Jaw Length"),
	JawWidth UMETA(DisplayName = "Jaw Width"),
	JawHeight UMETA(DisplayName = "Jaw Height"),

	NoseLength UMETA(DisplayName = "Nose Length"),
	NoseWidth UMETA(DisplayName = "Nose Width"),
	NoseHeight UMETA(DisplayName = "Nose Height"),
	
	MouthThickness UMETA(DisplayName = "Mouth Thickness"),
	MouthWidth UMETA(DisplayName = "Mouth Width"),
	MouthHeight UMETA(DisplayName = "Mouth Height"),

	AccessoriesNoseLength UMETA(DisplayName = "Accessories Nose Length"),
	AccessoriesNoseWidth UMETA(DisplayName = "Accessories Nose Width"),
	AccessoriesNoseHeight UMETA(DisplayName = "Accessories Nose Height"),
};
ENUM_RANGE_BY_FIRST_AND_LAST(EShibBodyMorphs, EShibBodyMorphs::NONE , EShibBodyMorphs::AccessoriesNoseHeight);


UENUM(Blueprintable)
enum class EShibAccessory : uint8
{
	NONE,
	HeadTop,
	Face,
	Ears,
	Back,
	Arm,
	LeftArm,
	RightArm,
	Neck,
};
ENUM_RANGE_BY_FIRST_AND_LAST(EShibAccessory, EShibAccessory::NONE , EShibAccessory::Neck);

UENUM(Blueprintable)
enum class EShibTattoo : uint8
{
	NONE,
	Head,
	Chest,
	Back,
	LeftArm,
	RightArm,
	Legs,
	Hands,
};
ENUM_RANGE_BY_FIRST_AND_LAST(EShibTattoo, EShibTattoo::NONE , EShibTattoo::Hands);


UENUM(Blueprintable)
enum class EGender : uint8
{
	NONE,
	Male,
	Female,
	MAX UMETA(Hidden),
};

UENUM(Blueprintable)
enum class EGridType : uint8
{
	NONE,
	RowOriented,
	ColumnOriented,
	MAX UMETA(Hidden),
};

UENUM(Blueprintable)
enum class EFullBodySubType : uint8
{
	NONE,
	Short UMETA(DisplayName = "Short"),
	Long UMETA(DisplayName = "Long"),
	MAX UMETA(Hidden),
};


UENUM(Blueprintable)
enum class EBottomSubType : uint8
{
	NONE,
	Short UMETA(DisplayName = "Short"),
	Capris UMETA(DisplayName = "Capris"),
	LongWithTucked UMETA(DisplayName = "LongWithTucked"),
	LongWithoutTucked UMETA(DisplayName = "LongWithoutTucked"),
	MAX UMETA(Hidden),
};

UENUM(Blueprintable)
enum class EShoeSubType : uint8
{
	NONE,
	Short UMETA(DisplayName = "Short"),
	Long UMETA(DisplayName = "Long"),
	MAX UMETA(Hidden),
};

USTRUCT(BlueprintType)
struct FWeightData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<USkeletalMesh>> WeightBaseMeshs;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<USkeletalMesh>> WeightTopMeshs;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<USkeletalMesh>> WeightBottomMeshs;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<USkeletalMesh>> WeightFullbodyMeshs;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<USkeletalMesh>> WeightShoeMeshs;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<USkeletalMesh>> WeightAccessoryMeshs;
};

// Structs //

USTRUCT(BlueprintType)
struct FMorphData
{
	GENERATED_BODY()

	/*FMorphData& operator=(const FMorphData& Other)
	{
		if (this != &Other)
		{
			BodyMorphs = Other.BodyMorphs;
			ArmMorphs = Other.ArmMorphs;
			BackMorphs = Other.BackMorphs;
			HeadTopMorphs = Other.HeadTopMorphs;
			NeckMorphs = Other.NeckMorphs;
			FaceMorphs = Other.FaceMorphs;
			EarsMorphs = Other.EarsMorphs;
			HeadMorphs = Other.HeadMorphs;
			PantMorphs = Other.PantMorphs;
			ShirtMorphs = Other.ShirtMorphs;
			FullBodyMorphs = Other.FullBodyMorphs;
			UpperUndergarmentMorphs = Other.UpperUndergarmentMorphs;
			LowerUndergarmentMorphs = Other.LowerUndergarmentMorphs;
			FootWearMorphs = Other.FootWearMorphs;
			NoseHeightMorphs = Other.NoseHeightMorphs;
			NoseLengthMorphs = Other.NoseLengthMorphs;
			NoseWidthMorphs = Other.NoseWidthMorphs;
			JawLengthMorphs = Other.JawLengthMorphs;
			JawWidthMorphs = Other.JawWidthMorphs;
			JawHeightMorphs = Other.JawHeightMorphs;
			MouthLengthMorphs = Other.MouthLengthMorphs;
			MouthWidthMorphs = Other.MouthWidthMorphs;
			MouthHeightMorphs = Other.MouthHeightMorphs;
			AccessoriesNoseLengthMorphs = Other.AccessoriesNoseLengthMorphs;
			AccessoriesNoseHeightMorphs = Other.AccessoriesNoseHeightMorphs;
			AccessoriesNoseWidthMorphs = Other.AccessoriesNoseWidthMorphs;

		}
		
		return *this;
	}*/
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Upper Body Morphs Group")
	TArray<FName> BodyMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Upper Body Morphs Group")
	TArray<FName> ArmMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Upper Body Morphs Group")
	TArray<FName> BackMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> HeadTopMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> NeckMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> FaceMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> EarsMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> HeadMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Lower Body Morphs Group")
	TArray<FName> PantMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Lower Body Morphs Group")
	TArray<FName> FootWearMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Upper Body Morphs Group")
	TArray<FName> ShirtMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Upper Body Morphs Group")
	TArray<FName> FullBodyMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Upper Body Morphs Group")
	TArray<FName> UpperUndergarmentMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Lower Body Morphs Group")
	TArray<FName> LowerUndergarmentMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> NoseLengthMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> NoseWidthMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> NoseHeightMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> JawLengthMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> JawWidthMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> JawHeightMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> MouthLengthMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> MouthWidthMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Head Morphs Group")
	TArray<FName> MouthHeightMorphs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Accessories Morphs Group")
	TArray<FName> AccessoriesNoseLengthMorphs;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Accessories Morphs Group")
	TArray<FName> AccessoriesNoseWidthMorphs;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Accessories Morphs Group")
	TArray<FName> AccessoriesNoseHeightMorphs;
};


USTRUCT(BlueprintType)
struct FBase : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGender Gender{ EGender::NONE };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MeshID{ 0 };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USkeletalMesh> Mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeightData WeightData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimMontage> AnimMontage;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<USkeletalMesh> UpperUndergarment;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<USkeletalMesh> lowerUndergarment;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FMorphData MorphData;
	
};

USTRUCT(BlueprintType)
struct FMeshVariations
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USkeletalMesh> MeshVariation{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<USkeletalMesh>> WeightMeshVariation{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EBottomSubType BottomSubType { EBottomSubType::NONE };
};

USTRUCT(BlueprintType)
struct FMeshTypeCompatibility
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EShibMesh MeshType{ EShibMesh::NONE };

	// UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "MeshType==EShibMesh::Bottom", EditConditionHides))
	// EBottomSubType BottomSubType{ EBottomSubType::NONE };
	//
	// UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "MeshType==EShibMesh::Shoe", EditConditionHides))
	// EShoeSubType ShoeSubType{ EShoeSubType::NONE };
	//
	// UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "MeshType==EShibMesh::FullBody", EditConditionHides))
	// EFullBodySubType FullBodySubType{EFullBodySubType::NONE};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "MeshType==EShibMesh::Accessory", EditConditionHides))
	EShibAccessory AccessoryType{ EShibAccessory::NONE };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bReplaceExisting = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDataTableRowHandle> Excluding;
};

USTRUCT(BlueprintType)
struct FMesh : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bAllowHair{ true };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InlineEditConditionToggle))
	bool bMesh{ false };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bMesh"))
	EShibMesh MeshType{ EShibMesh::NONE };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "MeshType==EShibMesh::Bottom", EditConditionHides))
	EBottomSubType BottomSubType{ EBottomSubType::NONE };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "MeshType==EShibMesh::Shoe", EditConditionHides))
	EShoeSubType ShoeSubType{ EShoeSubType::NONE };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "MeshType==EShibMesh::FullBody", EditConditionHides))
	EFullBodySubType FullBodySubType{EFullBodySubType::NONE};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InlineEditConditionToggle))
	bool bAccessory{ false };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bAccessory"))
	EShibAccessory AccessoryType{ EShibAccessory::NONE };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FMeshTypeCompatibility> NonCompatibleTypes;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDataTableRowHandle> NonCompatible;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGender Gender{ EGender::NONE };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MeshID{ 99999999 };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture> Thumbnail{ nullptr };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USkeletalMesh> Mesh{ nullptr };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<USkeletalMesh>> WeightMesh{};

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	// Contains the different meshes for different weight/body index, from underweight to overweight
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<USkeletalMesh>> Meshes;
	
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InlineEditConditionToggle))
	bool bVariations{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bMesh"))
	TArray<FMeshVariations> MeshVariations;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> Morphs{};
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ColorParameter{ "" };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaterialIndex{ 0 };
};

USTRUCT(BlueprintType)
struct FShibAvatarSkeletalBodyVariations : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 VariationID{ 0 };
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGender Gender{ EGender::NONE };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<EShibMesh> MeshCombinations;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EBottomSubType BottomSubType{ EBottomSubType::NONE };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EFullBodySubType FullBodySubType{EFullBodySubType::NONE};
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USkeletalMesh> BodyVariation;
};


USTRUCT(BlueprintType)
struct FShibMaterialWithThumbnail
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UMaterialInstance> MaterialInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture> MaterialThumbnail;
};


USTRUCT(BlueprintType)
struct FCustomButtonUIData
{
		GENERATED_BODY()
    public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int> MeshIDs;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<UTexture>> Thumbnails;
};

USTRUCT(BlueprintType)
struct FShibTexture : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InlineEditConditionToggle))
	bool bTexture{ false };
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	EShibTexture TextureType{ EShibTexture::NONE };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bTexture"))
	EShibTattoo TattooType{ EShibTattoo::NONE };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGender Gender{ EGender::NONE };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TextureID{ 99999999 };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture> Thumbnail{ nullptr };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FShibMaterialWithThumbnail> MaterialAndThumbnailData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<UTexture>> Texture{ nullptr };
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FName> TextureParameter{ "" };
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FName> Morph{};
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> RemoveMorph{};
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ColorParameter{ "" };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaterialIndex{ 0 };
};

USTRUCT(BlueprintType)
struct FShibColor : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InlineEditConditionToggle))
	bool bAccessory{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bAccessory"))
	EShibAccessory AccessoryType{ EShibAccessory::NONE };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InlineEditConditionToggle))
	bool bTattoo{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bTattoo"))
	EShibTattoo TattooType{ EShibTattoo::NONE };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InlineEditConditionToggle))
	bool bMesh{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bMesh"))
	EShibMesh MeshType{ EShibMesh::NONE };
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bShibTexture"))
	EShibTexture TextureType{ EShibTexture::NONE };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGender Gender{ EGender::NONE };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ColorID{ 99999999 };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture> Thumbnail{ nullptr };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ColorParameter{ "" };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaterialIndex{ 0 };
};


// Need these to use for thumbnail data 
USTRUCT(BlueprintType)
struct FUITattoo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TObjectPtr<UTexture>> Tattoo;
};

USTRUCT(BlueprintType)
struct FUIAccessories
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<TObjectPtr<UTexture>> Accessories;
};

USTRUCT(BlueprintType)
struct FUIThumbnails : public FTableRowBase
{
	GENERATED_BODY()
public:
	FUIThumbnails() {  }

	FUIThumbnails& operator=(const FUIThumbnails& Other)
	{
		if (this != &Other)
		{
			BodyPresets = Other.BodyPresets;
			HairStyles = Other.HairStyles;
			PantsChoices = Other.PantsChoices;
			ShirtChoices = Other.ShirtChoices;
			FullBody = Other.FullBody;
			ShoeChoices = Other.ShoeChoices;
			Undergarments = Other.Undergarments;
			Tattoos = Other.Tattoos;
			Accessories = Other.Accessories;
		}
		return *this;
	}

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<FName, TObjectPtr<UTexture>> BodyPresets{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<TObjectPtr<UTexture>, int32> HairStyles{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<TObjectPtr<UTexture>, int32> PantsChoices{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<TObjectPtr<UTexture>, int32> ShirtChoices{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<TObjectPtr<UTexture>, int32> FullBody{};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<TObjectPtr<UTexture>, int32> Undergarments{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<TObjectPtr<UTexture>, int32> ShoeChoices{};
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<FString, FUITattoo> Tattoos{};
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<FString, FUIAccessories> Accessories{};
};



USTRUCT(BlueprintType)
struct FTattoo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString TattooParam;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString TattooColorParam;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TObjectPtr<UTexture>, TObjectPtr<UTexture>> Tattoo;
};

USTRUCT(BlueprintType)
struct FNose
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> NoseMorph;
};

USTRUCT(BlueprintType)
struct FJaw
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> JawMorph;
};

USTRUCT(BlueprintType)
struct FMouth
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> MouthMorph;
};

USTRUCT(BlueprintType)
struct FEyebrow
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture> Base{ nullptr };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture> Normal{ nullptr };
};

USTRUCT(BlueprintType)
struct FPreset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Preset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture> Skin;
};

USTRUCT(BlueprintType)
struct FBaseAccessories
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USkeletalMesh> Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName SocketName;
};

USTRUCT(BlueprintType)
struct FAccessories
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TObjectPtr<UTexture>, FBaseAccessories> Accessories;
};

USTRUCT(BlueprintType)
struct FSkin
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture> Base;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture> Normal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTexture> ARM;
};


USTRUCT(BlueprintType)
struct FAvatarControllerData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> AvatarActor;

	UPROPERTY(BlueprintReadWrite)
	float CameraRotationSpeed{ 2.0f };

	UPROPERTY(BlueprintReadWrite)
	float CameraZoomSpeed{ 15.0f };

	UPROPERTY(BlueprintReadWrite)
	bool bAllowCameraMovement{ true };

	UPROPERTY(BlueprintReadWrite)
	TArray<float> TargetDistances{ 300.0f,80.0f,40.0f,50.0f,40.0f,55.0f,55.0f,170.0f,160.0f,120.0f,220.0f,
		80.0f,80.0f,120.0f,120.0f,95.0f,95.0f,120.0f,120.0f,100.0f, 100.0f };

	// Changed this first index from FRotator(-10.0f,0.0f,0.0f) to FRotator(0.0f,0.0f,0.0f)
	UPROPERTY(BlueprintReadWrite)
	TArray<FRotator> TargetRotations{ FRotator(0.0f,0.0f,0.0f),FRotator(-7.0f,27.0f,0.0f),FRotator(8.0f,15.0f,0.0f), FRotator(4.0f,25.0f,0.0f),
		FRotator(2.0f,8.0f,0.0f), FRotator(-5.0f,15.0f,0.0f), FRotator(-5.0f,15.0f,0.0f), FRotator(-7.0f,27.0f,0.0f), FRotator(-7.0f,27.0f,0.0f),
		FRotator(-7.0f,27.0f,0.0f), FRotator(-5.0f,10.0f,0.0f), FRotator(-2.0f,10.0f,0.0f), FRotator(0.0f,10.0f,0.0f), FRotator(-2.0f,70.0f,0.0f),
		FRotator(-2.0f,-50.0f,0.0f), FRotator(0.0f,90.0f,0.0f), FRotator(0.0f,45.0f,0.0f), FRotator(-2.0f,60.0f,0.0f), FRotator(-2.0f,-20.0f,0.0f),
		FRotator(-2.0f,20.0f,0.0f), FRotator(-2.0f,-160.0,0.0f) };

	UPROPERTY(BlueprintReadWrite)
	TArray<float> TargetHeights{ 115.0f,182.0f,187.0f,182.0f,180.0f,175.0f,175.0f,140.0f,100.0f,
		60.0f,130.0f,182.0f,177.0f,160.0f,160.0f,130.0f,130.0f,100.0f,100.0f,135.0f,135.0f };

	UPROPERTY(BlueprintReadWrite)
	TArray<float> FemaleTargetHeights{ 115.0f,170.0f,175.0f,170.0f,168.0f,162.0f,162.0f,140.0f,100.0f,
		60.0f,115.0f,170.0f,165.0f,150.0f,150.0f,120.0f,120.0f,90.0f,90.0f,125.0f,125.0f };

	UPROPERTY(BlueprintReadWrite)
	float TargetHeightMultiplier{ 1.0f };

	UPROPERTY(BlueprintReadWrite)
	int32 TargetBodyPartIndex{ 0 };

	UPROPERTY(BlueprintReadWrite)
	float TargetHeight{ 0.0f };

	UPROPERTY(BlueprintReadWrite)
	bool bFocusPoint{ false };

	UPROPERTY(BlueprintReadWrite)
	float ZoomTarget{ 0.0f };

	UPROPERTY(BlueprintReadWrite)
	float MaxZoomOutPoint{ 400.0f };

	UPROPERTY(BlueprintReadWrite)
	float ZoomInReductionValue{ 2.0f };

	UPROPERTY(BlueprintReadWrite)
	float AvatarRotationSpeed{ -2.0f };

	UPROPERTY(BlueprintReadWrite)
	bool bEnableRightClick{ false };

	UPROPERTY(BlueprintReadWrite)
	bool bSetIsRotating{ false };

	UPROPERTY(BlueprintReadWrite)
	float CurrentAvatarRotation{ 0.0f };

	UPROPERTY(BlueprintReadWrite)
	float AvatarZScale{ 0.0f };
};

USTRUCT(BlueprintType)
struct FTattooHistory
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FString TattooType;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTexture> TattooTexture = nullptr;
};


USTRUCT(BlueprintType)
struct FHSV
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	float Hue{ 0.0f };
	UPROPERTY(BlueprintReadWrite)
	float Saturation{ 0.0f };
	UPROPERTY(BlueprintReadWrite)
	float Value{ 1.0f };

	static FLinearColor GetDefaultRGBValue()
	{
		return UKismetMathLibrary::HSVToRGB(0,0,1);
	}
};



UCLASS(BlueprintType)
class UShibAvatarDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", AssetRegistrySearchable)
	int AssetID{-1};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> Base;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> SkeletalBodyVariations;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> Top;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> Bottom;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> FullBody;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> Shoes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> Hair;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> Accessory;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> Tattoo;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> Skin;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> Preset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> Eyebrow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UDataTable> Undergarments;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Data Thumbnails")
	TObjectPtr<UDataTable> UIThumbnails;
	
	// Group data into maps //
	
	// TMap for DataTables of Type FMesh
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Grouped Tables")
	TMap<EShibMesh,TObjectPtr<UDataTable>> MeshAssetDataTables;

	//TMap of DataTables of Type FShibTexture
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Grouped Tables")
	TMap<EShibTexture,TObjectPtr<UDataTable>> ShibTextureAssetDataTables;
};


//USTRUCT(BlueprintType)
//struct FMeshVariationsTesting : public FTableRowBase
//{
//	GENERATED_BODY()
//public:
//	
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InlineEditConditionToggle))
//	bool bMesh{ false };
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bMesh"))
//	EShibMesh MeshType{ EShibMesh::NONE };
//
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	EGender Gender{ EGender::NONE };
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	int32 MeshID{ 99999999 };
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	TObjectPtr<UTexture> Thumbnail{ nullptr };
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	TObjectPtr<USkeletalMesh> Mesh{ nullptr };
//};


// UCLASS(Blueprintable)
// class UHistory : public UObject
// {
// 	GENERATED_BODY()
// public:
// 	static UHistory* Get();
// 	const FChanges& GetCurrentChanges() const;
// 	const FChanges& GetPreviousChanges() const;
// 	FChanges& GetChanges() { return CurrentChange; }
// 	FChanges& GetPrevious() { return PreviousChange; }
// 	void UpdateChange(const FChanges& NewChange);
//
// 	template <typename StructType, typename MemberType>
// 	void ModifyMember(StructType& StructInstance, MemberType StructType::* Member, const MemberType& NewValue = MemberType())
// 	{
// 		StructInstance.*Member = NewValue;
// 	}
//
// 	template <typename StructType, typename MemberType>
// 	MemberType& GetMember(StructType& StructInstance, MemberType StructType::* Member)
// 	{
// 		return StructInstance.*Member;
// 	}
// private:
// 	UHistory()
// 	{
// 	}
// 	static UHistory* Instance;
// 	FChanges CurrentChange;
// 	FChanges PreviousChange;
// };


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMeshSelect, const FMesh&, Mesh);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTextureSelect, const FShibTexture&, Texture);

UCLASS(MinimalAPI)
class UShibLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};

// Helpers
template<typename Key, typename Value>
Key GetRandomKeyFromMap(const TMap<Key, Value>& MyMap)
{
	
	// Extract keys into an array
	TArray<Key> Keys;
	MyMap.GetKeys(Keys);
	// Check if the map is not empty
	if (Keys.Num() > 0)
	{
		// Generate a random index
		int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, Keys.Num() - 1);
		// Return the key at the random index
		return Keys[RandomIndex];
	}
	return Key();
}

