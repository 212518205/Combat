// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendStructTypes.h"
#include "GameFramework/PlayerController.h"
#include "KitsunePlayerControllerBase.generated.h"

class UKitsuneInputConfig;
/**
 * 
 */
UCLASS()
class KITSUNE_API AKitsunePlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void OnRep_Pawn() override;

protected:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void TravelToLevel(const FString& LevelName, TSubclassOf<AGameModeBase> GameModeClass, const bool bSeamless);
	
	virtual void OnControlledPawnChanged();
	virtual UKitsuneInputConfig* GetInputConfig() { return nullptr; }
	virtual bool ShouldApplyInputForPawn(const APawn* InPawn);
	/***   ...APlayerController Interface Begin...   ***/
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	/***   ...APlayerController Interface End...     ***/
	
	/***  SaveGame   `BC@` ***/
	UFUNCTION(Server, Reliable)
	void Server_ReportLocalCredential(const FString& InCredential);
	void OnCredentialReported(const FString& InCredential) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FKitsuneInitialWidgetConfig WidgetConfig;

};
