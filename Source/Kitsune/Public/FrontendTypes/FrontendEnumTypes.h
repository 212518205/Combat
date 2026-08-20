#pragma once
#include "CoreMinimal.h"
#include "FrontendEnumTypes.generated.h"

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	Ok,
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
	InValid
};

UENUM()
enum class EKitsuneSuccessType: uint8
{
	Failed,
	Successful
};

UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EItemFeature : uint32
{
	None = 0 UMETA(Hidden),                    
	Locked = 1 << 0 UMETA(DisplayName = "已锁定"),
	Discardable = 1 << 1 UMETA(DisplayName = "可丢弃"),
	Tradable = 1 << 2 UMETA(DisplayName = "可交易"),
	Usable = 1 << 3 UMETA(DisplayName = "可使用"),   // 建议改成 Usable
	Interactable = 1 << 4 UMETA(DisplayName = "可交互"),
	Equipable = 1 << 5 UMETA(DisplayName = "可装备"),
	Combinable = 1 << 6 UMETA(DisplayName = "可合成"),
	Stackable = 1 << 7 UMETA(DisplayName = "可堆叠"),
};
ENUM_CLASS_FLAGS(EItemFeature);

UENUM(BlueprintType)
enum class ETileViewEntryType : uint8
{
	DefaultMenuIcon,
	LongerMenuIcon,
	WiderMenuIcon,
};

UENUM()
enum class EInstanceModifyType
{
	None UMETA(Hidden),
	AddStackCount,
	RemoveItem,
	AddItem,
};

UENUM(BlueprintType)
enum class EItemCategory: uint8
{
	Equipment UMETA(DisplayName = "装备"),
	CommonItems UMETA(DisplayName = "常用道具"),
	QuestItems UMETA(DisplayName = "任务道具"),
};

UENUM()
enum class EAbilityChanged: uint8
{
	AddAbility,
	RemoveAbility,
};
