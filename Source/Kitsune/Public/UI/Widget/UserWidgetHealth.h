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
	/*Function Begin*/

	virtual void SetCharacterStateViewModel(UCharacterStateViewModel* CS_VM) override;
	/*Function End*/

	/*Variable Begin*/

	/*Variable End*/
protected:
	/*Function Begin*/
	virtual void NativeConstruct() override;

	/*Function End*/

	/*Variable Begin*/

	/*Variable End*/
};
