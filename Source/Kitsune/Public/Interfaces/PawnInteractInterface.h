// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PawnInteractInterface.generated.h"

class UInteractionComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPawnInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KITSUNE_API IPawnInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UInteractionComponent* GetInteractionComp() = 0;
};
