// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CommonButtonBase.h"
#include "BackBagEntryBase.generated.h"

class UInventorySlotData;
class UCommonTextBlock;
class UCommonLazyImage;
class UItemTrait_Stack;
class UItemTrait_Interact;
class UItemTrait_Display;
class UInventoryItemInstance;
/**
 * 
 */
UCLASS()
class KITSUNE_API UBackBagEntryBase : public UCommonButtonBase, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	void RefreshEntry() const;
	
protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	TSoftObjectPtr<UTexture2D> T_BackGroundImage;

	/***  BindWidget   `BC@` ***/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_Background;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_Display;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> TextBlock_ShowNew;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> TextBlock_LockFlag;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> TextBlock_StackCount;

private:
	UPROPERTY(Transient)
	TObjectPtr<UInventoryItemInstance> CachedItemInstance;
	
	UPROPERTY(Transient)
	TObjectPtr<UInventorySlotData> CachedSlotData;
	
	UPROPERTY(Transient)
	TObjectPtr<UItemTrait_Display> CachedTraitDisplay;
	
	UPROPERTY(Transient)
	TObjectPtr<UItemTrait_Interact> CachedTraitInteract;
	
	UPROPERTY(Transient)
	TObjectPtr<UItemTrait_Stack> CachedTraitStack;
};
