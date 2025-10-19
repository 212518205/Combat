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
	AKitsuneCharacter* KitsuneCharacter = CastChecked<AKitsuneCharacter>(GetOwningPawn());

	if (OldWeaponBase)
	{
		OldWeaponBase->UnequipWeaponFromCharacter(KitsuneCharacter);
	}

	CurrentWeaponTag = WeaponTag;

	if (NewWeaponBase)
	{
		NewWeaponBase->EquipWeaponToCharacter(KitsuneCharacter);
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

	Debug::Print(GetOwningPawn()->GetName() + TEXT(" Hit-->") + HitActor->GetName(),-1,FColor::Green);
	if (const AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(HitActor))
	{
		if (const UKitsuneAttributeSet* KitsuneAttributeSet = Cast<UKitsuneAttributeSet>(Enemy->GetAttributeSet()))
		{
			Debug::Print(TEXT("MaxHealth"), KitsuneAttributeSet->GetMaxHealth());
			Debug::Print(TEXT("Health"), KitsuneAttributeSet->GetHealth());
			Debug::Print(TEXT("AttackPower"), KitsuneAttributeSet->GetAttackPower());
			Debug::Print(TEXT("GetCriticalDamage"), KitsuneAttributeSet->GetCriticalChance());
			Debug::Print(TEXT("GetCriticalDamage"), KitsuneAttributeSet->GetCriticalDamage());
		}

	}
}

void UKitsuneCombatComponent::OnPulledFromTargetActor(AActor* InteractedActor)
{
	Debug::Print(GetOwningPawn()->GetName() + TEXT(" Hit-->") + InteractedActor->GetName(),-1,FColor::Red);
}
