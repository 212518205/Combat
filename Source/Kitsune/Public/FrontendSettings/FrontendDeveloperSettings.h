// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "FrontendDeveloperSettings.generated.h"

class UWidgetActivatableBase;
/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Frontend UI Settings"))
class KITSUNE_API UFrontendDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta = (ForceInlineRow, Categories = "UI.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UWidgetActivatableBase>> FrontendWidgetMap;
	
	UPROPERTY(Config, EditAnywhere, Category = "UIInfo", meta = (Categories = "UI.CategoryDisplay"))
	TMap<FGameplayTag, FCategoryInfoGroup> CategoryDisplayGroups;

};
