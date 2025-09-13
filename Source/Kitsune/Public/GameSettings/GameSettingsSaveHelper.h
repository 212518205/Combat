// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UKitsuneGameUserSettings;
/**
 * 
 */
class KITSUNE_API FGameSettingsSaveHelper
{
public:
	FGameSettingsSaveHelper();
	// ReSharper disable once CppNonExplicitConvertingConstructor
	FGameSettingsSaveHelper(const FString& InFunctionPath);

	FString GetValueAsString() const;
	void SetValueFromString(const FString& InValue) const;

private:
	/***  Cached variable ****/
	TWeakObjectPtr<UKitsuneGameUserSettings> CachedGameUserSettings;

	FString CachedDynamicFunctionPath;
};
