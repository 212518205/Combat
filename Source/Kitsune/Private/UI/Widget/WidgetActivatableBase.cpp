// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/WidgetActivatableBase.h"

#include "UIManagerSubsystem.h"
#include "Player/KitsunePlayerController.h"



UPlayerViewModel* UWidgetActivatableBase::GetLocalPlayerViewModel()
{
	if (!CachedLocalViewModel)
	{
		CachedLocalViewModel = UUIManagerSubsystem::GetUIManager(GetOwningPlayer())->TryGetViewModelByActor<UPlayerViewModel>(GetOwningPlayerPawn());
	}

	return CachedLocalViewModel;
}

AKitsunePlayerController* UWidgetActivatableBase::GetOwningKitsunePlayerController()
{
	if (!CachedOwningKitsunePC.IsValid())
	{
		CachedOwningKitsunePC = GetOwningPlayer<AKitsunePlayerController>();
	}
	return CachedOwningKitsunePC.IsValid() ? CachedOwningKitsunePC.Get() : nullptr;
}
