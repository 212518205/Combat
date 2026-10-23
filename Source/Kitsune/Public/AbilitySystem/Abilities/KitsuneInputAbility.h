// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/KitsuneGameplayAbility.h"
#include "KitsuneInputAbility.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneInputAbility : public UKitsuneGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                          const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           const FGameplayAbilityActivationInfo ActivationInfo) override;
	
protected:
	virtual void OnPressed();
	virtual void OnReleased();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnPressed")
	void K2_OnPressed();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnReleased")
	void K2_OnReleased();
};
