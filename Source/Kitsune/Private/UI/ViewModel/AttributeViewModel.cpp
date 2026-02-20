// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/AttributeViewModel.h"

#include "FrontendDebugHelper.h"
#include "Characters/KitsuneCharacter.h"


void UAttributeViewModel::NativeInitialize()
{
	Super::NativeInitialize();

	if (const ACharacterBase* OwningCharacter = Cast<ACharacterBase>(OwningPawn))
	{
		CachedKitsuneASC = Cast<UKitsuneAbilitySystemComponent>(OwningCharacter->GetAbilitySystemComponent());
		CachedKitsuneAttributeSet = Cast<UKitsuneAttributeSet>(OwningCharacter->GetAttributeSet());
	}

	if (!(CachedKitsuneAttributeSet && CachedKitsuneASC))return;
	CachedKitsuneASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			Health = Data.NewValue;
			OnHealthPercentChanged.Broadcast();
		}
	);

	CachedKitsuneASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxHealth = Data.NewValue;
			OnHealthPercentChanged.Broadcast();
		}
	);

	CachedKitsuneASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			Stamina = Data.NewValue;
			OnStaminaPercentChanged.Broadcast();
		}
	);

	CachedKitsuneASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxStamina = Data.NewValue;
			OnStaminaPercentChanged.Broadcast();
		}
	);
}
