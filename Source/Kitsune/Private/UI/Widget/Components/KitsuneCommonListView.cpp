// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/KitsuneCommonListView.h"

#include "UI/DataObjects/ListDataObjectCollection.h"
#include "UI/Widget/Components/ListEntryMapping.h"
#include "UI/Widget/Option/ListEntries/WidgetListEntryBase.h"

UUserWidget& UKitsuneCommonListView::OnGenerateEntryWidgetInternal(UObject* Item,
                                                                   TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}

	if (const auto FoundEntryClass=ListEntryMapping->FindEntryClassByDataObject(CastChecked<UListDataObjectBase>(Item)))
	{
		return GenerateTypedEntry<UWidgetListEntryBase>(FoundEntryClass, OwnerTable);
	}
	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

bool UKitsuneCommonListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	return !FirstSelectedItem->IsA<UListDataObjectCollection>();
}
