// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WorldSubsystem/KitsuneFactionSubsystem.h"

#include "Game/Data/DataAssetFactionMatrix.h"

UKitsuneFactionSubsystem* UKitsuneFactionSubsystem::GetFactionSubsystem(const UObject* WorldObjectContext)
{
	if (!WorldObjectContext)return nullptr;
	if (const UWorld* World = WorldObjectContext->GetWorld())
	{
		return World->GetSubsystem<UKitsuneFactionSubsystem>();
	}
	return nullptr;
}

void UKitsuneFactionSubsystem::ApplyPreset(UDataAssetFactionMatrix* Matrix)
{
	if (!Matrix)return;
	RuntimeTable = Matrix->Preset.Table;
}

void UKitsuneFactionSubsystem::SetAttitude(const uint8 FromTeamID, const uint8 ToTeamID,
	const ETeamAttitude::Type Attitude, const bool bBidirectional)
{
	if (!IsServer())return;
	RuntimeTable.FindOrAdd(FromTeamID).Attitudes.Add(ToTeamID, Attitude);
	if (bBidirectional)
	{
		RuntimeTable.FindOrAdd(ToTeamID).Attitudes.Add(FromTeamID, Attitude);
	}
}

ETeamAttitude::Type UKitsuneFactionSubsystem::GetAttitude(const uint8 FromTeamID, const uint8 ToTeamID)
{
	if (FromTeamID == ToTeamID)return ETeamAttitude::Friendly;
	if (const FFactionRelation* Relation = RuntimeTable.Find(FromTeamID))
	{
		if (const TEnumAsByte<ETeamAttitude::Type>* Attitude = Relation->Attitudes.Find(ToTeamID))
		{
			return *Attitude;
		}
		return Relation->DefaultAttitude;
	}
	return ETeamAttitude::Neutral;
}

bool UKitsuneFactionSubsystem::IsServer() const
{
	const UWorld* World = GetWorld();
	if (!World)return false;
	return World->GetNetMode() != NM_Client;
}
