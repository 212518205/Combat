// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "WidgetListEntryBase.generated.h"

class UCommonTextBlock;
class UListDataObjectBase;
/**
 * 
 */
UCLASS()
class KITSUNE_API UWidgetListEntryBase : public UCommonUserWidget,public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void OnOwningListDataHandle(UListDataObjectBase* ItemData);

	/** Variable Begin*/
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_ListEntryTitle;
	/** Variable End*/
};
