
#include "Game/SaveGame/KitsuneSaveGame.h"

#include "Kismet/GameplayStatics.h"


namespace KitsuneSave
{
	static FString SlotName = TEXT("KitsuneSave");
	static int32 UserIndex = 0;
}

UKitsuneSaveGame* UKitsuneSaveGame::LoadOrCreate()
{
	if (UKitsuneSaveGame* Existing = Cast<UKitsuneSaveGame>(UGameplayStatics::LoadGameFromSlot(KitsuneSave::SlotName, KitsuneSave::UserIndex)))
	{
		return Existing;
	}
	
	return Cast<UKitsuneSaveGame>(UGameplayStatics::CreateSaveGameObject(UKitsuneSaveGame::StaticClass()));
}

void UKitsuneSaveGame::SaveToSlot()
{
	UGameplayStatics::SaveGameToSlot(this, KitsuneSave::SlotName, KitsuneSave::UserIndex);
}
