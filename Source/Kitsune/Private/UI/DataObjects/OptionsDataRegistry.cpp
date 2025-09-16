// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DataObjects/OptionsDataRegistry.h"
#include "InputCoreTypes.h"
#include "EnhancedInputSubsystems.h"
#include "FrontendDebugHelper.h"
#include "GameSettings/GameSettingsSaveHelper.h"
#include "GameSettings/KitsuneGameUserSettings.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "UI/DataObjects/ListDataObjectCollection.h"
#include "UI/DataObjects/ListDataObjectRemap.h"
#include "UI/DataObjects/ListDataObjectString.h"

#define MAKE_SETTINGS_SAVE_HELPER_SHARED_PTR(GetOrSetFunctionName) MakeShared<FGameSettingsSaveHelper>(	\
GET_FUNCTION_NAME_STRING_CHECKED(UKitsuneGameUserSettings, GetOrSetFunctionName))

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayTab();
	InitAudioTab();
	InitVideoTab();
	InitControlTab(InOwningLocalPlayer);
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

	for (const UListDataObjectCollection* FoundCollection = *FoundCollectionPtr; UListDataObjectBase* ChildListData:FoundCollection->GetChildListData())
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

	//Game Difficulty
	{
		UListDataObjectString* GameDifficulty = NewObject<UListDataObjectString>();
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("游戏难度")));
		GameDifficulty->SetDescriptionRichText(FText::FromString(TEXT("调整游戏难度：<Bold>简单：</>侧重剧情体验，轻松战斗｜<Bold>一般：</>适度挑战战斗｜<Bold>困难：</>高强度战斗考验｜<Bold>地狱：</>极限挑战，不推荐新手尝试")));
		GameDifficulty->AddDynamicOption(TEXT("容易"));
		GameDifficulty->AddDynamicOption(TEXT("一般"));
		GameDifficulty->AddDynamicOption(TEXT("困难"));
		GameDifficulty->AddDynamicOption(TEXT("地狱"));
		GameDifficulty->SetDynamicSettingsGetterHelper(MAKE_SETTINGS_SAVE_HELPER_SHARED_PTR(GetCurrentGameDifficulty));
		GameDifficulty->SetDynamicSettingsSetterHelper(MAKE_SETTINGS_SAVE_HELPER_SHARED_PTR(SetCurrentGameDifficulty));
		GameDifficulty->SetCurrentDisplayOption(GameDifficulty->GetGameSettingsOption());
		GameplayTabCollection->AddChildListData(GameDifficulty);
	}

	// 自动瞄准
	{
		UListDataObjectString* AutoAim = NewObject<UListDataObjectString>();
		AutoAim->SetDataID(FName("AutoAim"));
		AutoAim->SetDataDisplayName(FText::FromString(TEXT("自动瞄准")));
		AutoAim->SetDescriptionRichText(FText::FromString(TEXT("控制战斗中的自动瞄准功能：<Bold>关闭：</>完全手动瞄准｜<Bold>弱：</>轻微辅助瞄准｜<Bold>中：</>中等强度辅助｜<Bold>强：</>自动锁定目标")));
		AutoAim->AddDynamicOption(TEXT("关闭"));
		AutoAim->AddDynamicOption(TEXT("下"));
		AutoAim->AddDynamicOption(TEXT("中"));
		AutoAim->AddDynamicOption(TEXT("上"));
		AutoAim->SetDynamicSettingsGetterHelper(MAKE_SETTINGS_SAVE_HELPER_SHARED_PTR(GetCurrentAutoAim));
		AutoAim->SetDynamicSettingsSetterHelper(MAKE_SETTINGS_SAVE_HELPER_SHARED_PTR(SetCurrentAutoAim));
		AutoAim->SetCurrentDisplayOption(AutoAim->GetGameSettingsOption());
		GameplayTabCollection->AddChildListData(AutoAim);
	}

	// 自动存档
	{
		UListDataObjectString* AutoSave = NewObject<UListDataObjectString>();
		AutoSave->SetDataID(FName("AutoSave"));
		AutoSave->SetDataDisplayName(FText::FromString(TEXT("自动存档")));
		AutoSave->SetDescriptionRichText(FText::FromString(TEXT("设置自动存档频率：<Bold>关闭：</>不自动存档｜<Bold>低：</>关键节点存档｜<Bold>中：</>定期和关键节点存档｜<Bold>高：</>频繁自动存档")));
		AutoSave->AddDynamicOption(TEXT("关闭"));
		AutoSave->AddDynamicOption(TEXT("低"));
		AutoSave->AddDynamicOption(TEXT("中"));
		AutoSave->AddDynamicOption(TEXT("高"));
		AutoSave->SetDynamicSettingsGetterHelper(MAKE_SETTINGS_SAVE_HELPER_SHARED_PTR(GetCurrentAutoSave));
		AutoSave->SetDynamicSettingsSetterHelper(MAKE_SETTINGS_SAVE_HELPER_SHARED_PTR(SetCurrentAutoSave));
		AutoSave->SetCurrentDisplayOption(AutoSave->GetGameSettingsOption());
		GameplayTabCollection->AddChildListData(AutoSave);
	}

	//无效
	{
		UListDataObjectString* Test2 = NewObject<UListDataObjectString>();
		Test2->SetDataID(FName("Test2"));
		Test2->SetDataDisplayName(FText::FromString(TEXT("无效")));
		Test2->AddDynamicOption(TEXT("测试1"));
		Test2->AddDynamicOption(TEXT("测试2"));
		Test2->AddDynamicOption(TEXT("测试3"));
		Test2->AddDynamicOption(TEXT("测试4"));
		Test2->AddDynamicOption(TEXT("测试5"));
		Test2->SetCurrentDisplayOption(FString());
		GameplayTabCollection->AddChildListData(Test2);
	}
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

