// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"

#include "FrontendDebugHelper.h"
#include "Engine/AssetManager.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Net/UnrealNetwork.h"


namespace 
{
	UInventoryItemDefinition* TryLoadItemDefByPath(const FSoftObjectPath& Path)
	{
		if (!Path.IsValid())return nullptr;
		if (UObject* Loaded = Path.TryLoad())
		{
			if (UInventoryItemDefinition* ItemDef = Cast<UInventoryItemDefinition>(Loaded))
			{
				return ItemDef;
			}
			if (const UBlueprint* Blueprint = Cast<UBlueprint>(Loaded))
			{
				return Cast<UInventoryItemDefinition>(Blueprint->GeneratedClass->GetDefaultObject());
			}
		}
		return nullptr;
	}	
}


UInventoryItemInstance::UInventoryItemInstance()
{
	ItemID = FGuid::NewGuid();
}

void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemDef);
	DOREPLIFETIME(ThisClass, ItemFeatures);
	DOREPLIFETIME(ThisClass, ItemID);
	DOREPLIFETIME(ThisClass, StackCount);
}

bool UInventoryItemInstance::IsSupportedForNetworking() const
{
	return true;
}

UInventoryItemInstance* UInventoryItemInstance::CreateInstanceCopy(UObject* NewOuter) const
{
	UInventoryItemInstance* Copy = NewObject<UInventoryItemInstance>(NewOuter);
	Copy->ItemDef = ItemDef;
	Copy->ItemFeatures = ItemFeatures;
	Copy->ItemID = ItemID;
	Copy->StackCount = StackCount;
	
	return Copy;
}

FSerializedItem UInventoryItemInstance::ToSerialized() const
{
	FSerializedItem Data;
	Data.ItemDefID = ItemDef ? ItemDef->GetPrimaryAssetId() : FPrimaryAssetId();
	Data.ItemDefPath = FSoftObjectPath(ItemDef);
	Data.StackCount = StackCount;
	Data.ItemFeature = ItemFeatures;
	return Data;
}

UInventoryItemInstance* UInventoryItemInstance::CreateFromSerialized(UObject* NewOuter, const FSerializedItem& Serialized)
{
	UInventoryItemInstance* ItemInstance = NewObject<UInventoryItemInstance>(NewOuter);
	ItemInstance->ItemDef = ResolveItemDef(Serialized);
	if (!ItemInstance->GetItemDef())
	{
		Debug::Print(TEXT("ItemDef解析失败"));
		return nullptr;
	}
	ItemInstance->StackCount = Serialized.StackCount;
	ItemInstance->ItemFeatures = Serialized.ItemFeature;
	return ItemInstance;
}

UInventoryItemDefinition* UInventoryItemInstance::ResolveItemDef(const FSerializedItem& Serialized)
{
	if (UInventoryItemDefinition* Def = TryLoadItemDefByPath(Serialized.ItemDefPath))
	{
		return Def;
	}
	if (Serialized.ItemDefID.IsValid())
	{
		const FSoftObjectPath Path = UAssetManager::Get().GetPrimaryAssetPath(Serialized.ItemDefID);
		return TryLoadItemDefByPath(Path);
	}
	return nullptr;
}

