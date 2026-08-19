// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "KitsuneGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual FGameInstancePIEResult InitializeForPlayInEditor(int32 PIEInstanceIndex, const FGameInstancePIEParameters& Params) override;
	
	FString GetPIEOverrideCredential() const { return PIEOverrideCredential; }
	
protected:
	FString PIEOverrideCredential;
	
};
