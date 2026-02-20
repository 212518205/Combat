// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "AttributeViewModel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttributeChangedDelegate);

/**
 * 
 */
UCLASS()
class KITSUNE_API UAttributeViewModel : public UViewModelBase
{
	GENERATED_BODY()

public:

	virtual void NativeInitialize() override;

	/*** `@BC`   描述: 已捕获的属性值变化后需在蓝图处理   `BC@` ***/
	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnAttributeChangedDelegate OnHealthPercentChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnAttributeChangedDelegate OnStaminaPercentChanged;

protected:
	/*** `@BC`   描述: ViewModel需捕获的属性值   `BC@` ***/
	UPROPERTY(BlueprintReadOnly,Category = "ViewModel | Attribute")
	float Health = 0.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | Attribute")
	float MaxHealth = 0.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | Attribute")
	float Stamina = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | Attribute")
	float MaxStamina = 0.f;

	/*** `@BC`   描述: Pawn衍生变量   `BC@` ***/
	UPROPERTY(BlueprintReadWrite, Category = "Character")
	TObjectPtr<UKitsuneAbilitySystemComponent> CachedKitsuneASC;

	UPROPERTY(BlueprintReadWrite, Category = "Character")
	TObjectPtr<UKitsuneAttributeSet> CachedKitsuneAttributeSet;


};
