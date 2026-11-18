// [VMREFACTOR] 新增：取 ViewModel 的唯一入口
#include "FunctionLibrary/KitsuneViewModelFunctionLibrary.h"

#include "UI/ViewModel/ViewModelBase.h"
#include "UI/ViewModel/ViewModelWorldSubsystem.h"

UViewModelBase* UKitsuneViewModelFunctionLibrary::FindViewModel(AActor* Owner,
                                                               const TSubclassOf<UViewModelBase> ViewModelClass,
                                                               EViewModelAccessResult& ExecResult)
{
	ExecResult = EViewModelAccessResult::Invalid;

	if (!IsValid(Owner) || !ViewModelClass) return nullptr;

	UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(Owner);

	// [VMREFACTOR] 蓝图传的是 UClass，没法实例化模板，所以走 FindByClass
	UViewModelBase* ViewModel = VMReg ? VMReg->FindByClass(Owner, ViewModelClass) : nullptr;

	// [VMREFACTOR] 只有"存在 且 Bind 完成"才放行 Valid：未就绪时读到的全是默认值，等同于没拿到
	if (!ViewModel || !ViewModel->IsViewModelReady()) return nullptr;

	ExecResult = EViewModelAccessResult::Valid;
	return ViewModel;
}
