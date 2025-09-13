﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Option/ListEntries/WidgetListEntryBase.h"

#include "CommonTextBlock.h"
#include "UI/DataObjects/ListDataObjectBase.h"

void UWidgetListEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	OnOwningListDataHandle(CastChecked<UListDataObjectBase>(ListItemObject));
}

void UWidgetListEntryBase::OnOwningListDataHandle(UListDataObjectBase* ItemData)
{
	
}
