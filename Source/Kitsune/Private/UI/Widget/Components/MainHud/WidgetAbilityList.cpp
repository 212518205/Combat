// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Components/MainHud/WidgetAbilityList.h"

// [VMREFACTOR] 不再 include UIManagerSubsystem.h / PlayerViewModel.h / StructTypes.h
#include "Components/HorizontalBox.h"
#include "UI/ViewModel/ASCViewModel.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "UI/ViewModel/ViewModelWorldSubsystem.h"
#include "UI/Widget/Components/MainHud/WidgetAbilityEntry.h"

UASCViewModel* UWidgetAbilityList::GetASCViewModel() const
{
	// [VMREFACTOR] 直接 Find，不再懒创建：创建权在数据源（AKitsuneCharacter::InitAbilityInfo）
	UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this);
	return VMReg ? VMReg->Find<UASCViewModel>(GetOwningPlayerPawn()) : nullptr;
}

void UWidgetAbilityList::NativeConstruct()
{
	Super::NativeConstruct();

	BuildSlots();

	TryBindViewModel();

	// [VMREFACTOR] 兜底：HUD 早于 ASC 就绪时也能补上（客户端要等一个网络往返）
	// [VMREFACTOR] Construct/Destruct 成对，解绑统一走 RemoveAll，所以这里不需要去重判断
	if (!BoundViewModel.IsValid())
	{
		if (UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this))
		{
			VMReg->OnViewModelReady.AddUObject(this, &ThisClass::HandleViewModelReady);
		}
	}
}

void UWidgetAbilityList::BuildSlots()
{
	// [VMREFACTOR] 幂等不靠 bool：槽位数与"控件树里已有的子节点数"的差额就是要补的数量
	// [VMREFACTOR] 原实现每次 NativeConstruct 都全量新建并 AddChild，而 NativeDestruct 不清空，
	// [VMREFACTOR] Widget 被重新挂载时旧 Entry 还在、新 Entry 永远 Collapsed，会在树上不断堆积

	if (!HorizontalBox_Up || !HorizontalBox_Down)
	{
		UE_LOG(LogTemp, Warning, TEXT("[VMREFACTOR] WidgetAbilityList: BindWidget 缺失（HorizontalBox_Up/Down）"));
		return;
	}

	if (!AbilityEntryClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[VMREFACTOR] WidgetAbilityList: AbilityEntryClass 未配置"));
		return;
	}

	const auto EnsureRow = [this](UHorizontalBox* Row, const int32 DesiredCount)
	{
		// 槽位数被改小（Designer 里调整菱形布局）时裁掉多余节点
		while (Row->GetChildrenCount() > DesiredCount)
		{
			Row->RemoveChildAt(Row->GetChildrenCount() - 1);
		}

		while (Row->GetChildrenCount() < DesiredCount)
		{
			UWidgetAbilityEntry* Entry = CreateWidget<UWidgetAbilityEntry>(this, AbilityEntryClass);
			if (!Entry) break;

			Entry->SetVisibility(ESlateVisibility::Collapsed);
			Row->AddChild(Entry);
		}
	};

	EnsureRow(HorizontalBox_Up, UpRowSlotCount);
	EnsureRow(HorizontalBox_Down, DownRowSlotCount);
}

void UWidgetAbilityList::TryBindViewModel()
{
	// [VMREFACTOR] 当前 Pawn 对应的 VM 就是唯一真相：
	// [VMREFACTOR]   同一个 VM（含都为 nullptr）→ 无需任何动作
	// [VMREFACTOR]   VM 变了（换 Pawn）→ 先解绑旧的再绑新的
	UASCViewModel* VM = GetASCViewModel();
	if (BoundViewModel.Get() == VM) return;

	if (UASCViewModel* OldVM = BoundViewModel.Get())
	{
		OldVM->OnAbilityChanged.RemoveDynamic(this, &ThisClass::HandleAbilityChanged);
	}
	BoundViewModel = nullptr;

	// [VMREFACTOR] 新的还没就绪就等 OnViewModelReady 广播，等到了会再进来一次
	if (!VM || !VM->IsViewModelReady()) return;

	VM->OnAbilityChanged.AddUniqueDynamic(this, &ThisClass::HandleAbilityChanged);
	BoundViewModel = VM;

	// [VMREFACTOR] 绑上后立刻刷一次；之后靠 OnAbilityChanged 触发（仍是全量刷新，槽位只有个位数）
	RefreshAbilitySlots();
}

