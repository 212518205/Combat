// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WidgetActivatableBase.h"
#include "Player/KitsunePlayerController.h"

AKitsunePlayerController* UWidgetActivatableBase::GetOwningKitsunePlayerController()
{
	if (!CachedOwningKitsunePC.IsValid())
	{
		CachedOwningKitsunePC = GetOwningPlayer<AKitsunePlayerController>();
	}
	return CachedOwningKitsunePC.IsValid() ? CachedOwningKitsunePC.Get() : nullptr;
}
