// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/Option/ListEntries/WidgetListEntryBase.h"
#include "ListEntryInteract.generated.h"

class UItemTrait_Interact;
/**
 * 
 */
UCLASS()
class KITSUNE_API UListEntryInteract : public UWidgetListEntryBase
{
	GENERATED_BODY()

protected:
	virtual void OnOwningListDataHandle(UObject* ItemData) override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_PromptKey;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_PromptMessage;

private:
	UPROPERTY(Transient)
	TObjectPtr<UItemTrait_Interact> CachedInteractTrait;
};
