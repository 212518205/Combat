// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KitsuneGameplayAbility.h"
#include "KitsuneInstantAbility.generated.h"

/**
 * 实时类技能，激活即结束，如施加GE，切换状态等，不播动画或不关心动画是否播完
 */
UCLASS()
class KITSUNE_API UKitsuneInstantAbility : public UKitsuneGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	
	virtual void OnInstantExecute();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnInstantExecute")
	void K2_OnInstantExecute();
	
};
