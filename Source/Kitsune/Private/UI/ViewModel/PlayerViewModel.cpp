// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/ViewModel/PlayerViewModel.h"

#include "AbilitySystem/Abilities/KitsuneGameplayAbility.h"
#include "DebugHelper.h"
#include "Actor/Weapon/DataAssetStartDataWeapon.h"
#include "Actor/Weapon/WeaponBase.h"
#include "Component/Combat/KitsuneCombatComponent.h"
#include "FunctionLibrary/KitsuneInventoryFunctionLibrary.h"
#include "FunctionLibrary/KitsuneWidgetFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/Trait/InventoryItemTrait_Display.h"
#include "Inventory/Trait/InventoryItemTrait_Stack.h"
#include "UI/DataObjects/Inventory/InventorySlotData.h"

namespace
{
	bool IsShowableInAbilityBar(const UKitsuneGameplayAbility* Ability)
	{
		return Ability && Ability->AbilityTypeTag != KitsuneGameplayTags::Ability_Type_Hidden;
	}

	FAbilityUIData MakeAbilityUIData(const FGameplayAbilitySpec& Spec)
	{
		FAbilityUIData Data;
		Data.SpecHandle = Spec.Handle;

		if (const UKitsuneGameplayAbility* Ability = Cast<UKitsuneGameplayAbility>(Spec.Ability))
		{
			Data.DisplayName = Ability->GetDisplayName();
			Data.Icon = Ability->GetAbilityIcon();
			Data.AbilityTriggerKeyIcon = Ability->GetAbilityTriggerKeyIcon();
			Data.CooldownTag = Ability->GetCooldownTag();
			Data.Priority = Ability->GetPriority();
		}

		return Data;
	}
	
	const TMap<FName, FInventoryCategoryConfig>& GetInventoryCategoryInfoMap()
	{
		static const TMap<FName, FInventoryCategoryConfig> Map =
			UKitsuneWidgetFunctionLibrary::GetCategoryNameByModuleTag(
				KitsuneGameplayTags::UI_CategoryDisplay_Inventory_Item).CategoryInfo;
		return Map;
	}
}

bool UPlayerViewModel::NativeInitialize(APawn* InPawn)
{
	const bool bSuperInitSuccess = Super::NativeInitialize(InPawn);

	if (UInventoryComponent* ISC = InPawn ? InPawn->FindComponentByClass<UInventoryComponent>() : nullptr)
	{
		BindInventorySystemComponent(ISC);
	}
	else
	{
		Debug::Print(TEXT("无法获取库存系统组件"));
	}

	return bSuperInitSuccess;
}

void UPlayerViewModel::BindInventorySystemComponent(UInventoryComponent* ISC)
{
	Super::BindInventorySystemComponent(ISC);

	if (!ISC)return;

	ISC->CapacityChanged.AddDynamic(this, &ThisClass::HandleCategoryCapacityChanged);
	ISC->ItemChanged.AddDynamic(this, &ThisClass::HandleInventoryItemChanged);

	RebuildInventoryMirror(ISC);
}

void UPlayerViewModel::BindCombatComponent(UKitsuneCombatComponent* CombatComp)
{
	Super::BindCombatComponent(CombatComp);
	
	if (!CombatComp)return;
	CombatComp->OnCurrentWeaponChanged.AddUObject(this, &ThisClass::HandleCurrentWeaponChanged);
	RebuildCombatMirror(CombatComp);
}

void UPlayerViewModel::BindAbilitySystemComponent(UKitsuneAbilitySystemComponent* ASC)
{
	Super::BindAbilitySystemComponent(ASC);

	if (!ASC)return;

	ASC->OnAbilityNumChanged.AddUObject(this, &ThisClass::HandleAbilityNumChanged);

	RebuildAbilityUIData(ASC);
}

void UPlayerViewModel::HandleAbilityNumChanged(FGameplayAbilitySpec Spec, EAbilityChangeType Type)
{
	switch (Type)
	{
	case EAbilityChangeType::AddAbility:
		AbilityUIData.Add(MakeAbilityUIData(Spec));
		break;

	case EAbilityChangeType::RemoveAbility:
		AbilityUIData.RemoveAll([&Spec](const FAbilityUIData& Entry)
		{
			return Entry.SpecHandle == Spec.Handle;
		});
		break;
	}

	AbilityUIData.Sort([](const FAbilityUIData& A, const FAbilityUIData& B)
	{
		return A.Priority < B.Priority;
	});

	OnGameplayAbilityChanged.Broadcast(Spec.Handle);
}

