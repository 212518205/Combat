// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Characters/KitsuneCharacter.h"
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
	static UUIManagerSubsystem* GetUIManager(const UObject* WorldContextObject);
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UWidgetPrimaryLayout* GetRegisteredPrimaryLayout()const {return RegisteredPrimaryLayout;}
	AKitsuneCharacter* GetLocalPlayerPawn() const;

	/*** `@BC`   描述: ViewModel相关   `BC@` ***/
	UFUNCTION(BlueprintCallable, Category = "ViewModel", meta = (ExpandBoolAsExecs = "bIsValid"))
	UPlayerViewModel* GetLocalViewModel(bool& bIsValid);

	template<typename T = UAttributeViewModel>
	T* TryGetViewModelByActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "ViewModel")
	UAttributeViewModel* GetViewModelByPawn(APawn* InPawn);
	
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
	TMap<APawn*, UAttributeViewModel*> RegisteredViewModels;
	/** Function End*/
};

template <class T>
T* UUIManagerSubsystem::TryGetViewModelByActor(AActor* InActor)
{
	if (!InActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryGetViewModelByActor: InActor is null"));
		return nullptr;
	}

	APawn* InPawn = Cast<APawn>(InActor);
	if (!InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryGetViewModelByActor: Actor [%s] is not a Pawn, ViewModel not available"), *InActor->GetName());
		return nullptr;
	}

	if (UAttributeViewModel** FoundViewModel = RegisteredViewModels.Find(InPawn))
	{
		return CastChecked<T>(*FoundViewModel);
	}

	T* ViewModel = UViewModelBase::GetViewModel<T>(InPawn->GetController(), InPawn);
	RegisteredViewModels.Add(InPawn, ViewModel);

	return ViewModel;
}
