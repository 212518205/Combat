// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/KitsuneCharacter.h"

#include "UIManagerSubsystem.h"
#include "Characters/Data/DataAssetStartDataBase.h"
#include "Component/Combat/PlayerCombatComponent.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "Component/Interaction/InteractionComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Inventory/InventorySystemComponent.h"
#include"Game/KitsunePlayerState.h"
#include "Game/GameInstanceSubsystem/KitsuneSaveSubsystem.h"
#include "UI/ViewModel/AttributeViewModel.h"

AKitsuneCharacter::AKitsuneCharacter()
{
	bReplicateUsingRegisteredSubObjectList = true;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("CombatComponent"));
	InteractComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractComponent"));
	InventorySystemComp = CreateDefaultSubobject<UInventorySystemComponent>(TEXT("InventorySystemComponent"));
	
	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectionBox"));
	DetectionBox->SetupAttachment(GetRootComponent());
	DetectionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionBox->SetCollisionObjectType(ECC_WorldDynamic);
	DetectionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectionBox->SetCollisionResponseToChannel(ECC_GameTraceChannel13, ECR_Overlap);
	DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnDetectionBoxBeginOverlap);
	DetectionBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnDetectionBoxEndOverlap);
	
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
	
	BindAndLoadSave();
	
	
}

UKitsuneCombatComponent* AKitsuneCharacter::GetKitsuneCombatComponent() const
{
	return CombatComponent;
}

UInteractionComponent* AKitsuneCharacter::GetInteractionComp()
{
	return InteractComponent;
}

void AKitsuneCharacter::BindAndLoadSave() const
{
	const AKitsunePlayerState* KitsunePlayerState = Cast<AKitsunePlayerState>(GetPlayerState());
	if (!KitsunePlayerState || KitsunePlayerState->GetPlayerUID() <= 0)return;
	if (!HasAuthority())return;
	
	if (UKitsuneSaveSubsystem* Subsystem = UKitsuneSaveSubsystem::GetSaveSubsystem(this))
	{
		for (UActorComponent* SavableComp : GetComponentsByInterface(USavableInterface::StaticClass()))
		{
			Subsystem->RegisterForSaving(KitsunePlayerState->GetPlayerUID(), SavableComp);
		}
		Subsystem->LoadGameForPlayer(KitsunePlayerState->GetPlayerUID());
	}
}

void AKitsuneCharacter::InitAbilityInfo()
{
	Super::InitAbilityInfo();

	AKitsunePlayerState* KitsunePlayerState = GetPlayerState<AKitsunePlayerState>();
	check(KitsunePlayerState);
	AbilitySystemComp = KitsunePlayerState->GetAbilitySystemComponent();
	AbilitySystemComp->InitAbilityActorInfo(KitsunePlayerState, this);

	AttributeSet = KitsunePlayerState->GetAttributeSet();

	//if (const ENetMode NetMode = GetNetMode(); NetMode == NM_Client || NetMode == NM_Standalone || NetMode == NM_ListenServer)
	if (GetNetMode() != NM_DedicatedServer)
	{
		UUIManagerSubsystem::GetUIManager(GetWorld())->TryGetViewModelByActor<UPlayerViewModel>(this);
		OnWidgetComponentInitialized();
	}

	if (HasAuthority() && InitialInfoData && GetAbilitySystemComponent())
	{
		InitialInfoData->InitAbilityAndEffectToASC(GetAbilitySystemComponent(), CharacterLevel);
		SetCharacterProperties(InitialInfoData->CharacterProperties);
	}
}

UPlayerViewModel* AKitsuneCharacter::GetOwningViewModel()
{
	return UUIManagerSubsystem::GetUIManager(GetWorld())->TryGetViewModelByActor<UPlayerViewModel>(this);
}

void AKitsuneCharacter::OnDetectionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)return;
	/*** TODO: 可选，检查OtherActor是否是可锁定的目标，目前靠ECC_GameTraceChannel13检查，即为Enemy通道... [2026年8月27日 0:25:58 来自`@BC@`] ***/
	
	if (UPlayerCombatComponent* Combat = FindComponentByClass<UPlayerCombatComponent>())
	{
		Combat->LockedTargetInto(OtherActor);
	}
}

void AKitsuneCharacter::OnDetectionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)return;
	
	if (UPlayerCombatComponent* Combat = FindComponentByClass<UPlayerCombatComponent>())
	{
		Combat->LockedTargetLeave(OtherActor);
	}
}

