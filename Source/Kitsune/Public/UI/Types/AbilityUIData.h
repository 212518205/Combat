// [VMREFACTOR] 新增文件：从 CommonTypes/StructTypes.h 迁移出来的 UI 领域类型
// [VMREFACTOR] 目的：UI 侧不再承载 FGameplayAbilitySpecHandle（GAS 内部运行时句柄）
// [VMREFACTOR] 身份改为 SlotIndex（技能栏槽位），由 UKitsuneGameplayAbility::SlotIndex 配置
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilityUIData.generated.h"

class UTexture2D;

/**
 * [VMREFACTOR] 技能栏条目：一份纯粹的展示快照
 * [VMREFACTOR] 刻意不包含 SpecHandle：句柄会随"移除→重新授予"变化，而槽位不会
 */
USTRUCT(BlueprintType)
struct FAbilityUIData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	FString DisplayName;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UTexture2D> AbilityTriggerKeyIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	FGameplayTag CooldownTag;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	int32 Priority = 0;

	/** [VMREFACTOR] 技能栏槽位序号；INDEX_NONE 表示技能没配槽位，由 UASCViewModel 按 Priority 顺序补齐 */
	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	int32 SlotIndex = INDEX_NONE;

	/** [VMREFACTOR] 数据是否可用（有槽位即视为可用，替代原来的 SpecHandle.IsValid()） */
	bool IsValid() const { return SlotIndex != INDEX_NONE; }
};
