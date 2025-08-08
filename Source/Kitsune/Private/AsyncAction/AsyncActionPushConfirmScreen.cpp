// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncAction/AsyncActionPushConfirmScreen.h"
#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "UIManagerSubsystem.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "UI/Widget/WidgetConfirmScreen.h"

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

	const UUIManagerSubsystem* UIManagerSubsystem = UUIManagerSubsystem::GetUIManager(CachedOwningWorld.Get());

	TArray<FConfirmScreenButtonInfo> ButtonInfos;
	switch (CachedScreenType)
	{
	case EConfirmScreenType::Ok:
		ButtonInfos.Add(FConfirmScreenButtonInfo(FText::FromString(TEXT("确定")), EConfirmScreenButtonResult::Confirmed));
		break;
		
	case EConfirmScreenType::OkCancel:
		ButtonInfos.Add(FConfirmScreenButtonInfo(FText::FromString(TEXT("确定")), EConfirmScreenButtonResult::Confirmed));
		ButtonInfos.Add(FConfirmScreenButtonInfo(FText::FromString(TEXT("取消")), EConfirmScreenButtonResult::Cancelled));
		break;

	case EConfirmScreenType::YesNo:
		ButtonInfos.Add(FConfirmScreenButtonInfo(FText::FromString(TEXT("是")), EConfirmScreenButtonResult::Confirmed));
		ButtonInfos.Add(FConfirmScreenButtonInfo(FText::FromString(TEXT("否")), EConfirmScreenButtonResult::Cancelled));
		break;

	default:
		break;
	}

	UIManagerSubsystem->PushSoftWidgetToStackAsync(
		FGameplayTag::RequestGameplayTag(FName("UI.WidgetStack.ModalStack")),
		UFrontendBlueprintFunctionLibrary::GetScreenSoftWidgetClassByTag(FGameplayTag::RequestGameplayTag(FName("UI.Widget.ConfirmScreen"))),
		[this,ButtonInfos](EAsyncPushWidgetState InState, UWidgetActivatableBase* PushedWidget)
		{
			UWidgetConfirmScreen* ConfirmScreen = CastChecked<UWidgetConfirmScreen>(PushedWidget);
			if (InState==EAsyncPushWidgetState::OnCreateBeforePush)
			{
				
				ConfirmScreen->InitScreenInfo(CachedTitle, CachedMessage, ButtonInfos);
				ConfirmScreen->ScreenClosed.AddDynamic(this, &UAsyncActionPushConfirmScreen::ConfirmScreenButtonClicked);
			}

			SetReadyToDestroy();
		});
}

void UAsyncActionPushConfirmScreen::ConfirmScreenButtonClicked(const EConfirmScreenButtonResult Result)
{
	OnButtonClicked.Broadcast(Result);
}
