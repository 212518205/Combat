// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetAbilityList.generated.h"

class UASCViewModel;
class UViewModelBase;
class UWidgetAbilityEntry;
class UHorizontalBox;

/**
 * [VMREFACTOR] 技能栏（菱形：上排 3 / 下排 2）
 * [VMREFACTOR] 只消费 UASCViewModel，不创建 VM；槽位由 FAbilityUIData::SlotIndex 决定
 * [VMREFACTOR] TODO 下一步（未做）：把 5 个 Entry 直接在 WBP Designer 里摆好，
 * [VMREFACTOR] TODO 代码侧只用 GetChildAt(SlotIndex)，就能彻底删掉 BuildSlots 与两个 RowCount
 */
UCLASS()
class KITSUNE_API UWidgetAbilityList : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/** [VMREFACTOR] 委托改为无参：全量刷新不需要知道"是哪个句柄变了" */
	UFUNCTION()
	void HandleAbilityChanged();

	/** [VMREFACTOR] 三时序补绑：VM 未就绪时先等，不放弃 */
	void TryBindViewModel();
	void HandleViewModelReady(AActor* Owner, UViewModelBase* VM);

	/** [VMREFACTOR] 建槽位（幂等由"控件树里已经有子节点"表达，不用 bool） */
	void BuildSlots();

	void RefreshAbilitySlots();

	/** [VMREFACTOR] 每次按当前 Pawn 解析，天然带"换 Pawn 失效"；返回 nullptr 表示还没有 VM */
	UASCViewModel* GetASCViewModel() const;

	UWidget* GetEntryByIndex(int32 Index) const;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Up;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Down;

	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TSubclassOf<UWidgetAbilityEntry> AbilityEntryClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 UpRowSlotCount = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DownRowSlotCount = 2;

private:
	/** [VMREFACTOR] "当前绑到哪个 VM"：有效即已绑，指针本身就是状态，不再额外维护 bBound */
	TWeakObjectPtr<UASCViewModel> BoundViewModel;
};
