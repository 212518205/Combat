// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/WidgetActivatableBase.h"
#include "WidgetMainHudScreen.generated.h"

class UPlayerViewModel;
/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetMainHudScreen : public UWidgetActivatableBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void InitializeMainHudScreen();

	UPROPERTY(BlueprintReadOnly, Category = "ViewModel")
	TObjectPtr<UPlayerViewModel> CachedPlayerViewModel;
};
