// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Weapon/DataAssetStartDataWeapon.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"



void UDataAssetStartDataWeapon::GiveAbilitiesToASC(UAbilitySystemComponent* TargetASC, int32 AbilityLevel)
{
	RemoveWeaponInitialAbilityFromASC(TargetASC);

	GrantAbilitiesToASC(TargetASC, OwningAbilities, AbilityLevel);
}

void UDataAssetStartDataWeapon::ClearAbilitiesFromASC(UAbilitySystemComponent* TargetASC) const
{
	TArray<FGameplayAbilitySpecHandle> ClearAbilitySpecHandles;

	for (const FGameplayAbilitySpec& AbilitySpec:TargetASC->GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTag(AbilityParentTag))
		{
			ClearAbilitySpecHandles.Add(AbilitySpec.Handle);
		}
	}

	for (FGameplayAbilitySpecHandle SpecHandle:ClearAbilitySpecHandles)
	{
		TargetASC->ClearAbility(SpecHandle);
	}

	GiveWeaponInitialAbilityToASC(TargetASC);
}

void UDataAssetStartDataWeapon::GiveWeaponInitialAbilityToASC(UAbilitySystemComponent* TargetASC) const
{
	if (!InitialCarriedAbility.AbilityClass)return;

	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(InitialCarriedAbility.AbilityClass, 1);
	AbilitySpec.SourceObject = TargetASC->GetAvatarActor();
	AbilitySpec.GetDynamicSpecSourceTags().AddTag(InitialCarriedAbility.AbilityTag);

	TargetASC->GiveAbility(AbilitySpec);
}

void UDataAssetStartDataWeapon::RemoveWeaponInitialAbilityFromASC(UAbilitySystemComponent* TargetASC) const
{
	for (const FGameplayAbilitySpec& AbilitySpec : TargetASC->GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTag(InitialCarriedAbility.AbilityTag))
		{
			TargetASC->ClearAbility(AbilitySpec.Handle);
		}
	}
}
