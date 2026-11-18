// [VMREFACTOR] 重写：原来是个空壳（TryInitializedContextData 里还是半截代码），
// [VMREFACTOR] 现在接管 PlayerViewModel 里的武器图标 + 准心/感知状态
#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "CombatViewModel.generated.h"

class AWeaponBase;
class UKitsuneCombatComponent;
class UTexture2D;

/** [VMREFACTOR] 当前武器变化（独立声明，避免和 ASCViewModel 的委托头文件耦合） */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponChangedDelegate);

/** [VMREFACTOR] 准心/感知状态变化 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReticleStateChangedDelegate);

/**
 * [VMREFACTOR] 数据源 = UKitsuneCombatComponent；只给 IsLocallyControlled() 的 Pawn 建
 */
UCLASS()
class KITSUNE_API UCombatViewModel : public UViewModelBase
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;

	/** [VMREFACTOR] 幂等；原来 PlayerViewModel::BindCombatComponent 从来没有调用点，这里首次真正生效 */
	void BindCombatComponent(UKitsuneCombatComponent* CombatComp);

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnWeaponChangedDelegate OnWeaponChanged;

	UPROPERTY(BlueprintAssignable, Category = "ViewModel | Delegate")
	FOnReticleStateChangedDelegate OnReticleStateChanged;

	UPROPERTY(BlueprintReadOnly, Category = "ViewModel | WeaponIcon")
	TSoftObjectPtr<UTexture2D> WeaponIcon;

	/** [VMREFACTOR] 参数名改为 bEnable，语义明确是"开关"而不是"状态查询" */
	void SetSensingEnabled(const bool bEnable);

	bool IsSensing() const { return bIsSensing; }

private:
	void HandleCurrentWeaponChanged(AWeaponBase* NewWeapon);

	/** [VMREFACTOR] 只存 weak 数据源 */
	TWeakObjectPtr<UKitsuneCombatComponent> CachedComp;

	bool bIsSensing = true;
};
