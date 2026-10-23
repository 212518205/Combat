#pragma once
#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "CommonTypes/EnumTypes.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "StructTypes.generated.h"



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
	bool bShowAsNew = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Tile")
	FText HotKeyText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu Tile")
	int32 RequiredLevel = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu Tile")
	EListEntryIconStyle EntryType = EListEntryIconStyle::DefaultMenuIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu Tile", meta = (Categories = "UI.Widget"))
	FGameplayTag ScreenToOpen;

};

USTRUCT(BlueprintType)
struct FInventoryCategoryConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FText DisplayName;

	/***  初始已解锁的格数（玩家开局可用的格数，剩余显示为锁格）   `BC@` ***/
	UPROPERTY(EditDefaultsOnly)
	int32 InitialUnlockedSlots = 10;

	/***  单次解锁格子数量   `BC@` ***/
	UPROPERTY(EditDefaultsOnly)
	int32 SlotsPerUnlock = 5;

	/***  解锁花费   `BC@` ***/
	UPROPERTY(EditDefaultsOnly)
	int32 UnlockCost = 500;

	// @BC: 固定显示的锁格数量（视觉提示）。与容量无关——永远显示 N 个
	UPROPERTY(EditDefaultsOnly)
	int32 LockSlotCount = 10;

	UPROPERTY(EditDefaultsOnly)
	int32 SortOrder = 0;

};

USTRUCT(BlueprintType)
struct FCategoryInfoGroup
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FInventoryCategoryConfig> CategoryInfo;

};

USTRUCT(BlueprintType)
struct FCategoryCapacityEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CategoryID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 UnlockedSlots = 0;

	FCategoryCapacityEntry() = default;
	FCategoryCapacityEntry(const FName InCategoryID, const int32 InUnlockedSlots)
		: CategoryID(InCategoryID), UnlockedSlots(InUnlockedSlots) {}

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
	int32 ItemFeature = static_cast<int32>(EInventoryItemFeature::None);

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
	TObjectPtr<UTexture2D> AbilityTriggerKeyIcon;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(BlueprintReadOnly)
	int Priority = -1;
	
	bool IsValid() const
	{
		return Icon && Priority >= 0;
	}
};

USTRUCT(BlueprintType)
struct FFactionRelation
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<uint8, TEnumAsByte<ETeamAttitude::Type>> Attitudes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ETeamAttitude::Type> DefaultAttitude = ETeamAttitude::Neutral;
	
};

USTRUCT(BlueprintType)
struct FFactionPreset
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<uint8, FFactionRelation> Table;
};

USTRUCT(BlueprintType)
struct FKitsuneInitialWidgetConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "UI.WidgetStack"))
	FGameplayTag StackTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "UI.Widget"))
	FGameplayTag WidgetTag;

	bool IsValid() const { return StackTag.IsValid() && WidgetTag.IsValid(); }
};
