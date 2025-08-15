// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Option/WidgetOptionScreen.h"

#include "FrontendDebugHelper.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "UI/DataObjects/ListDataObjectCollection.h"
#include "UI/DataObjects/OptionsDataRegistry.h"
#include "UI/Widget/Components/KitsuneTabListWidgetBase.h"


void UWidgetOptionScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetAction, true,
				FSimpleDelegate::CreateUObject(this, &UWidgetOptionScreen::OnResetBoundActionTriggered)
			)
		);
	}
	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &UWidgetOptionScreen::OnBackBoundActionTriggered)
		)
	);

}

void UWidgetOptionScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (const UListDataObjectCollection* Collection:GetDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!Collection)continue;

		const FName TabID = Collection->GetDataID();
		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID))continue;

		TabListWidget_OptionsTabs->RequestRegisterTab(TabID, Collection->GetDataDisplayName());
	}
}

UOptionsDataRegistry* UWidgetOptionScreen::GetDataRegistry()
{
	if (!OwningDataRegistry)
	{
		OwningDataRegistry = NewObject<UOptionsDataRegistry>();
		OwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}
	check(OwningDataRegistry);
	return OwningDataRegistry;
}

void UWidgetOptionScreen::OnResetBoundActionTriggered()
{
	Debug::Print(TEXT("Reset all Setting"));
}

void UWidgetOptionScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
