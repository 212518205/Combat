// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "TileViewEntryBase.generated.h"

class UCommonTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class KITSUNE_API UTileViewEntryBase : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	/***   ...IUserObjectListEntry Interface Begin...   ***/
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	/***   ...IUserObjectListEntry Interface End...     ***/

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Image_DisplayImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock>  CommonTextBlock_DisplayName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock>  CommonTextBlock_ShowAsNewText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_HotKeyText;

};
