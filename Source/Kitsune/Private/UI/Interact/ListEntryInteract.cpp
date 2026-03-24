// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Interact/ListEntryInteract.h"

#include "CommonTextBlock.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Trait/ItemTrait_Interact.h"

void UListEntryInteract::OnOwningListDataHandle(UObject* ItemData)
{
	Super::OnOwningListDataHandle(ItemData);

	if (UItemTrait_Interact* TraitInteract = Cast<UItemTrait_Interact>(ItemData))
	{
		CachedInteractTrait = TraitInteract;
		CommonTextBlock_PromptKey->SetText(FText::FromString(CachedInteractTrait->PromptKey.GetDisplayName().ToString()));
		CommonTextBlock_PromptMessage->SetText(CachedInteractTrait->InteractPrompt);
	}
}
