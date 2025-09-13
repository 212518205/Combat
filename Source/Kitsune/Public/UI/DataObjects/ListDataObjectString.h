// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DataObjects/ListDataObjectValue.h"
#include "ListDataObjectString.generated.h"

class FGameSettingsSaveHelper;
/**
 * 
 */
UCLASS()
class KITSUNE_API UListDataObjectString : public UListDataObjectValue
{
	GENERATED_BODY()

public:
	/** Function Begin*/
	void AddDynamicOption(const FString& InDisplayOption);
	void SetCurrentDisplayOption(FString InDisplayOption);
	FString GetGameSettingsOption() const;
	/** Function End*/
	FORCEINLINE FString GetCurrentDisplayOption() { return CurrentDisplayOption; }
	FORCEINLINE TArray<FString> GetDisplayOptions() { return DisplayOptions; }

protected:
	virtual void NotifySettingsModified(const FString NewSettings) override;

private:
	/** Variable Begin*/
	FString CurrentDisplayOption;
	TArray<FString> DisplayOptions;
	/** Variable End*/
};
