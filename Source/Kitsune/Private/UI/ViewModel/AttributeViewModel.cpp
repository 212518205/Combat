// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/AttributeViewModel.h"

#include "FrontendDebugHelper.h"

void UAttributeViewModel::BindCallback()
{
	Super::BindCallback();

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			Health = Data.NewValue;
			Debug::Print(TEXT("New Value: %f")+FString::FromInt(Health));
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxHealth = Data.NewValue;
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			Stamina = Data.NewValue;
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxStamina = Data.NewValue;
		}
	);
}
