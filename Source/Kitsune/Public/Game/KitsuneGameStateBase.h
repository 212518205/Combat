// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "KitsuneGameStateBase.generated.h"

class UDataAssetFactionMatrix;
/**
 * 
 */
UCLASS()
class KITSUNE_API AKitsuneGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetFactionPreset(UDataAssetFactionMatrix* Matrix);
	UDataAssetFactionMatrix* GetFactionPreset() const { return FactionPreset; }
	
protected:
	UFUNCTION()
	void OnRep_FactionPreset() const;
	
	UPROPERTY(ReplicatedUsing = OnRep_FactionPreset, BlueprintReadOnly)
	TObjectPtr<UDataAssetFactionMatrix> FactionPreset;
	
};
