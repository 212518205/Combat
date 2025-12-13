// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/KitsuneAbilitySystemComponent.h"

#include "AbilitySyetem/Abilities/KitsuneGameplayAbility.h"


void UKitsuneAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	for (auto& AbilitySpec:GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))continue;

		if (!AbilitySpec.IsActive())
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UKitsuneAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	 
}

bool UKitsuneAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag ActivateAbilityTag)
{
	check(ActivateAbilityTag.IsValid());

	TArray<FGameplayAbilitySpec*> FoundAbilitySpec;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(ActivateAbilityTag.GetSingleTagContainer(), FoundAbilitySpec);

	if (!FoundAbilitySpec.IsEmpty())
	{
		const int32 ActivatableAbilityIndex = FMath::RandRange(0, FoundAbilitySpec.Num() - 1);
		FGameplayAbilitySpec* SpecToActive = FoundAbilitySpec[ActivatableAbilityIndex];

		if (!SpecToActive->IsActive())
		{
			return TryActivateAbility(SpecToActive->Handle);
		}

	}

	return false;
}

