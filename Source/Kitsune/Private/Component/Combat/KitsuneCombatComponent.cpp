// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Combat/KitsuneCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FrontendDebugHelper.h"
#include "UIManagerSubsystem.h"
#include "AbilitySyetem/KitsuneAttributeSet.h"
#include "Actor/Weapon/DataAssetStartDataWeapon.h"
#include "Actor/Weapon/WeaponBase.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/KitsuneCharacter.h"
#include "Components/BoxComponent.h"
#include "FunctionLibrary/KitsuneFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UI/ViewModel/PlayerViewModel.h"


void UKitsuneCombatComponent::RegisterCarriedWeapon_Implementation(AWeaponBase* NewWeapon, const bool bRegisterAndEquip)
{
	if (NewWeapon)
	{
		CarriedWeapons.Add(NewWeapon);

		NewWeapon->WeaponBeginOverlap.BindUObject(this, &ThisClass::OnHitTargetActor);
		NewWeapon->WeaponEndOverlap.BindUObject(this, &ThisClass::OnPulledFromTargetActor);

		if (const ACharacterBase* CharacterBase = Cast<ACharacterBase>(GetOwningPawn())) {
			NewWeapon->GiveWeaponInitialAbilityToASC(CharacterBase->GetAbilitySystemComponent());
		}
		if (bRegisterAndEquip)
		{
			SetCurrentWeapon(NewWeapon->WeaponTag);
		}
	}
}

UKitsuneCombatComponent::UKitsuneCombatComponent()
{
	SetIsReplicated(true);
}

AWeaponBase* UKitsuneCombatComponent::FindWeaponByTag(const FGameplayTag& WeaponTag) const
{
	const TObjectPtr<AWeaponBase>* FoundWeapon = CarriedWeapons.FindByPredicate(
		[&WeaponTag](const TObjectPtr<AWeaponBase>& Weapon)
		{
			return Weapon && Weapon->WeaponTag == WeaponTag;
		});

	return FoundWeapon ? *FoundWeapon : nullptr;
}

AWeaponBase* UKitsuneCombatComponent::GetCurrentCarriedWeapon() const
{
	if (CurrentWeaponTag.IsValid())
	{
		return FindWeaponByTag(CurrentWeaponTag);
	}
	return nullptr;
}

void UKitsuneCombatComponent::SetCurrentWeapon_Implementation(const FGameplayTag& WeaponTag)
{
	ACharacterBase* OwningCharacter = Cast<ACharacterBase>(GetOwningPawn());



	if (const AWeaponBase* LastWeapon = GetCurrentCarriedWeapon())
	{
		LastWeapon->UnequipWeaponFromCharacter(OwningCharacter);
	}
	if (const AWeaponBase* CurrentFoundWeapon = FindWeaponByTag(WeaponTag))
	{
		CurrentFoundWeapon->EquipWeaponToCharacter(OwningCharacter);
	}

	KitsuneNet::SetReplicatedProperty(this, CurrentWeaponTag, WeaponTag, &ThisClass::SwitchWeaponIcon);
	
}



void UKitsuneCombatComponent::ToggleWeaponCollision_Implementation(bool bEnable)
{
	AWeaponBase* ToggledWeapon = GetCurrentCarriedWeapon();
	if (!ToggledWeapon)return;
	if (bEnable)
	{
		ToggledWeapon->GetWeaponBoxCollision()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		ToggledWeapon->GetWeaponBoxCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OverlappedActors.Empty();
	}
}

void UKitsuneCombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UKitsuneCombatComponent, CurrentWeaponTag, COND_None);
	DOREPLIFETIME_CONDITION(UKitsuneCombatComponent, CarriedWeapons, COND_None);
}

void UKitsuneCombatComponent::OnRep_CurrentWeaponTag()
{

	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		SwitchWeaponIcon();
	}
}

void UKitsuneCombatComponent::SwitchWeaponIcon()
{
	const AWeaponBase* CurrentWeapon = GetCurrentCarriedWeapon();
	if (!GetOwningPawn()->IsA(AKitsuneCharacter::StaticClass()))return;
	TSoftObjectPtr<UTexture2D> WeaponIcon;
	if (CurrentWeapon)
	{
		WeaponIcon = CurrentWeapon->GetWeaponInfo()->WeaponIcon;
	}
	if (UPlayerViewModel* LocalViewModel = UUIManagerSubsystem::GetUIManager(GetWorld())->TryGetViewModelByActor<UPlayerViewModel>(GetOwningPawn()))
	{
		LocalViewModel->SetPlayerWeaponIcon(WeaponIcon);
	}
}

void UKitsuneCombatComponent::OnHitTargetActor(AActor* HitActor)
{

}

void UKitsuneCombatComponent::OnPulledFromTargetActor(AActor* InteractedActor)
{
}
