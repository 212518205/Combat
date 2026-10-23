// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/Entry/Backpack/BackpackEntryBase.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "DebugHelper.h"
#include "CommonTypes/StructTypes.h"
#include "FunctionLibrary/KitsuneInventoryFunctionLibrary.h"
#include "FunctionLibrary/KitsuneWidgetFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Trait/InventoryItemTrait_Display.h"

void UBackpackEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	CachedSlotData = Cast<UInventorySlotData>(ListItemObject);
	if (!CachedSlotData)
	{
		ClearEntryStyle();
		Image_LockFlag->SetVisibility(ESlateVisibility::Collapsed);
		Button_Icon->SetIsEnabled(false);
		return;
	}
	
	if (const UInventoryItemInstance* ItemInstance = CachedSlotData->ItemInstance)
	{
		if (const UInventoryItemTrait_Display* Trait_Display = UInventoryFunctionLibrary::FindItemDefinitionTrait<UInventoryItemTrait_Display>(ItemInstance->GetItemDef()))
		{
			SetButtonIcon(Trait_Display->DisplayIcon.LoadSynchronous());
		}
		
		RefreshStackCount();
		
		const bool bShowLockFlag = !ItemInstance->HasFeature(EInventoryItemFeature::Discardable) || ItemInstance->HasFeature(EInventoryItemFeature::Locked);
		Image_LockFlag->SetVisibility(bShowLockFlag ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		
		TextBlock_ShowNew->SetText(FText::FromString(TEXT("新")));
		Button_Icon->SetIsEnabled(true);
	}
	else if (CachedSlotData->bIsLocked)
	{
		ClearEntryStyle();
		if (UTexture2D* LockTexture = LockBackgroundIcon.LoadSynchronous())
		{
			SetButtonIcon(LockTexture);
		}
		else
		{
			SetButtonIcon(nullptr);
			Debug::Print(TEXT("[BackpackEntry] LockBackgroundIcon 未配置或加载失败,锁定格不显示锁图"));
		}
		Button_Icon->SetIsEnabled(true);
	}else
	{
		ClearEntryStyle();
		Button_Icon->SetIsEnabled(false);
	}
	
	BP_BackpackEntryInitialize();
}

void UBackpackEntryBase::ClearEntryStyle() const
{
	TextBlock_ShowNew->SetText(FText::GetEmpty());
	TextBlock_StackCount->SetText(FText::GetEmpty());
		Image_LockFlag->SetVisibility(ESlateVisibility::Collapsed);
	SetButtonIcon(nullptr);
}

bool UBackpackEntryBase::IsItemEntry() const
{
	return GetItemInstance() != nullptr;
}

bool UBackpackEntryBase::IsEmptyEntry() const
{
	return GetItemInstance() == nullptr && CachedSlotData && !CachedSlotData->bIsLocked;
}

bool UBackpackEntryBase::IsLockEntry() const
{
	return CachedSlotData && CachedSlotData->bIsLocked;
}

UInventoryItemInstance* UBackpackEntryBase::GetItemInstance() const
{
	if (!CachedSlotData)
	{
		return nullptr;
	}
	return CachedSlotData->ItemInstance;
}

FText UBackpackEntryBase::GetUnlockMessage() const
{
	if (!CachedSlotData)return FText::GetEmpty();

	int32 SlotsPerUnlock = 1;
	const TMap<FName, FInventoryCategoryConfig> CategoryInfo =
		UKitsuneWidgetFunctionLibrary::GetCategoryNameByModuleTag(
			KitsuneGameplayTags::UI_CategoryDisplay_Inventory_Item).CategoryInfo;
	if (const FInventoryCategoryConfig* Info = CategoryInfo.Find(CachedSlotData->CategoryID))
	{
		SlotsPerUnlock = Info->SlotsPerUnlock;
	}

	return FText::Format(
		NSLOCTEXT("Inventory", "UnlockSlotConfirm", "是否花费 {0} 金币解锁 {1} 个格子？"),
		FText::AsNumber(CachedSlotData->UnlockCost),
		FText::AsNumber(SlotsPerUnlock)
	);
}

void UBackpackEntryBase::RefreshStackCount() const
{
	if (CachedSlotData && CachedSlotData->StackCount > 0)
	{
		TextBlock_StackCount->SetText(FText::AsNumber(CachedSlotData->StackCount));
	}
	else
	{
		TextBlock_StackCount->SetText(FText::GetEmpty());
	}
}

void UBackpackEntryBase::SetButtonIcon(UTexture2D* IconTexture) const
{
	FButtonStyle Style = Button_Icon->GetStyle();
	Style.Normal.SetResourceObject(IconTexture);
	Style.Hovered.SetResourceObject(IconTexture);
	Style.Pressed.SetResourceObject(IconTexture);
	Button_Icon->SetStyle(Style);
}


