#include "UI/Widget/WidgetActivatableBase.h"

#include "Player/KitsunePlayerController.h"
#include "UIManagerSubsystem.h"

UWidgetPrimaryLayout* UWidgetActivatableBase::GetPrimaryLayout() const
{
	const UUIManagerSubsystem* UIManager = UUIManagerSubsystem::GetUIManager(GetWorld());
	return UIManager ? UIManager->GetRegisteredPrimaryLayout() : nullptr;
}

AKitsunePlayerController* UWidgetActivatableBase::GetOwningKitsunePlayerController()
{
	if (!CachedOwningKitsunePC.IsValid())
	{
		CachedOwningKitsunePC = GetOwningPlayer<AKitsunePlayerController>();
	}
	return CachedOwningKitsunePC.IsValid() ? CachedOwningKitsunePC.Get() : nullptr;
}

UViewModelBase* UWidgetActivatableBase::GetLocalViewModelByClass(const TSubclassOf<UViewModelBase> Class,
                                                                EViewModelAccessResult& ExecResult) const
{
	return UKitsuneViewModelFunctionLibrary::FindViewModel(GetOwningPlayerPawn(), Class, ExecResult);
}
