// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Combat/KitsuneCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FrontendDebugHelper.h"
#include "AbilitySyetem/KitsuneAttributeSet.h"
#include "Actor/Weapon/WeaponBase.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/KitsuneCharacter.h"
#include "Components/BoxComponent.h"


void UKitsuneCombatComponent::RegisterCarriedWeapon(const FGameplayTag InWeaponTag, AWeaponBase* NewWeapon,
                                                    const bool bRegisterAndEquip)
{
	if (InWeaponTag.IsValid() && NewWeapon)
	{
		CarriedWeaponMap.Add(InWeaponTag, NewWeapon);

		NewWeapon->WeaponBeginOverlap.BindUObject(this, &ThisClass::OnHitTargetActor);
		NewWeapon->WeaponEndOverlap.BindUObject(this, &ThisClass::OnPulledFromTargetActor);

		if (const ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetOwningPawn())) {
			NewWeapon->GiveWeaponInitialAbilityToASC(CharacterBase->GetAbilitySystemComponent());
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
	ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetOwningPawn());

	if (OldWeaponBase)
	{
		OldWeaponBase->UnequipWeaponFromCharacter(CharacterBase);
	}

	CurrentWeaponTag = WeaponTag;

	if (NewWeaponBase)
	{
		NewWeaponBase->EquipWeaponToCharacter(CharacterBase);
	}

	return NewWeaponBase;
}

void UKitsuneCombatComponent::ToggleWeaponCollision(const bool bEnable)
{
	AWeaponBase* ToggledWeapon = GetCurrentCarriedWeapon();
	if (!ToggledWeapon)return;
	if (bEnable)
	{
		ToggledWeapon->GetWeaponBoxCollision()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}else
	{
		ToggledWeapon->GetWeaponBoxCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OverlappedActors.Empty();
	}
}

void UKitsuneCombatComponent::OnHitTargetActor(AActor* HitActor)
{

}

void UKitsuneCombatComponent::OnPulledFromTargetActor(AActor* InteractedActor)
{
}
