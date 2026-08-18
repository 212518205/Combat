// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "Inventory/InventoryItemInstance.h"
#include "InventorySlotData.generated.h"

class UInventoryItemInstance;

/**
 * 背包槽位数据：一个槽位对应背包界面上的一格（可能是物品、空格子或锁定格）
 */
UCLASS(BlueprintType)
class KITSUNE_API UInventorySlotData : public UObject
{
	GENERATED_BODY()
public:
	UInventorySlotData(){}

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryItemInstance> ItemInstance = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bIsLocked = false;

	UPROPERTY(BlueprintReadOnly)
	int32 UnlockCost = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int32 StackCount = 0;
	
	UPROPERTY(BlueprintReadOnly)
	FName CategoryID;
	
	FText GetUnlockMessage()
	{
		TMap<FName, FInventoryInfo> CategoryInfo = UFrontendBlueprintFunctionLibrary::GetCategoryNameByModuleTag(
			 KitsuneGameplayTags::UI_CategoryDisplay_Inventory_Item).CategoryInfo;
		int32 OnceUnlockCount = 7;
		if (const FInventoryInfo* Info = CategoryInfo.Find(CategoryID))
		{
			OnceUnlockCount = Info->OnceUnlockCount;
		}
		
		return FText::Format(
			NSLOCTEXT("Inventory", "UnlockSlotConfirm", "是否花费 {0} 金币解锁 {1} 个格子？"),
			FText::AsNumber(UnlockCost),
			FText::AsNumber(OnceUnlockCount)
		);
	}
};


/**
 * 分类分组：按 CategoryID 聚合的一组槽位，用于背包分类 Tab 展示
 */
USTRUCT()
struct FInventoryCategoryGroup
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FText CategoryDisplayName;

	UPROPERTY()
	TArray<UInventorySlotData*> CategorySlots;
};
