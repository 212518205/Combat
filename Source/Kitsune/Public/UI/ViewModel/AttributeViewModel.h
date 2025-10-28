// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "AttributeViewModel.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UAttributeViewModel : public UViewModelBase
{
	GENERATED_BODY()

public:
	virtual void BindCallback() override;

protected:
	/** Variable Begin*/
	UPROPERTY(BlueprintReadOnly,Category = "Main Attribute Value")
	float Health = 0.f;
	
	UPROPERTY(BlueprintReadOnly,Category = "Main Attribute Value")
	float MaxHealth = 0.f;
	
	UPROPERTY(BlueprintReadOnly,Category = "Main Attribute Value")
	float Stamina = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Main Attribute Value")
	float MaxStamina = 0.f;



	/** Variable End*/
};
