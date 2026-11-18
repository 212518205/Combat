// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"

#include "DebugHelper.h"
#include "Engine/AssetManager.h"
#include "FunctionLibrary/KitsuneInventoryFunctionLibrary.h"
#include "FunctionLibrary/KitsuneWidgetFunctionLibrary.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemInstance.h"
#include "CommonTypes/StructTypes.h"
#include "Game/KitsunePlayerState.h"
#include "Game/GameInstanceSubsystem/KitsuneSaveSubsystem.h"
#include "Game/SaveGame/KitsuneSaveGame.h"
#include "Inventory/Trait/InventoryItemTrait_Stack.h"
#include "Net/UnrealNetwork.h"
// [VMREFACTOR] 组件自己创建 + 绑定 VM
#include "GameFramework/Pawn.h"
#include "UI/ViewModel/InventoryViewModel.h"
#include "UI/ViewModel/ViewModelWorldSubsystem.h"


void FInventoryArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	if (!Owner)return;
	
	for (const int32 Index : AddedIndices)
	{
		if (UInventoryItemInstance* ItemInstance = Items[Index].ItemInstance)
		{
			Owner->ItemChanged.Broadcast(ItemInstance, EInventoryItemChangeType::Added);
		}
	}
}

void FInventoryArray::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	if (!Owner)return;
	
	for (const int32 Index : ChangedIndices)
	{
		if (UInventoryItemInstance* ItemInstance = Items[Index].ItemInstance)
		{
			Owner->ItemChanged.Broadcast(ItemInstance, EInventoryItemChangeType::StackChanged);
		}
	}
}

void FInventoryArray::PreReplicatedRemove(const TArrayView<int32> RemoveIndices, int32 FinalSize)
{
	if (!Owner)return;
	
	for (const int32 Index : RemoveIndices)
	{
		if (UInventoryItemInstance* ItemInstance = Items[Index].ItemInstance)
		{
			Owner->ItemChanged.Broadcast(ItemInstance, EInventoryItemChangeType::Removed);
		}
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InventoryItems.Owner = this;

	if (GetOwner() && GetOwner()->HasAuthority() && UnlockedByCategory.Num() == 0)
	{
		const FCategoryInfoGroup InfoGroup = UKitsuneWidgetFunctionLibrary::GetCategoryNameByModuleTag(
			KitsuneGameplayTags::UI_CategoryDisplay_Inventory_Item);
		for (const auto& [CategoryID, Config] : InfoGroup.CategoryInfo)
		{
			UnlockedByCategory.Emplace(CategoryID, Config.InitialUnlockedSlots);
		}
	}

	// [VMREFACTOR] 背包只给本地控制的 Pawn 建（背包数据只有自主代理有）
	EnsureInventoryViewModel();
}

void UInventoryComponent::EnsureInventoryViewModel()
{
	const APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn || !Pawn->IsLocallyControlled()) return;

	if (UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this))
	{
		VMReg->GetOrCreate<UInventoryViewModel>(GetOwner())->BindInventoryComponent(this);
	}
}

void UInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (GetOwner()->HasAuthority())
	{
		if (UKitsuneSaveSubsystem* SaveSubsystem = UKitsuneSaveSubsystem::GetSaveSubsystem(GetOwner()))
		{
			SaveSubsystem->UnRegisterForSaving(this);
		}
	}
}

