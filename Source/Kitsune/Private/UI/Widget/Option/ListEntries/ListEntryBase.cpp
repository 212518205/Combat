// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Option/ListEntries/ListEntryBase.h"

#include "CommonTextBlock.h"
#include "UI/DataObjects/ListDataObjectBase.h"

void UListEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	OnOwningListDataHandle(ListItemObject);
}

void UListEntryBase::OnOwningListDataHandle(UObject* ItemData)
{
}
