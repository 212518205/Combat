// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KitsuneCharacter.h"

#include "FrontendDebugHelper.h"
#include "UIManagerSubsystem.h"
#include "Characters/Data/DataAssetStartDataBase.h"
#include "Component/Combat/PlayerCombatComponent.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "Component/Interaction/InteractionComponent.h"
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
	InteractComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractComponent"));
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

UInteractionComponent* AKitsuneCharacter::GetInteractionComp()
{
	return InteractComponent;
}

void AKitsuneCharacter::InitAbilityInfo()
{
	Super::InitAbilityInfo();

	AKitsunePlayerState* KitsunePlayerState = GetPlayerState<AKitsunePlayerState>();
	check(KitsunePlayerState);
	AbilitySystemComponent = KitsunePlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(KitsunePlayerState, this);

	AttributeSet = KitsunePlayerState->GetAttributeSet();

	if (const ENetMode NetMode = GetNetMode(); NetMode == NM_Client || NetMode == NM_Standalone || NetMode == NM_ListenServer)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
			UPlayerViewModel* ViewModel = UUIManagerSubsystem::GetUIManager(GetWorld())->TryGetViewModelByActor<UPlayerViewModel>(this);
		}
	}

	if (InitialInfoData && GetAbilitySystemComponent())
	{
		InitialInfoData->InitAbilityAndEffectToASC(GetAbilitySystemComponent(), CharacterLevel);
	}
}
