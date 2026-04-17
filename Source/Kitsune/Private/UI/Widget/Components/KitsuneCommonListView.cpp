// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/KitsuneCommonListView.h"

#include "FrontendDebugHelper.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/InventoryItemTrait.h"
#include "Inventory/Trait/ItemTrait_Interact.h"
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

	if (UInventoryItemInstance* ItemInstance = Cast<UInventoryItemInstance>(Item))
	{
		const UItemTrait_Interact* TraitInteract = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Interact>(ItemInstance->GetItemDef());
		if (!TraitInteract)
		{
			Debug::Print(TEXT("无交互信息"));
			return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
		}
		if (const auto FoundEntryClass = ListEntryMapping->FindEntryClassByItemTrait(TraitInteract))
		{
			return GenerateTypedEntry<UWidgetListEntryBase>(FoundEntryClass, OwnerTable);
		}
	}
	else
	{
		if (const auto FoundEntryClass = ListEntryMapping->FindEntryClassByDataObject(CastChecked<UListDataObjectBase>(Item)))
		{
			return GenerateTypedEntry<UWidgetListEntryBase>(FoundEntryClass, OwnerTable);
		}
	}
	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}

bool UKitsuneCommonListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	if (!FirstSelectedItem)return false;
	
	return !FirstSelectedItem->IsA<UListDataObjectCollection>();
}
