// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/KitsuneAttributeSet.h"
#include"Net/UnrealNetwork.h"

UKitsuneAttributeSet::UKitsuneAttributeSet()
{
	
}

void UKitsuneAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKitsuneAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UKitsuneAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, Health, OldHealth);
}

void UKitsuneAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKitsuneAttributeSet, MaxHealth, OldMaxHealth);
}
