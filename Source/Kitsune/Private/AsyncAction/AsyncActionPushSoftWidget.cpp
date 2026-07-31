// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncAction/AsyncActionPushSoftWidget.h"

#include "UIManagerSubsystem.h"
#include "UI/Widget/WidgetActivatableBase.h"

UAsyncActionPushSoftWidget* UAsyncActionPushSoftWidget::PushSoftWidgetByTag(const UObject* WorldContextObject,
                                                                       APlayerController* OwningPlayerController,
                                                                       UPARAM(meta = (Categories = "UI.Widget")) FGameplayTag InWidgetTag,
                                                                       UPARAM(meta = (Categories = "UI.WidgetStack")) FGameplayTag InWidgetStackTag, bool bFocusOnNewlyPushedWidget)
{	
	checkf(InWidgetTag.IsValid(), TEXT("PushSoftWidgetToStack was passed a null soft widget class "));
	if (GEngine)
	{
		if (UWorld* World=GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncActionPushSoftWidget* Node = NewObject<UAsyncActionPushSoftWidget>();

			Node->CachedOwningWorld = World;
			Node->CachedOwningPC = OwningPlayerController;
			Node->CachedWidgetTag = InWidgetTag;
			Node->CachedWidgetStackTag = InWidgetStackTag;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;

			Node->RegisterWithGameInstance(World);
			return Node;
		}
	}
	return nullptr;
}

void UAsyncActionPushSoftWidget::Activate()
{
	const UUIManagerSubsystem* UIManagerSubsystem = UUIManagerSubsystem::GetUIManager(CachedOwningWorld.Get());

	UIManagerSubsystem->PushSoftWidgetToStackAsync(CachedWidgetStackTag, CachedWidgetTag,
	                                               [this](EAsyncPushWidgetState InPushState, UWidgetActivatableBase* PushedWidget)
	                                               {
	                                               		if (!PushedWidget)return;
		                                               switch (InPushState)
		                                               {
		                                               case EAsyncPushWidgetState::OnCreatedBeforePush:
			                                               PushedWidget->SetOwningPlayer(CachedOwningPC.Get());
			                                               OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
			                                               break;

		                                               case EAsyncPushWidgetState::AfterPush:
			                                               AfterPush.Broadcast(PushedWidget);
			                                               if (bCachedFocusOnNewlyPushedWidget)
			                                               {
				                                               if (UWidget* WidgetToFocus=PushedWidget->GetDesiredFocusTarget())
				                                               {
					                                               WidgetToFocus->SetFocus();
				                                               }
			                                               }
			                                               break;

		                                               default:
			                                               break;
		                                               }
	                                               }
	);
}
