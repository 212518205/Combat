// [VMREFACTOR] 三时序补绑实现
#include "UI/Widget/Components/CharacterWidget/CharacterWidgetBase.h"

#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/ASCViewModel.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "UI/ViewModel/ViewModelWorldSubsystem.h"

void UCharacterWidgetBase::OnBindAndInitialized(APawn* Pawn)
{
	// [VMREFACTOR] 允许重复调用（WidgetComponent::InitWidget 与 InitAbilityInfo 各会来一次）
	OwningPawn = Pawn;

	if (IsLocalPlayerPawn())
	{
		// 本地玩家不显示头顶血条
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	TryBindViewModel();
}

void UCharacterWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	// [VMREFACTOR] 兜底：VM 晚于 Widget 就绪（客户端要等一个网络往返）也能补上
	// [VMREFACTOR] Construct/Destruct 成对，解绑统一走 RemoveAll，所以这里不需要去重判断
	if (!BoundViewModel.IsValid())
	{
		if (UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this))
		{
			VMReg->OnViewModelReady.AddUObject(this, &ThisClass::HandleViewModelReady);
		}
	}
}

void UCharacterWidgetBase::TryBindViewModel()
{
	// [VMREFACTOR] 用"绑到哪个 VM"本身表达状态：同一个 VM（含都为 nullptr）就无事可做
	UASCViewModel* VM = UKitsuneViewModelFunctionLibrary::GetViewModel<UASCViewModel>(OwningPawn.Get());

	// [VMREFACTOR] 关键差异：没拿到 / 没 Ready 时"不放弃"，等 OnViewModelReady 广播
	if (!VM || !VM->IsViewModelReady() || BoundViewModel.Get() == VM) return;

	// [VMREFACTOR] 换了 VM（重生）：先把旧 VM 上的绑定（含蓝图绑的）清掉
	if (UASCViewModel* OldVM = BoundViewModel.Get())
	{
		OldVM->OnHealthPercentChanged.RemoveAll(this);
	}

	BoundViewModel = VM;

	// [VMREFACTOR] 具体委托绑定仍由蓝图在 BP_OnBindAndInitialized 里完成（保持原有分工），
	// [VMREFACTOR] C++ 只负责"什么时候可以绑"这个时序问题
	BP_OnBindAndInitialized();
}

void UCharacterWidgetBase::HandleViewModelReady(AActor* Owner, UViewModelBase* VM)
{
	// [VMREFACTOR] 广播是全项目共享的，必须校验 Owner 与 VM 类型
	if (Owner == OwningPawn.Get() && VM && VM->IsA<UASCViewModel>())
	{
		TryBindViewModel();
	}
}

void UCharacterWidgetBase::NativeDestruct()
{
	if (UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this))
	{
		VMReg->OnViewModelReady.RemoveAll(this);
	}

	// [VMREFACTOR] 从"已绑的那个"解绑，而不是重新解析一次
	if (UASCViewModel* VM = BoundViewModel.Get())
	{
		VM->OnHealthPercentChanged.RemoveAll(this);
	}
	BoundViewModel = nullptr;

	Super::NativeDestruct();
}

bool UCharacterWidgetBase::IsLocalPlayerPawn() const
{
	const APawn* Pawn = OwningPawn.Get();
	if (!Pawn) return false;

	const APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	return PC && PC->GetPawn() == Pawn;
}

UViewModelBase* UCharacterWidgetBase::GetOwningViewModelByClass(const TSubclassOf<UViewModelBase> Class,
                                                               EViewModelAccessResult& ExecResult) const
{
	// [VMREFACTOR] Owner 是 OwningPawn：世界 Widget 挂在某个 Actor 上，
	// [VMREFACTOR] 若改用 GetOwningPlayerPawn() 会取到本地玩家的 VM（或直接取不到）
	return UKitsuneViewModelFunctionLibrary::FindViewModel(OwningPawn.Get(), Class, ExecResult);
}
