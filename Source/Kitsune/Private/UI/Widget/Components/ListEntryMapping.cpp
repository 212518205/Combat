// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/ListEntryMapping.h"
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
