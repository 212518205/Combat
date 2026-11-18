#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
// [VMREFACTOR] 需要 EViewModelAccessResult；取 VM 的实现复用函数库
#include "FunctionLibrary/KitsuneViewModelFunctionLibrary.h"
#include "CharacterWidgetBase.generated.h"

class UASCViewModel;
class UViewModelBase;

/**
 * 
 */
UCLASS()
class KITSUNE_API UCharacterWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnBindAndInitialized(APawn* Pawn);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = OnBindAndInitialized))
	void BP_OnBindAndInitialized();

	UFUNCTION(BlueprintPure)
	bool IsLocalPlayerPawn() const;

protected:
	UFUNCTION(BlueprintCallable, Category = "ViewModel",
		meta = (DeterminesOutputType = "Class", ExpandEnumAsExecs = "ExecResult"))
	UViewModelBase* GetOwningViewModelByClass(TSubclassOf<UViewModelBase> Class,
	                                          EViewModelAccessResult& ExecResult) const;

	void TryBindViewModel();

	void HandleViewModelReady(AActor* Owner, UViewModelBase* VM);

	/** [VMREFACTOR] Construct/Destruct 成对：Ready 广播的订阅与解绑放在这里，就不用去重判断 */
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/** [VMREFACTOR] weak：Actor 销毁后自动失效，不会悬垂也不会反向保活 */
	UPROPERTY(BlueprintReadOnly, Category = "CharacterWidget")
	TWeakObjectPtr<APawn> OwningPawn;

	/** [VMREFACTOR] "当前绑到哪个 VM"：非空即已绑，指针本身就是状态，不用 bBound */
	UPROPERTY(BlueprintReadOnly, Category = "CharacterWidget")
	TWeakObjectPtr<UASCViewModel> BoundViewModel;
};
