// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerSubsystem.h"
#include "FrontendDebugHelper.h"
#include "UI/Widget/WidgetActivatableBase.h"
#include "Engine/AssetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "UI/Widget/WidgetPrimaryLayout.h"

UUIManagerSubsystem* UUIManagerSubsystem::GetUIManager(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
		return UGameInstance::GetSubsystem<UUIManagerSubsystem>(World->GetGameInstance());
	}
	return nullptr;
}

bool UUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance()) /* Check is Server? Is server return false*/
	{
		TArray<UClass*> UIManagerClasses;
		GetDerivedClasses(GetClass(), UIManagerClasses);

		return UIManagerClasses.IsEmpty();	/* Return whether it has been created*/
	}
	return false;
}

void UUIManagerSubsystem::RegisterPrimaryLayoutWidget(UWidgetPrimaryLayout* InPrimaryLayout)
{
	check(InPrimaryLayout);
	RegisteredPrimaryLayout = InPrimaryLayout;
	Debug::Print(TEXT("Primary layout widget stored"));
}

void UUIManagerSubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
	TSoftClassPtr<UWidgetActivatableBase> InSoftWidgetClass,
	TFunction<void(EAsyncPushWidgetState, UWidgetActivatableBase*)> AsyncPushCallback)const
{
	check(!InSoftWidgetClass.IsNull());

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(InSoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[InSoftWidgetClass,this, InWidgetStackTag,AsyncPushCallback]()
			{
				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
				check(LoadedWidgetClass && RegisteredPrimaryLayout);
				UCommonActivatableWidgetContainerBase* FoundWidgetStack = RegisteredPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);

				UWidgetActivatableBase* CreatedWidget =  FoundWidgetStack->AddWidget<UWidgetActivatableBase>(LoadedWidgetClass,
					[AsyncPushCallback](UWidgetActivatableBase& CreatedWidgetInstance)
					{
						AsyncPushCallback(EAsyncPushWidgetState::OnCreateBeforePush, &CreatedWidgetInstance);
					}
				);
				AsyncPushCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			})
	);
}
