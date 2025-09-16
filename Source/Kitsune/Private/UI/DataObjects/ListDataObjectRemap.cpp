// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DataObjects/ListDataObjectRemap.h"

bool UListDataObjectRemap::IsGamepadKey() const
{
	return CurrentKey.IsGamepadKey();
}

void UListDataObjectRemap::UserMappableKeyChanged(const FKey& InNewKey, const EOptionsListDataModifyReason Reason)
{
	if (Reason==EOptionsListDataModifyReason::ResetToDefault)
	{
		CurrentKey = DefaultKey;
	}else
	{
		CurrentKey = InNewKey;
	}

	FPlayerKeyMapping* Mapping = GetPlayerKeyMapping();
	Mapping->SetCurrentKey(CurrentKey);

	InputUserSettings->ApplySettings();
	InputUserSettings->SaveSettings();
}

FPlayerKeyMapping* UListDataObjectRemap::GetPlayerKeyMapping() const
{
	FMapPlayerKeyArgs KeyArgs;
	KeyArgs.MappingName = GetDataID();
	KeyArgs.Slot = CurrentKeySlot;

	return Profiles->FindKeyMapping(KeyArgs);
}
