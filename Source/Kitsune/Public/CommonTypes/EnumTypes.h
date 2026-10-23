#pragma once
#include "CoreMinimal.h"
#include "EnumTypes.generated.h"

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	OK,
	YesNo,
	OkCancel,
	Unknown UMETA(Hidden)
};
 
UENUM(BlueprintType)
enum class EConfirmScreenButtonResult : uint8
{
	Confirmed,
	Cancelled,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EOptionsListDataModifyReason : uint8
{
	DirectlyModified,
	DependencyModified,
	ResetToDefault
};

UENUM()
enum class EKitsuneValidType: uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EKitsuneSuccessType: uint8
{
	Failed,
	Successful
};

UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EInventoryItemFeature : uint32
{
	None = 0 UMETA(Hidden),
	Locked = 1 << 0 UMETA(DisplayName = "已锁定"),
	Discardable = 1 << 1 UMETA(DisplayName = "可丢弃"),
	Tradable = 1 << 2 UMETA(DisplayName = "可交易"),
	Usable = 1 << 3 UMETA(DisplayName = "可使用"),
	Interactable = 1 << 4 UMETA(DisplayName = "可交互"),
	Equipable = 1 << 5 UMETA(DisplayName = "可装备"),
	Combinable = 1 << 6 UMETA(DisplayName = "可合成"),
};
ENUM_CLASS_FLAGS(EInventoryItemFeature);

UENUM(BlueprintType)
enum class EListEntryIconStyle : uint8
{
	DefaultMenuIcon,
	LongerMenuIcon,
	WiderMenuIcon,
};

UENUM()
enum class EInventoryItemChangeType
{
	None UMETA(Hidden),
	Added,
	StackChanged,
	Removed,
};

UENUM()
enum class EAbilityChangeType: uint8
{
	AddAbility,
	RemoveAbility,
};

UENUM(BlueprintType)
enum class EKitsuneTeamID: uint8
{
	TeamA = 0 UMETA(DisplayName = "友方"),
	TeamB = 1 UMETA(DisplayName = "敌方"),
	Neutral =2 UMETA(DisplayName = "中立"),
	Invalid = 255 UMETA(Hidden),
};
