// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "ShibLibrary.h"
#include "UISubsystem.generated.h"

class AShibController;

/**
* 
*/
UCLASS(MinimalAPI)
class UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 FocusIndexHistory {0};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UButton* PreviousButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FButtonStyle PreviousButtonStyle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<EShibAccessory, TObjectPtr<UButton>> AccessorySelectedButtons;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<EShibTattoo, TObjectPtr<UButton>> TattooSelectedButtons;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	UFUNCTION(BlueprintCallable)
	void ChangeAvatarFocus(int32 FocusIndex);
	UFUNCTION(BlueprintCallable)
	void ChangeAvatarFocusExit(int32 FocusIndex);

	
	UFUNCTION(BlueprintCallable)
	const AShibController* GetShibController() const { return ShibController; }
	UFUNCTION(BlueprintCallable)
	void SetShibController(AShibController* InShibController) { ShibController = InShibController; }
	

private:
	UPROPERTY()
	TObjectPtr<AShibController> ShibController;

};