// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/AttributeViewModel.h"


void UAttributeViewModel::BindCallback()
{
	Super::BindCallback();

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			Health = Data.NewValue;
			OnHealthPercentChanged.Broadcast();
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxHealth = Data.NewValue;
			OnHealthPercentChanged.Broadcast();
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			Stamina = Data.NewValue;
			OnStaminaPercentChanged.Broadcast();
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxStamina = Data.NewValue;
			OnStaminaPercentChanged.Broadcast();
		}
	);
}
