// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DataObjects/OptionsDataRegistry.h"

#include "UI/DataObjects/ListDataObjectCollection.h"

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayTab();
	InitAudioTab();
	InitVideoTab();
	InitControlTab();
}

TArray<UListDataObjectBase*> UOptionsDataRegistry::GetAllListDataByTabID(const FName& InTabID)
{
	TArray<UListDataObjectBase*> OutListDataset;
	if (!InTabID.IsValid())return OutListDataset;

	const auto FoundCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InTabID](const UListDataObjectCollection* Element)
		{
			return Element->GetDataID() == InTabID;
		}
	);

	UListDataObjectCollection* FoundCollection = *FoundCollectionPtr;

	for (UListDataObjectBase* ChildListData:FoundCollection->GetChildListData())
	{
		if (ChildListData)
		{
			OutListDataset.Add(ChildListData);
		}
		if (ChildListData->HasChildListData())
		{
			FindChildListDataRecursively(ChildListData, OutListDataset);
		}
	}

	return OutListDataset;
}

void UOptionsDataRegistry::InitGameplayTab()
{
	UListDataObjectCollection* GameplayTabCollection = NewObject<UListDataObjectCollection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString(TEXT("游戏")));
	RegisteredOptionsTabCollections.Add(GameplayTabCollection);


}

void UOptionsDataRegistry::InitAudioTab()
{
	UListDataObjectCollection* AudioTabCollection = NewObject<UListDataObjectCollection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("画面")));
	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoTab()
{
	UListDataObjectCollection* VideoTabCollection = NewObject<UListDataObjectCollection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("音频")));
	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlTab() 
{
	UListDataObjectCollection* ControlTabCollection = NewObject<UListDataObjectCollection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("控制")));
	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObjectBase* InParentData,
	TArray<UListDataObjectBase*>& OutFoundChildListData)
{
	if (!InParentData||!InParentData->HasChildListData())return;

	for (UListDataObjectBase* ChildListData:InParentData->GetChildListData())
	{
		if (ChildListData)
		{
			OutFoundChildListData.Add(ChildListData);
		}
		if (ChildListData->HasChildListData())
		{
			FindChildListDataRecursively(InParentData, OutFoundChildListData);
		}
	}
}
