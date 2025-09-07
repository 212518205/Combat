// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "KitsuneAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	/** Public Function Begin*/
	void BindDelegateCallback();

	void AddInitialAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	/** Public Function End*/
	/** Public Variable Begin*/
	FEffectAssetTags EffectAssetTags;
	/** Public Variable End*/
protected:
	/** Protected Function Begin*/
	void GameplayEffectAppliedCallback(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle Handle) const;
	/** Protected Function End*/

};
