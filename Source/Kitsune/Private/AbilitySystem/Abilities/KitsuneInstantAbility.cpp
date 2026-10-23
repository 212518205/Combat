// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/KitsuneInstantAbility.h"

void UKitsuneInstantAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	OnInstantExecute();
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UKitsuneInstantAbility::OnInstantExecute()
{
	K2_OnInstantExecute();
}
