// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KitsuneActivatableWidgetStack.h"

#include "FrontendDebugHelper.h"
#include "UI/Widget/WidgetActivatableBase.h"

UKitsuneActivatableWidgetStack::UKitsuneActivatableWidgetStack(const FObjectInitializer& Initializer)
	:Super(Initializer)
{
}

UWidgetActivatableBase* UKitsuneActivatableWidgetStack::PushWidget(UPARAM(meta = (Categories = "UI.Widget")) const FGameplayTag InWidgetTag, UClass* WidgetClass)
{
	if (!InWidgetTag.IsValid() || !WidgetClass || !MySwitcher)return nullptr;
	
	UWidgetActivatableBase* WidgetToShow = nullptr;
	if (UWidgetActivatableBase** Found = CachedWidgets.Find(InWidgetTag))
	{
		WidgetToShow = *Found;
		
		if (StackWidgets.Contains(WidgetToShow))
		{
			StackWidgets.Remove(WidgetToShow);
			if (const TSharedPtr<SWidget> SlateWidget = WidgetToShow->GetCachedWidget(); SlateWidget)
			{
				MySwitcher->RemoveSlot(SlateWidget.ToSharedRef());
			}
		}
	}

	if (!WidgetToShow)
	{
		WidgetToShow = NewObject<UWidgetActivatableBase>(this, WidgetClass);
		if (!WidgetToShow)return nullptr;
		CachedWidgets.Add(InWidgetTag, WidgetToShow);
	}
	
	StackWidgets.Add(WidgetToShow);

	const TSharedRef<SWidget> SlateWidget = WidgetToShow->TakeWidget();
	MySwitcher->AddSlot() [SlateWidget];

	const int32 NewIndex = MySwitcher->GetNumWidgets() - 1;
	MySwitcher->TransitionToIndex(NewIndex);
	
	return WidgetToShow;
}

void UKitsuneActivatableWidgetStack::PopWidget()
{
	if (!MySwitcher || StackWidgets.Num() == 0)return;
	
	UWidgetActivatableBase* TopWidget = StackWidgets.Last();
	if (!TopWidget)return;
	
	if (TopWidget->IsActivated())
	{
		TopWidget->DeactivateWidget();
	}
	
	StackWidgets.Pop();
	
	if (const TSharedPtr<SWidget> SlateWidget = TopWidget->GetCachedWidget())
	{
		MySwitcher->RemoveSlot(SlateWidget.ToSharedRef());
	}
	
	if (StackWidgets.Num() > 0)
	{
		const int32 NewTopIndex = MySwitcher->GetNumWidgets() - 1;
		MySwitcher->TransitionToIndex(NewTopIndex);
	}else
	{
		CurrentTopWidget = nullptr;
	}
	/***  栈为空时不应该隐藏，本身就无任何显示，通过PrimaryLayoutWidget中的四个Stack来进行优先级显示   `BC@` ***/
}

UWidgetActivatableBase* UKitsuneActivatableWidgetStack::GetTopWidget()
{
	return StackWidgets.Num() > 0 ? StackWidgets.Last() : nullptr;
}

UWidgetActivatableBase* UKitsuneActivatableWidgetStack::GetActivatableWidgetByTag(UPARAM(meta = (Categories = "UI.Widget")) const FGameplayTag InWidgetTag) const
{
	return CachedWidgets.FindRef(InWidgetTag);
}

void UKitsuneActivatableWidgetStack::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	
	CachedWidgets.Empty();
	StackWidgets.Empty();
	CurrentTopWidget = nullptr;
	MySwitcher.Reset();
}

TSharedRef<SWidget> UKitsuneActivatableWidgetStack::RebuildWidget()
{
	return SNew(SOverlay) 
		+ SOverlay::Slot()
		[
			SAssignNew(MySwitcher, SCommonAnimatedSwitcher)
			.TransitionCurveType(ETransitionCurve::Linear)
			.TransitionDuration(0.3f)
			.TransitionType(ECommonSwitcherTransition::FadeOnly)
			.OnActiveIndexChanged_UObject(this, &ThisClass::HandleActiveIndexChanged)
			.OnIsTransitioningChanged_UObject(this, &ThisClass::HandleTransitioningChanged)
		];
}

void UKitsuneActivatableWidgetStack::HandleActiveIndexChanged(int32 NewIndex)
{
	if (!MySwitcher)return;

	if (UWidgetActivatableBase* NewWidget = StackWidgets.IsValidIndex(NewIndex) ? StackWidgets[NewIndex] : nullptr; NewWidget != CurrentTopWidget)
	{
		if (CurrentTopWidget->IsActivated())
		{
			CurrentTopWidget->DeactivateWidget();
		}
		CurrentTopWidget = NewWidget;
		if (CurrentTopWidget && !CurrentTopWidget->IsActivated())
		{
			CurrentTopWidget->ActivateWidget();
		}
	}else
	{
		if (CurrentTopWidget && !CurrentTopWidget->IsActivated())
		{
			CurrentTopWidget->ActivateWidget();
		}
	}
}

void UKitsuneActivatableWidgetStack::HandleTransitioningChanged(bool bIsTransitioning)
{
}
