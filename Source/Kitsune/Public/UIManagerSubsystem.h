// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/Widget/Button/KitsuneCommonButtonBase.h"
#include "UIManagerSubsystem.generated.h"

class UWidgetActivatableBase;
struct FGameplayTag;
class UWidgetPrimaryLayout;

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
	/** Function Begin*/
	static UUIManagerSubsystem* GetUIManager(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable)
	void RegisterPrimaryLayoutWidget(UWidgetPrimaryLayout* InPrimaryLayout);

	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
		TSoftClassPtr<UWidgetActivatableBase> InSoftWidgetClass,
		TFunction<void(EAsyncPushWidgetState, UWidgetActivatableBase*)> AsyncPushCallback)const;

	void PushConfirmScreenToModalStackAsync(
		const EConfirmScreenType InScreenType, const FText& InScreenTitle, const FText& InScreenMsg, 
		TFunction<void(EConfirmScreenButtonResult)> ButtonClickedCallback) const ;
	/** Function End*/
	/** Variable Begin*/
	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionUpdate ButtonDescriptionUpdateDelegate;
	/** Variable End*/

protected:
	/** Function Begin*/
	UPROPERTY()
	UWidgetPrimaryLayout* RegisteredPrimaryLayout=nullptr;
	/** Function End*/
};
