// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/ListEntryMapping.h"

#include "Inventory/InventoryItemInstance.h"
#include "Inventory/InventoryItemTrait.h"
#include "UI/DataObjects/ListDataObjectBase.h"

TSubclassOf<UWidgetListEntryBase> UListEntryMapping::FindEntryClassByDataObject(const UListDataObjectBase* InListDataObject) const
{
	check(InListDataObject)

	for (UClass* DataObjectKey=InListDataObject->GetClass();DataObjectKey;DataObjectKey=DataObjectKey->GetSuperClass())
	{
		if (TSubclassOf<UListDataObjectBase> ConvertedDataObjectClass = TSubclassOf<UListDataObjectBase>(DataObjectKey))
		{
			if (ListEntryClassMapping.Contains(ConvertedDataObjectClass))
			{
				return ListEntryClassMapping.FindRef(ConvertedDataObjectClass);
			}
		}
	}

	return {};
}

TSubclassOf<UWidgetListEntryBase> UListEntryMapping::FindEntryClassByItemInstance(
	UInventoryItemInstance* InItemInstance) const
{
	check(InItemInstance);

	for (UClass* DataObjectKey = InItemInstance->GetClass(); DataObjectKey; DataObjectKey = DataObjectKey->GetSuperClass())
	{
		if (TSubclassOf<UInventoryItemInstance> ConvertedItemInstanceClass = TSubclassOf<UInventoryItemInstance>(DataObjectKey))
		{
			if (InteractListEntryMapping.Contains(ConvertedItemInstanceClass))
			{
				return InteractListEntryMapping.FindRef(ConvertedItemInstanceClass);
			}
		}
	}
	return {};
}

