// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/Entry/BackBag/BackBagEntryBase.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/InventorySystemComponent.h"
#include "Inventory/Trait/ItemTrait_Display.h"

void UBackBagEntryBase::RefreshEntry() const
{
	RefreshStackCount();
}

void UBackBagEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
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
		if (const UItemTrait_Display* Trait_Display = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Display>(ItemInstance->GetItemDef()))
		{
			SetButtonIcon(Trait_Display->DisplayIcon.LoadSynchronous());
		}
		
		RefreshStackCount();
		
		const bool bShowLockFlag = !ItemInstance->HasFeature(EItemFeature::Discardable) || ItemInstance->HasFeature(EItemFeature::Locked);
		Image_LockFlag->SetVisibility(bShowLockFlag ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		
		TextBlock_ShowNew->SetText(FText::FromString(TEXT("新")));
		Button_Icon->SetIsEnabled(true);
	}
	else if (CachedSlotData->bIsLocked)
	{
		ClearEntryStyle();
		// 锁图放在按钮图标上。LoadSynchronous 未配置/路径错误时会返回 nullptr,这里打日志方便排查。
		if (UTexture2D* LockTexture = LockBackgroundIcon.LoadSynchronous())
		{
			SetButtonIcon(LockTexture);
		}
		else
		{
			SetButtonIcon(nullptr);
			UE_LOG(LogTemp, Warning, TEXT("[BackBagEntry] LockBackgroundIcon 未配置或加载失败,锁定格不显示锁图"));
		}
		Button_Icon->SetIsEnabled(true);
	}else
	{
		ClearEntryStyle();
		Button_Icon->SetIsEnabled(false);
	}
	
	BP_BackBagEntryInitialize();
}

void UBackBagEntryBase::ClearEntryStyle() const
{
	TextBlock_ShowNew->SetText(FText::GetEmpty());
	TextBlock_StackCount->SetText(FText::GetEmpty());
		Image_LockFlag->SetVisibility(ESlateVisibility::Collapsed);
	SetButtonIcon(nullptr);
}

bool UBackBagEntryBase::IsItemEntry() const
{
	return GetItemInstance() != nullptr;
}

bool UBackBagEntryBase::IsEmptyEntry() const
{
	return GetItemInstance() == nullptr && CachedSlotData && !CachedSlotData->bIsLocked;
}

bool UBackBagEntryBase::IsLockEntry() const
{
	return CachedSlotData && CachedSlotData->bIsLocked;
}

UInventoryItemInstance* UBackBagEntryBase::GetItemInstance() const
{
	if (!CachedSlotData)
	{
		return nullptr;
	}
	return CachedSlotData->ItemInstance;
}

void UBackBagEntryBase::RefreshStackCount() const
{
	if (const UInventoryItemInstance* ItemInstance = GetItemInstance(); ItemInstance && ItemInstance->HasFeature(EItemFeature::Stackable))
	{
		TextBlock_StackCount->SetText(FText::AsNumber(CachedSlotData->StackCount));
	}
	else
	{
		TextBlock_StackCount->SetText(FText::GetEmpty());
	}
}

void UBackBagEntryBase::SetButtonIcon(UTexture2D* IconTexture) const
{
	FButtonStyle Style = Button_Icon->GetStyle();
	Style.Normal.SetResourceObject(IconTexture);
	Style.Hovered.SetResourceObject(IconTexture);
	Style.Pressed.SetResourceObject(IconTexture);
	Button_Icon->SetStyle(Style);
}


