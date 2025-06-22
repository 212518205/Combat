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
}

void UCharacterStateViewModel::BindCallbacksToDependencies()
{
	UKitsuneAttributeSet* KitsuneAttributeSet = CastChecked<UKitsuneAttributeSet>(AttributeSet);
	///初始化方式错误！！
	KitsuneAttributeSet->InitHealth(50.f);
	KitsuneAttributeSet->InitMaxHealth(1000.f);
	Health = KitsuneAttributeSet->GetHealth();
	MaxHealth = KitsuneAttributeSet->GetMaxHealth();

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		KitsuneAttributeSet->GetHealthAttribute()).AddUObject(this, &UCharacterStateViewModel::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		KitsuneAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UCharacterStateViewModel::MaxHealthChanged);
}

void UCharacterStateViewModel::HealthChanged(const FOnAttributeChangeData& Data) 
{
	Health = Data.NewValue;
}

void UCharacterStateViewModel::MaxHealthChanged(const FOnAttributeChangeData& Data) 
{
	MaxHealth = Data.NewValue;
}

