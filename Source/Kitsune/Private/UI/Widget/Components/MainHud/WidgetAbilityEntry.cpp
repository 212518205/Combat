// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/MainHud/WidgetAbilityEntry.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UWidgetAbilityEntry::AbilityInitialize(const FAbilityUIData& Data)
{
	if (!Data.SpecHandle.IsValid())return;
	Image_Ability->SetBrushFromTexture(Data.Icon);
	TextBlock_Key->SetText(Data.TriggerKey.GetDisplayName());
}
