// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Option/ListEntries/ListEntryString.h"
#include "Types/SlateEnums.h"
#include "CommonTextBlock.h"
#include "DebugHelper.h"
#include "UI/DataObjects/ListDataObjectString.h"
#include "UI/Widget/Components/KitsuneComboBoxString.h"

void UListEntryString::OnOwningListDataHandle(UObject* ItemData)
{
	Super::OnOwningListDataHandle(ItemData);

	CachedDataObjectString = CastChecked<UListDataObjectString>(ItemData);
	CommonTextBlock_ListEntryTitle->SetText(CachedDataObjectString->GetDataDisplayName());

	ComboBoxString_OptionalData->AddOptions(CachedDataObjectString->GetDisplayOptions());
	ComboBoxString_OptionalData->ClearSelection();
	ComboBoxString_OptionalData->SetSelectedOption(CachedDataObjectString->GetCurrentDisplayOption());


	ComboBoxString_OptionalData->RefreshOptions();
	ComboBoxString_OptionalData->OnSelectionChanged.RemoveAll(this);
	ComboBoxString_OptionalData->OnSelectionChanged.AddUniqueDynamic(this, &ThisClass::OnSelectedOptionChanged);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UListEntryString::OnSelectedOptionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)  // NOLINT(performance-unnecessary-value-param)
{
	if (SelectionType!=ESelectInfo::Direct)
	{
		CachedDataObjectString->SetCurrentDisplayOption(SelectedItem);
	}
}
