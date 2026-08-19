// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/KitsunePlayerState.h"
#include"AbilitySyetem/KitsuneAbilitySystemComponent.h"
#include"AbilitySyetem/KitsuneAttributeSet.h"
#include "Net/UnrealNetwork.h"

AKitsunePlayerState::AKitsunePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UKitsuneAbilitySystemComponent>(
		"AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UKitsuneAttributeSet>("AttributeSet");

	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* AKitsunePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AKitsunePlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void AKitsunePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, PlayerUID);
}

void AKitsunePlayerState::OnRep_PlayerUID()
{
}
