// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/KitsuneHUD.h"

#include "Blueprint/UserWidget.h"
#include "Characters/KitsuneCharacter.h"
#include "UI/ViewModel/CharacterStateViewModel.h"
#include "UI/Widget/UserWidgetHealth.h"
#include "UI/Widget/UserWidgetOverlay.h"

void AKitsuneHUD::InitCharacterState(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
                                     UAttributeSet* AS)
{
	const FModelSet ModelSet(PC, PS, ASC, AS);
	CharStateWidget = CreateWidget<UUserWidgetOverlay>(GetWorld(), OverlayWidgetClass);
	CharStateWidget->AddToViewport();
	CharStateWidget->SetCharacterStateViewModel(GetCSViewModel(ModelSet));
}

void AKitsuneHUD::BeginPlay()
{
	Super::BeginPlay();

}

UCharacterStateViewModel* AKitsuneHUD::GetCSViewModel(const FModelSet& ModelSet)
{
	if (!CSViewModel)
	{
		CSViewModel = NewObject<UCharacterStateViewModel>(this, CSViewModelClass);
		CSViewModel->Initialize(ModelSet);
	}
	return CSViewModel;
}
