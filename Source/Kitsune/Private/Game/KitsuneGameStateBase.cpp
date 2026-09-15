// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/KitsuneGameStateBase.h"

#include "FunctionLibrary/KitsuneTeamStatics.h"
#include "Net/UnrealNetwork.h"

void AKitsuneGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(AKitsuneGameStateBase, FactionPreset, COND_None, REPNOTIFY_Always);
}

void AKitsuneGameStateBase::SetFactionPreset(UDataAssetFactionMatrix* Matrix)
{
	if (!HasAuthority() || !Matrix)return;
	FactionPreset = Matrix;
	OnRep_FactionPreset();
}

void AKitsuneGameStateBase::OnRep_FactionPreset() const
{
	UKitsuneTeamStatics::ApplyFactionPreset(this, FactionPreset);
}
