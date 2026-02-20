// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/ViewModel/EnemyViewModel.h"
#include "UI/ViewModel/PlayerViewModel.h"
#include "UI/Widget/Components/KitsuneCommonButtonBase.h"
#include "UIManagerSubsystem.generated.h"

class UPlayerViewModel;
class UViewModelBase;
class UAttributeViewModel;
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

	/*** `@BC`   描述: ViewModel相关   `BC@` ***/
	UFUNCTION(BlueprintCallable, Category = "ViewModel", meta = (ExpandBoolAsExecs = "bIsValid"))
	UPlayerViewModel* GetLocalViewModel(bool& bIsValid);

	template<typename T = UAttributeViewModel>
	T* TryGetViewModelByActor(APawn* InPawn);

	UFUNCTION(BlueprintCallable, Category = "ViewModel")
	UAttributeViewModel* GetViewModelByPawn(APawn* InPawn);

	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
		TSoftClassPtr<UWidgetActivatableBase> InSoftWidgetClass,
		TFunction<void(EAsyncPushWidgetState, UWidgetActivatableBase*)> AsyncPushCallback)const;

	void PushConfirmScreenToModalStackAsync(
		const EConfirmScreenType InScreenType, const FText& InScreenTitle, const FText& InScreenMsg, 
		TFunction<void(EConfirmScreenButtonResult)> ButtonClickedCallback) const ;

	UFUNCTION(BlueprintCallable)
	void RegisterPrimaryLayoutWidget(UWidgetPrimaryLayout* InPrimaryLayout);


	UFUNCTION(BlueprintCallable)
	void DeActivableStackByTag(FGameplayTag InTag)const;
	/** Function End*/

	/** Variable Begin*/
	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionUpdate ButtonDescriptionUpdateDelegate;
	/** Variable End*/

protected:
	/** Function Begin*/
	UPROPERTY()
	UWidgetPrimaryLayout* RegisteredPrimaryLayout=nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Registered ViewModel")
	TMap<APawn*, UAttributeViewModel*> RegisteredViewModels;
	/** Function End*/
};

template <class T>
T* UUIManagerSubsystem::TryGetViewModelByActor(APawn* InPawn)
{
	if (UAttributeViewModel** FoundViewModel = RegisteredViewModels.Find(InPawn))
	{
		return CastChecked<T>(*FoundViewModel);
	}

	/*** REFACTOR: 需要添加继承自AbilityInterface的类，并且把获取属性集和能力系统组件的接口用接口类替换... [2025年10月28日 21:18:55 来自`@BC@`] ***/

	T* ViewModel = UViewModelBase::GetViewModel<T>(InPawn->GetController(), InPawn);
	RegisteredViewModels.Add(InPawn, ViewModel);

	return ViewModel;
}
