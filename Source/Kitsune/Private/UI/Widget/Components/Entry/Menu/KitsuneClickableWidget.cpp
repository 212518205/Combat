// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/Entry/Menu/KitsuneClickableWidget.h"

#include "FrontendDebugHelper.h"

void UKitsuneClickableWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OnMouseEnter.AddUObject(this, &ThisClass::OnHovered);
	OnMouseLeave.AddUObject(this, &ThisClass::OnUnHovered);
	OnMouseDown.AddUObject(this, &ThisClass::OnPressed);
	OnMouseUp.AddUObject(this, &ThisClass::OnReleased);
}

void UKitsuneClickableWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	bIsHovered = true;
	OnMouseEnter.Broadcast();
}

void UKitsuneClickableWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bIsHovered = false;
	OnMouseLeave.Broadcast();
}

FReply UKitsuneClickableWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsClicked = true;
	OnMouseDown.Broadcast();

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UKitsuneClickableWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsClicked = false;
	if (bIsHovered)
	{
		OnMouseUp.Broadcast();
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UKitsuneClickableWidget::OnHovered()
{
	MouseEnterEvent();
}

void UKitsuneClickableWidget::OnUnHovered()
{
	MouseLeaveEvent();
}

void UKitsuneClickableWidget::OnPressed()
{
	MouseDownEvent();
}

void UKitsuneClickableWidget::OnReleased()
{
	MouseUpEvent();
}
