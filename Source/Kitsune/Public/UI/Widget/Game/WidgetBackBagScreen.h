// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventorySystemComponent.h"
#include "UI/Widget/WidgetActivatableBase.h"
#include "WidgetBackBagScreen.generated.h"

class UKitsuneCommonTileView;
class UKitsuneTabListWidgetBase;
/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetBackBagScreen : public UWidgetActivatableBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnInitialized() override;
	
	TArray<TPair<FName, FInventoryCategoryGroup>> GetAllCategoryItems();
	
	UPROPERTY(meta = (BindWidget))
	UKitsuneTabListWidgetBase* TabList_BagCategory;
	
	UPROPERTY(meta = (BindWidget))
	UKitsuneCommonTileView* TileView_BackBag;
	
	UFUNCTION()
	void OnTabSelected(const FName TabID);
	
private:
	TArray<UInventorySlotData*> BuildCategorySlots(const FName& CategoryID, TArray<UInventorySlotData*>& Slots);
	
};