void UWidgetAbilityList::HandleViewModelReady(AActor* Owner, UViewModelBase* VM)
{
	// [VMREFACTOR] 广播是全项目共享的，必须校验 Owner 与 VM 类型
	if (Owner != GetOwningPlayerPawn() || !VM || !VM->IsA<UASCViewModel>()) return;

	TryBindViewModel();

	// [VMREFACTOR] 绑上后就不再需要监听；未绑上则继续等下一次广播
	if (BoundViewModel.IsValid())
	{
		if (UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this))
		{
			VMReg->OnViewModelReady.RemoveAll(this);
		}
	}
}

void UWidgetAbilityList::NativeDestruct()
{
	// [VMREFACTOR] 只解绑委托；槽位节点保留（BuildSlots 会按差额补齐，不需要清空重建）
	if (UASCViewModel* VM = BoundViewModel.Get())
	{
		VM->OnAbilityChanged.RemoveDynamic(this, &ThisClass::HandleAbilityChanged);
	}
	BoundViewModel = nullptr;

	if (UViewModelWorldSubsystem* VMReg = UViewModelWorldSubsystem::Get(this))
	{
		VMReg->OnViewModelReady.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void UWidgetAbilityList::HandleAbilityChanged()
{
	RefreshAbilitySlots();
}

void UWidgetAbilityList::RefreshAbilitySlots()
{
	// [VMREFACTOR] 只读"已绑定的那个 VM"：Delegate 来自它，数据也应来自它
	const UASCViewModel* VM = BoundViewModel.Get();
	if (!VM) return;

	const TArray<FAbilityUIData> Snapshot = VM->GetPlayerAbilities();
	const int32 TotalSlots = UpRowSlotCount + DownRowSlotCount;

	if (Snapshot.Num() > TotalSlots)
	{
		// [VMREFACTOR] 原实现用 FMath::Min 静默截断，多出来的技能完全不显示且无日志
		UE_LOG(LogTemp, Warning,
			TEXT("[VMREFACTOR] WidgetAbilityList: 技能数(%d) 超过槽位数(%d)，超出部分不会显示"),
			Snapshot.Num(), TotalSlots);
	}

	// [VMREFACTOR] 槽位 → 数据 的一次性映射；不再用"数组下标 = 槽位下标"
	TMap<int32, const FAbilityUIData*> DataBySlot;
	for (const FAbilityUIData& Data : Snapshot)
	{
		if (Data.SlotIndex < 0 || Data.SlotIndex >= TotalSlots)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("[VMREFACTOR] WidgetAbilityList: 技能 [%s] 的 SlotIndex(%d) 超出槽位范围 [0,%d)"),
				*Data.DisplayName, Data.SlotIndex, TotalSlots);
			continue;
		}

		DataBySlot.Add(Data.SlotIndex, &Data);
	}

	for (int32 Index = 0; Index < TotalSlots; ++Index)
	{
		UWidgetAbilityEntry* Entry = Cast<UWidgetAbilityEntry>(GetEntryByIndex(Index));
		if (!Entry) continue;

		if (const FAbilityUIData* const* Found = DataBySlot.Find(Index))
		{
			Entry->AbilityEntryInitialize(**Found);
		}
		else
		{
			Entry->ClearAbilityEntry();
		}
	}
}

UWidget* UWidgetAbilityList::GetEntryByIndex(const int32 Index) const
{
	if (!HorizontalBox_Up || !HorizontalBox_Down)return nullptr;

	// [VMREFACTOR] 上排填满再填下排，保持原来的菱形布局顺序（0..UpRowSlotCount-1 在上排）
	if (Index < UpRowSlotCount)
	{
		return Index < HorizontalBox_Up->GetChildrenCount() ? HorizontalBox_Up->GetChildAt(Index) : nullptr;
	}

	const int32 DownIndex = Index - UpRowSlotCount;
	return DownIndex < HorizontalBox_Down->GetChildrenCount() ? HorizontalBox_Down->GetChildAt(DownIndex) : nullptr;
}
