// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/PlayerViewModel.h"

void UPlayerViewModel::BindCallback()
{
	Super::BindCallback();
}

void UPlayerViewModel::SetPlayerWeaponIcon(const TSoftObjectPtr<UTexture2D> InWeaponIcon)
{
	WeaponIcon = InWeaponIcon;
	OnWeaponChanged.Broadcast();
}