void UPlayerViewModel::HandleCurrentWeaponChanged(AWeaponBase* NewWeapon)
{
	if (!NewWeapon || !NewWeapon->GetWeaponInfo())return;
	WeaponIcon = NewWeapon->GetWeaponInfo()->WeaponIcon;
	OnWeaponChanged.Broadcast();
}

void UPlayerViewModel::RebuildAbilityUIData(const UKitsuneAbilitySystemComponent* ASC)
{
	AbilityUIData.Reset();

	if (!ASC)return;

	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (IsShowableInAbilityBar(Cast<UKitsuneGameplayAbility>(Spec.Ability)))
		{
			AbilityUIData.Add(MakeAbilityUIData(Spec));
		}
	}
	
	AbilityUIData.Sort([](const FAbilityUIData& A, const FAbilityUIData& B)
	{
		return A.Priority < B.Priority;
	});
}

void UPlayerViewModel::RebuildInventoryMirror(const UInventoryComponent* ISC)
{
	if (!ISC)return;

	ItemsByCategory.Reset();
	for (UInventoryItemInstance* Instance : ISC->GetAllItemInstances())
	{
		if (!Instance || !Instance->GetItemDef())continue;
		if (const UInventoryItemTrait_Display* TraitDisplay = UInventoryFunctionLibrary::FindItemDefinitionTrait<UInventoryItemTrait_Display>(Instance->GetItemDef()))
		{
			ItemsByCategory.FindOrAdd(TraitDisplay->CategoryID).Items.Add(Instance);
		}
	}
	
	CapacityByCategory.Reset();
	for (const auto& [CategoryID, Info] : GetInventoryCategoryInfoMap())
	{
		CapacityByCategory.Add(CategoryID, ISC->GetCapacityByCategoryID(CategoryID));
	}
}

void UPlayerViewModel::RebuildCombatMirror(const UKitsuneCombatComponent* CombatComp)
{
	if (!CombatComp)return;
	if (const AWeaponBase* CurrentWeapon = CombatComp->GetCurrentCarriedWeapon())
	{
		if (CurrentWeapon->GetWeaponInfo())
		{
			WeaponIcon = CurrentWeapon->GetWeaponInfo()->WeaponIcon;
		}
	}
}

void UPlayerViewModel::AddInteractableItemInstance(UInventoryItemInstance* ItemInstance)
{
	if (ItemInstance && !OverlappedItemInstances.Contains(ItemInstance))
	{
		OverlappedItemInstances.Add(ItemInstance);
		OnInteractableItemChange.Broadcast(ItemInstance, EItemInstanceAction::EAddInstance);
	}
}

void UPlayerViewModel::RemoveInteractableItemInstance(UInventoryItemInstance* ItemInstance)
{
	if (ItemInstance && OverlappedItemInstances.Contains(ItemInstance))
	{
		OverlappedItemInstances.Remove(ItemInstance);
		OnInteractableItemChange.Broadcast(ItemInstance, EItemInstanceAction::ERemoveInstance);
	}
}

void UPlayerViewModel::SetIsSensing(bool bInIsSensing)
{
	if (bIsSensing != bInIsSensing)
	{
		bIsSensing = bInIsSensing;
		OnReticleStateChanged.Broadcast();
	}
}

TArray<TPair<FName, FText>> UPlayerViewModel::GetAllInventoryCategories() const
{
	const TMap<FName, FInventoryCategoryConfig>& InfoMap = GetInventoryCategoryInfoMap();

	TArray<TPair<FName, FText>> Result;
	Result.Reserve(InfoMap.Num());
	for (const auto& [CategoryID, Info] : InfoMap)
	{
		Result.Emplace(CategoryID, Info.DisplayName);
	}

	Result.Sort([&InfoMap](const TPair<FName, FText>& A, const TPair<FName, FText>& B)
	{
		const int32 OrderA = InfoMap.Contains(A.Key) ? InfoMap[A.Key].SortOrder : INT_MAX;
		const int32 OrderB = InfoMap.Contains(B.Key) ? InfoMap[B.Key].SortOrder : INT_MAX;
		return OrderA < OrderB;
	});

	return Result;
}

