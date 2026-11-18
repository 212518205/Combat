// [VMREFACTOR] 新增文件：交互镜像
#include "UI/ViewModel/InteractionViewModel.h"

#include "Component/Interaction/InteractionComponent.h"
#include "Inventory/InventoryItemInstance.h"

void UInteractionViewModel::Deinitialize()
{
	OverlappedItemInstances.Reset();
	CachedComp.Reset();

	Super::Deinitialize();
}

void UInteractionViewModel::BindInteractionComponent(UInteractionComponent* Comp)
{
	if (!Comp)
	{
		UE_LOG(LogTemp, Warning, TEXT("[VMREFACTOR] UInteractionViewModel: 交互组件未就绪，等待下一次 Bind"));
		return;
	}

	Deinitialize();
	CachedComp = Comp;

	MarkReady();
}

void UInteractionViewModel::AddInteractableItemInstance(UInventoryItemInstance* ItemInstance)
{
	if (!ItemInstance || OverlappedItemInstances.Contains(ItemInstance)) return;

	OverlappedItemInstances.Add(ItemInstance);
	OnInteractableItemChange.Broadcast(ItemInstance, EItemInstanceAction::EAddInstance);
}

void UInteractionViewModel::RemoveInteractableItemInstance(UInventoryItemInstance* ItemInstance)
{
	if (!ItemInstance || OverlappedItemInstances.Remove(ItemInstance) == 0) return;

	OnInteractableItemChange.Broadcast(ItemInstance, EItemInstanceAction::ERemoveInstance);
}
