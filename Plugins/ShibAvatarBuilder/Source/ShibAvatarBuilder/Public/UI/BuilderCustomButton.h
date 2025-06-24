// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "ShibLibrary.h"

#include "BuilderCustomButton.generated.h"

/**
* 
*/
UCLASS(MinimalAPI)
class UBuilderCustomButton : public UButton
{
	GENERATED_BODY()
public:
    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
    EButtonType ButtonType = EButtonType::NONE;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "ButtonType == EButtonType::BodySubMenu", EditConditionHides))
    EBodySubMenu BodySubMenu = EBodySubMenu::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "ButtonType == EButtonType::MainMenu", EditConditionHides))
    EMainMenu MainMenu = EMainMenu::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "ButtonType == EButtonType::HeadSubMenu", EditConditionHides))
    EHeadSubMenu HeadSubMenu = EHeadSubMenu::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "HeadSubMenu == EHeadSubMenu::Head", EditConditionHides))
    EHeadPreset HeadPreset = EHeadPreset::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "HeadSubMenu == EHeadSubMenu::Hair", EditConditionHides))
    EHairPreset HairPreset = EHairPreset::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "HeadSubMenu == EHeadSubMenu::Makeup", EditConditionHides))
    EMakeupPreset MakeupPreset = EMakeupPreset::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "HeadSubMenu == EHeadSubMenu::Tattoo", EditConditionHides))
    ETattooPreset TattooPreset = ETattooPreset::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "ButtonType == EButtonType::OutfitSubMenu", EditConditionHides))
    EOutfitSubMenu OutfitSubMeny = EOutfitSubMenu::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "OutfitSubMeny == EOutfitSubMenu::Top", EditConditionHides))
    EOutfitTopPreset OutfitTopPreset = EOutfitTopPreset::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "OutfitSubMeny == EOutfitSubMenu::Bottom", EditConditionHides))
    EOutfitBottomPreset OutfitBottomPreset = EOutfitBottomPreset::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "OutfitSubMeny == EOutfitSubMenu::FullBody", EditConditionHides))
    EOutfitFullBodyPreset OutfitFullBodyPreset = EOutfitFullBodyPreset::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "OutfitSubMeny == EOutfitSubMenu::Shoes", EditConditionHides))
    EOutfitShoesPreset OutfitShoesPreset = EOutfitShoesPreset::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "ButtonType == EButtonType::AccessoriesSubMenu", EditConditionHides))
    EAccessoriesSubMenu AccessoriesSubMenu = EAccessoriesSubMenu::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "AccessoriesSubMenu == EAccessoriesSubMenu::Top", EditConditionHides))
    EAccessoriesTopPreset AccessoriesTopPreset = EAccessoriesTopPreset::NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", EditCondition = "AccessoriesSubMenu == EAccessoriesSubMenu::Bottom", EditConditionHides))
    EAccessoriesBodyPreset AccessoriesBodyPreset = EAccessoriesBodyPreset::NONE;*/

    virtual void SynchronizeProperties() override;

    UFUNCTION(BlueprintCallable)
    const UButton* GetButton() const { return this; }

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
    TObjectPtr<UTexture> NormalImage;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
    TObjectPtr<UTexture> HoverImage;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
    TObjectPtr<UTexture> ClickedImage;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
    TObjectPtr<USoundBase> HoveredSound;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
    TObjectPtr<USoundBase> PressedSound;
        
private:
    void SetButtonStyle();
};