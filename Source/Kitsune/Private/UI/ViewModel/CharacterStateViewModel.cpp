// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/CharacterStateViewModel.h"
#include "AbilitySyetem/KitsuneAttributeSet.h"
#include "Characters/KitsuneCharacter.h"

void UCharacterStateViewModel::Initialize(const FModelSet& ModelSet)
{
	PlayerController = ModelSet.PlayerController;
	PlayerState = ModelSet.PlayerState;
	AbilitySystemComponent = ModelSet.AbilitySystemComponent;
	AttributeSet = ModelSet.AttributeSet;

	BindCallbacksToDependencies();

	UKitsuneAttributeSet* KitsuneAttributeSet = CastChecked<UKitsuneAttributeSet>(AttributeSet);
	KitsuneAttributeSet->SetHealth(59.f);
	KitsuneAttributeSet->SetMaxHealth(100.f);
	Health = KitsuneAttributeSet->GetHealth();
	MaxHealth = KitsuneAttributeSet->GetMaxHealth();
	OnHealthChanged.Broadcast(KitsuneAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(KitsuneAttributeSet->GetMaxHealth());
}

void UCharacterStateViewModel::BindCallbacksToDependencies()
{
	const UKitsuneAttributeSet* KitsuneAttributeSet = CastChecked<UKitsuneAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		KitsuneAttributeSet->GetHealthAttribute()).AddUObject(this, &UCharacterStateViewModel::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		KitsuneAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UCharacterStateViewModel::MaxHealthChanged);

}

void UCharacterStateViewModel::HealthChanged(const FOnAttributeChangeData& Data) 
{
	Health = Data.NewValue;
	OnHealthChanged.Broadcast(Health);
}

void UCharacterStateViewModel::MaxHealthChanged(const FOnAttributeChangeData& Data) 
{
	MaxHealth = Data.NewValue;
	OnMaxHealthChanged.Broadcast(MaxHealth);
}

