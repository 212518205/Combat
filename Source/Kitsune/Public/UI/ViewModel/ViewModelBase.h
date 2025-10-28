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
	template<typename T>
	static T* GetViewModel(UAbilitySystemComponent* InASC, UAttributeSet* InAS);


	virtual void BindCallback() {}

protected:
	UPROPERTY(BlueprintReadOnly,Category="Player",meta = (AllowPrivateAccess = "true"))
	UKitsuneAbilitySystemComponent* AbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadOnly,Category="Player",meta = (AllowPrivateAccess = "true"))
	UKitsuneAttributeSet* AttributeSet = nullptr;
};

template<typename T>
inline T* UViewModelBase::GetViewModel(UAbilitySystemComponent* InASC, UAttributeSet* InAS)
{
	T* ViewModel = NewObject<T>();
	ViewModel->AbilitySystemComponent = Cast<UKitsuneAbilitySystemComponent>(InASC);
	ViewModel->AttributeSet = Cast<UKitsuneAttributeSet>(InAS);
	return ViewModel;
}
