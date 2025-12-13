// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UIManagerSubsystem.h"
#include "Characters/CharacterBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "FrontendBlueprintFunctionLibrary.generated.h"

class UKitsuneAttributeSet;
class UWidgetActivatableBase;
/**
 * 
 */
UCLASS()
class KITSUNE_API UFrontendBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Frontend | Screen")
	static TSoftClassPtr<UWidgetActivatableBase> GetScreenSoftWidgetClassByTag(UPARAM(meta=(Categories="UI.Widget"))
		const FGameplayTag InWidgetTag);

	UFUNCTION(BlueprintPure, Category = "Frontend | ViewModel", meta = (WorldContext = "WorldContextObject"))
	static UPlayerViewModel* GetPlayerViewModel(const UObject* WorldContextObject);

};

