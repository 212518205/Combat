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
	if (!CachedSlotData->ItemInstance)return;
	if (CachedItemInstance->HasFeature(EItemFeature::Stackable))TextBlock_StackCount->SetText(FText::AsNumber(CachedSlotData->StackCount));
}

void UBackBagEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	CachedSlotData = Cast<UInventorySlotData>(ListItemObject);
	if (!CachedSlotData)return;
	
	if (CachedSlotData->ItemInstance)
	{
		CachedItemInstance = CachedSlotData->ItemInstance;
	
		CachedTraitDisplay = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Display>(CachedItemInstance->GetItemDef());
		CachedTraitInteract = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Interact>(CachedItemInstance->GetItemDef());
		CachedTraitStack = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Stack>(CachedItemInstance->GetItemDef());
		if (CachedItemInstance->HasFeature(EItemFeature::Stackable))TextBlock_StackCount->SetText(FText::AsNumber(CachedSlotData->StackCount));
		else TextBlock_StackCount->SetText(FText::GetEmpty());
	
		Image_Display->SetBrushFromSoftTexture(CachedTraitDisplay->DisplayIcon);
		TextBlock_ShowNew->SetText(FText::FromString(TEXT("新")));
	}else if (CachedSlotData->bIsLocked)
	{
		TextBlock_ShowNew->SetText(FText::FromString(TEXT("锁定")));
		TextBlock_StackCount->SetText(FText::GetEmpty());
		Image_Display->SetBrushFromSoftTexture(nullptr);
	}else
	{
		TextBlock_ShowNew->SetText(FText::GetEmpty());
		TextBlock_StackCount->SetText(FText::GetEmpty());
		Image_Display->SetBrushFromSoftTexture(nullptr);
	}
	Image_Background->SetBrushFromSoftTexture(T_BackGroundImage);
}


