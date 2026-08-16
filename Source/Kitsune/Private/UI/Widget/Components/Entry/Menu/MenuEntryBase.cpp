// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/Entry/Menu/MenuEntryBase.h"

#include "CommonTextBlock.h"
#include "UIManagerSubsystem.h"
#include "Components/Image.h"
#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "UI/DataObjects/Inventory/MenuEntryData.h"
#include "UI/Widget/WidgetActivatableBase.h"


void UMenuEntryBase::NativeOnEntryInitialize(UObject* ListItemObject)
{
	if (UMenuEntryData* EntryData = Cast<UMenuEntryData>(ListItemObject))
	{
		CachedEntryData = EntryData;

		CommonTextBlock_DisplayName->SetText(EntryData->DisplayName);
		CommonTextBlock_HotKeyText->SetText(EntryData->HotKeyText);
		CommonTextBlock_ShowAsNewText->SetText(EntryData->ShowAsNewText);
		
		BP_EntryInitialize();
	}
}
