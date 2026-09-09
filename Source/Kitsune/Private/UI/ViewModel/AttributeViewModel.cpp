// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/AttributeViewModel.h"

#include "Characters/KitsuneCharacter.h"


bool UAttributeViewModel::NativeInitialize()
{
	Super::NativeInitialize();

	if (const ACharacterBase* OwningCharacter = Cast<ACharacterBase>(OwningPawn))
	{
		CachedKitsuneASC = Cast<UKitsuneAbilitySystemComponent>(OwningCharacter->GetAbilitySystemComponent());
		CachedKitsuneAttributeSet = Cast<UKitsuneAttributeSet>(OwningCharacter->GetAttributeSet());
	}
	else
	{
		return false;
	}

	if (!(CachedKitsuneAttributeSet && CachedKitsuneASC))return false;
	CachedKitsuneASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			Health = Data.NewValue;
			OnHealthPercentChanged.Broadcast();
		});

	CachedKitsuneASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxHealth = Data.NewValue;
			OnHealthPercentChanged.Broadcast();
		});

	CachedKitsuneASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			Stamina = Data.NewValue;
			OnStaminaPercentChanged.Broadcast();
		});

	CachedKitsuneASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxStamina = Data.NewValue;
			OnStaminaPercentChanged.Broadcast();
		});
	
	CachedKitsuneASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetVigorAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			Vigor = Data.NewValue;
			OnVigorPercentChanged.Broadcast();
		});
	
	CachedKitsuneASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxVigorAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxVigor = Data.NewValue;
			OnVigorPercentChanged.Broadcast();
		});
	
	CachedKitsuneASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetDamageTakenAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			TakenDamage = Data.NewValue;
			OnTakenDamageChanged.Broadcast();
		});
	
	Health    = CachedKitsuneASC->GetNumericAttribute(UKitsuneAttributeSet::GetHealthAttribute());
	MaxHealth = CachedKitsuneASC->GetNumericAttribute(UKitsuneAttributeSet::GetMaxHealthAttribute());
	Stamina   = CachedKitsuneASC->GetNumericAttribute(UKitsuneAttributeSet::GetStaminaAttribute());
	MaxStamina= CachedKitsuneASC->GetNumericAttribute(UKitsuneAttributeSet::GetMaxStaminaAttribute());
	Vigor     = CachedKitsuneASC->GetNumericAttribute(UKitsuneAttributeSet::GetVigorAttribute());
	MaxVigor  = CachedKitsuneASC->GetNumericAttribute(UKitsuneAttributeSet::GetMaxVigorAttribute());

	OnHealthPercentChanged.Broadcast();
	OnStaminaPercentChanged.Broadcast();
	OnVigorPercentChanged.Broadcast();
	
	return true;
}
