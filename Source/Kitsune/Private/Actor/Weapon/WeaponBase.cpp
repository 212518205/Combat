// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Weapon/WeaponBase.h"

#include "Actor/Weapon/DataAssetStartDataWeapon.h"
#include "Characters/KitsuneCharacter.h"
#include "Components/BoxComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemOverlapBox"));
	ItemOverlapBox->SetupAttachment(GetRootComponent());
	ItemOverlapBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ItemOverlapBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnWeaponBeginOverlap);
	ItemOverlapBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnWeaponEndOverlap);
}

void AWeaponBase::EquipWeaponToCharacter(ACharacterBase* TargetCharacter) const
{
	WeaponDataInfo->GiveAbilitiesToASC(TargetCharacter->GetAbilitySystemComponent(), WeaponLevel);
	WeaponDataInfo->ModifyCharacterData(TargetCharacter);
}

void AWeaponBase::GiveWeaponInitialAbilityToASC(UAbilitySystemComponent* TargetASC) const
{
	WeaponDataInfo->GiveWeaponInitialAbilityToASC(TargetASC);
}

void AWeaponBase::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const APawn* WeaponPawn = GetInstigator<APawn>();
	check(WeaponPawn)

	if (APawn* HitPawn=Cast<APawn>(OtherActor))
	{
		WeaponBeginOverlap.ExecuteIfBound(HitPawn);
	}
}

void AWeaponBase::OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const APawn* WeaponPawn = GetInstigator<APawn>();
	check(WeaponPawn)

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		WeaponEndOverlap.ExecuteIfBound(HitPawn);
	}
}

void AWeaponBase::UnequipWeaponFromCharacter(ACharacterBase* TargetCharacter) const
{
	WeaponDataInfo->ClearAbilitiesFromASC(TargetCharacter->GetAbilitySystemComponent());
	WeaponDataInfo->ResetCharacterData(TargetCharacter);
}
