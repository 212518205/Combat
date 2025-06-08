// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/KitsuneHUD.h"

#include "Blueprint/UserWidget.h"
#include "Characters/KitsuneCharacter.h"
#include "UI/Widget/UserWidgetHealth.h"
#include "UI/Widget/UserWidgetOverlay.h"

void AKitsuneHUD::InitCharacterState(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
                                     UAttributeSet* AS)
{
	const FModelSet ModelSet(PC, PS, ASC, AS);
	UUserWidget* Widget = CreateWidget(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
	CharStateWidget = Cast<UUserWidgetOverlay>(Widget);
	checkf(CharStateWidget, TEXT("Overlay ×ª»»Ê§°Ü,ÔÚHUDÖÐ"));
	CharStateWidget->GetHealthWidget()->SetViewModel(ModelSet);
}

void AKitsuneHUD::BeginPlay()
{
	Super::BeginPlay();

}
