// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/KitsuneComboBoxString.h"

#include "FrontendDebugHelper.h"

void UKitsuneComboBoxString::AddOptions(const TArray<FString>& InOptions)
{
	Options.Empty();
	for (const auto& OptionName:InOptions)
	{
			AddOption(OptionName);
	}
}

void UKitsuneComboBoxString::HandleOpening()
{
	Super::HandleOpening();
	Debug::Print(GetSelectedOption() + TEXT("被点击,index == ")+ FString::FromInt(GetSelectedIndex()));
	for (const auto& OptionName : Options)
	{
		Debug::Print(*OptionName);
	}
}
