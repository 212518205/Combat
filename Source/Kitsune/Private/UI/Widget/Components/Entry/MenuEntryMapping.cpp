// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/Entry/MenuEntryMapping.h"


TSubclassOf<UMenuEntryBase> UMenuEntryMapping::FindEntryClassByEntryType(const ETileViewEntryType& InEntryType) const
{
	for (auto [EntryType, TileEntryClass] : TileEntryClassMapping)
	{
		if (EntryType == InEntryType)return TileEntryClass;
	}

	return {};
}
