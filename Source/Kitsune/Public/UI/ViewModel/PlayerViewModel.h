// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/AttributeViewModel.h"
#include "PlayerViewModel.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UPlayerViewModel : public UAttributeViewModel
{
	GENERATED_BODY()

public:
	virtual void BindCallback() override;

	UFUNCTION(BlueprintCallable)
	void SetPlayerWeaponIcon(TSoftObjectPtr<UTexture2D> InWeaponIcon);

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnAttributeChangedDelegate OnWeaponChanged;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | WeaponIcon")
	TSoftObjectPtr<UTexture2D> WeaponIcon;
};
