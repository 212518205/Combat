// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KitsuneInputAbility.h"
#include "KitsuneChanneledAbility.generated.h"

/**
 * 引导型能力父类：按住持续、松开结束。
 */
UCLASS(Abstract, Blueprintable)
class KITSUNE_API UKitsuneChanneledAbility : public UKitsuneInputAbility
{
	GENERATED_BODY()

public:
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           const FGameplayAbilityActivationInfo ActivationInfo) override;

	UFUNCTION(BlueprintPure)
	float GetChannelTime() const;

	UFUNCTION(BlueprintPure)
	bool IsChanneling() const;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	virtual void OnChannelBegin();
	virtual void OnChannelEnd(float ChannelTime);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnChannelBegin();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnChannelEnd(float ChannelTime);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Shared.Status"))
	FGameplayTag ChannelStateTag;

private:
	double ChannelStartTime = 0.0;
};
