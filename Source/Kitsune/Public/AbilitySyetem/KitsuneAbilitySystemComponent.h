// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "KitsuneAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/*** `@BC`   描述: 输入触发依据InputTag激活对应能力   `BC@` ***/
	void OnAbilityInputPressed(const FGameplayTag& InputTag);
	void OnAbilityInputReleased(const FGameplayTag& InputTag);
};
