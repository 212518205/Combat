// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/KitsuneTabListWidgetBase.h"

#include "Editor/WidgetCompilerLog.h"
#include "UI/Widget/Components/KitsuneCommonButtonBase.h"


#if WITH_EDITOR
void UKitsuneTabListWidgetBase::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	RegisterTab(InTabID, TabButtonEntryWidgetClass, nullptr);
	if (const UKitsuneCommonButtonBase* Button=Cast<UKitsuneCommonButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		Button->SetButtonText(InTabDisplayName);
	}
}

void UKitsuneTabListWidgetBase::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(
			TEXT("Needs a valid entry widget class to function properly-->") + GetClass()->GetName()
			)
		);
	}
}
#endif
