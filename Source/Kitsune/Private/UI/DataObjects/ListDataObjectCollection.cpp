// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DataObjects/ListDataObjectCollection.h"

void UListDataObjectCollection::AddChildListData(UListDataObjectBase* InChildListData)
{
	InChildListData->InitDataObject();
	InChildListData->SetParentData(this);

	ChildListData.Add(InChildListData);
}

TArray<UListDataObjectBase*> UListDataObjectCollection::GetChildListData() const
{
	return ChildListData;
}

bool UListDataObjectCollection::HasChildListData() const
{
	return !ChildListData.IsEmpty();
}