TArray<UInventorySlotData*> UPlayerViewModel::GetDisplaySlotsByCategory(const FName CategoryID) const
{
	const TMap<FName, FInventoryCategoryConfig>& InfoMap = GetInventoryCategoryInfoMap();
	const FInventoryCategoryConfig* Info = InfoMap.Find(CategoryID);
	if (!Info)
	{
		return {};
	}

	int32 Capacity = Info->InitialUnlockedSlots;
	if (const int32* Found = CapacityByCategory.Find(CategoryID))
	{
		Capacity = *Found;
	}

	const int32 LockCount = Info->LockSlotCount;

	TArray<UInventorySlotData*> Slots;
	int32 UsedSlots = 0;

	if (const FInventoryCategoryItems* Mirror = ItemsByCategory.Find(CategoryID))
	{
		for (const TObjectPtr<UInventoryItemInstance>& ItemPtr : Mirror->Items)
		{
			UInventoryItemInstance* Item = ItemPtr.Get();
			if (!Item || !Item->GetItemDef())continue;

			const UInventoryItemTrait_Stack* TraitStack = UInventoryFunctionLibrary::FindItemDefinitionTrait<UInventoryItemTrait_Stack>(Item->GetItemDef());
			const bool bStackable = TraitStack && TraitStack->MaxStackCount > 1;
			const int32 MaxStack = bStackable ? FMath::Max(1, TraitStack->MaxStackCount) : 1;
			int32 Remaining = FMath::Max(1, Item->GetStackCount());

			while (Remaining > 0)
			{
				UInventorySlotData* Data = NewObject<UInventorySlotData>(const_cast<UPlayerViewModel*>(this));
				Data->ItemInstance = Item;
				Data->StackCount = bStackable ? FMath::Min(MaxStack, Remaining) : 0;
				Data->bIsLocked = false;
				Data->UnlockCost = 0;
				Data->CategoryID = CategoryID;
				Slots.Add(Data);
				++UsedSlots;
				Remaining -= MaxStack;
			}
		}
	}

	/***  空格   `BC@` ***/
	const int32 EmptyCount = Capacity - UsedSlots;
	for (int32 i = 0; i < EmptyCount; ++i)
	{
		UInventorySlotData* EmptySlot = NewObject<UInventorySlotData>(const_cast<UPlayerViewModel*>(this));
		EmptySlot->bIsLocked = false;
		EmptySlot->CategoryID = CategoryID;
		Slots.Add(EmptySlot);
	}

	/***  锁格   `BC@` ***/
	for (int32 i = 0; i < LockCount; ++i)
	{
		UInventorySlotData* LockedSlot = NewObject<UInventorySlotData>(const_cast<UPlayerViewModel*>(this));
		LockedSlot->bIsLocked = true;
		LockedSlot->UnlockCost = Info->UnlockCost;
		LockedSlot->CategoryID = CategoryID;
		Slots.Add(LockedSlot);
	}

	return Slots;
}

int32 UPlayerViewModel::GetCategoryCapacity(const FName CategoryID) const
{
	return CapacityByCategory.FindRef(CategoryID);
}

bool UPlayerViewModel::FindAbilityUIData(const FGameplayAbilitySpecHandle& SpecHandle, FAbilityUIData& OutData) const
{
	const FAbilityUIData* Found = AbilityUIData.FindByPredicate([&SpecHandle](const FAbilityUIData& Entry)
	{
		return Entry.SpecHandle == SpecHandle;
	});

	if (!Found)return false;

	OutData = *Found;
	return true;
}

void UPlayerViewModel::HandleCategoryCapacityChanged(FName CategoryID, int32 CategoryCapacity)
{
	CapacityByCategory.Add(CategoryID, CategoryCapacity);
	CategoryCapacityChanged.Broadcast(CategoryID, CategoryCapacity);
}

void UPlayerViewModel::HandleInventoryItemChanged(UInventoryItemInstance* ItemInstance, EInventoryItemChangeType ChangeType)
{
	if (!ItemInstance || !ItemInstance->GetItemDef())return;

	const UInventoryItemTrait_Display* TraitDisplay = UInventoryFunctionLibrary::FindItemDefinitionTrait<UInventoryItemTrait_Display>(ItemInstance->GetItemDef());
	if (!TraitDisplay)return;

	switch (ChangeType)
	{
	case EInventoryItemChangeType::Added:
		{
			if (TArray<TObjectPtr<UInventoryItemInstance>>& Mirror = ItemsByCategory.FindOrAdd(TraitDisplay->CategoryID).Items; !Mirror.Contains(ItemInstance))
			{
				Mirror.Add(ItemInstance);
			}
		}
		break;

	case EInventoryItemChangeType::StackChanged:
		break;

	case EInventoryItemChangeType::Removed:
		{
			if (FInventoryCategoryItems* Mirror = ItemsByCategory.Find(TraitDisplay->CategoryID))
			{
				Mirror->Items.Remove(ItemInstance);
			}
		}
		break;

	default:
		break;
	}
	
	OnInventoryItemChanged.Broadcast(ItemInstance, ChangeType);
}
