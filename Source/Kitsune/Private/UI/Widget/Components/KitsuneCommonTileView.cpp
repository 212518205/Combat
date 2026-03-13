// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/KitsuneCommonTileView.h"

#include "FrontendDebugHelper.h"
#include "UI/DataObjects/Inventory/TileViewEntryData.h"
#include "UI/Widget/Components/Entry/TileEntryMapping.h"
#include "UI/Widget/Components/Entry/Menu/TileViewEntryBase.h"

UUserWidget& UKitsuneCommonTileView::OnGenerateEntryWidgetInternal(UObject* Item,
                                                                   TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	UUserWidget& Entry = Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	if (const UTileViewEntryData* EntryData = Cast<UTileViewEntryData>(Item))
	{
		if (const TSubclassOf<UTileViewEntryBase> FoundEntryClass = TileEntryMapping->FindEntryClassByEntryType(EntryData->EntryType))
		{
			return GenerateTypedEntry(FoundEntryClass, OwnerTable);
		}
	}

	return Entry;
}

bool UKitsuneCommonTileView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	return Super::OnIsSelectableOrNavigableInternal(FirstSelectedItem);
}
