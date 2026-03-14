// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MenuEntryBase.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class KITSUNE_API UMenuEntryBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	/***   ...IUserObjectListEntry Interface Begin...   ***/
	void NativeOnEntryInitialize(UObject* ListItemObject) const;
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
