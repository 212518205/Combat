// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/AttributeViewModel.h"

#include "DebugHelper.h"
#include "Characters/KitsuneCharacter.h"
#include "FunctionLibrary/KitsuneFunctionLibrary.h"


bool UAttributeViewModel::NativeInitialize(APawn* InPawn)
{
	Super::NativeInitialize(InPawn);
	
	if (UKitsuneAbilitySystemComponent* ASC = UKitsuneFunctionLibrary::NativeGetKitsuneASCFromActor(InPawn))
	{
		BindAbilitySystemComponent(ASC);
	}
	else
	{
		Debug::Print(TEXT("无法获取技能系统组件"));
	}
	
	return true;
}

void UAttributeViewModel::BindAbilitySystemComponent(UKitsuneAbilitySystemComponent* ASC)
{
	Super::BindAbilitySystemComponent(ASC);
	
	ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			Health = Data.NewValue;
			OnHealthPercentChanged.Broadcast();
		});

	ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxHealth = Data.NewValue;
			OnHealthPercentChanged.Broadcast();
		});

	ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			Stamina = Data.NewValue;
			OnStaminaPercentChanged.Broadcast();
		});

	ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxStamina = Data.NewValue;
			OnStaminaPercentChanged.Broadcast();
		});
	
	ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetVigorAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			Vigor = Data.NewValue;
			OnVigorPercentChanged.Broadcast();
		});
	
	ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxVigorAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MaxVigor = Data.NewValue;
			OnVigorPercentChanged.Broadcast();
		});
	
	ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetDamageTakenAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			TakenDamage = Data.NewValue;
			OnTakenDamageChanged.Broadcast();
		});
	
	Health    = ASC->GetNumericAttribute(UKitsuneAttributeSet::GetHealthAttribute());
	MaxHealth = ASC->GetNumericAttribute(UKitsuneAttributeSet::GetMaxHealthAttribute());
	Stamina   = ASC->GetNumericAttribute(UKitsuneAttributeSet::GetStaminaAttribute());
	MaxStamina= ASC->GetNumericAttribute(UKitsuneAttributeSet::GetMaxStaminaAttribute());
	Vigor     = ASC->GetNumericAttribute(UKitsuneAttributeSet::GetVigorAttribute());
	MaxVigor  = ASC->GetNumericAttribute(UKitsuneAttributeSet::GetMaxVigorAttribute());

	OnHealthPercentChanged.Broadcast();
	OnStaminaPercentChanged.Broadcast();
	OnVigorPercentChanged.Broadcast();
}
