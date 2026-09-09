// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/CharacterWidget/WidgetCharacterComponentObject.h"

#include "UIManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UWidgetCharacterComponentObject::OnBindAndInitialized(APawn* Pawn)
{
	OwningPawn = Pawn;
	if (IsLocalPlayerPawn())
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	if (!GetOwningViewModel() || GetOwningViewModel()->OnHealthPercentChanged.IsBound())return;
	BP_OnBindAndInitialized();
}

bool UWidgetCharacterComponentObject::IsLocalPlayerPawn() const
{
	if (!OwningPawn)return false;
	const APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	return PC && PC->GetPawn() == OwningPawn;
}

UAttributeViewModel* UWidgetCharacterComponentObject::GetOwningViewModel() const
{
	if (UUIManagerSubsystem* Subsystem = UUIManagerSubsystem::GetUIManager(OwningPawn))
	{
		return Subsystem->GetViewModelByPawn(OwningPawn);
	}
	return nullptr;
}
