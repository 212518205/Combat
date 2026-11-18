
#include "UIManagerSubsystem.h"

#include "GameFramework/PlayerController.h"
#include "UI/Widget/WidgetActivatableBase.h"
#include "Engine/AssetManager.h"
#include "FunctionLibrary/KitsuneWidgetFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "UI/Widget/WidgetConfirmScreen.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Input/CommonUIActionRouterBase.h"
#include "UI/KitsuneActivatableWidgetStack.h"
#include "UI/Widget/WidgetPrimaryLayout.h"

UUIManagerSubsystem* UUIManagerSubsystem::GetUIManager(const UObject* WorldContextObject)
{
	if (GEngine && WorldContextObject)
	{
		const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,
		                                                         EGetWorldErrorMode::LogAndReturnNull);
		if (World)
		{
			return UGameInstance::GetSubsystem<UUIManagerSubsystem>(World->GetGameInstance());
		}
	}
	return nullptr;
}

bool UUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance()) 
	{
		TArray<UClass*> UIManagerClasses;
		GetDerivedClasses(GetClass(), UIManagerClasses);

		return UIManagerClasses.IsEmpty(); 
	}
	return false;
}

void UUIManagerSubsystem::OnWorldChangedCleanup(const UWorld* NewWorld)
{
	if (RegisteredPrimaryLayout
		&& (!IsValid(RegisteredPrimaryLayout) || RegisteredPrimaryLayout->GetWorld() != NewWorld))
	{
		RegisteredPrimaryLayout = nullptr;
	}

}

void UUIManagerSubsystem::RegisterPrimaryLayoutWidget(UWidgetPrimaryLayout* InPrimaryLayout)
{
	check(InPrimaryLayout);
	RegisteredPrimaryLayout = InPrimaryLayout;
}

void UUIManagerSubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
                                                     const FGameplayTag InWidgetTag,
                                                     TFunction<void(EAsyncPushWidgetState, UWidgetActivatableBase*)>
                                                     AsyncPushCallback) const
{
	check(InWidgetTag.IsValid());

	TSoftClassPtr<UWidgetActivatableBase> WidgetClass =
		UKitsuneWidgetFunctionLibrary::GetScreenSoftWidgetClassByTag(InWidgetTag);
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WidgetClass.ToSoftObjectPath(),
	                                                             FStreamableDelegate::CreateLambda(
		                                                             [this, InWidgetStackTag, AsyncPushCallback,
			                                                             WidgetClass]()
		                                                             {
			                                                             UClass* LoadedWidgetClass = WidgetClass.Get();
			                                                             check(LoadedWidgetClass &&
				                                                             RegisteredPrimaryLayout);
			                                                             UKitsuneActivatableWidgetStack*
				                                                             FoundWidgetStack = RegisteredPrimaryLayout
				                                                             ->FindWidgetStackByTag(InWidgetStackTag);

			                                                             UWidgetActivatableBase* CreatedWidget =
				                                                             FoundWidgetStack->AddWidget<
					                                                             UWidgetActivatableBase>(
					                                                             LoadedWidgetClass,
					                                                             [AsyncPushCallback](
					                                                             UWidgetActivatableBase&
					                                                             CreatedWidgetInstance)
					                                                             {
						                                                             AsyncPushCallback(
							                                                             EAsyncPushWidgetState::OnCreatedBeforePush,
							                                                             &CreatedWidgetInstance);
					                                                             });
			                                                             RegisteredPrimaryLayout->UpdateInteractState();
			                                                             AsyncPushCallback(
				                                                             EAsyncPushWidgetState::AfterPush,
				                                                             CreatedWidget);
		                                                             })
	);
}

void UUIManagerSubsystem::PushConfirmScreenToModalStackAsync(const EConfirmScreenType InScreenType,
                                                             const FText& InScreenTitle, const FText& InScreenMsg,
                                                             TFunction<void(EConfirmScreenButtonResult)>
                                                             ButtonClickedCallback) const
{
	UConfirmScreenInfoObject* CreatedInfoObject = nullptr;

	switch (InScreenType)
	{
	case EConfirmScreenType::OK:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOKScreen(InScreenTitle, InScreenMsg);
		break;

	case EConfirmScreenType::YesNo:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateYesNoScreen(InScreenTitle, InScreenMsg);
		break;

	case EConfirmScreenType::OkCancel:
		CreatedInfoObject = UConfirmScreenInfoObject::CreateOkCancelScreen(InScreenTitle, InScreenMsg);
		break;

	case EConfirmScreenType::Unknown:
		break;
	default:
		break;
	}

	check(CreatedInfoObject);

	PushSoftWidgetToStackAsync(
		KitsuneGameplayTags::UI_WidgetStack_ModalStack,
		KitsuneGameplayTags::UI_Widget_ConfirmScreen,
		[CreatedInfoObject, ButtonClickedCallback](EAsyncPushWidgetState InPushState,
		                                           UWidgetActivatableBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidgetConfirmScreen* CreatedConfirmScreen = CastChecked<UWidgetConfirmScreen>(PushedWidget);
				CreatedConfirmScreen->InitConfirmScreen(CreatedInfoObject, ButtonClickedCallback);
			}
		}
	);
}

UCommonUIActionRouterBase* GetCommonUIActionRouter(const APlayerController* InPC)
{
	if (!InPC)
	{
		return nullptr;
	}

	ULocalPlayer* LocalPlayer = InPC->GetLocalPlayer();
	if (!LocalPlayer)
	{
		return nullptr;
	}

	return LocalPlayer->GetSubsystem<UCommonUIActionRouterBase>();
}

void UUIManagerSubsystem::ExistedSetInputMode(
	APlayerController* InPC,
	const ECommonInputMode InMode,
	const EMouseCaptureMode InCaptureMode,
	const bool bHideCursor)
{
	if (!InPC || !InPC->GetWorld())
	{
		return;
	}

	const FUIInputConfig Config(InMode, InCaptureMode, bHideCursor);

	if (UCommonUIActionRouterBase* Router = GetCommonUIActionRouter(InPC))
	{
		Router->SetActiveUIInputConfig(Config, InPC);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ExistedSetInputMode: Failed to get CommonUIActionRouterBase"));
	}

	InPC->bShowMouseCursor = !bHideCursor;
	InPC->bEnableMouseOverEvents = !bHideCursor;
	InPC->bEnableClickEvents = !bHideCursor;
}
