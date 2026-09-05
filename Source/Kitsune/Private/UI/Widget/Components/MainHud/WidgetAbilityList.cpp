// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/MainHud/WidgetAbilityList.h"

#include "UIManagerSubsystem.h"
#include "Components/HorizontalBox.h"
#include "UI/ViewModel/PlayerViewModel.h"
#include "UI/Widget/Components/MainHud/WidgetAbilityEntry.h"

void UWidgetAbilityList::NativeConstruct()
{
	Super::NativeConstruct();
	
	for (int32 Index = 0; Index < UpRowSlotCount; Index++)
	{
		UWidgetAbilityEntry* Entry = CreateWidget<UWidgetAbilityEntry>(this, AbilityEntryClass);
		if (!Entry)
		{
			continue;
		}
		Entry->SetVisibility(ESlateVisibility::Collapsed);
		HorizontalBox_Up->AddChild(Entry);
	}
	for (int32 Index = 0; Index < DownRowSlotCount; Index++)
	{
		UWidgetAbilityEntry* Entry = CreateWidget<UWidgetAbilityEntry>(this, AbilityEntryClass);
		if (!Entry)
		{
			continue;
		}
		Entry->SetVisibility(ESlateVisibility::Collapsed);
		HorizontalBox_Down->AddChild(Entry);
	}
	
	
	if (UPlayerViewModel* LocalVM = GetLocalPlayerViewModel())
	{
		LocalVM->OnGameplayAbilityChanged.AddDynamic(this, &ThisClass::HandleAbilityChanged);
		RefreshAbilitySlots();
	}
}

void UWidgetAbilityList::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void UWidgetAbilityList::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (UPlayerViewModel* LocalVM = GetLocalPlayerViewModel())
	{
		LocalVM->OnGameplayAbilityChanged.RemoveDynamic(this, &ThisClass::HandleAbilityChanged);
	}
}

void UWidgetAbilityList::HandleAbilityChanged(FGameplayAbilitySpecHandle& SpecHandle)
{
	RefreshAbilitySlots();
}

void UWidgetAbilityList::RefreshAbilitySlots()
{
	const UPlayerViewModel* LocalVM = GetLocalPlayerViewModel();
	if (!LocalVM)return;
	const TArray<FAbilityUIData> Snapshot = LocalVM->GetPlayerAbilities();
	const int32 TotalSlots = UpRowSlotCount + DownRowSlotCount;
	const int32 ShowCount = FMath::Min(TotalSlots, Snapshot.Num());
	
	for (int32 Index = 0; Index < TotalSlots; Index++)
	{
		UWidgetAbilityEntry* Entry = Cast<UWidgetAbilityEntry>(GetEntryByIndex(Index));
		if (!Entry)continue;
		if (Index < ShowCount)
		{
			Entry->AbilityEntryInitialize(Snapshot[Index]);
		}
		else
		{
			Entry->ClearAbilityEntry();
		}
	}
}

UWidget* UWidgetAbilityList::GetEntryByIndex(const int32 Index) const
{
	if (!HorizontalBox_Up || !HorizontalBox_Down)return nullptr;
	if (Index < UpRowSlotCount)
	{
		return Index < HorizontalBox_Up->GetChildrenCount() ? HorizontalBox_Up->GetChildAt(Index) : nullptr;
	}
	const int32 DownIndex = Index - UpRowSlotCount;
	return DownIndex < HorizontalBox_Down->GetChildrenCount() ? HorizontalBox_Down->GetChildAt(DownIndex) : nullptr;
}

UPlayerViewModel* UWidgetAbilityList::GetLocalPlayerViewModel()
{
	if (!CachedLocalViewModel)
	{
		CachedLocalViewModel = UUIManagerSubsystem::GetUIManager(GetOwningPlayer())->TryGetViewModelByActor<UPlayerViewModel>(GetOwningPlayerPawn());
	}

	return CachedLocalViewModel;
}
