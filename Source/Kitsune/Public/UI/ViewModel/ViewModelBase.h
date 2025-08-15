// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/KitsunePlayerState.h"
#include "Player/KitsunePlayerController.h"
#include "AbilitySyetem/KitsuneAbilitySystemComponent.h"
#include "AbilitySyetem/KitsuneAttributeSet.h"
#include "UObject/NoExportTypes.h"
#include "ViewModelBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class KITSUNE_API UViewModelBase : public UObject
{
	GENERATED_BODY()

public:
	/** Function Begin*/
	template<typename T>
	static T* GetViewModel(APlayerState* InPS, APlayerController* InPC,
		UAbilitySystemComponent* InASC, UAttributeSet* InAS);
	/** Function End*/

private:
	/** Variable Begin*/
	UPROPERTY(BlueprintReadOnly,Category="Player",meta = (AllowPrivateAccess = "true"))
	AKitsunePlayerState* PlayerState = nullptr;

	UPROPERTY(BlueprintReadOnly,Category="Player",meta = (AllowPrivateAccess = "true"))
	UKitsuneAbilitySystemComponent* AbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadOnly,Category="Player",meta = (AllowPrivateAccess = "true"))
	UKitsuneAttributeSet* AttributeSet = nullptr;

	UPROPERTY(BlueprintReadOnly,Category="Player",meta = (AllowPrivateAccess = "true"))
	AKitsunePlayerController* PlayerController = nullptr;
	/** Variable End*/
};

template<typename T>
inline T* UViewModelBase::GetViewModel(APlayerState* InPS, APlayerController* InPC, UAbilitySystemComponent* InASC, UAttributeSet* InAS)
{
	T* ViewModel = NewObject<T>();
	ViewModel->PlayerState = Cast<AKitsunePlayerState>(InPS);
	ViewModel->PlayerController = Cast<AKitsunePlayerController>(InPC);
	ViewModel->AbilitySystemComponent = Cast<UKitsuneAbilitySystemComponent>(InASC);
	ViewModel->AttributeSet = Cast<UKitsuneAttributeSet>(InAS);
	return ViewModel;
}
