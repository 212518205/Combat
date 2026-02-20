// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/PlayerViewModel.h"
#include "UI/Widget/WidgetActivatableBase.h"
#include "WidgetMainHudScreen.generated.h"

class UCommonTextBlock;
class UKitsuneCommonListView;
class UInventoryItemInstance;
class UPlayerViewModel;
/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetMainHudScreen : public UWidgetActivatableBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void OnInteractableItemChange(UInventoryItemInstance* ItemInstance, EItemInstanceAction InstanceAction);

	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void InitializeMainHudScreen();

	UPROPERTY(BlueprintReadOnly, Category = "ViewModel")
	TObjectPtr<UPlayerViewModel> CachedPlayerViewModel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UKitsuneCommonListView> CommonListView_Prompt;

};
