// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySystem.h"

#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Trait/ItemTrait_Display.h"
#include "Inventory/Trait/ItemTrait_Stack.h"
#include "Net/UnrealNetwork.h"

bool UInventorySystem::AddItem(UInventoryItemInstance* InItemInstance, const int32 InStackCount)
{
	if (!InItemInstance)
	{
		DebugPrintInventory();
		return false;
	}

	/***  传入nullptr将ItemInstance置为InItemInstance并Add到数组
	 *		初始化正确的ToStackCount`BC@` ***/
	auto ItemStack = [this, InItemInstance, InStackCount](UInventoryItemInstance* ItemInstance)
		{
			const UItemTrait_Stack* TraitStack = UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Stack>(InItemInstance->GetItemDef());
			if (!TraitStack)
			{
				Debug::Print(TEXT("无法从ItemInstance中获取到有效TraitStack，在UInventorySystem::AddItem中"));
				return false;
			}

			int32 ToStackCount = InStackCount ? InStackCount : InItemInstance->StackCount;
			if (!ItemInstance)
			{
				ItemInstance = InItemInstance;
				InventoryItems.Add(ItemInstance);
				ItemChanged.Broadcast(ItemInstance, EInstanceModifyType::AddItem);
			}

			while (ToStackCount > 0)
			{
				if (ToStackCount < TraitStack->MaxStackCount - ItemInstance->StackCount)
				{
					ItemInstance->StackCount += ToStackCount;
					ItemChanged.Broadcast(ItemInstance, EInstanceModifyType::AddStackCount);
					ToStackCount = 0;
				}else
				{
					ToStackCount -= TraitStack->MaxStackCount - ItemInstance->StackCount;
					ItemInstance->StackCount = TraitStack->MaxStackCount;
					ItemInstance = NewObject<UInventoryItemInstance>(this);
					ItemInstance->ItemDef = InItemInstance->GetItemDef();
					ItemInstance->ItemFeatures = InItemInstance->ItemFeatures;
					ItemInstance->StackCount = 0;
					InventoryItems.Add(ItemInstance);
					ItemChanged.Broadcast(ItemInstance, EInstanceModifyType::AddItem);
				}
			}
			return true;
		};

	for (UInventoryItemInstance*& ItemInstance : InventoryItems)
	{
		if (!ItemInstance)
		{
			Debug::Print(TEXT("在添加物品时物品数组中出现ItemInstance为空"));
			continue;
		}

		if (ItemInstance->GetItemDef() == InItemInstance->GetItemDef() && ItemInstance->HasFeature(EItemFeature::Stackable))
		{
			return ItemStack(ItemInstance);
		}
	}

	if (InItemInstance->HasFeature(EItemFeature::Stackable))
	{
		return ItemStack(nullptr);
	}

	InventoryItems.Add(InItemInstance);
	ItemChanged.Broadcast(InItemInstance, EInstanceModifyType::AddItem);
	if (InStackCount != 1)
	{
		Debug::Print(TEXT("该物品不可堆叠，应传入StackCount为1"));
	}
	return true;
}

void UInventorySystem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventorySystem, InventoryItems);
}

void UInventorySystem::DebugPrintInventory()
{if (!GEngine) return;

	if (InventoryItems.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("背包为空"));
		UE_LOG(LogTemp, Warning, TEXT("Inventory is empty."));
		return;
	}

	FString Header = FString::Printf(TEXT("===== 背包物品 (%d 件) ====="), InventoryItems.Num());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Header);
	UE_LOG(LogTemp, Log, TEXT("%s"), *Header);

	for (int32 i = 0; i < InventoryItems.Num(); ++i)
	{
		const UInventoryItemInstance* Item = InventoryItems[i];
		if (!Item) continue;

		const UInventoryItemDefinition* ItemDef = Item->GetItemDef();
		if (!ItemDef) continue;

		// 使用之前定义的模板函数获取 Display Trait
		const UItemTrait_Display* DisplayTrait = 
			UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_Display>(ItemDef);

		FString DisplayName = DisplayTrait ? DisplayTrait->DisplayName.ToString() : TEXT("未知物品");
		FString Description = DisplayTrait ? DisplayTrait->Description.ToString() : TEXT("无描述");
		int32 Stack = Item->StackCount;

		FString Line = FString::Printf(TEXT("[%d] %s x%d | %s"), i, *DisplayName, Stack, *Description);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, Line);
		UE_LOG(LogTemp, Log, TEXT("%s"), *Line);
	}
}
