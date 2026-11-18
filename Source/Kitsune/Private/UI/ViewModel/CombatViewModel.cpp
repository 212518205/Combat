// [VMREFACTOR] 重写：武器图标 / 准心状态
#include "UI/ViewModel/CombatViewModel.h"

#include "Actor/Weapon/DataAssetStartDataWeapon.h"
#include "Actor/Weapon/WeaponBase.h"
#include "Component/Combat/KitsuneCombatComponent.h"

void UCombatViewModel::Deinitialize()
{
	// [VMREFACTOR] IsValid：待销毁对象的 Get() 仍非空，解绑会踩到已释放的委托
	if (UKitsuneCombatComponent* Comp = CachedComp.Get(); IsValid(Comp))
	{
		Comp->OnCurrentWeaponChanged.RemoveAll(this);
	}

	CachedComp.Reset();

	Super::Deinitialize();
}

void UCombatViewModel::BindCombatComponent(UKitsuneCombatComponent* CombatComp)
{
	if (!CombatComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("[VMREFACTOR] UCombatViewModel: 战斗组件未就绪，等待下一次 Bind"));
		return;
	}

	Deinitialize();

	CombatComp->OnCurrentWeaponChanged.AddUObject(this, &ThisClass::HandleCurrentWeaponChanged);
	CachedComp = CombatComp;

	// [VMREFACTOR] 先补一次当前武器，保证 MarkReady 时已经有值
	HandleCurrentWeaponChanged(CombatComp->GetCurrentCarriedWeapon());

	MarkReady();
}

void UCombatViewModel::HandleCurrentWeaponChanged(AWeaponBase* NewWeapon)
{
	// [VMREFACTOR] 原实现在 NewWeapon 为空时直接 return，会留着上一把武器的图标
	if (!NewWeapon || !NewWeapon->GetWeaponInfo())
	{
		WeaponIcon.Reset();
	}
	else
	{
		WeaponIcon = NewWeapon->GetWeaponInfo()->WeaponIcon;
	}

	OnWeaponChanged.Broadcast();
}

void UCombatViewModel::SetSensingEnabled(const bool bEnable)
{
	if (bIsSensing == bEnable) return;

	bIsSensing = bEnable;
	OnReticleStateChanged.Broadcast();
}
