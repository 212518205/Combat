// [VMREFACTOR] 新增文件：由 AttributeViewModel 改名 + 吸收 PlayerViewModel 的技能/属性部分
// [VMREFACTOR] 一个数据源（ASC）一个 VM：7 个属性 + 等级 + 技能栏数据
#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "UI/Types/AbilityUIData.h"
#include "ASCViewModel.generated.h"

class UKitsuneAbilitySystemComponent;
struct FGameplayAbilitySpec;
struct FOnAttributeChangeData;

/** [VMREFACTOR] 无参数数值变化通知（生命/耐力/架势/受伤） */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttributeChangedDelegate);

/** [VMREFACTOR] 技能栏条目变化：原为 FGameplayAbilitySpecHandle&，UI 不需要知道是哪个句柄 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityChangedDelegate);

/**
 * [VMREFACTOR] ASC 是唯一跨 Actor 的数据源（挂在 PlayerState 上），
 * [VMREFACTOR] 所以注册 key 是 Pawn，Bind 参数是 PlayerState 的 ASC
 */
UCLASS()
class KITSUNE_API UASCViewModel : public UViewModelBase
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;

	/** [VMREFACTOR] 幂等：内部先全解绑再绑；InitAbilityInfo 会被调两次 */
	void BindAbilitySystemComponent(UKitsuneAbilitySystemComponent* ASC);

	/*** 属性变化（全员可见，血条用） ***/
	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnAttributeChangedDelegate OnHealthPercentChanged;

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnAttributeChangedDelegate OnStaminaPercentChanged;

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnAttributeChangedDelegate OnVigorPercentChanged;

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnAttributeChangedDelegate OnTakenDamageChanged;

	/*** 技能栏变化（同源，消费者可能变：自己 / 队友大招） ***/
	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnAbilityChangedDelegate OnAbilityChanged;

	int32 GetLocalPlayerLevel() const { return LocalPlayerLevel; }

	/** [VMREFACTOR] 技能栏快照（已按 SlotIndex 排序，空槽不出现在数组里） */
	const TArray<FAbilityUIData>& GetPlayerAbilities() const { return AbilityUIData; }

	/** [VMREFACTOR] 按槽位取数据，替代原来按 SpecHandle 查 */
	bool FindAbilityBySlot(const int32 SlotIndex, FAbilityUIData& OutData) const;

	/*** ViewModel 需捕获的属性值（public：UMG Property Binding 需要能读到） ***/
	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | Attribute")
	float Health = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | Attribute")
	float MaxHealth = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | Attribute")
	float Stamina = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | Attribute")
	float MaxStamina = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | Attribute")
	float Vigor = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | Attribute")
	float MaxVigor = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | Attribute")
	float TakenDamage = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "ViewModel | Player")
	int32 LocalPlayerLevel = 20;

	/*** 技能栏数据 ***/
	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | Ability")
	TArray<FAbilityUIData> AbilityUIData;

private:
	void HandleHealthChanged(const FOnAttributeChangeData& Data);
	void HandleStaminaChanged(const FOnAttributeChangeData& Data);
	void HandleVigorChanged(const FOnAttributeChangeData& Data);
	void HandleTakenDamageChanged(const FOnAttributeChangeData& Data);

	/** [VMREFACTOR] 全量重建：技能增删只有个位数、动作游戏规模可忽略，换来零状态同步 bug */
	void RebuildAbilityUIData();

	/** [VMREFACTOR] 统一排序 + 给未配置槽位的技能补 SlotIndex */
	void ApplySlotOrdering();

	void BroadcastInitialAttributes();

	TWeakObjectPtr<UKitsuneAbilitySystemComponent> CachedASC;
};
