// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "KitsuneActivatableWidgetStack.generated.h"

class UWidgetActivatableBase;
/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneActivatableWidgetStack : public UCommonActivatableWidgetContainerBase
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintCallable)
	void PopWidget();
	
	UFUNCTION(BlueprintCallable)
	void ClearStack();
	
protected:
	virtual void OnWidgetAddedToList(UCommonActivatableWidget& AddedWidget) override;
	
};
