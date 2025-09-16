// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FrontendBlueprintFunctionLibrary.generated.h"

class UWidgetActivatableBase;
/**
 * 
 */
UCLASS()
class KITSUNE_API UFrontendBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Function Begin*/
	UFUNCTION(BlueprintPure, Category = "Frontend Function Library")
	static TSoftClassPtr<UWidgetActivatableBase> GetScreenSoftWidgetClassByTag(UPARAM(meta=(Categories="UI.Widget")) const FGameplayTag InWidgetTag);

	UFUNCTION(BlueprintPure, Category = "Frontend Function Library")
	static TSoftObjectPtr<UTexture2D> GetKeyIconByFKey(const FKey& InCurrentKey);
	/** Function End*/
};
