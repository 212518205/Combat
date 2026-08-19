
#include "Game/SaveGame/KitsuneSaveGame.h"

#include "Kismet/GameplayStatics.h"


namespace KitsuneSave
{
	static int32 UserIndex = 0;
}

UKitsuneSaveGame* UKitsuneSaveGame::LoadOrCreate(const FString& SlotName)
{	
	if (UKitsuneSaveGame* Existing = Cast<UKitsuneSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, KitsuneSave::UserIndex)))
	{
		return Existing;
	}
	return Cast<UKitsuneSaveGame>(UGameplayStatics::CreateSaveGameObject(UKitsuneSaveGame::StaticClass()));
}

void UKitsuneSaveGame::SaveToSlot(const FString& SlotName)
{
	UGameplayStatics::SaveGameToSlot(this, SlotName, KitsuneSave::UserIndex);
}

const FString& UKitsuneSaveGame::GetGlobalSlotName()
{
	static const FString GlobalSlotName = TEXT("KitsuneSave_Global");
	return GlobalSlotName;
}

FString UKitsuneSaveGame::MakePlayerSlotName(const int64 PlayerUID)
{
	return FString::Printf(TEXT("KitsuneSave_%lld"), PlayerUID);
}

UGlobalSaveGame* UGlobalSaveGame::LoadOrCreate()
{
	if (UGlobalSaveGame* Existing = Cast<UGlobalSaveGame>(UGameplayStatics::LoadGameFromSlot(UKitsuneSaveGame::GetGlobalSlotName(), KitsuneSave::UserIndex)))
	{
		return Existing;
	}
	
	return Cast<UGlobalSaveGame>(UGameplayStatics::CreateSaveGameObject(UGlobalSaveGame::StaticClass()));
}

void UGlobalSaveGame::SaveToSlot()
{
	UGameplayStatics::SaveGameToSlot(this, UKitsuneSaveGame::GetGlobalSlotName(), KitsuneSave::UserIndex);
}