void UInventoryComponent::AddItem_Implementation(UInventoryItemInstance* InItemInstance)
{
	if (!InItemInstance || !InItemInstance->GetItemDef())
	{
		return;
	}

	const UInventoryItemDefinition* InItemDef = InItemInstance->ItemDef;
	const int32 InItemFeature = InItemInstance->ItemFeatures;
	const int32 InStackCount = InItemInstance->StackCount;
	const UInventoryItemTrait_Stack* InStackTrait = UInventoryFunctionLibrary::FindItemDefinitionTrait<UInventoryItemTrait_Stack>(InItemDef);
	const bool bIsStackable = InStackTrait && InStackTrait->MaxStackCount > 1;
	const int32 ToAdd = FMath::Max(InStackCount, 1);

	if (bIsStackable)
	{
		for (FInventoryEntry& Entry : InventoryItems.Items)
		{
			if (UInventoryItemInstance* Item = Entry.ItemInstance;
				Item && Item->GetItemDef() == InItemDef && Item->ItemFeatures == InItemFeature)
			{
				Item->StackCount += ToAdd;
				InventoryItems.MarkItemDirty(Entry);
				if (InItemInstance->GetOwningActor())
				{
					InItemInstance->GetOwningActor()->Destroy();
				}
				if (GetNetMode() == NM_Standalone ||
					(GetNetMode() == NM_ListenServer && GetOwner()))
				{
					ItemChanged.Broadcast(Item, EInventoryItemChangeType::StackChanged);
				}
				return;
			}
		}
	}

	UInventoryItemInstance* NewItem = InItemInstance->CreateInstanceCopy(this);

	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
	{
		AddReplicatedSubObject(NewItem);
	}

	if (InItemInstance->GetOwningActor())
	{
		InItemInstance->GetOwningActor()->Destroy();
	}

	FInventoryEntry& NewEntry = InventoryItems.Items.AddDefaulted_GetRef();
	NewEntry.ItemInstance = NewItem;
	InventoryItems.MarkItemDirty(NewEntry);

	if (GetNetMode() == NM_Standalone ||
	    (GetNetMode() == NM_ListenServer && GetOwner()))
	{
		ItemChanged.Broadcast(NewItem, EInventoryItemChangeType::Added);
	}
	
	if (UKitsuneSaveSubsystem* Subsystem = UKitsuneSaveSubsystem::GetSaveSubsystem(GetOwner()))
	{
		Subsystem->MarkDirty(GetSavePlayerUID());
	}
}


void UInventoryComponent::SaveTo(UKitsuneSaveGame* SaveGame)
{
	if (!SaveGame)return;
	
	auto& [SaveItems, SaveInventoryCapacity] = SaveGame->Inventory;
	SaveItems.Reset();
	for (FInventoryEntry& Entry : InventoryItems.Items)
	{
		const UInventoryItemInstance* Instance = Entry.ItemInstance; 
		if (!Instance || !Instance->GetItemDef())
		{
			Debug::Print(TEXT("SaveTo 序列化失败"));
			continue;
		}
		SaveItems.Add(Instance->ToSerialized());
	}
	SaveInventoryCapacity = UnlockedByCategory;
}

void UInventoryComponent::LoadFrom(const UKitsuneSaveGame* SaveGame)
{
	if (!SaveGame)return;
	InventoryItems.Items.Reset();
	
	auto& [SaveItems, SaveInventoryCapacity] = SaveGame->Inventory;
	UnlockedByCategory = SaveInventoryCapacity;
	const UAssetManager& AM = UAssetManager::Get();
	for (const auto& Serialized : SaveItems)
	{
		UInventoryItemInstance* ItemInstance = UInventoryItemInstance::CreateFromSerialized(this, Serialized);
		if (!ItemInstance)
		{
			Debug::Print(TEXT("UInventoryComponent::LoadFrom加载失败"));
			continue;
		}
		
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
		{
			AddReplicatedSubObject(ItemInstance);
		}
		FInventoryEntry& Entry = InventoryItems.Items.AddDefaulted_GetRef();
		Entry.ItemInstance = ItemInstance;
		InventoryItems.MarkItemDirty(Entry);
	}
	
	if (ENetMode NetMode = GetNetMode();
	NetMode == NM_Standalone || (NetMode == NM_ListenServer && GetOwner()))
	{
	for (FInventoryEntry& Entry : InventoryItems.Items)
	{
		if (Entry.ItemInstance)
		{
			ItemChanged.Broadcast(Entry.ItemInstance, EInventoryItemChangeType::Added);
		}
	}
	for (const FCategoryCapacityEntry& Entry : UnlockedByCategory)
	{
		CapacityChanged.Broadcast(Entry.CategoryID, Entry.UnlockedSlots);
	}
	}
	
}

