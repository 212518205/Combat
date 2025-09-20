// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "KitsuneInputConfig.generated.h"

/**
 * 
 */

class UInputAction;

USTRUCT(BlueprintType)
struct FKitsuneInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly,meta = (Categories = "InputTag"))
	FGameplayTag InputTag = FGameplayTag();

};

UCLASS()
class KITSUNE_API UKitsuneInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TArray<FKitsuneInputAction> InputActions;

	const UInputAction* FindInputActionByTag(const FGameplayTag& InTag, const bool bNoFindLog = false);
};
