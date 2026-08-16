// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// ===== [修改] 由 UKitsuneClickableWidget 改为 UCommonButtonBase，统一到 CommonUI 的可点击体系 =====
#include "CommonButtonBase.h"
#include "Components/Image.h"
#include "MenuEntryBase.generated.h"

class UMenuEntryData;
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
	UFUNCTION(BlueprintCallable)
	void NativeOnEntryInitialize(UObject* ListItemObject);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_EntryInitialize();
	 
protected:	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock>  CommonTextBlock_DisplayName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock>  CommonTextBlock_ShowAsNewText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CommonTextBlock_HotKeyText;

	UPROPERTY(BlueprintReadOnly)
	UMenuEntryData* CachedEntryData;

};
