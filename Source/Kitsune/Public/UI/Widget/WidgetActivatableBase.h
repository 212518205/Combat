// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "WidgetActivatableBase.generated.h"

class AKitsunePlayerController;
/**
 * 
 */
UCLASS(Abstract,BlueprintType,meta=(DisableNaiveTick))
class KITSUNE_API UWidgetActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	/** Function Begin*/
	UFUNCTION(BlueprintPure)
	AKitsunePlayerController* GetOwningKitsunePlayerController();
	/** Function End*/
private:
	/** Variable Begin*/
	TWeakObjectPtr<AKitsunePlayerController> CachedOwningKitsunePC;
	/** Variable End*/

};
