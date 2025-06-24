// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "ShibLibrary.h"

#include "ShibAvatarCharacter.generated.h"

class UUISubsystem;

UCLASS(MinimalAPI)
class AShibAvatarCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AShibAvatarCharacter();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USceneCaptureComponent2D* GetCloseFar() const
	{
		return CloseFar;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USceneCaptureComponent2D* GetCloseUp() const
	{
		return CloseUp;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetBody() const
	{
		return Body;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetBody(USkeletalMesh* NewMesh = nullptr) const
	{
		Body->SetSkeletalMeshAsset(NewMesh);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetShoes() const
	{
		return Shoes;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetShoes(USkeletalMesh* NewShoes = nullptr) const
	{
		Shoes->SetSkeletalMeshAsset(NewShoes);
		//UpperUndergarment->SetVisibility(true);
	}
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetBottoms() const
	{
		return Bottoms;
	}
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetBottom(USkeletalMesh* NewBottom = nullptr) const
	{
		Bottoms->SetSkeletalMeshAsset(NewBottom);
		//UpperUndergarment->SetVisibility(false);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetHair() const
	{
		return Hair;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetHair(USkeletalMesh* NewHair = nullptr) const
	{
		Hair->SetSkeletalMeshAsset(NewHair);
		//UpperUndergarment->SetVisibility(true);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetTop() const
	{
		return Top;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetTop(USkeletalMesh* NewTop = nullptr) const
	{
		Top->SetSkeletalMeshAsset(NewTop);
		//UpperUndergarment->SetVisibility(true);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetFullBody() const
	{
		return FullBody;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetUpperUndergarment() const
	{
		return UpperUndergarment;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetLowerUndergarment() const
	{
		return LowerUndergarment;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetFullBody(USkeletalMesh* NewFullBody = nullptr) const
	{
		FullBody->SetSkeletalMeshAsset(NewFullBody);
		//UpperUndergarment->SetVisibility(false);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetLeftArm(USkeletalMesh* NewArmMesh = nullptr) const
	{
		LeftArm->SetSkeletalMeshAsset(NewArmMesh);
		//UpperUndergarment->SetVisibility(true);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetRightArm(USkeletalMesh* NewArmMesh = nullptr) const
	{
		RightArm->SetSkeletalMeshAsset(NewArmMesh);
		//UpperUndergarment->SetVisibility(true);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetBack(USkeletalMesh* NewBackMesh = nullptr) const
	{
		Back->SetSkeletalMeshAsset(NewBackMesh);
		//UpperUndergarment->SetVisibility(true);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetHeadTop(USkeletalMesh* NewHeadTop = nullptr) const
	{
		HeadTop->SetSkeletalMeshAsset(NewHeadTop);
		FullBody->SetSkeletalMesh(nullptr);
		Hair->SetSkeletalMesh(nullptr);
		//UpperUndergarment->SetVisibility(true);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetNeck(USkeletalMesh* NewNeckMesh = nullptr) const
	{
		Neck->SetSkeletalMeshAsset(NewNeckMesh);
		//UpperUndergarment->SetVisibility(true);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetFace(USkeletalMesh* NewFaceMesh = nullptr) const
	{
		Face->SetSkeletalMeshAsset(NewFaceMesh);
		//UpperUndergarment->SetVisibility(true);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetEars(USkeletalMesh* NewEarsMesh = nullptr) const
	{
		Ears->SetSkeletalMeshAsset(NewEarsMesh);
		//UpperUndergarment->SetVisibility(true);
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetLeftArm() const { return LeftArm; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetRightArm() const { return RightArm; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetBack() const { return Back; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetHeadTop() const { return HeadTop; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetNeck() const { return Neck; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetFace() const { return Face; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetEars() const { return Ears; }

	//UFUNCTION(BlueprintCallable)
	//const FCharacterMorphData& GetCharacterMorphData() const { return CharacterMorphData; }

	//UFUNCTION(BlueprintCallable)
	//void SetCharacterMorphData(const FCharacterMorphData& InCharacterData) { CharacterMorphData = InCharacterData; }

	UFUNCTION(BlueprintCallable)
	UUISubsystem* GetUISubsystem() const { return UISubsystem; }

	UFUNCTION(BlueprintCallable)
	void SetUISubsystem(UUISubsystem* NewSubsystem) { UISubsystem = NewSubsystem; }
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetBodyDMI();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetEyesDMI();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetLeftArmDMI();

	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetRightArmDMI();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetBackDMI();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetHeadTopDMI();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetNeckDMI();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetFaceDMI();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetEarsDMI();

	// TEMP fix for SKMs with multiple materials
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetFullBodyTopDMI();
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetFullBodyBottomDMI();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetShoesDMI();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetBottomsDMI();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetHairDMI();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetTopDMI();

	UFUNCTION(BlueprintCallable)
	void ResetAvatar();

	float MontagePosition;
	void SetCurrentMontagePosition();
	void PlayMontageFromCurrentPosition(UAnimationAsset* AnimToPlay);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UShibAvatarDataAsset* AvatarDataAsset;
protected:
	

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	TObjectPtr<UUISubsystem> UISubsystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneCaptureComponent2D> CloseFar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneCaptureComponent2D> CloseUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> LeftArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> RightArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HeadTop;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Neck;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Face;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Ears;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FullBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> UpperUndergarment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> LowerUndergarment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Shoes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Bottoms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Hair;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Top;

	
	//UPROPERTY()
	//FCharacterMorphData CharacterMorphData;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> BodyDMI;

private:

	UPROPERTY()
	TObjectPtr<UMaterialInterface> DefaultBodyMat;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> DefaultEyeMat;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> EyesDMI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> LeftArmDMI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> RightArmDMI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> BackDMI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> HeadTopDMI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> NeckDMI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> FaceDMI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> EarsDMI;

	// TEMP fix for SKMs with multiple materials
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> FullBodyTopDMI;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> FullBodyBottomDMI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> ShoesDMI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> BottomsDMI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> HairDMI;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstanceDynamic> TopDMI;
	
};