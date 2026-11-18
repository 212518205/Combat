// [VMREFACTOR] 新增：取 ViewModel 的唯一入口（蓝图一个、C++ 一个）
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/ViewModel/ViewModelBase.h"
// [VMREFACTOR] 下面的 C++ 模板体需要 Find<T> 的完整定义
#include "UI/ViewModel/ViewModelWorldSubsystem.h"
#include "KitsuneViewModelFunctionLibrary.generated.h"

/**
 * [VMREFACTOR] 取 VM 的结果，直接展平成两个执行引脚（Valid / Invalid）
 * [VMREFACTOR] 语义：Valid = 拿到 VM 且 Bind 已完成（IsViewModelReady()）
 */
UENUM(BlueprintType)
enum class EViewModelAccessResult : uint8
{
	/** 拿到 VM 且已就绪，可以安全读数据/绑委托 */
	Valid UMETA(DisplayName = "Valid"),
	/** 还没有 VM，或 VM 存在但尚未 Bind 完成 */
	Invalid UMETA(DisplayName = "Invalid"),
};

/**
 * [VMREFACTOR] 只做"查询"，永远不创建 VM
 * [VMREFACTOR] 创建权在数据源（AKitsuneCharacter / AEnemyCharacter / 各组件），
 * [VMREFACTOR] 否则 BP 里随手取一次就会在错误时机造出空壳 VM
 */
UCLASS()
class KITSUNE_API UKitsuneViewModelFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * [VMREFACTOR] 蓝图唯一入口：Subsystem + FindByClass + IsValid 合并成一个节点
	 * [VMREFACTOR] DeterminesOutputType：选了 UASCViewModel，输出引脚就是 UASCViewModel*，不用 Cast
	 * [VMREFACTOR] Owner 传 Self / Get Owning Player Pawn / 目标 Actor 都可以；新增 VM 类型不需要加新函数
	 */
	UFUNCTION(BlueprintCallable, Category = "Kitsune | ViewModel",
		meta = (DeterminesOutputType = "ViewModelClass", ExpandEnumAsExecs = "ExecResult"))
	static UViewModelBase* FindViewModel(AActor* Owner, TSubclassOf<UViewModelBase> ViewModelClass,
	                                     EViewModelAccessResult& ExecResult);

	/** [VMREFACTOR] C++ 唯一入口：替代"拉 Subsystem + Find"两行；只查不建 */
	template <class T>
	static T* GetViewModel(AActor* Owner)
	{
		if (!IsValid(Owner)) return nullptr;

		UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(Owner);
		return VMReg ? VMReg->Find<T>(Owner) : nullptr;
	}
};
