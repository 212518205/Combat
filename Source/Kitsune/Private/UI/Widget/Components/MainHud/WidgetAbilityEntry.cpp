// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/MainHud/WidgetAbilityEntry.h"

#include "Components/Image.h"

void UWidgetAbilityEntry::AbilityEntryInitialize_Implementation(const FAbilityUIData& Data)
{
	// [VMREFACTOR] 原来是 Data.SpecHandle.IsValid()，改为槽位有效性判断（数据结构里已无 GAS 句柄）
	if (!Data.IsValid()) return;

	if (Image_Ability)
	{
		Image_Ability->SetBrushFromTexture(Data.Icon);
	}

	if (Image_TriggerKey)
	{
		Image_TriggerKey->SetBrushFromTexture(Data.AbilityTriggerKeyIcon);
	}

	SetVisibility(ESlateVisibility::Visible);
}

void UWidgetAbilityEntry::ClearAbilityEntry_Implementation()
{
	if (Image_Ability)
	{
		Image_Ability->SetBrushFromTexture(nullptr);
	}

	if (Image_TriggerKey)
	{
		Image_TriggerKey->SetBrushFromTexture(nullptr);
	}

	SetVisibility(ESlateVisibility::Collapsed);
}
