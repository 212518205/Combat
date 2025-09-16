// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"

#include "FrontendSettings/FrontendDeveloperSettings.h"

TSoftClassPtr<UWidgetActivatableBase> UFrontendBlueprintFunctionLibrary::GetScreenSoftWidgetClassByTag(
	UPARAM(meta = (Categories = "UI.Widget")) const FGameplayTag InWidgetTag)
{
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	checkf(FrontendDeveloperSettings->FrontendWidgetMap.Contains(InWidgetTag),
		TEXT("Could not find the corresponding widget under the tag %s"), *InWidgetTag.ToString());

	return FrontendDeveloperSettings->FrontendWidgetMap.FindRef(InWidgetTag);
}

TSoftObjectPtr<UTexture2D> UFrontendBlueprintFunctionLibrary::GetKeyIconByFKey(const FKey& InCurrentKey)
{
	const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
	checkf(FrontendDeveloperSettings->KeyIconMapping.Contains(InCurrentKey),
		TEXT("Could not find the key icon under the key %s"), *InCurrentKey.ToString());

	return FrontendDeveloperSettings->KeyIconMapping.FindRef(InCurrentKey);
}

