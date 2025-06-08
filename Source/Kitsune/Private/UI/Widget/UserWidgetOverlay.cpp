// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/UserWidgetOverlay.h"

#include "UI/Widget/UserWidgetHealth.h"

UUserWidgetOverlay::UUserWidgetOverlay()
{

}

void UUserWidgetOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	UUserWidget* Widget = CreateWidget(GetWorld(), WidgetHealthClass);
	WidgetHealth = Cast<UUserWidgetHealth>(Widget);
	checkf(WidgetHealth, TEXT("WidgetHealth Cast Ê§°Ü£¬ÔÚOverlayÖÐ"));
}


