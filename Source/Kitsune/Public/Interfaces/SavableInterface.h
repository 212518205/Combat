// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavableInterface.generated.h"

class UKitsuneSaveGame;
// This class does not need to be modified.
UINTERFACE()
class USavableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KITSUNE_API ISavableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SaveTo(UKitsuneSaveGame* SaveGame) = 0;
	
	virtual void LoadFrom(const UKitsuneSaveGame* SaveGame) = 0;
	
};
