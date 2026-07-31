// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KitsuneActivatableWidgetStack.h"

#include "UI/Widget/WidgetActivatableBase.h"


void UKitsuneActivatableWidgetStack::PopWidget()
{
	if (UCommonActivatableWidget* ActiveWidget = GetActiveWidget())
	{
		RemoveWidget(*ActiveWidget);
	}
}

void UKitsuneActivatableWidgetStack::ClearStack()
{
	/*** TODO: 只是SwitchWidgetIndex，暂时没看是否是移除 以及重新AddWidget是否会出现错误... [2026年7月31日 15:44:28 来自`@BC@`] ***/
	
	ClearWidgets();
	
}

void UKitsuneActivatableWidgetStack::OnWidgetAddedToList(UCommonActivatableWidget& AddedWidget)
{
	if (MySwitcher)
	{
		MySwitcher->AddSlot() [AddedWidget.TakeWidget()];

		SetSwitcherIndex(MySwitcher->GetNumWidgets() - 1);
	}
}
