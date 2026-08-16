// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CommonButtonBase.h"
#include "UI/DataObjects/Inventory/InventorySlotData.h"
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
class KITSUNE_API UBackBagEntryBase : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	void RefreshEntry() const;
	
protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_BackBagEntryInitialize();

	/***  BindWidget   `BC@` ***/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_Background;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_LockFlag;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> TextBlock_ShowNew;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_Icon;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> TextBlock_StackCount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> LockBackgroundIcon;
	
	UFUNCTION(BlueprintCallable)
	void ClearEntryStyle() const;
	
	UFUNCTION(BlueprintCallable)
	bool IsItemEntry() const;
	
	UFUNCTION(BlueprintCallable)
	bool IsEmptyEntry() const;
	
	UFUNCTION(BlueprintCallable)
	bool IsLockEntry() const;

private:
	UFUNCTION(BlueprintCallable)
	UInventoryItemInstance* GetItemInstance() const;
	
	void RefreshStackCount() const;
	
	void SetButtonIcon(UTexture2D* IconTexture) const;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventorySlotData> CachedSlotData;
	
};
