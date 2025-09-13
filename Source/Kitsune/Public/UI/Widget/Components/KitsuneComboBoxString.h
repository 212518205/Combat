// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ComboBoxString.h"
#include "KitsuneComboBoxString.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneComboBoxString : public UComboBoxString
{
	GENERATED_BODY()

public:
	void AddOptions(const TArray<FString>& InOptions);
protected:
	virtual void HandleOpening() override;
};
