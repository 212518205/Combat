// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KitsunePlayerControllerBase.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Characters/KitsuneCharacter.h"
#include "Component/Combat/KitsuneCombatComponent.h"
#include "Game/KitsunePlayerState.h"
#include "Game/GameInstanceSubsystem/KitsuneSaveSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "Input/KitsuneInputConfig.h"
#include "UserSettings/EnhancedInputUserSettings.h"

static FString BuildTravelURL(const FString& LevelName, const TSubclassOf<AGameModeBase> GameModeClass,
                              const bool bSeamless)
{
	TArray<FString> Options;
	if (GameModeClass)
	{
		Options.Add(FString::Printf(TEXT("game=%s"), *GameModeClass->GetPathName()));
	}
	if (bSeamless)
	{
		Options.Add(TEXT("SeamlessTravel"));
	}

	return Options.IsEmpty()
		       ? LevelName
		       : FString::Printf(TEXT("%s?%s"), *LevelName, *FString::Join(Options, TEXT("?")));
}


void AKitsunePlayerControllerBase::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	OnControlledPawnChanged();
}

void AKitsunePlayerControllerBase::TravelToLevel_Implementation(const FString& LevelName,
                                                                const TSubclassOf<AGameModeBase> GameModeClass,
                                                                const bool bSeamless)
{
	UWorld* World = GetWorld();
	if (!World || LevelName.IsEmpty())return;
	const FString TravelURL = BuildTravelURL(LevelName, GameModeClass, bSeamless);

	World->ServerTravel(TravelURL, true);
}

void AKitsunePlayerControllerBase::OnControlledPawnChanged()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (!Subsystem)return;

	UKitsuneInputConfig* KitsuneInputConfig = GetInputConfig();
	if (!KitsuneInputConfig)return;

	if (const bool bIsApplyInput = KitsuneInputConfig && ShouldApplyInputForPawn(GetPawn()); !bIsApplyInput)
	{
		for (const FKitsuneInputMapping& MappingContext : KitsuneInputConfig->InputMappings)
		{
			if (const UInputMappingContext* IMC = MappingContext.IMC.Get())
			{
				Subsystem->RemoveMappingContext(IMC);
			}
		}
		return;
	}
	for (const auto& [IMC, Priority, bRegisterWithSettings] : KitsuneInputConfig->InputMappings)
	{
		if (!IMC)continue;
		if (bRegisterWithSettings)
		{
			if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
			{
				Settings->RegisterInputMappingContext(IMC.Get());
			}
		}
		Subsystem->AddMappingContext(IMC.Get(), Priority);
	}
}

bool AKitsunePlayerControllerBase::ShouldApplyInputForPawn(const APawn* InPawn)
{
	return InPawn && InPawn->Implements<UPawnInteractInterface>();
}

void AKitsunePlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (UKitsuneSaveSubsystem* SaveSubsystem = UKitsuneSaveSubsystem::GetSaveSubsystem(this); SaveSubsystem &&
		IsLocalController())
	{
		const FString LocalCredential = SaveSubsystem->GetOrCreateLocalCredential();
		if (HasAuthority())
		{
			OnCredentialReported(LocalCredential);
		}
		else
		{
			Server_ReportLocalCredential(LocalCredential);
		}
	}
}

void AKitsunePlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AKitsunePlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OnControlledPawnChanged();
}

void AKitsunePlayerControllerBase::OnUnPossess()
{
	Super::OnUnPossess();

	OnControlledPawnChanged();
}

void AKitsunePlayerControllerBase::Server_ReportLocalCredential_Implementation(const FString& InCredential)
{
	OnCredentialReported(InCredential);
}

void AKitsunePlayerControllerBase::OnCredentialReported(const FString& InCredential) const
{
	AKitsunePlayerState* KitsunePlayerState = GetPlayerState<AKitsunePlayerState>();
	if (!KitsunePlayerState)return;
	if (const int64 PlayerUID = UKitsuneSaveSubsystem::ResolvePlayerCredential(InCredential); PlayerUID >= 0)
	{
		KitsunePlayerState->SetPlayerUID(PlayerUID);
		if (const AKitsuneCharacter* KitsuneCharacter = Cast<AKitsuneCharacter>(GetPawn()))
		{
			KitsuneCharacter->BindAndLoadSave();
		}
	}
}
