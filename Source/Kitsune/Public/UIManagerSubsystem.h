// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Characters/KitsuneCharacter.h"
#include "CommonTypes/EnumTypes.h"
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
	static UUIManagerSubsystem* GetUIManager(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	
	void OnWorldChangedCleanup(const UWorld* NewWorld);

	UWidgetPrimaryLayout* GetRegisteredPrimaryLayout()const {return RegisteredPrimaryLayout;}
	AKitsuneCharacter* GetLocalPlayerPawn() const;

	/*** `@BC`   描述: ViewModel相关   `BC@` ***/
	UFUNCTION(BlueprintCallable, Category = "ViewModel", meta = (ExpandBoolAsExecs = "bIsValid"))
	UPlayerViewModel* GetLocalPlayerViewModel(bool& bIsValid);
	
	UPlayerViewModel* GetLocalPlayerViewModel();

	/** 取用 ViewModel：未注册时会创建并注册（仅用于 Pawn 自身初始化） */
	template<typename T = UAttributeViewModel>
	T* TryGetViewModel(AActor* InActor);

	/** 只读查询已注册的 ViewModel：不创建、不注册，未注册或类型不匹配时返回 nullptr（供 UI 侧使用） */
	template<typename T = UAttributeViewModel>
	T* FindViewModel(AActor* InActor) const;

	void UnRegisterViewModel(const AActor* InActor);
	
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
	/** Function Begin*/
	UPROPERTY(BlueprintReadOnly)
	UWidgetPrimaryLayout* RegisteredPrimaryLayout=nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Registered ViewModel")
	TMap<AActor*, UAttributeViewModel*> RegisteredViewModels;
	/** Function End*/
};

template <class T>
T* UUIManagerSubsystem::TryGetViewModel(AActor* InActor)
{
	if (!InActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryGetViewModel: InActor is null"));
		return nullptr;
	}

	APawn* InPawn = Cast<APawn>(InActor);
	if (!InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryGetViewModel: Actor [%s] is not a Pawn, ViewModel not available"), *InActor->GetName());
		return nullptr;
	}

	if (UAttributeViewModel** FoundViewModel = RegisteredViewModels.Find(InPawn))
	{
		if (T* CastedViewModel = Cast<T>(*FoundViewModel))
		{
			return CastedViewModel;
		}
		UE_LOG(LogTemp, Warning,
			TEXT("TryGetViewModel: Actor [%s] 已注册的 ViewModel 类型不匹配，返回 nullptr"), *InActor->GetName());
		return nullptr;
	}

	T* ViewModel = UViewModelBase::GetViewModel<T>(InPawn->GetController(), InPawn);
	if (!ViewModel)
	{
		Debug::Print(FString::Printf(TEXT("%s ViewModel 初始化失败"), *InActor->GetName()));
		return nullptr;
	}
	RegisteredViewModels.Add(InPawn, ViewModel);

	return ViewModel;
}

template <class T>
T* UUIManagerSubsystem::FindViewModel(AActor* InActor) const
{
	if (!InActor) return nullptr;

	if (UAttributeViewModel* const* FoundViewModel = RegisteredViewModels.Find(InActor))
	{
		return Cast<T>(*FoundViewModel);
	}

	return nullptr;
}
