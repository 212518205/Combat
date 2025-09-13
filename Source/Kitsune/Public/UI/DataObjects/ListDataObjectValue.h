// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DataObjects/ListDataObjectBase.h"
#include "ListDataObjectValue.generated.h"

class FGameSettingsSaveHelper;
/**
 * 
 */
UCLASS()
class KITSUNE_API UListDataObjectValue : public UListDataObjectBase
{
	GENERATED_BODY()

public:
	void SetDynamicSettingsGetterHelper(const TSharedPtr<FGameSettingsSaveHelper>& InGetterHelper) { DynamicSettingsGetterHelper = InGetterHelper; }
	void SetDynamicSettingsSetterHelper(const TSharedPtr<FGameSettingsSaveHelper>& InSetterHelper) { DynamicSettingsSetterHelper = InSetterHelper; }
	void SetAutoSave(const bool InbAutoSave) { bAutoSave = InbAutoSave; }
protected:
	/** Function Begin*/
	virtual void OnDataInitialized() override;
	virtual void NotifySettingsModified(const FString NewSettings);
	/** Function End*/

	TSharedPtr<FGameSettingsSaveHelper> DynamicSettingsGetterHelper;
	TSharedPtr<FGameSettingsSaveHelper> DynamicSettingsSetterHelper;
	bool bAutoSave = true;
};
