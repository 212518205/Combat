// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"

#include <string>

#include "FrontendDebugHelper.h"
#include "UIManagerSubsystem.h"
#include "AbilitySyetem/KitsuneAbilitySystemComponent.h"
#include "AbilitySyetem/KitsuneAttributeSet.h"
#include "Characters/Data/DataAssetStartDataEnemy.h"
#include "Component/Combat/EnemyKitsuneCombatComponent.h"
#include "Components/WidgetComponent.h"
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

	CombatComponent = CreateDefaultSubobject<UEnemyKitsuneCombatComponent>(TEXT("CombatComponent"));
	EnemyWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyWidgetComponent"));
	EnemyWidgetComponent->SetupAttachment(GetRootComponent());
	
	LockMarkerComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockMarkerComp"));
	LockMarkerComponent->SetupAttachment(GetMesh(), LockMarkerSocketName);
	LockMarkerComponent->SetWidgetSpace(EWidgetSpace::Screen);
	LockMarkerComponent->SetDrawSize(FVector2D(32.f, 32.f));
	LockMarkerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LockMarkerComponent->SetVisibility(false);

	SetNetUpdateFrequency(100.f);
}

void AEnemyCharacter::InitAbilityInfo()
{
	Super::InitAbilityInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	if (HasAuthority())
	{
		StartData->InitAbilityAndEffectToASC(GetAbilitySystemComponent(), 1);
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityInfo();
}

UKitsuneCombatComponent* AEnemyCharacter::GetKitsuneCombatComponent() const
{
	return CombatComponent;
}

void AEnemyCharacter::SetLockMarkerVisible(const bool bVisible)
{
	if (LockMarkerComponent)
	{
		LockMarkerComponent->SetVisibility(bVisible);
	}
}

UEnemyViewModel* AEnemyCharacter::GetEnemyViewModel()
{
	if (EnemyViewModel)return EnemyViewModel;
	EnemyViewModel = UUIManagerSubsystem::GetUIManager(this)->TryGetViewModelByActor<UEnemyViewModel>(this);

	return EnemyViewModel;
}
