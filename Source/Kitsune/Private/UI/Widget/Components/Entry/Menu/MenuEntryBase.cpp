// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/Entry/Menu/MenuEntryBase.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "UI/DataObjects/Inventory/MenuEntryData.h"

void UMenuEntryBase::NativeOnEntryInitialize(UObject* ListItemObject) const
{
	if (const UMenuEntryData* EntryData = Cast<UMenuEntryData>(ListItemObject))
	{
		Image_DisplayImage->SetBrushFromTexture(EntryData->DisplayIcon);
		CommonTextBlock_DisplayName->SetText(EntryData->DisplayName);
		CommonTextBlock_HotKeyText->SetText(EntryData->HotKeyText);
		CommonTextBlock_ShowAsNewText->SetText(EntryData->ShowAsNewText);
	}

}
