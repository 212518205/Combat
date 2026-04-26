// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/Entry/BackBag/BackBagEntryBase.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Trait/ItemTrait_Display.h"

void UBackBagEntryBase::RefreshEntry() const
{
	if (CachedItemInstance->HasFeature(EItemFeature::Stackable))TextBlock_StackCount->SetText(FText::AsNumber(CachedItemInstance->StackCount));
}

void UBackBagEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
	CachedItemInstance = Cast<UInventoryItemInstance>(ListItemObject);
	if (!CachedItemInstance)return;
	
	CachedTraitDisplay = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Display>(CachedItemInstance->GetItemDef());
	CachedTraitInteract = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Interact>(CachedItemInstance->GetItemDef());
	CachedTraitStack = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Stack>(CachedItemInstance->GetItemDef());
	
	Image_Display->SetBrushFromSoftTexture(CachedTraitDisplay->DisplayIcon);
	Image_Background->SetBrushFromSoftTexture(T_BackGroundImage);
	Image_Background->SetColorAndOpacity(DefaultIconColor);
	TextBlock_ShowNew->SetText(FText::FromString(TEXT("新")));
	if (CachedItemInstance->HasFeature(EItemFeature::Stackable))TextBlock_StackCount->SetText(FText::AsNumber(CachedItemInstance->StackCount));
}

void UBackBagEntryBase::OnHovered()
{
	Super::OnHovered();
	
	if (Image_Background)
	{
		Image_Background->SetColorAndOpacity(HoveredIconColor);
	}
}

void UBackBagEntryBase::OnUnHovered()
{
	Super::OnUnHovered();
	
	if (Image_Background)
	{
		Image_Background->SetColorAndOpacity(DefaultIconColor);
	}
}

void UBackBagEntryBase::OnPressed()
{
	Super::OnPressed();
	
	if (Image_Background)
	{
		Image_Background->SetColorAndOpacity(ClickedIconColor);
	}
}

void UBackBagEntryBase::OnReleased()
{
	Super::OnReleased();
	
	if (Image_Background)
	{
		Image_Background->SetColorAndOpacity(DefaultIconColor);
	}
}
