// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/InventoryComponent.h"
#include "UI/ViewModel/AttributeViewModel.h"
#include "PlayerViewModel.generated.h"

class UInventorySlotData;
class AWeaponBase;

UENUM()
enum class EItemInstanceAction : uint8
{
	EAddInstance = 1,
	ERemoveInstance = 2,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractableItemChangeDelegate, UInventoryItemInstance*, ItemInstance, EItemInstanceAction, InstanceAction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCategoryCapacityChangedDelegate, FName, CategoryID, int32, CategoryCapacity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayAbilityChangedDelegate, FGameplayAbilitySpecHandle&, Handle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReticleStateChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChangedDelegate, UInventoryItemInstance*, ItemInstance, EInventoryItemChangeType, ChangeType);

USTRUCT()
struct FInventoryCategoryItems
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<UInventoryItemInstance>> Items;
};

UCLASS()
class KITSUNE_API UPlayerViewModel : public UAttributeViewModel
{
	GENERATED_BODY()

public:
	virtual bool NativeInitialize(APawn* InPawn) override;

	/***  变化委托   ***/
	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnAttributeChangedDelegate OnWeaponChanged;

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnCategoryCapacityChangedDelegate CategoryCapacityChanged;

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnInteractableItemChangeDelegate OnInteractableItemChange;

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnGameplayAbilityChangedDelegate OnGameplayAbilityChanged;

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnReticleStateChangedDelegate OnReticleStateChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnInventoryItemChangedDelegate OnInventoryItemChanged;

	/***  武器   ***/
	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | WeaponIcon")
	TSoftObjectPtr<UTexture2D> WeaponIcon;

	/***  准心 / 感知   ***/
	void SetIsSensing(bool bInIsSensing);

	/***  交互物品：与所属玩家重叠的物品   ***/
	void AddInteractableItemInstance(UInventoryItemInstance* ItemInstance);
	void RemoveInteractableItemInstance(UInventoryItemInstance* ItemInstance);
	const TArray<UInventoryItemInstance*>& GetOverlappedItemInstances() const { return OverlappedItemInstances; }

	/***  库存查询   ***/
	TArray<TPair<FName, FText>> GetAllInventoryCategories() const;   // 全部分类 Tab（按 SortOrder 排序）
	TArray<UInventorySlotData*> GetDisplaySlotsByCategory(FName CategoryID) const;
	int32 GetCategoryCapacity(const FName CategoryID) const;                                 // 对应分类的已解锁格数

	/***  技能UI数据   `BC@` ***/
	const TArray<FAbilityUIData>& GetPlayerAbilities() const { return AbilityUIData; }
	bool FindAbilityUIData(const FGameplayAbilitySpecHandle& SpecHandle, FAbilityUIData& OutData) const;

protected:
	virtual void BindAbilitySystemComponent(UKitsuneAbilitySystemComponent* ASC) override;
	virtual void BindInventorySystemComponent(UInventoryComponent* ISC) override;
	virtual void BindCombatComponent(UKitsuneCombatComponent* CombatComp) override;

	UFUNCTION()
	void HandleCategoryCapacityChanged(FName CategoryID, int32 CategoryCapacity);
	UFUNCTION()
	void HandleInventoryItemChanged(UInventoryItemInstance* ItemInstance, EInventoryItemChangeType ChangeType);
	void HandleAbilityNumChanged(FGameplayAbilitySpec Spec, EAbilityChangeType Type);
	void HandleCurrentWeaponChanged(AWeaponBase* NewWeapon);
	
	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | Ability")
	TArray<FAbilityUIData> AbilityUIData;

	UPROPERTY(BlueprintReadWrite, Category = "ViewModel | Interact")
	TArray<UInventoryItemInstance*> OverlappedItemInstances;

private:
	void RebuildAbilityUIData(const UKitsuneAbilitySystemComponent* ASC);
	void RebuildInventoryMirror(const UInventoryComponent* ISC);
	void RebuildCombatMirror(const UKitsuneCombatComponent* CombatComp);
	
	UPROPERTY(Transient)
	TMap<FName, int32> CapacityByCategory;
	
	UPROPERTY(Transient)
	TMap<FName, FInventoryCategoryItems> ItemsByCategory;

	bool bIsSensing = true;
};
