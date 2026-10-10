// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/Option/ListEntries/ListEntryBase.h"
#include "ListEntryString.generated.h"

class UCommonTextBlock;
class UListDataObjectString;
class UKitsuneComboBoxString;
/**
 * 
 */
UCLASS()
class KITSUNE_API UListEntryString : public UListEntryBase
{
	GENERATED_BODY()

protected:
	virtual void OnOwningListDataHandle(UObject* ItemData) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UKitsuneComboBoxString> ComboBoxString_OptionalData;

	UFUNCTION()
	void OnSelectedOptionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

private:
	/** Variable Begin*/
	UPROPERTY(Transient)
	TObjectPtr<UListDataObjectString> CachedDataObjectString;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_ListEntryTitle;
	/** Variable End*/
};
