// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/MainHud/WidgetAbilityEntry.h"

#include "Components/Image.h"

void UWidgetAbilityEntry::AbilityEntryInitialize_Implementation(const FAbilityUIData& Data)
{	
	if (!Data.SpecHandle.IsValid())return;
	Image_Ability->SetBrushFromTexture(Data.Icon);
	Image_TriggerKey->SetBrushFromTexture(Data.AbilityTriggerKeyIcon);
	SetVisibility(ESlateVisibility::Visible);
}

void UWidgetAbilityEntry::ClearAbilityEntry_Implementation()
{	
	Image_Ability->SetBrushFromTexture(nullptr);
	Image_TriggerKey->SetBrushFromTexture(nullptr);
	SetVisibility(ESlateVisibility::Collapsed);
}
