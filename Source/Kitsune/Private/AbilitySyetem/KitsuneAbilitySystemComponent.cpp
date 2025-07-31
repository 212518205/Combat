// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/KitsuneAbilitySystemComponent.h"

void UKitsuneAbilitySystemComponent::BindDelegateCallback()
{
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UKitsuneAbilitySystemComponent::GameplayEffectAppliedCallback);
}

void UKitsuneAbilitySystemComponent::GameplayEffectAppliedCallback(UAbilitySystemComponent* AbilitySystemComponent,
                                                                   const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle Handle)
{
    FGameplayTagContainer TagContainer;
    GameplayEffectSpec.GetAllAssetTags(TagContainer);
    EffectAssetTags.Broadcast(TagContainer);
}
