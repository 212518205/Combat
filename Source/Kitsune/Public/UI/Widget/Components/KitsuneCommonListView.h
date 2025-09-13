// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "KitsuneCommonListView.generated.h"

class UListEntryMapping;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, meta = (DisableNaiveTick))
class KITSUNE_API UKitsuneCommonListView : public UCommonListView
{
	GENERATED_BODY()

public:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;

private:
	UPROPERTY(EditAnywhere, Category = "List Entry Settings")
	TObjectPtr<UListEntryMapping> ListEntryMapping;
};
