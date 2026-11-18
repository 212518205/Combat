// [VMREFACTOR] 新增文件：ASC → UI 的唯一数据通道
#include "UI/ViewModel/ASCViewModel.h"

#include "AbilitySystem/KitsuneAbilitySystemComponent.h"
#include "AbilitySystem/KitsuneAttributeSet.h"
#include "AbilitySystem/Abilities/KitsuneGameplayAbility.h"
#include "GameplayTag/KitsuneGameplayTag.h"

namespace
{
	/** [VMREFACTOR] Hidden 类型不进技能栏（与 ASC 广播侧的过滤条件保持一致） */
	bool IsShowableInAbilityBar(const UKitsuneGameplayAbility* Ability)
	{
		return Ability && Ability->AbilityTypeTag != KitsuneGameplayTags::Ability_Type_Hidden;
	}

	/** [VMREFACTOR] 纯展示快照：不含任何 GAS 句柄 */
	FAbilityUIData MakeAbilityUIData(const UKitsuneGameplayAbility* Ability)
	{
		FAbilityUIData Data;

		if (!Ability) return Data;

		Data.DisplayName = Ability->GetDisplayName();
		Data.Icon = Ability->GetAbilityIcon();
		Data.AbilityTriggerKeyIcon = Ability->GetAbilityTriggerKeyIcon();
		Data.CooldownTag = Ability->GetCooldownTag();
		Data.Priority = Ability->GetPriority();
		Data.SlotIndex = Ability->GetSlotIndex();

		return Data;
	}
}

void UASCViewModel::Deinitialize()
{
	// [VMREFACTOR] 7 个属性委托 + 1 个能力委托全部按 this 解绑：可被多次调用
	// [VMREFACTOR] 必须 IsValid：TWeakObjectPtr::Get() 对"已标记待销毁"的对象仍返回非空指针，
	// [VMREFACTOR] 世界销毁时 ASC/PlayerState 往往比本 VM 先失效，直接解绑会踩到已释放的委托
	if (UKitsuneAbilitySystemComponent* ASC = CachedASC.Get(); IsValid(ASC))
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxHealthAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetStaminaAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxStaminaAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetVigorAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetMaxVigorAttribute()).RemoveAll(this);
		ASC->GetGameplayAttributeValueChangeDelegate(UKitsuneAttributeSet::GetDamageTakenAttribute()).RemoveAll(this);

		ASC->OnAbilityNumChanged.RemoveAll(this);
	}

	CachedASC.Reset();

	Super::Deinitialize();
}

