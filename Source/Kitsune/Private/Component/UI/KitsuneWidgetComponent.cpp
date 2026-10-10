// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/UI/KitsuneWidgetComponent.h"

#include "UI/Widget/Components/CharacterWidget/CharacterWidgetBase.h"


void UKitsuneWidgetComponent::InitWidget()
{
	Super::InitWidget();
	
	AActor* Owner = GetOwner();
	if (const APawn* OwnerPawn = Cast<APawn>(Owner))
	{
		if (UCharacterWidgetBase* StatusWidget = Cast<UCharacterWidgetBase>(GetUserWidgetObject()))
		{
			StatusWidget->OnBindAndInitialized(const_cast<APawn*>(OwnerPawn));
		}
	}
}
