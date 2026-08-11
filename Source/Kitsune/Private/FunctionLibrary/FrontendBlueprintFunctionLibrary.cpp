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

FCategoryInfoGroup UFrontendBlueprintFunctionLibrary::GetCategoryNameByModuleTag(const FGameplayTag ModuleTag)
{
	const UFrontendDeveloperSettings* Settings = GetDefault<UFrontendDeveloperSettings>();
	
	checkf(Settings->CategoryDisplayGroups.Contains(ModuleTag),
		TEXT("无该 %s 模块对应的分类名字"), *ModuleTag.ToString());
	
	return Settings->CategoryDisplayGroups.FindRef(ModuleTag);
}



