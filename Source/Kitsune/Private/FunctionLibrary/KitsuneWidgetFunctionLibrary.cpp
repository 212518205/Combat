// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/KitsuneWidgetFunctionLibrary.h"

#include "UISettings/KitsuneUIDeveloperSettings.h"

TSoftClassPtr<UWidgetActivatableBase> UKitsuneWidgetFunctionLibrary::GetScreenSoftWidgetClassByTag(
	UPARAM(meta = (Categories = "UI.Widget")) const FGameplayTag InWidgetTag)
{
	const UKitsuneUIDeveloperSettings* KitsuneUIDeveloperSettings = GetDefault<UKitsuneUIDeveloperSettings>();
	checkf(KitsuneUIDeveloperSettings->WidgetClassMap.Contains(InWidgetTag),
		TEXT("Could not find the corresponding widget under the tag %s"), *InWidgetTag.ToString());

	return KitsuneUIDeveloperSettings->WidgetClassMap.FindRef(InWidgetTag);
}

FCategoryInfoGroup UKitsuneWidgetFunctionLibrary::GetCategoryNameByModuleTag(const FGameplayTag ModuleTag)
{
	const UKitsuneUIDeveloperSettings* Settings = GetDefault<UKitsuneUIDeveloperSettings>();
	
	checkf(Settings->CategoryDisplayGroups.Contains(ModuleTag),
		TEXT("无该 %s 模块对应的分类名字"), *ModuleTag.ToString());
	
	return Settings->CategoryDisplayGroups.FindRef(ModuleTag);
}



