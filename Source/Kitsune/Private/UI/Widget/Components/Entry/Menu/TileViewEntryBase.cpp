// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/Entry/Menu/TileViewEntryBase.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "UI/DataObjects/Inventory/TileViewEntryData.h"

void UTileViewEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	if (const UTileViewEntryData* EntryData = Cast<UTileViewEntryData>(ListItemObject))
	{
		Image_DisplayImage->SetBrushFromTexture(EntryData->DisplayIcon);
		CommonTextBlock_DisplayName->SetText(EntryData->DisplayName);
		CommonTextBlock_HotKeyText->SetText(EntryData->HotKeyText);
		CommonTextBlock_ShowAsNewText->SetText(EntryData->ShowAsNewText);
	}

}
