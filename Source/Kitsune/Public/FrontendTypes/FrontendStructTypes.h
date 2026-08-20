#pragma once
#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "GameplayTagContainer.h"
#include "FrontendStructTypes.generated.h"



USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
	GENERATED_BODY()

public:
	FConfirmScreenButtonInfo(){}
	FConfirmScreenButtonInfo(const FText& InButtonTextDisplay,const EConfirmScreenButtonResult InButtonResult):
		ButtonTextDisplay(InButtonTextDisplay),ButtonResult(InButtonResult){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonResult ButtonResult = EConfirmScreenButtonResult::Unknown;
};


USTRUCT(BlueprintType)
struct FMenuTileRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Tile")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Tile")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Tile")
	bool bIsShowAsNew = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Tile")
	FText HotKeyText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Tile")
	int32 RequiredLevel = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu Tile")
	ETileViewEntryType EntryType = ETileViewEntryType::DefaultMenuIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu Tile", meta = (Categories = "UI.Widget"))
	FGameplayTag ScreenToOpen;

};

USTRUCT()
struct FInventoryInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	FText CategoryName;

	/***  初始给玩家准备的格子容量，后续可自行花费金币增加容量   `BC@` ***/
	UPROPERTY(EditDefaultsOnly)
	int32 InitialCapacity = 10;

	/***  用于提示用户可解锁格子默认显示的待锁格子数量   `BC@` ***/
	UPROPERTY(EditDefaultsOnly)
	int32 LockSlotCount = 10;

	/***  单次解锁格子数量   `BC@` ***/
	UPROPERTY(EditDefaultsOnly)
	int32 OnceUnlockCount = 5;

	/***  解锁花费   `BC@` ***/
	UPROPERTY(EditDefaultsOnly)
	int32 UnLockCost = 500;

	UPROPERTY(EditDefaultsOnly)
	int32 SortOrder = 0;

};

USTRUCT(BlueprintType)
struct FCategoryInfoGroup
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FInventoryInfo> CategoryInfo;
	
};

USTRUCT()
struct FSerializedItem
{
	GENERATED_BODY()
	
	UPROPERTY()
	FPrimaryAssetId ItemDefID;
	
	UPROPERTY()
	FSoftObjectPath ItemDefPath;
	
	UPROPERTY()
	int32 StackCount = 1;
	
	UPROPERTY()
	int32 ItemFeature = static_cast<int32>(EItemFeature::None);
	
};

USTRUCT(BlueprintType)
struct FCategoryCapacityEntry
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName CategoryID;
	
	UPROPERTY()
	int32 CategoryCapacity = 0;
	
};

/***  PlayerViewModel: 技能信息 用于UI获取显示  `BC@` ***/
USTRUCT(BlueprintType)
struct FAbilityUIData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAbilitySpecHandle SpecHandle;
	
	UPROPERTY(BlueprintReadOnly)
	FString DisplayName;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(BlueprintReadOnly)
	FKey TriggerKey;
	
	UPROPERTY(BlueprintReadOnly)
	int Priority = 0;
};
























