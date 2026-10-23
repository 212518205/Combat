// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/KitsuneEventAbility.h"

void UKitsuneEventAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	CachedTriggerEventData = TriggerEventData ? *TriggerEventData : FGameplayEventData();
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);;
	}
}
