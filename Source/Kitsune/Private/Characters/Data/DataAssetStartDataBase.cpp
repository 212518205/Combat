// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Data/DataAssetStartDataBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

void UDataAssetStartDataBase::InitializeForASC(UAbilitySystemComponent* InTargetASC, const int32 AbilityLevel)
{
	check(InTargetASC);

	GrantAbilitiesToASC(InTargetASC, GiveAndActiveAbilities, AbilityLevel);
	GrantAbilitiesToASC(InTargetASC, ReactivableAbilities, AbilityLevel);

	ApplyGameplayEffectToASC(InTargetASC, DefaultGameplayEffects, AbilityLevel);
}

void UDataAssetStartDataBase::GrantAbilitiesToASC(UAbilitySystemComponent* TargetASC,
	TArray<TSubclassOf<UGameplayAbility>>& UnassignedAbilities, const int32 AbilityLevel)
{
	for (const auto& AbilityClass:UnassignedAbilities)
	{
		if (!AbilityClass)continue;

		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,AbilityLevel);
		AbilitySpec.SourceObject = TargetASC->GetAvatarActor();

		TargetASC->GiveAbility(AbilitySpec);
	}
}

void UDataAssetStartDataBase::ApplyGameplayEffectToASC(UAbilitySystemComponent* TargetASC,
	TArray<TSubclassOf<UGameplayEffect>>& UnassignedEffect, const int32 EffectLevel)
{
	for (const auto& GameplayEffectClass:UnassignedEffect)
	{
		if (!GameplayEffectClass)return;

		const FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, EffectLevel, ContextHandle);
		TargetASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
	}	
}
