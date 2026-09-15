// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/KitsuneGameModeBase.h"

#include "Game/KitsuneGameStateBase.h"

void AKitsuneGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority() || !FactionPreset.IsValid())return;
	if (AKitsuneGameStateBase* KitsuneGameState = GetGameState<AKitsuneGameStateBase>())
	{
		KitsuneGameState->SetFactionPreset(FactionPreset.LoadSynchronous());
	}
}
