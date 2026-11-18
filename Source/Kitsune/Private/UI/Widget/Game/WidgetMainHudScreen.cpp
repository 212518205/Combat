#include "UI/Widget/Game/WidgetMainHudScreen.h"

#include "UIManagerSubsystem.h"
#include "Inventory/InventoryItemInstance.h"
#include "UI/ViewModel/ASCViewModel.h"
#include "UI/ViewModel/InteractionViewModel.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "UI/ViewModel/ViewModelWorldSubsystem.h"
#include "UI/Widget/Components/KitsuneCommonListView.h"
#include "UI/Widget/Components/MainHud/WidgetAbilityList.h"

UInteractionViewModel* UWidgetMainHudScreen::GetInteractionViewModel() const
{
	const UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this);
	return VMReg ? VMReg->Find<UInteractionViewModel>(GetOwningPlayerPawn()) : nullptr;
}

void UWidgetMainHudScreen::TryBindViewModels()
{
	UInteractionViewModel* InteractVM = GetInteractionViewModel();
	if (BoundInteractionViewModel.Get() == InteractVM) return;

	if (UInteractionViewModel* OldVM = BoundInteractionViewModel.Get())
	{
		OldVM->OnInteractableItemChange.RemoveDynamic(this, &ThisClass::OnInteractableItemChange);
	}
	BoundInteractionViewModel = nullptr;

	if (!InteractVM || !InteractVM->IsViewModelReady()) return;

	InteractVM->OnInteractableItemChange.AddUniqueDynamic(this, &ThisClass::OnInteractableItemChange);
	BoundInteractionViewModel = InteractVM;
}

void UWidgetMainHudScreen::HandleViewModelReady(AActor* Owner, UViewModelBase* VM)
{
	if (Owner != GetOwningPlayerPawn() || !VM) return;

	TryBindViewModels();
}

void UWidgetMainHudScreen::InitializeMainHudScreen()
{
	CachedUIManager = UUIManagerSubsystem::GetUIManager(GetOwningPlayer());

	TryBindViewModels();

	if (const UASCViewModel* ASCVM = GetViewModel<UASCViewModel>())
	{
		ASCVM->OnHealthPercentChanged.Broadcast();
		ASCVM->OnStaminaPercentChanged.Broadcast();
		ASCVM->OnVigorPercentChanged.Broadcast();
	}
}

void UWidgetMainHudScreen::OnInteractableItemChange_Implementation(UInventoryItemInstance* ItemInstance,
                                                                   EItemInstanceAction InstanceAction)
{
	if (!ItemInstance)return;

	check(CommonListView_Prompt);

	const UInteractionViewModel* InteractVM = BoundInteractionViewModel ? BoundInteractionViewModel.Get() : GetInteractionViewModel();
	const bool bOverlapped = InteractVM && InteractVM->GetOverlappedItemInstances().Contains(ItemInstance);

	switch (InstanceAction)
	{
	case EItemInstanceAction::EAddInstance:
		if (bOverlapped)
		{
			CommonListView_Prompt->AddItem(ItemInstance);
		}
		break;

	case EItemInstanceAction::ERemoveInstance:
		if (!bOverlapped)
		{
			CommonListView_Prompt->RemoveItem(ItemInstance);
		}
		break;

	default:
		break;
	}

	CommonListView_Prompt->RequestRefresh();
}

UInventoryItemInstance* UWidgetMainHudScreen::GetSelectedItemInstance() const
{
	if (!CommonListView_Prompt) return nullptr;

	UInventoryItemInstance* ItemInstance = CommonListView_Prompt->GetSelectedItem<UInventoryItemInstance>();
	if (CommonListView_Prompt->GetListItems().Num() == 0)return nullptr;
	if (!ItemInstance)
	{
		ItemInstance = Cast<UInventoryItemInstance>(CommonListView_Prompt->GetItemAt(0));
	}
	
	return ItemInstance;
}

void UWidgetMainHudScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	TryBindViewModels();

	if (!GetViewModel<UASCViewModel>())
	{
		if (UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this))
		{
			VMReg->OnViewModelReady.AddUObject(this, &ThisClass::HandleViewModelReady);
		}
	}
}

void UWidgetMainHudScreen::NativeOnDeactivated()
{
	if (UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this))
	{
		VMReg->OnViewModelReady.RemoveAll(this);
	}

	Super::NativeOnDeactivated();
}

void UWidgetMainHudScreen::NativeDestruct()
{
	if (UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this))
	{
		VMReg->OnViewModelReady.RemoveAll(this);
	}

	if (UInteractionViewModel* InteractVM = BoundInteractionViewModel.Get())
	{
		InteractVM->OnInteractableItemChange.RemoveAll(this);
	}
	BoundInteractionViewModel = nullptr;

	Super::NativeDestruct();
}

FReply UWidgetMainHudScreen::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!CommonListView_Prompt)return FReply::Unhandled();
	if (const float WheelDelta = InMouseEvent.GetWheelDelta(); WheelDelta != 0.f)
	{
		const int32 Offset = WheelDelta > 0.f ? -1 : 1;
		ChangeSelectionByOffset(Offset);
		return FReply::Handled();
	}
	
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

FReply UWidgetMainHudScreen::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UWidgetMainHudScreen::ChangeSelectionByOffset(const int32 Offset) const
{
	if (!CommonListView_Prompt)return;

	const int EntryNum = CommonListView_Prompt->GetNumItems();
	if (EntryNum == 0)return;

	const UObject* CurItem = CommonListView_Prompt->GetSelectedItem();
	int CurIndex = CommonListView_Prompt->GetIndexForItem(CurItem);
	CurIndex = FMath::Max(CurIndex, 0);

	int NewIndex = (CurIndex + Offset) % EntryNum;
	if (NewIndex < 0)NewIndex += EntryNum;
	CommonListView_Prompt->SetSelectedIndex(NewIndex);
}
