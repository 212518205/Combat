// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Data/DataAssetStartDataBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffect.h"

void UDataAssetStartDataBase::InitAbilityAndEffectToASC(UAbilitySystemComponent* InTargetASC, const int32 AbilityLevel)
{
	check(InTargetASC);

	GrantAbilitiesToASC(InTargetASC, OwningAbilities, AbilityLevel);
	ApplyGameplayEffectToASC(InTargetASC, DefaultGameplayEffects, AbilityLevel);
}

void UDataAssetStartDataBase::GrantAbilitiesToASC(UAbilitySystemComponent* TargetASC,
                                                  TArray<FAbilityInfo>& UnassignedAbilities, const int32 AbilityLevel)
{
	for (const auto& [AbilityClass,AbilityTag] : UnassignedAbilities)
	{
		if (!AbilityClass)continue;

		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,AbilityLevel);
		AbilitySpec.SourceObject = TargetASC->GetAvatarActor();
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityTag);

		TargetASC->GiveAbility(AbilitySpec);
	}
}

void UDataAssetStartDataBase::ApplyGameplayEffectToASC(UAbilitySystemComponent* TargetASC,
	TArray<TSoftClassPtr<UGameplayEffect>>& UnassignedEffect, const int32 EffectLevel)
{
	for (const auto& SoftEffectClass : UnassignedEffect)
	{
		if (SoftEffectClass.IsNull())continue;

		UClass* GameplayEffectClass = SoftEffectClass.LoadSynchronous();
		if (!GameplayEffectClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Kitsune] DefaultGameplayEffects 加载失败，已跳过：%s"),
				*SoftEffectClass.ToString());
			continue;
		}

		const FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, EffectLevel, ContextHandle);
		TargetASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
	}	
}
