// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/UserWidgetBase.h"
#include "UserWidgetHealth.generated.h"

struct FModelSet;
class UCharacterStateViewModel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChangedEvent, float, NewValue);

/**
 * 
 */
UCLASS()
class KITSUNE_API UUserWidgetHealth : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	void SetViewModel(const FModelSet& ModelSet);

	~UUserWidgetHealth() { UE_LOG(LogTemp, Warning, TEXT("111111111aaaaaaaaaaa")); }

	UFUNCTION(BlueprintPure,Category="Attribute")
	float GetHealthPercent() const { return MaxHealth > 0 ? Health / MaxHealth : 0; }

	UPROPERTY(BlueprintAssignable)
	FHealthChangedEvent OnHealthChangedEvent;

	UPROPERTY(BlueprintAssignable)
	FHealthChangedEvent OnMaxHealthChangedEvent;
protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable,Category="Attribute")
	void UpdateHealthDisplay(float NewHealth);

	UFUNCTION(BlueprintCallable,Category="Attribute")
	void UpdateMaxHealthDisplay(float NewMaxHealth);

	UPROPERTY(EditAnywhere,Category="ViewModel")
	TSubclassOf<UCharacterStateViewModel> CharStateVMClass;

	UPROPERTY(BlueprintReadOnly,Category="Attribute",meta=(FiledNotify))
	float Health;
	 
	UPROPERTY(EditAnywhere,Category="Attribute",meta=(FieldNotify))
	float MaxHealth;

};
