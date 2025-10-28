// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KitsuneCharacter.h"

#include "UIManagerSubsystem.h"
#include "Characters/Data/DataAssetStartDataBase.h"
#include "Component/Combat/PlayerCombatComponent.h"
#include"GameFramework/CharacterMovementComponent.h"
#include"Game/KitsunePlayerState.h"
#include "UI/ViewModel/AttributeViewModel.h"

AKitsuneCharacter::AKitsuneCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("CombatComponent"));
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

UKitsuneCombatComponent* AKitsuneCharacter::GetKitsuneCombatComponent() const
{
	return CombatComponent;
}

void AKitsuneCharacter::InitAbilityInfo()
{
	Super::InitAbilityInfo();

	AKitsunePlayerState* KitsunePlayerState = GetPlayerState<AKitsunePlayerState>();
	check(KitsunePlayerState);
	AbilitySystemComponent = KitsunePlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(KitsunePlayerState, this);

	AttributeSet = KitsunePlayerState->GetAttributeSet();

	if (IsLocallyControlled()&&GetNetMode()!=NM_DedicatedServer)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
			UPlayerViewModel* ViewModel = UAttributeViewModel::GetViewModel<UPlayerViewModel>(AbilitySystemComponent, AttributeSet);
			UUIManagerSubsystem::GetUIManager(GetWorld())->RegisterPlayerViewModel(ViewModel);
		}
	}

	if (InitialInfoData && GetAbilitySystemComponent())
	{
		InitialInfoData->InitAbilityAndEffectToASC(GetAbilitySystemComponent(), CharacterLevel);
	}
}
