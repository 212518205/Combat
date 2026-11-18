#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "FunctionLibrary/KitsuneViewModelFunctionLibrary.h"
#include "WidgetActivatableBase.generated.h"

class UWidgetPrimaryLayout;
class UASCViewModel;
class AKitsunePlayerController;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class KITSUNE_API UWidgetActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	AKitsunePlayerController* GetOwningKitsunePlayerController();

	/***  DeterminesOutputType 会将返回值自动转为Class类型   `BC@` ***/
	UFUNCTION(BlueprintCallable, Category = "ViewModel",
		meta = (DeterminesOutputType = "Class", ExpandEnumAsExecs = "ExecResult"))
	UViewModelBase* GetLocalViewModelByClass(TSubclassOf<UViewModelBase> Class,
	                                         EViewModelAccessResult& ExecResult) const;

	UFUNCTION(BlueprintPure)
	UWidgetPrimaryLayout* GetPrimaryLayout() const;

	template <class T>
	T* GetViewModel() const;

private:
	TWeakObjectPtr<AKitsunePlayerController> CachedOwningKitsunePC;
};

template <class T>
T* UWidgetActivatableBase::GetViewModel() const
{
	return UKitsuneViewModelFunctionLibrary::GetViewModel<T>(GetOwningPlayerPawn());
}
