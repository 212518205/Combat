// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UserWidgetHealth.h"

#include "UI/ViewModel/CharacterStateViewModel.h"

void UUserWidgetHealth::SetViewModel(const FModelSet& ModelSet) 
{
	if (CharStateVM)
	{
		UE_LOG(LogTemp,Warning,TEXT("CharStateVM already init"))
	}

	CharStateVM = NewObject<UCharacterStateViewModel>(this, CharStateVMClass);
	CharStateVM->Initialize(ModelSet);
	if (CharStateVM)
	{
		CharStateVM->OnHealthChanged.AddDynamic(this, &UUserWidgetHealth::UpdateHealthDisplay);
		CharStateVM->OnMaxHealthChanged.AddDynamic(this, &UUserWidgetHealth::UpdateMaxHealthDisplay);

		Health = 100;
		MaxHealth = 200;
	}
}


void UUserWidgetHealth::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUserWidgetHealth::UpdateHealthDisplay(float NewHealth)
{
	Health = NewHealth;
	OnHealthChangedEvent.Broadcast(Health);
	UE_LOG(LogTemp, Warning, TEXT("In UpdateHealth UpdateHealthDisplay: Health=%f, "
		"MaxHealth=%f, Percent=%f"), Health, MaxHealth, GetHealthPercent());
}

void UUserWidgetHealth::UpdateMaxHealthDisplay(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
	OnMaxHealthChangedEvent.Broadcast(MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("In UpdateMaxHealth UpdateHealthDisplay: Health=%f, "
		"MaxHealth=%f, Percent=%f"), Health, MaxHealth, GetHealthPercent());
}
