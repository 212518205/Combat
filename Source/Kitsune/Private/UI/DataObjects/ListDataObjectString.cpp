// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DataObjects/ListDataObjectString.h"

#include "GameSettings/GameSettingsSaveHelper.h"
#include "GameSettings/KitsuneGameUserSettings.h"

void UListDataObjectString::AddDynamicOption(const FString& InDisplayOption)
{
	DisplayOptions.Add(InDisplayOption);
}

void UListDataObjectString::SetCurrentDisplayOption(FString InDisplayOption)
{
	CurrentDisplayOption = InDisplayOption;

	//T0D0 使用GameUserSettings保存当前内容
	NotifySettingsModified(InDisplayOption);
}

FString UListDataObjectString::GetGameSettingsOption() const
{
	if (!DynamicSettingsGetterHelper)return{};
	return DynamicSettingsGetterHelper->GetValueAsString();
}

void UListDataObjectString::NotifySettingsModified(const FString NewSettings)
{
	Super::NotifySettingsModified(NewSettings);

	if (bAutoSave&&DynamicSettingsSetterHelper)
	{
		DynamicSettingsSetterHelper->SetValueFromString(NewSettings);
		UKitsuneGameUserSettings::GetGameUserSettings()->SaveSettings();
	}
}
