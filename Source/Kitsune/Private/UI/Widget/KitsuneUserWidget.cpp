// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/KitsuneUserWidget.h"

#include "UI/WidgetController/KitsuneWidgetController.h"


void UKitsuneUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
