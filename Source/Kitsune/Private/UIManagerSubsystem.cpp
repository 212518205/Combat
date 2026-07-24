// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerSubsystem.h"

#include "Characters/KitsuneCharacter.h"
#include "UI/Widget/WidgetActivatableBase.h"
#include "Engine/AssetManager.h"
#include "FunctionLibrary/FrontendBlueprintFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/AttributeViewModel.h"
#include "UI/ViewModel/PlayerViewModel.h"
#include "UI/Widget/WidgetConfirmScreen.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Input/CommonUIActionRouterBase.h"
#include "UI/KitsuneActivatableWidgetStack.h"
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

AKitsuneCharacter* UUIManagerSubsystem::GetLocalPlayerPawn() const
{
	const UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance) return nullptr;

	const UWorld* World = GameInstance->GetWorld();
	if (!World) return nullptr;

	const APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC) return nullptr;

	APawn* Pawn = PC->GetPawn();
	return Cast<AKitsuneCharacter>(Pawn);
}

UPlayerViewModel* UUIManagerSubsystem::GetLocalViewModel(bool& bIsValid)
{
	bIsValid = false;

	if (APawn* Pawn = GetLocalPlayerPawn())
	{
		UPlayerViewModel* VM = TryGetViewModelByActor<UPlayerViewModel>(Pawn);
		if (VM)
		{
			bIsValid = true;
		}
		return VM;
	}
	
	return nullptr;
}



void UUIManagerSubsystem::RegisterPrimaryLayoutWidget(UWidgetPrimaryLayout* InPrimaryLayout)
{
	check(InPrimaryLayout);
	RegisteredPrimaryLayout = InPrimaryLayout;
}

UAttributeViewModel* UUIManagerSubsystem::GetViewModelByPawn(APawn* InPawn)
{
	if (!InPawn)return nullptr;

	UAttributeViewModel** FoundViewModel = RegisteredViewModels.Find(InPawn);
	return FoundViewModel ? *FoundViewModel : nullptr;
}

void UUIManagerSubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
                                                     const FGameplayTag InWidgetTag,
                                                     TFunction<void(EAsyncPushWidgetState, UWidgetActivatableBase*)> AsyncPushCallback)const
{
	check(InWidgetTag.IsValid());
	
	TSoftClassPtr<UWidgetActivatableBase> WidgetClass = UFrontendBlueprintFunctionLibrary::GetScreenSoftWidgetClassByTag(InWidgetTag);
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[InWidgetTag, this, InWidgetStackTag, AsyncPushCallback, WidgetClass]()
			{
				
				UClass* LoadedWidgetClass = WidgetClass.Get();
				check(LoadedWidgetClass && RegisteredPrimaryLayout);
				UKitsuneActivatableWidgetStack* FoundWidgetStack = RegisteredPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);
				
				AsyncPushCallback(EAsyncPushWidgetState::AfterPush, nullptr);
				UWidgetActivatableBase* CreatedWidget =  FoundWidgetStack->PushWidget(InWidgetTag, LoadedWidgetClass);
				RegisteredPrimaryLayout->UpdateInteractState();
				AsyncPushCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			})
	);
}

void UUIManagerSubsystem::PushConfirmScreenToModalStackAsync(const EConfirmScreenType InScreenType,
	const FText& InScreenTitle, const FText& InScreenMsg,
	TFunction<void(EConfirmScreenButtonResult)> ButtonClickedCallback)const 
{
	UConfirmScreenInfoObject* CreatedInfoObject = nullptr;

	switch (InScreenType)
	{
	case EConfirmScreenType::Ok:
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
		[CreatedInfoObject, ButtonClickedCallback](EAsyncPushWidgetState InPushState, UWidgetActivatableBase* PushedWidget)
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