// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "MenuEntryData.generated.h"


/**
 * 
 */
UCLASS()
class KITSUNE_API UMenuEntryData : public UObject
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
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ScreenToOpen;

};
