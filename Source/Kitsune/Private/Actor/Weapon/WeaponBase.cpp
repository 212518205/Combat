// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Weapon/WeaponBase.h"

#include "Actor/Weapon/DataAssetStartDataWeapon.h"
#include "Components/BoxComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemOverlapBox"));
	ItemOverlapBox->SetupAttachment(GetRootComponent());
	ItemOverlapBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponBase::GiveWeaponAbilitiesToASC(UAbilitySystemComponent* TargetASC) const
{
	WeaponDataInfo->GiveAbilitiesToASC(TargetASC, WeaponLevel);
}

void AWeaponBase::GiveWeaponInitialAbilityToASC(UAbilitySystemComponent* TargetASC) const
{
	WeaponDataInfo->GiveWeaponInitialAbilityToASC(TargetASC);
}

void AWeaponBase::ClearWeaponAbilitiesFromASC(UAbilitySystemComponent* TargetASC) const
{
	WeaponDataInfo->ClearAbilitiesFromASC(TargetASC);
}
