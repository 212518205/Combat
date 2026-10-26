// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KitsuneInputAbility.h"
#include "KitsuneToggleAbility.generated.h"

/**
 * 开关型能力父类：第一次按下开启（结算一次花费），再按一次关闭。
 */
UCLASS(Abstract, Blueprintable)
class KITSUNE_API UKitsuneToggleAbility : public UKitsuneInputAbility
{
	GENERATED_BODY()

public:
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                          const FGameplayAbilityActivationInfo ActivationInfo) override;

	UFUNCTION(BlueprintPure)
	bool IsToggleOn() const;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	virtual void OnToggleOn();
	virtual void OnToggleOff();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnToggleOn();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnToggleOff();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Shared.Status"))
	FGameplayTag ToggleStateTag;
};
