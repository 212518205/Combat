// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KitsuneCharacter.h"
#include"GameFramework/CharacterMovementComponent.h"
#include"AbilitySyetem/KitsuneAbilitySystemComponent.h"
#include"Game/KitsunePlayerState.h"

AKitsuneCharacter::AKitsuneCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	Sheath = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sheath"));
	Sheath->SetupAttachment(GetMesh(), FName("HipsSheathSocket"));
	Sheath->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	Sword->SetupAttachment(GetMesh(), FName("HandSwordSocket"));
	Sword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKitsuneCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AKitsunePlayerState* KitsunePlayerState = GetPlayerState<AKitsunePlayerState>();
	check(KitsunePlayerState);
	AbilitySystemComponent = KitsunePlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(KitsunePlayerState, this);

	AttributeSet = KitsunePlayerState->GetAttributeSet();
}

void AKitsuneCharacter::OnRep_PlayerState()
{
	AKitsunePlayerState* KitsunePlayerState = GetPlayerState<AKitsunePlayerState>();
	check(KitsunePlayerState);
	AbilitySystemComponent = KitsunePlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(KitsunePlayerState, this);

	AttributeSet = KitsunePlayerState->GetAttributeSet();
}
