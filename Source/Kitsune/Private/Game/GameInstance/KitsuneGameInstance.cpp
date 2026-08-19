// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameInstance/KitsuneGameInstance.h"

#include "Game/GameInstanceSubsystem/KitsuneSaveSubsystem.h"

FGameInstancePIEResult UKitsuneGameInstance::InitializeForPlayInEditor(int32 PIEInstanceIndex,
                                                                       const FGameInstancePIEParameters& Params)
{
	PIEOverrideCredential = FString::Printf(TEXT("PIE_Client_%d"), PIEInstanceIndex);
	
	return Super::InitializeForPlayInEditor(PIEInstanceIndex, Params);
}
