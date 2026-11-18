// [VMREFACTOR] 新增文件：数据源 = UInteractionComponent
#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "InteractionViewModel.generated.h"

class UInventoryItemInstance;
class UInteractionComponent;

/** [VMREFACTOR] 从 PlayerViewModel.h 搬过来的枚举 */
UENUM()
enum class EItemInstanceAction : uint8
{
	EAddInstance = 1,
	ERemoveInstance = 2,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractableItemChangeDelegate, UInventoryItemInstance*, ItemInstance, EItemInstanceAction, InstanceAction);

/**
 * [VMREFACTOR] 重叠可交互物：只给 IsLocallyControlled() 的 Pawn 建
 */
UCLASS()
class KITSUNE_API UInteractionViewModel : public UViewModelBase
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;

	/** [VMREFACTOR] 幂等：只记组件引用，真正的数据由组件推送进来 */
	void BindInteractionComponent(UInteractionComponent* Comp);

	void AddInteractableItemInstance(UInventoryItemInstance* ItemInstance);
	void RemoveInteractableItemInstance(UInventoryItemInstance* ItemInstance);

	const TArray<UInventoryItemInstance*>& GetOverlappedItemInstances() const { return OverlappedItemInstances; }

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnInteractableItemChangeDelegate OnInteractableItemChange;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "ViewModel | Interact")
	TArray<UInventoryItemInstance*> OverlappedItemInstances;

	/** [VMREFACTOR] 只存 weak：不反向保活组件 */
	TWeakObjectPtr<UInteractionComponent> CachedComp;
};
