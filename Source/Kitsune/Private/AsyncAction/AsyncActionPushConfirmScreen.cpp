// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncAction/AsyncActionPushConfirmScreen.h"
#include "UI/Widget/WidgetConfirmScreen.h"
#include "UIManagerSubsystem.h"

UAsyncActionPushConfirmScreen* UAsyncActionPushConfirmScreen::PushConfirmScreen(const UObject* WorldContextObject,
                                                                                const EConfirmScreenType ScreenType, const FText Title, const FText Message)
{
	if (GEngine)
	{
		if (UWorld* World=GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncActionPushConfirmScreen* Node = NewObject<UAsyncActionPushConfirmScreen>();
			Node->CachedOwningWorld = World;
			Node->CachedScreenType = ScreenType;
			Node->CachedTitle = Title;
			Node->CachedMessage = Message;
			return Node;
		}
	}
	return nullptr;
}

void UAsyncActionPushConfirmScreen::Activate()
{
	Super::Activate();

	UUIManagerSubsystem::GetUIManager(CachedOwningWorld.Get())->PushConfirmScreenToModalStackAsync(
		CachedScreenType,
		CachedTitle,
		CachedMessage,
		[this](EConfirmScreenButtonResult ClickedButtonType)
		{
			OnButtonClicked.Broadcast(ClickedButtonType);
			SetReadyToDestroy();
		}
	);
}
