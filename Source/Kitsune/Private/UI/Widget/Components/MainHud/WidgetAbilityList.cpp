// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/MainHud/WidgetAbilityList.h"

#include "Components/HorizontalBox.h"
#include "UI/Widget/Components/MainHud/WidgetAbilityEntry.h"

void UWidgetAbilityList::FillAbilityList(const TArray<FAbilityUIData>& InAbilityUIDates)
{
	HorizontalBox_Up->ClearChildren();
	HorizontalBox_Down->ClearChildren();
	AbilityUIDates = InAbilityUIDates;
	for (int32 Index = 0; Index < AbilityUIDates.Num(); Index++)
	{
		UWidgetAbilityEntry* Entry = CreateWidget<UWidgetAbilityEntry>(this, AbilityEntryClass);
		if (!Entry)
		{
			continue;
		}
		Entry->AbilityInitialize(AbilityUIDates[Index]);
		if (HorizontalBox_Up->GetChildrenCount() < 2)
		{
			HorizontalBox_Up->AddChild(Entry);
		}
		else
		{
			HorizontalBox_Down->AddChild(Entry);
		}
	}
	
	BP_FillAbilityList();
}

void UWidgetAbilityList::BP_FillAbilityList_Implementation()
{
}

