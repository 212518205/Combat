#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CommonTypes/EnumTypes.h"
#include "GameplayTag/KitsuneGameplayTag.h"
#include "KitsuneGameplayAbility.generated.h"

class UKitsuneCombatComponent;

UENUM()
enum class EKitsuneAbilityActivationPolicy :uint8
{
	OnTriggered,
	OnGiven,
};

/**
 * 
 */
UCLASS()
class KITSUNE_API UKitsuneGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/*** `@BC`   描述: 蓝图纯函数   `BC@` ***/
	UFUNCTION(BlueprintPure, Category = "Kitsune | Ability")
	virtual UKitsuneCombatComponent* GetPawnCombatComponentFromActorInfo() const;
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info", meta = (Categories = "Ability.Type"))
	FGameplayTag AbilityTypeTag = KitsuneGameplayTags::Ability_Type_Hidden;

	/*** `@BC`   描述: 技能栏显示数据，供 ViewModel 组装 FAbilityUIData   `BC@` ***/
	UTexture2D* GetAbilityTriggerKeyIcon() const { return AbilityTriggerKeyIcon; }
	FString GetDisplayName() const { return DisplayName; }
	UTexture2D* GetAbilityIcon() const { return AbilityIcon; }
	FGameplayTag GetCooldownTag() const { return CooldownTag; }
	int GetPriority() const { return Priority; }

	/** [VMREFACTOR] 技能栏槽位：0 = 菱形左上，依次排列；INDEX_NONE 表示由 UASCViewModel 按 Priority 自动补位 */
	int32 GetSlotIndex() const { return SlotIndex; }

protected:
	/*** `@BC`   描述: 追加能力Give是否激活策略   `BC@` ***/	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping")
	bool bMotionWarpingEnable = false;

	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping", meta = (EditCondition = bMotionWarpingEnable, EditConditionHides = bMotionWarpingEnable))
	FName TargetName = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, Category = "MotionWarping", meta = (EditCondition = bMotionWarpingEnable, EditConditionHides = bMotionWarpingEnable))
	float MaxWarpingDist = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kitsune | Policy")
	EKitsuneAbilityActivationPolicy ActivationPolicy = EKitsuneAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "Kitsune | Player")
	FGameplayEffectSpecHandle MakeDamageEffectSpecHandle(const TSubclassOf<UGameplayEffect> EffectClass,
		FGameplayTag AttackType,
		float AttackPowerMultiplier) const;

	static FActiveGameplayEffectHandle NativeApplyGameplayEffectSpecToTarget(const FGameplayEffectSpecHandle& SpecHandle,AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Kitsune | Ability",
		meta = (DisplayName = "ApplyGameplayEffectSpecToTarget", ExpandEnumAsExecs = ApplySuccessType))
	static FActiveGameplayEffectHandle BP_ApplyGameplayEffectSpecToTarget(const FGameplayEffectSpecHandle& SpecHandle,
		AActor* TargetActor, EKitsuneSuccessType& ApplySuccessType);
	
	UFUNCTION(BlueprintCallable)
	void UpdateWarpingTarget() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info")
	TObjectPtr<UTexture2D> AbilityTriggerKeyIcon;
	
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info")
	TObjectPtr<UTexture2D> AbilityIcon;
	
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info")
	FString DisplayName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info")
	int Priority = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info", meta = (Categories = "Player.AbilityCooldown"))
	FGameplayTag CooldownTag = FGameplayTag::EmptyTag;

	/** [VMREFACTOR] 技能栏槽位（-1 = 未配置，自动按 Priority 补位）；配了槽位才会显示在其他玩家的技能栏上 */
	UPROPERTY(EditDefaultsOnly, Category = "Kitsune | Info")
	int32 SlotIndex = INDEX_NONE;
	
};
