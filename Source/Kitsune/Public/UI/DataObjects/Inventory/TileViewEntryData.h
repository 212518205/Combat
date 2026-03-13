// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "UObject/NoExportTypes.h"
#include "TileViewEntryData.generated.h"


/**
 * 
 */
UCLASS()
class KITSUNE_API UTileViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* DisplayIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName NameID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText HotKeyText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText ShowAsNewText;

	UPROPERTY(BlueprintReadOnly)
	int32 RequestLevel = 1;

	UPROPERTY(BlueprintReadOnly)
	ETileViewEntryType EntryType;

};
