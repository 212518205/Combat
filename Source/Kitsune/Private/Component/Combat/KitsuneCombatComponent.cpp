// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Combat/KitsuneCombatComponent.h"

#include "Actor/Weapon/WeaponBase.h"
#include "Characters/KitsuneCharacter.h"


void UKitsuneCombatComponent::RegisterCarriedWeapon(const FGameplayTag InWeaponTag, AWeaponBase* NewWeapon,
	const bool bRegisterAndEquip)
{
	if (InWeaponTag.IsValid() && NewWeapon)
	{
		CarriedWeaponMap.Add(InWeaponTag, NewWeapon);
		if (const AKitsuneCharacter* KitsuneCharacter = CastChecked<AKitsuneCharacter>(GetOwningPawn())) {
			NewWeapon->GiveWeaponInitialAbilityToASC(KitsuneCharacter->GetAbilitySystemComponent());
		}
		if (bRegisterAndEquip)
		{
			SetCurrentWeapon(InWeaponTag);
		}
	}
}

AWeaponBase* UKitsuneCombatComponent::FindWeaponByTag(const FGameplayTag& WeaponTag) const
{
	if (const TObjectPtr<AWeaponBase>* FoundWeapon = CarriedWeaponMap.Find(WeaponTag))
	{
		return *FoundWeapon;
	}
	return nullptr;
}

AWeaponBase* UKitsuneCombatComponent::GetCurrentCarriedWeapon() const
{
	if (CurrentWeaponTag.IsValid())
	{
		return FindWeaponByTag(CurrentWeaponTag);
	}
	return nullptr;
}

AWeaponBase* UKitsuneCombatComponent::SetCurrentWeapon(const FGameplayTag& WeaponTag)
{
	AWeaponBase* NewWeaponBase = FindWeaponByTag(WeaponTag);
	const AWeaponBase* OldWeaponBase = GetCurrentCarriedWeapon();
	UAbilitySystemComponent* AbilitySystemComponent = CastChecked<AKitsuneCharacter>(GetOwningPawn())->
		GetAbilitySystemComponent();

	if (OldWeaponBase)
	{
		OldWeaponBase->ClearWeaponAbilitiesFromASC(AbilitySystemComponent);
	}

	CurrentWeaponTag = WeaponTag;

	if (NewWeaponBase)
	{
		NewWeaponBase->GiveWeaponAbilitiesToASC(AbilitySystemComponent);
	}

	return NewWeaponBase;
}
