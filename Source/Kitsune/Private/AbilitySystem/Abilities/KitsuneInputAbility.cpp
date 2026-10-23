// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/KitsuneInputAbility.h"

void UKitsuneInputAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	OnPressed();
}

void UKitsuneInputAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	OnReleased();
}

void UKitsuneInputAbility::OnPressed()
{
	K2_OnPressed();
}

void UKitsuneInputAbility::OnReleased()
{
	K2_OnReleased();
}
