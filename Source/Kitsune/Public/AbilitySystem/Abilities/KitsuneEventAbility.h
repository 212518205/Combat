// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KitsuneGameplayAbility.h"
#include "KitsuneEventAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class KITSUNE_API UKitsuneEventAbility : public UKitsuneGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FGameplayEventData CachedTriggerEventData;
};
