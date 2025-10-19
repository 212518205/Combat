// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"

#include <string>

#include "FrontendDebugHelper.h"
#include "AbilitySyetem/KitsuneAbilitySystemComponent.h"
#include "AbilitySyetem/KitsuneAttributeSet.h"
#include "Characters/Data/DataAssetStartDataEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UKitsuneAbilitySystemComponent>(
		"AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UKitsuneAttributeSet>("AttributeSet");

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;



	SetNetUpdateFrequency(100.f);
}

void AEnemyCharacter::InitAbilityInfo()
{
	Super::InitAbilityInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	StartData->InitAbilityAndEffectToASC(GetAbilitySystemComponent(), 1);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityInfo();
}
