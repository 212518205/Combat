// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySyetem/KitsuneAbilitySystemComponent.h"
#include "AbilitySyetem/KitsuneAttributeSet.h"
#include "ViewModelBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class KITSUNE_API UViewModelBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void NativeInitialize() {}

	template <class T = UViewModelBase>
	static T* GetViewModel(AController* InController, APawn* InPawn);

protected:
	UPROPERTY(BlueprintReadOnly,Category="Player")
	AController* OwningController = nullptr;

	UPROPERTY(BlueprintReadOnly,Category="Player")
	APawn* OwningPawn = nullptr;
};

template<class T>
inline T* UViewModelBase::GetViewModel(AController* InController, APawn* InPawn)
{
	T* ViewModel = NewObject<T>();
	ViewModel->OwningController = InController;
	ViewModel->OwningPawn = InPawn;
	ViewModel->NativeInitialize();
	return ViewModel;
}
