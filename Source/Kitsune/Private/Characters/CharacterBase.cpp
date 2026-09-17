// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"
#include "MotionWarpingComponent.h"
#include "UIManagerSubsystem.h"
#include "Component/UI/KitsuneWidgetComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/Widget/Components/CharacterWidget/WidgetCharacterComponentObject.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	PrimaryActorTick.bCanEverTick = false;

	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
	
	LockMarkerComp = CreateDefaultSubobject<UKitsuneWidgetComponent>(TEXT("LockMarkerComp"));
	LockMarkerComp->SetupAttachment(GetMesh(), LockMarkerSocketName);
	LockMarkerComp->SetWidgetSpace(EWidgetSpace::Screen);
	LockMarkerComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	DamagePopupComp = CreateDefaultSubobject<UKitsuneWidgetComponent>(TEXT("DamagePopupComponent"));
	DamagePopupComp->SetupAttachment(GetRootComponent());
	DamagePopupComp->SetWidgetSpace(EWidgetSpace::Screen);
	DamagePopupComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	CharacterStateComp = CreateDefaultSubobject<UKitsuneWidgetComponent>(TEXT("CharacterStateComponent"));
	CharacterStateComp->SetupAttachment(GetRootComponent());
	CharacterStateComp->SetWidgetSpace(EWidgetSpace::Screen);
	CharacterStateComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	LockMarkerComp->SetVisibility(false);
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ACharacterBase::UpdateLockMarkerVisible(const bool bVisible)
{
	if (LockMarkerComp)
	{
		LockMarkerComp->SetVisibility(bVisible);
	}
}

FGenericTeamId ACharacterBase::GetGenericTeamId() const
{
	return ActorTeamID;
}

UAttributeSet* ACharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

void ACharacterBase::SetCharacterProperties(const FCharacterProperties& CharacterProperties) const
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->bOrientRotationToMovement = CharacterProperties.bOrientRotationToMovement;
	MovementComp->MaxWalkSpeed = CharacterProperties.MaxWalkSpeed;
	MovementComp->bUseControllerDesiredRotation = CharacterProperties.bUseControllerDesiredRotation;
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ACharacterBase, ActorTeamID, COND_None, REPNOTIFY_Always);
}

void ACharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UUIManagerSubsystem* UIManager = UUIManagerSubsystem::GetUIManager(this))
	{
		UIManager->UnRegisterViewModel(this);
	}
	
	Super::EndPlay(EndPlayReason);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::InitAbilityInfo()
{
	
}

void ACharacterBase::OnWidgetComponentInitialized()
{
	for (const UKitsuneWidgetComponent* WidgetComp : {DamagePopupComp, CharacterStateComp})
	{
		if (!WidgetComp)continue;
		if (UWidgetCharacterComponentObject* StateWidget = Cast<UWidgetCharacterComponentObject>(WidgetComp->GetUserWidgetObject()))
		{
			StateWidget->OnBindAndInitialized(this);
		}
	}
}
