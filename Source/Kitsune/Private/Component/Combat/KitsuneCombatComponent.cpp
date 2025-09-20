// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Combat/KitsuneCombatComponent.h"



void UKitsuneCombatComponent::RegisterCarriedWeapon(const FGameplayTag InWeaponTag, AWeaponBase* NewWeapon,
	const bool bRegisterAndEquip)
{
	if (InWeaponTag.IsValid()&&NewWeapon)
	{
		CarriedWeaponMap.Add(InWeaponTag, NewWeapon);
		if (bRegisterAndEquip)
		{
			CurrentWeaponTag = InWeaponTag;
		}
	}
}

TObjectPtr<AWeaponBase> UKitsuneCombatComponent::FindWeaponByTag(const FGameplayTag& WeaponTag) const
{
	if (const TObjectPtr<AWeaponBase>* FoundWeapon = CarriedWeaponMap.Find(WeaponTag))
	{
		return *FoundWeapon;
	}
	return nullptr;
}

TObjectPtr<AWeaponBase> UKitsuneCombatComponent::GetCurrentCarriedWeapon() const
{
	if (CurrentWeaponTag.IsValid())
	{
		return FindWeaponByTag(CurrentWeaponTag);
	}
	return nullptr;
}
