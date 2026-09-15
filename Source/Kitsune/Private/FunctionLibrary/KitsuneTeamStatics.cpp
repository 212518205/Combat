// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary/KitsuneTeamStatics.h"

#include "Game/WorldSubsystem/KitsuneFactionSubsystem.h"

void UKitsuneTeamStatics::ApplyFactionPreset(const UObject* WorldObjectContext, UDataAssetFactionMatrix* Matrix)
{
	if (UKitsuneFactionSubsystem* Subsystem = UKitsuneFactionSubsystem::GetFactionSubsystem(WorldObjectContext))
	{
		Subsystem->ApplyPreset(Matrix);
	}
}

int32 UKitsuneTeamStatics::GetTeamIDFromActor(const AActor* Actor)
{
	if (!Actor)return INDEX_NONE;
	if (const IGenericTeamAgentInterface* AgentInterface = Cast<IGenericTeamAgentInterface>(Actor))
	{
		return AgentInterface->GetGenericTeamId().GetId();
	}
	return INDEX_NONE;
}

ETeamAttitude::Type UKitsuneTeamStatics::GetAttitude(const AActor* FromActor, const AActor* ToActor)
{
	if (!FromActor || !ToActor)return ETeamAttitude::Neutral;

	const int32 From = GetTeamIDFromActor(FromActor);
	const int32 To = GetTeamIDFromActor(ToActor);
	if (From == INDEX_NONE || To == INDEX_NONE)return ETeamAttitude::Neutral;
	if (UKitsuneFactionSubsystem* Subsystem = UKitsuneFactionSubsystem::GetFactionSubsystem(FromActor))
	{
		return Subsystem->GetAttitude(static_cast<uint8>(From), static_cast<uint8>(To));
	}
	return ETeamAttitude::Neutral;
}

bool UKitsuneTeamStatics::IsHostile(const AActor* FromActor, const AActor* ToActor)
{
	return GetAttitude(FromActor, ToActor) == ETeamAttitude::Hostile;
}
