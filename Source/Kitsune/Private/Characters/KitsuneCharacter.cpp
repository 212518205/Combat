// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KitsuneCharacter.h"
#include"GameFramework/CharacterMovementComponent.h"
#include"AbilitySyetem/KitsuneAbilitySystemComponent.h"
#include"Game/KitsunePlayerState.h"
#include "Player/KitsunePlayerController.h"
#include "UI/HUD/KitsuneHUD.h"

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

	InitAbilityInfo();
}

void AKitsuneCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityInfo();
}

void AKitsuneCharacter::InitAbilityInfo()
{
	AKitsunePlayerState* KitsunePlayerState = GetPlayerState<AKitsunePlayerState>();
	check(KitsunePlayerState);
	AbilitySystemComponent = KitsunePlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(KitsunePlayerState, this);

	AttributeSet = KitsunePlayerState->GetAttributeSet();
	Cast<UKitsuneAbilitySystemComponent>(AbilitySystemComponent)->BindDelegateCallback();
}