int64 UInventoryComponent::GetSavePlayerUID()
{
	if (const APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		if (const AKitsunePlayerState* PlayerState = Pawn->GetPlayerState<AKitsunePlayerState>())
		{
			return PlayerState->GetPlayerUID();
		}
	}
	return 0;
}

TArray<UInventoryItemInstance*> UInventoryComponent::GetAllItemInstances() const
{
	TArray<UInventoryItemInstance*> Result;
	for (const FInventoryEntry& Entry : InventoryItems.Items)
	{
		if (UInventoryItemInstance* Item = Entry.ItemInstance)
		{
			Result.Add(Item);
		}
	}
	return Result;
}

void UInventoryComponent::UnlockCategorySlots_Implementation(const FName CategoryID)
{
	const FInventoryCategoryConfig* Info = GetCategoryInfo(CategoryID);
	if (!Info)return;
	if (!TrySpendGold(Info->UnlockCost))return;
	
	const int32 CurrentCapacity = GetCapacityByCategoryID(CategoryID);
	const int32 NewCapacity = CurrentCapacity + Info->SlotsPerUnlock;
	
	if (FCategoryCapacityEntry* Existing = UnlockedByCategory.FindByPredicate(
		[CategoryID](const FCategoryCapacityEntry& E){ return E.CategoryID == CategoryID; }))
	{
		Existing->UnlockedSlots = NewCapacity;
	}
	else
	{
		UnlockedByCategory.Emplace(CategoryID, NewCapacity);
	}
	
	if (GetNetMode() == NM_Standalone ||
		(GetNetMode() == NM_ListenServer && GetOwner()))
	{
		CapacityChanged.Broadcast(CategoryID, NewCapacity);
	}
	
	if (UKitsuneSaveSubsystem* Subsystem = UKitsuneSaveSubsystem::GetSaveSubsystem(GetOwner()))
	{
		Subsystem->MarkDirty(GetSavePlayerUID());
	}
}

int32 UInventoryComponent::GetCapacityByCategoryID(const FName CategoryID) const
{
	for (const FCategoryCapacityEntry& Entry : UnlockedByCategory)
	{
		if (Entry.CategoryID == CategoryID)
		{
			return Entry.UnlockedSlots;
		}
	}
	return 0;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventoryComponent, InventoryItems);
	DOREPLIFETIME(UInventoryComponent, UnlockedByCategory);
}

void UInventoryComponent::OnRep_UnlockedByCategory(const TArray<FCategoryCapacityEntry>& OldValue)
{
	for (const FCategoryCapacityEntry& NewEntry : UnlockedByCategory)
	{
		const FCategoryCapacityEntry* OldEntry = OldValue.FindByPredicate(
			[&NewEntry](const FCategoryCapacityEntry& E){ return E.CategoryID == NewEntry.CategoryID; });
		if (!OldEntry || OldEntry->UnlockedSlots != NewEntry.UnlockedSlots)
		{
			CapacityChanged.Broadcast(NewEntry.CategoryID, NewEntry.UnlockedSlots);
		}
	}
}

const FInventoryCategoryConfig* UInventoryComponent::GetCategoryInfo(const FName CategoryID)
{
	static const TMap<FName, FInventoryCategoryConfig> CategoryInfo =
		 UKitsuneWidgetFunctionLibrary::GetCategoryNameByModuleTag(
			 KitsuneGameplayTags::UI_CategoryDisplay_Inventory_Item).CategoryInfo;

	return CategoryInfo.Find(CategoryID);
}

bool UInventoryComponent::TrySpendGold(int32 Cost)
{
	/*** TODO: 先默认返回true以供测试解锁背包格子使用，后续应改为从属性集获取属性来解锁... [2026年8月17日 21:52:42 来自`@BC@`] ***/
	return true;
}

