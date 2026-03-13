// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTileView.h"
#include "KitsuneCommonTileView.generated.h"

class UTileEntryMapping;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, meta = (DisableNaiveTick))
class KITSUNE_API UKitsuneCommonTileView : public UCommonTileView
{
	GENERATED_BODY()

protected:
	/***   ...UCommonTileView Interface Begin...   ***/
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;

	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;
	/***   ...UCommonTileView Interface End...     ***/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTileEntryMapping* TileEntryMapping;
};
