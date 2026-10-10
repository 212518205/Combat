
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CommonTypes/StructTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KitsuneWidgetFunctionLibrary.generated.h"

class UKitsuneAttributeSet;
class UWidgetActivatableBase;
/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneWidgetFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "UI | Screen")
	static TSoftClassPtr<UWidgetActivatableBase> GetScreenSoftWidgetClassByTag(UPARAM(meta=(Categories="UI.Widget"))
		const FGameplayTag InWidgetTag);

	UFUNCTION(BlueprintPure, Category = "UI | Display")
	static FCategoryInfoGroup GetCategoryNameByModuleTag(UPARAM(meta=(Categories="UI.CategoryDisplay"))
		const FGameplayTag ModuleTag);

};

