// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "KitsuneGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/** Variable Begin*/
	UPROPERTY(EditDefaultsOnly,Category="Input")
	FGameplayTag StartupInputTag;
	/** Variable End*/
};
