// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Option/ListEntries/WidgetListEntryRemap.h"

#include "Components/InputKeySelector.h"
#include "UI/DataObjects/ListDataObjectRemap.h"
#include "UI/Widget/Components/KitsuneCommonButtonBase.h"



void UWidgetListEntryRemap::OnOwningListDataHandle(UListDataObjectBase* ItemData)
{
	Super::OnOwningListDataHandle(ItemData);

	CachedDataObjectRemap = CastChecked<UListDataObjectRemap>(ItemData);

	InputKeySelector_MappableKey->SetSelectedKey(CachedDataObjectRemap->GetCurrentKey());
	InputKeySelector_MappableKey->OnKeySelected.AddUniqueDynamic(this, &ThisClass::OnMappableKeyChanged);

	KitsuneButton_ResetImage->OnClicked().AddLambda(
		[this]()
		{
			InputKeySelector_MappableKey->SetSelectedKey(CachedDataObjectRemap->GetDefaultKey());
		}
	);
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppPassValueParameterByConstReference
void UWidgetListEntryRemap::OnMappableKeyChanged(FInputChord SelectedKey)
{
	CachedDataObjectRemap->UserMappableKeyChanged(SelectedKey.Key, EOptionsListDataModifyReason::DirectlyModified);
}

