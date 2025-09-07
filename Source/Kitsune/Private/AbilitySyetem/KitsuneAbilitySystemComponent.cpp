// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/KitsuneAbilitySystemComponent.h"

#include "AbilitySyetem/Abilities/KitsuneGameplayAbility.h"

void UKitsuneAbilitySystemComponent::BindDelegateCallback()
{
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UKitsuneAbilitySystemComponent::GameplayEffectAppliedCallback);
}

void UKitsuneAbilitySystemComponent::AddInitialAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
    for (const TSubclassOf<UGameplayAbility>& Ability:StartupAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
        if (const UKitsuneGameplayAbility* KitsuneAbility=Cast<UKitsuneGameplayAbility>(AbilitySpec.Ability))
        {
            AbilitySpec.GetDynamicSpecSourceTags().AddTag(KitsuneAbility->StartupInputTag);
            GiveAbility(AbilitySpec);
        }
    }
}

void UKitsuneAbilitySystemComponent::GameplayEffectAppliedCallback(UAbilitySystemComponent* AbilitySystemComponent,
                                                                   const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle Handle) const
{
    FGameplayTagContainer TagContainer;
    GameplayEffectSpec.GetAllAssetTags(TagContainer);
    EffectAssetTags.Broadcast(TagContainer);
}
