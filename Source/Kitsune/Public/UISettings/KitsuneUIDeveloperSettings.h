// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "CommonTypes/StructTypes.h"
#include "KitsuneUIDeveloperSettings.generated.h"

class UWidgetActivatableBase;
/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Kitsune UI Settings"))
class KITSUNE_API UKitsuneUIDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta = (ForceInlineRow, Categories = "UI.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UWidgetActivatableBase>> WidgetClassMap;
	
	UPROPERTY(Config, EditAnywhere, Category = "UIInfo", meta = (Categories = "UI.CategoryDisplay"))
	TMap<FGameplayTag, FCategoryInfoGroup> CategoryDisplayGroups;

};
