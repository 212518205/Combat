// [VMREFACTOR] 新增文件：数据源 = UInventoryComponent
#pragma once

#include "CoreMinimal.h"
#include "CommonTypes/EnumTypes.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "InventoryViewModel.generated.h"

class UInventoryComponent;
class UInventoryItemInstance;
class UInventorySlotData;

/** [VMREFACTOR] 从 PlayerViewModel.h 搬过来的分类镜像容器 */
USTRUCT()
struct FInventoryCategoryItems
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TArray<TObjectPtr<UInventoryItemInstance>> Items;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCategoryCapacityChangedDelegate, FName, CategoryID, int32, CategoryCapacity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChangedDelegate, UInventoryItemInstance*, ItemInstance, EInventoryItemChangeType, ChangeType);

/**
 * [VMREFACTOR] 背包镜像：只给 IsLocallyControlled() 的 Pawn 建（背包数据只有自主代理有）
 */
UCLASS()
class KITSUNE_API UInventoryViewModel : public UViewModelBase
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;

	/** [VMREFACTOR] 幂等：先解绑再绑 */
	void BindInventoryComponent(UInventoryComponent* ISC);

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnCategoryCapacityChangedDelegate CategoryCapacityChanged;

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnInventoryItemChangedDelegate OnInventoryItemChanged;

	/***  库存查询   ***/
	TArray<TPair<FName, FText>> GetAllInventoryCategories() const;
	TArray<UInventorySlotData*> GetDisplaySlotsByCategory(FName CategoryID) const;
	int32 GetCategoryCapacity(const FName CategoryID) const;

private:
	UFUNCTION()
	void HandleCategoryCapacityChanged(FName CategoryID, int32 CategoryCapacity);

	UFUNCTION()
	void HandleInventoryItemChanged(UInventoryItemInstance* ItemInstance, EInventoryItemChangeType ChangeType);

	void RebuildInventoryMirror(const UInventoryComponent* ISC);

	UPROPERTY(Transient)
	TMap<FName, int32> CapacityByCategory;

	UPROPERTY(Transient)
	TMap<FName, FInventoryCategoryItems> ItemsByCategory;

	/** [VMREFACTOR] 只存 weak 数据源：不反向保活 Gameplay 对象 */
	TWeakObjectPtr<UInventoryComponent> CachedISC;
};
