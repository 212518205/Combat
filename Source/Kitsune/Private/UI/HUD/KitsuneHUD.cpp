// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/KitsuneHUD.h"

#include "Blueprint/UserWidget.h"

void AKitsuneHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}
