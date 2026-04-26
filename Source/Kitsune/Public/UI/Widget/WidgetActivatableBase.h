// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "WidgetActivatableBase.generated.h"

class UPlayerViewModel;
class AKitsunePlayerController;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class KITSUNE_API UWidgetActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	AKitsunePlayerController* GetOwningKitsunePlayerController();
	
	UFUNCTION(BlueprintCallable)
	UPlayerViewModel* GetLocalPlayerViewModel();
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPlayerViewModel> CachedLocalViewModel;
private:
	TWeakObjectPtr<AKitsunePlayerController> CachedOwningKitsunePC;

};