void UASCViewModel::BindAbilitySystemComponent(UKitsuneAbilitySystemComponent* ASC)
{
	// [VMREFACTOR] Bind 失败只记日志、保持 NotReady，等下一次 Bind（不要引入 Failed 终态）
	// [VMREFACTOR] IsValid：防止拿到"已待销毁"的 ASC
	if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Warning, TEXT("[VMREFACTOR] UASCViewModel: ASC 未就绪，等待下一次 Bind"));
		return;
	}

	// [VMREFACTOR] 幂等：先全解绑（InitAbilityInfo 会跑两次：PossessedBy + OnRep_PlayerState）
	Deinitialize();

	ASC->GetGameplayAttributeValueChangeDelegate(
		UKitsuneAttributeSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(
		UKitsuneAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(
		UKitsuneAttributeSet::GetStaminaAttribute()).AddUObject(this, &ThisClass::HandleStaminaChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(
		UKitsuneAttributeSet::GetMaxStaminaAttribute()).AddUObject(this, &ThisClass::HandleStaminaChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(
		UKitsuneAttributeSet::GetVigorAttribute()).AddUObject(this, &ThisClass::HandleVigorChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(
		UKitsuneAttributeSet::GetMaxVigorAttribute()).AddUObject(this, &ThisClass::HandleVigorChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(
		UKitsuneAttributeSet::GetDamageTakenAttribute()).AddUObject(this, &ThisClass::HandleTakenDamageChanged);

	// [VMREFACTOR] 技能增删：全量重建，避免维护"句柄 → 条目"的额外同步状态
	// [VMREFACTOR] FGameplayAbilitySpec 只在 .cpp 里出现，头文件签名不引入 GAS 类型
	ASC->OnAbilityNumChanged.AddWeakLambda(this,
		[this](FGameplayAbilitySpec Spec, EAbilityChangeType Type)
		{
			RebuildAbilityUIData();
			OnAbilityChanged.Broadcast();
		});

	CachedASC = ASC;

	// [VMREFACTOR] 本地性判据统一用 IsLocallyControlled()，不用 NetMode 组合
	Health = ASC->GetNumericAttribute(UKitsuneAttributeSet::GetHealthAttribute());
	MaxHealth = ASC->GetNumericAttribute(UKitsuneAttributeSet::GetMaxHealthAttribute());
	Stamina = ASC->GetNumericAttribute(UKitsuneAttributeSet::GetStaminaAttribute());
	MaxStamina = ASC->GetNumericAttribute(UKitsuneAttributeSet::GetMaxStaminaAttribute());
	Vigor = ASC->GetNumericAttribute(UKitsuneAttributeSet::GetVigorAttribute());
	MaxVigor = ASC->GetNumericAttribute(UKitsuneAttributeSet::GetMaxVigorAttribute());

	RebuildAbilityUIData();

	BroadcastInitialAttributes();

	MarkReady();
}

void UASCViewModel::RebuildAbilityUIData()
{
	AbilityUIData.Reset();

	const UKitsuneAbilitySystemComponent* ASC = CachedASC.Get();
	if (!IsValid(ASC)) return;

	// [VMREFACTOR] 本地性在这里现算，不额外存一个 bool 成员（成员会过期，派生值不会）
	const APawn* Pawn = Cast<APawn>(GetOwnerActor());
	const bool bRemote = Pawn && !Pawn->IsLocallyControlled();

	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		const UKitsuneGameplayAbility* Ability = Cast<UKitsuneGameplayAbility>(Spec.Ability);
		if (!IsShowableInAbilityBar(Ability)) continue;

		// [VMREFACTOR] 远端 Pawn 只保留显式配了槽位的技能（设计侧通过槽位表达"值得展示给别人"）
		if (bRemote && Ability->GetSlotIndex() == INDEX_NONE) continue;

		AbilityUIData.Add(MakeAbilityUIData(Ability));
	}

	ApplySlotOrdering();
}

void UASCViewModel::ApplySlotOrdering()
{
	// [VMREFACTOR] 1) 已显式占用的槽位
	TArray<int32> UsedSlots;
	for (const FAbilityUIData& Entry : AbilityUIData)
	{
		if (Entry.SlotIndex != INDEX_NONE)
		{
			UsedSlots.Add(Entry.SlotIndex);
		}
	}

	// [VMREFACTOR] 2) 没配槽位的按 Priority 决定补位顺序
	TArray<int32> PendingIndices;
	for (int32 Index = 0; Index < AbilityUIData.Num(); ++Index)
	{
		if (AbilityUIData[Index].SlotIndex == INDEX_NONE)
		{
			PendingIndices.Add(Index);
		}
	}

	PendingIndices.Sort([this](const int32 A, const int32 B)
	{
		return AbilityUIData[A].Priority < AbilityUIData[B].Priority;
	});

	// [VMREFACTOR] 3) 补最小的空闲槽位
	int32 NextSlot = 0;
	for (const int32 Index : PendingIndices)
	{
		while (UsedSlots.Contains(NextSlot)) ++NextSlot;
		AbilityUIData[Index].SlotIndex = NextSlot;
		UsedSlots.Add(NextSlot);
	}

	// [VMREFACTOR] 4) 快照按槽位排序，蓝图侧直接读也是有序的
	AbilityUIData.Sort([](const FAbilityUIData& A, const FAbilityUIData& B)
	{
		return A.SlotIndex < B.SlotIndex;
	});
}

bool UASCViewModel::FindAbilityBySlot(const int32 SlotIndex, FAbilityUIData& OutData) const
{
	for (const FAbilityUIData& Entry : AbilityUIData)
	{
		if (Entry.SlotIndex == SlotIndex)
		{
			OutData = Entry;
			return true;
		}
	}

	return false;
}

void UASCViewModel::BroadcastInitialAttributes()
{
	// [VMREFACTOR] 统一在 Bind 完成后广播一次初始值，替代 HUD 里手写的 3 行 Broadcast
	OnHealthPercentChanged.Broadcast();
	OnStaminaPercentChanged.Broadcast();
	OnVigorPercentChanged.Broadcast();
	OnTakenDamageChanged.Broadcast();
	OnAbilityChanged.Broadcast();
}

void UASCViewModel::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	// [VMREFACTOR] 同一次变化可能来自 Health 或 MaxHealth，靠 Attribute 区分
	if (Data.Attribute == UKitsuneAttributeSet::GetMaxHealthAttribute())
	{
		MaxHealth = Data.NewValue;
	}
	else
	{
		Health = Data.NewValue;
	}

	OnHealthPercentChanged.Broadcast();
}

void UASCViewModel::HandleStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (Data.Attribute == UKitsuneAttributeSet::GetMaxStaminaAttribute())
	{
		MaxStamina = Data.NewValue;
	}
	else
	{
		Stamina = Data.NewValue;
	}

	OnStaminaPercentChanged.Broadcast();
}

void UASCViewModel::HandleVigorChanged(const FOnAttributeChangeData& Data)
{
	if (Data.Attribute == UKitsuneAttributeSet::GetMaxVigorAttribute())
	{
		MaxVigor = Data.NewValue;
	}
	else
	{
		Vigor = Data.NewValue;
	}

	OnVigorPercentChanged.Broadcast();
}

void UASCViewModel::HandleTakenDamageChanged(const FOnAttributeChangeData& Data)
{
	TakenDamage = Data.NewValue;
	OnTakenDamageChanged.Broadcast();
}