void UOptionsDataRegistry::InitControlTab(const ULocalPlayer* InLocalPlayer) 
{
	UListDataObjectCollection* ControlTabCollection = NewObject<UListDataObjectCollection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("控制")));
	RegisteredOptionsTabCollections.Add(ControlTabCollection);

	UListDataObjectCollection* KeyboardMouseCollection = NewObject<UListDataObjectCollection>();
	KeyboardMouseCollection->SetDataID(FName("KeyboardMouseCollection"));
	KeyboardMouseCollection->SetDataDisplayName(FText::FromString(TEXT("键鼠")));

	UListDataObjectCollection* GamepadCollection = NewObject<UListDataObjectCollection>();
	GamepadCollection->SetDataID(FName("GamepadCollection"));
	GamepadCollection->SetDataDisplayName(FText::FromString(TEXT("手柄")));

	{
		const UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = InLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		for (UEnhancedInputUserSettings* InputUserSettings = EnhancedInputSubsystem->GetUserSettings(); const auto
		     & [
			     ProfilesName,Profiles] : InputUserSettings->GetAllAvailableKeyProfiles())
		{
			for (const auto& [RowName,MappingRow] :Profiles->GetPlayerMappingRows())
			{
				for (const FPlayerKeyMapping& Mapping:MappingRow.Mappings)
				{
					UListDataObjectRemap* UserMappableKey = NewObject<UListDataObjectRemap>();
					UserMappableKey->SetDataID(Mapping.GetMappingName());
					UserMappableKey->SetDataDisplayName(Mapping.GetDisplayName());
					UserMappableKey->SetCurrentKey(Mapping.GetCurrentKey());
					UserMappableKey->SetDefaultKey(Mapping.GetDefaultKey());
					UserMappableKey->SetCurrentKeySlot(Mapping.GetSlot());
					UserMappableKey->SetProfiles(Profiles);
					UserMappableKey->SetInputUserSettings(InputUserSettings);
					if (Mapping.GetCurrentKey().IsGamepadKey())
					{
						GamepadCollection->AddChildListData(UserMappableKey);
					}else
					{
						KeyboardMouseCollection->AddChildListData(UserMappableKey);
					}
				}
			}
		}
	}

	ControlTabCollection->AddChildListData(KeyboardMouseCollection);
	ControlTabCollection->AddChildListData(GamepadCollection);
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
