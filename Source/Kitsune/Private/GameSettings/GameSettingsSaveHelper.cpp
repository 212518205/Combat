// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/GameSettingsSaveHelper.h"

#include "PropertyPathHelpers.h"
#include "GameSettings/KitsuneGameUserSettings.h"

FGameSettingsSaveHelper::FGameSettingsSaveHelper()
{
}

FGameSettingsSaveHelper::FGameSettingsSaveHelper(const FString& InFunctionPath):
	CachedDynamicFunctionPath(InFunctionPath)
{
	CachedGameUserSettings = UKitsuneGameUserSettings::GetGameUserSettings();
}

FString FGameSettingsSaveHelper::GetValueAsString() const
{
	FString OutFString;

	PropertyPathHelpers::GetPropertyValueAsString(
		CachedGameUserSettings.Get(),
		CachedDynamicFunctionPath,
		OutFString
	);
	return OutFString;
}

void FGameSettingsSaveHelper::SetValueFromString(const FString& InValue) const
{
	PropertyPathHelpers::SetPropertyValueFromString(
		CachedGameUserSettings.Get(),
		CachedDynamicFunctionPath,
		InValue
	);
}


