// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Interact/ListEntryInteract.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Trait/ItemTrait_Interact.h"

void UListEntryInteract::OnOwningListDataHandle(UObject* ItemData)
{
	Super::OnOwningListDataHandle(ItemData);

	if (CachedItemInstance = Cast<UInventoryItemInstance>(ItemData); CachedItemInstance)
	{
		CachedInteractTrait = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Interact>(CachedItemInstance->GetItemDef());
		CommonTextBlock_PromptKey->SetText(FText::FromString(CachedInteractTrait->PromptKey.GetDisplayName().ToString()));
		CommonTextBlock_PromptMessage->SetText(CachedInteractTrait->InteractPrompt);
		Image_Background->SetOpacity(0.f);
	}
}

void UListEntryInteract::NativeOnItemSelectionChanged(bool bIsSelected)
{
	Super::NativeOnItemSelectionChanged(bIsSelected);
	
	if (bIsSelected)
	{
		Image_Background->SetOpacity(0.6f);
	}
	else
	{
		Image_Background->SetOpacity(0.f);
	}
}
