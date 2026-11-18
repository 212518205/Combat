// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "UI/Widget/WidgetActivatableBase.h"
#include "WidgetBackpackScreen.generated.h"

class UKitsuneCommonTileView;
class UKitsuneTabListWidgetBase;
// [VMREFACTOR] 背包全 C++ 消费，改为 UInventoryViewModel（蓝图零改动）
class UInventoryViewModel;
/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetBackpackScreen : public UWidgetActivatableBase
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnInitialized() override;
	
	TArray<TPair<FName, FText>> GetAllCategoryItems();

	/** [VMREFACTOR] 只 Find 不创建 */
	UInventoryViewModel* GetInventoryViewModel();
	
	UPROPERTY(meta = (BindWidget))
	UKitsuneTabListWidgetBase* TabList_BagCategory;
	
	UPROPERTY(meta = (BindWidget))
	UKitsuneCommonTileView* TileView_Backpack;
	
	UFUNCTION()
	void OnTabSelected(const FName TabID);
	
	UFUNCTION()
	void HandleCapacityChanged(FName CategoryID, int32 CategoryCapacity);
	
	UFUNCTION()
	void HandleInventoryItemChanged(UInventoryItemInstance* Instance, EInventoryItemChangeType ChangeType);
	
private:
	void RefreshCurrentTab();
	
	FName CurrentTabID = NAME_None; 
	
};
