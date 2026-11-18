#pragma once

#include "CoreMinimal.h"
#include "CommonTypes/EnumTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/Widget/Components/KitsuneCommonButtonBase.h"
#include "UIManagerSubsystem.generated.h"

class UWidgetActivatableBase;
class UWidgetPrimaryLayout;
struct FGameplayTag;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionUpdate, UKitsuneCommonButtonBase*, InButton, FText, InDescription);

/**
 * 
 */
UCLASS()
class KITSUNE_API UUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	static UUIManagerSubsystem* GetUIManager(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void OnWorldChangedCleanup(const UWorld* NewWorld);

	UWidgetPrimaryLayout* GetRegisteredPrimaryLayout() const { return RegisteredPrimaryLayout; }

	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
	                                const FGameplayTag InWidgetTag,
	                                TFunction<void(EAsyncPushWidgetState, UWidgetActivatableBase*)> AsyncPushCallback)const;

	void PushConfirmScreenToModalStackAsync(
		const EConfirmScreenType InScreenType, const FText& InScreenTitle, const FText& InScreenMsg, 
		TFunction<void(EConfirmScreenButtonResult)> ButtonClickedCallback) const ;
	static void ExistedSetInputMode(APlayerController* InPC, ECommonInputMode InMode, EMouseCaptureMode InCaptureMode,
	                                bool bHideCursor);

	UFUNCTION(BlueprintCallable)
	void RegisterPrimaryLayoutWidget(UWidgetPrimaryLayout* InPrimaryLayout);

	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionUpdate ButtonDescriptionUpdateDelegate;

protected:
	UPROPERTY(BlueprintReadOnly)
	UWidgetPrimaryLayout* RegisteredPrimaryLayout = nullptr;
};
