// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Weapon/WeaponBase.h"
#include "Components/BoxComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemOverlapBox"));
	ItemOverlapBox->SetupAttachment(GetRootComponent());
	ItemOverlapBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
