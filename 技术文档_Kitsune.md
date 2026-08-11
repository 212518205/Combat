# Kitsune — UE5 模块化游戏战斗框架技术文档

> **引擎版本:** Unreal Engine 5.6  
> **项目类型:** 第三人称动作 RPG 战斗框架原型  
> **开发语言:** C++ / Blueprint（C++ 约占 85 个头文件 + 85 个实现文件）  
> **网络架构:** 基于 UE Replication 的客户端-服务器架构  
> **开源协议:** MIT  
> **仓库地址:** https://github.com/212518205/Combat.git
> **开发周期:** 2025 年 8 月 — 至今（持续活跃开发中）

---

## 目录

1. [项目概述](#1-项目概述)
2. [技术架构总览](#2-技术架构总览)
3. [核心系统详解](#3-核心系统详解)
   - [3.1 Gameplay Ability System (GAS)](#31-gameplay-ability-system-gas)
   - [3.2 物品与背包系统](#32-物品与背包系统)
   - [3.3 UI 框架与 MVVM](#33-ui-框架与-mvvm)
   - [3.4 战斗系统](#34-战斗系统)
   - [3.5 AI 系统](#35-ai-系统)
   - [3.6 角色系统](#36-角色系统)
   - [3.7 输入系统](#37-输入系统)
   - [3.8 交互系统](#38-交互系统)
   - [3.9 动画系统](#39-动画系统)
   - [3.10 GameplayTag 体系](#310-gameplaytag-体系)
   - [3.11 配置与数据驱动](#311-配置与数据驱动)
4. [项目文件结构](#4-项目文件结构)
5. [技术栈与依赖](#5-技术栈与依赖)
6. [开发日志摘要](#6-开发日志摘要)

---

## 1. 项目概述

Kitsune是一个基于 Unreal Engine 5.6 的 **C++ 模块化战斗框架原型**。本项目的核心目标并非制作内容完整的游戏，而是**工程实践**——旨在构建一个**数据驱动、高度模块化、符合生产级开发规范**的客户端架构范本。

项目实践了 Unreal Engine 5 的核心框架技术栈，包括 Gameplay Ability System (GAS) 技能架构、自研 MVVM 数据驱动 UI 层、行为树 AI 决策系统，以及基于接口的泛化交互系统。所有系统设计均以**"可配置"、"可扩展"、"低耦合"**为首要原则。

### 设计理念

> **数据驱动，模块解耦。** 游戏逻辑与表现分离，系统功能通过数据资产（DataAsset）配置，核心模块间通过接口和事件通信。杜绝硬编码，所有技能参数、角色属性、AI 行为均可在编辑器中实时调整，运行时立即生效。

### 项目亮点

| 亮点 | 说明 |
|------|------|
| **完整的 GAS 框架** | AttributeSet（含 16+ 属性）、自定义 AbilitySystemComponent、ExecutionCalculation 伤害计算，DataAsset 驱动的技能初始化 |
| **Trait 式物品系统** | 基于 UPrimaryDataAsset + Trait 组合模式的可扩展物品定义，支持 Bitmask 物品特征标记和网络复制 |
| **自研 MVVM UI 架构** | 独立实现的 ViewModel 层（非 UE 内置 MVVM 插件），结合 CommonUI 的 ActivatableWidgetStack，GameplayTag 驱动页面导航和缓存复用 |
| **网络复制就绪** | 属性同步、物品系统复制、战斗组件 RPC，完整 Client-Server 架构 |
| **AI 行为树 + EQS** | 自定义 BTTask/BTService，AIPerception 感知系统，Team Attitude 阵营判定，Detour Crowd Avoidance 人群避让 |
| **Enhanced Input + GameplayTag** | InputAction → GameplayTag → ASC → GA 的解耦绑定链路，支持蓝图异步节点的异步 Widget 推送 |
| **数据驱动配置** | DeveloperSettings 管理 UI 映射和分类配置，DataAsset 体系管理角色/武器/输入的初始数据 |

---

## 2. 技术架构总览

```
┌─────────────────────────────────────────────────────────────┐
│                      UI Layer (UMG + CommonUI)               │
│  WidgetPrimaryLayout → WidgetStack × 4 → ActivatableWidget   │
│  UIManagerSubsystem (全局 UI 管理)                            │
│  ViewModel Layer (AttributeViewModel / PlayerViewModel)      │
├─────────────────────────────────────────────────────────────┤
│                    Gameplay Framework                         │
│  GameMode → PlayerController → PlayerState → Character       │
│  Input System (EnhancedInput + GameplayTag)                  │
├──────────────┬──────────────────────┬───────────────────────┤
│  GAS Layer   │  Inventory System    │  Combat System        │
│  ASC+AtrrSet │  Trait-based Items   │  Weapon Management    │
│  GA + GE     │  Replicated Slots    │  Hit Detection        │
│  ExecCalc    │  Bitmask Features    │  Damage Calculation   │
├──────────────┴──────────────────────┴───────────────────────┤
│                      AI System                               │
│  AIController → BehaviorTree → BTTask/BTService              │
│  Perception System (Sight) + Team Attitude                   │
├─────────────────────────────────────────────────────────────┤
│                    Data & Config Layer                       │
│  DataAsset (StartData) / DeveloperSettings / GameplayTags    │
└─────────────────────────────────────────────────────────────┘
```

### 模块依赖

```
Kitsune (Runtime Module)
  ├── Core, CoreUObject, Engine, InputCore
  ├── GameplayAbilities, GameplayTasks, GameplayTags (GAS 全家桶)
  ├── EnhancedInput, CommonInput (输入与跨平台输入)
  ├── CommonUI, UMG, Slate, SlateCore (UI 框架)
  ├── AIModule (行为树/AI/EQS)
  ├── MotionWarping, AnimGraphRuntime (动画系统)
  ├── PropertyPath (属性路径绑定)
  └── DeveloperSettings (编辑器内项目设置面板)
```

### 第三方插件

| 插件 | 用途 |
|------|------|
| **SwitchLanguage** | 运行时语言切换（商城插件） |
| **FlatNodes** | 蓝图节点扁平化优化（商城插件） |
| **MotionWarping** | 动画 Root Motion 自适应 |
| **ModelingToolsEditorMode** | 编辑器内建模工具 |

---

## 3. 核心系统详解

### 3.1 Gameplay Ability System (GAS)

项目对 UE5 GAS 进行了完整封装和扩展，是战斗和角色属性的核心支撑。

#### 3.1.1 Ability System Component

```cpp
// KitsuneAbilitySystemComponent.h
class UKitsuneAbilitySystemComponent : public UAbilitySystemComponent
{
    // 通过 GameplayTag 输入触发对应的 GameplayAbility
    void OnAbilityInputPressed(const FGameplayTag& InputTag);
    void OnAbilityInputReleased(const FGameplayTag& InputTag);

    // 运行时按 Tag 尝试激活能力
    bool TryActivateAbilityByTag(FGameplayTag ActivateAbilityTag);
};
```

**设计思路：** 将 EnhancedInput 的 InputAction 在 InputConfig 中映射为 GameplayTag，然后由 ASC 根据 Tag 激活对应 GA。这种间接映射方式实现了输入层与能力逻辑层的解耦。

#### 3.1.2 AttributeSet — 属性集

自研 `UKitsuneAttributeSet` 包含 **16+ 项属性**，分为三大类：

| 分类 | 属性 | 说明 |
|------|------|------|
| **基础资源** | Health / MaxHealth | 生命值 |
| | Stamina / MaxStamina | 耐力值 |
| | Vigor / MaxVigor | 活力值 |
| **战斗资源** | AttackPower | 攻击力 |
| | SpiritPower | 灵力（法术强度） |
| **战斗属性** | Toughness | 韧性（抗打断） |
| | Shield | 护盾 |
| | MentalFocus | 精神力集中 |
| | Armor | 护甲 |
| | CriticalChance / CriticalDamage | 暴击率 / 暴击伤害 |
| | SoulCritical / SoulCriticalDamage | 灵魂暴击（特殊暴击体系） |
| | ArmorPenetration | 护甲穿透 |
| | SoulFlare | 灵魂闪耀（特殊机制） |
| | DodgeChance | 闪避率 |
| | DamageTaken | 伤害减免 |

**技术特点：**
- 全部属性支持网络复制（`ReplicatedUsing`）
- 使用 `ATTRIBUTE_ACCESSORS` 宏自动生成 Getter/Setter/Initter（遵循 GAS 规范）
- `PreAttributeChange` 用于属性修约（Clamp/ClampMin 等）
- `PostGameplayEffectExecute` 处理 GE 执行后的逻辑（如死亡判定）
- 自定义 `FEffectProperties` 结构体统一管理 Effect 的 Source/Target 信息

#### 3.1.3 ExecutionCalculation — 伤害计算

自定义 `UExecCalcDamageTaken` 类，实现独立的伤害计算公式，支持护甲减免、暴击判定、灵魂暴击等多层伤害修正。

#### 3.1.4 GameplayAbility

`UKitsuneGameplayAbility` 作为项目 GA 基类，Blueprintable 支持蓝图子类化。已实现的技能包括：

- **武器技能：** 轻攻击（Light）、重攻击（Heavy）
- **装备技能：** 装备/卸载武器
- **移动技能：** 闪避（Dodge）
- **共享技能：** 受击反应（HitReact）、死亡（Dead）

---

### 3.2 物品与背包系统

项目实现了一套基于 **Trait 模式** 的灵活物品系统，是游戏中物品定义、存储、交互的核心。

#### 3.2.1 物品定义 — InventoryItemDefinition

```cpp
UCLASS(Blueprintable, meta = (PrimaryAssetType = "ItemDefinition"))
class UInventoryItemDefinition : public UPrimaryDataAsset
{
    // Trait 列表 — 组合式设计
    TArray<TObjectPtr<UInventoryItemTrait>> ItemTraits;
};
```

**设计理念：** 物品不由单一类定义，而是通过组合多个 Trait 来定义行为。每个 Trait 负责一个独立功能维度。

#### 3.2.2 Trait 体系

| Trait | 功能 |
|-------|------|
| `ItemTrait_Use` | 可使用（如药品回复生命） |
| `ItemTrait_Trade` | 可交易 |
| `ItemTrait_Interact` | 可交互（如机关道具） |
| `ItemTrait_Stack` | 可堆叠 & 堆叠上限 |
| `ItemTrait_Display` | 显示信息（名称、图标、描述） |

遍历 Trait 使用模板函数 + 宏：

```cpp
#define GET_TRAIT(ItemInstance, TraitSuffix) \
    UInventoryFunctionLibrary::FindItemDefinitionTrait<UItemTrait_##TraitSuffix>((ItemInstance)->GetItemDef())
```

#### 3.2.3 物品实例 — InventoryItemInstance

```cpp
class UInventoryItemInstance : public UObject
{
    UInventoryItemDefinition* ItemDef;  // 物品定义引用
    int32 StackCount;                   // 堆叠数（网络复制）
    int32 ItemFeatures;                 // Bitmask 特征标记（网络复制）
    AActor* OwningActor;                // 所有者引用
};
```

**Bitmask 特征系统：** 使用位掩码存储物品运行时状态，支持高性能批量查询：

| 标志位 | 含义 |
|--------|------|
| Locked (1<<0) | 已锁定 |
| Discardable (1<<1) | 可丢弃 |
| Tradable (1<<2) | 可交易 |
| Usable (1<<3) | 可使用 |
| Interactable (1<<4) | 可交互 |
| Equipable (1<<5) | 可装备 |
| Combinable (1<<6) | 可合成 |
| Stackable (1<<7) | 可堆叠 |

#### 3.2.4 背包系统 — InventorySystem

```cpp
class UInventorySystem : public UObject
{
    // 核心操作
    bool AddItem(UInventoryItemInstance* InItem, int32 InStackCount = 1);

    // 按分类获取物品
    TArray<TPair<FName, FInventoryCategoryGroup>> GetAllCategoryItem();
    TArray<UInventorySlotData*> GetAllItemsByCategory(FName CategoryID);

    // 网络复制物品列表
    TArray<UInventoryItemInstance*> InventoryItems;  // Replicated

    // 委托通知 UI 更新
    FOnInventoryItemChanged ItemChanged;
};
```

**Slot 系统：** `UInventorySlotData` 封装单个格子，包含：
- 物品实例引用
- 锁定状态（`bIsLocked`）
- 解锁费用（`UnlockCost`）

**分类系统：** 支持 Equipment / CommonItems / QuestItems / Currency 四类物品分类，在 DeveloperSettings 中配置每类的默认槽位数和锁定额度。

---

### 3.3 UI 框架与 MVVM

#### 3.3.1 全局 UI 管理 — UIManagerSubsystem

`UUIManagerSubsystem` 作为 `UGameInstanceSubsystem`，是 UI 系统的**全局管理器**：

```cpp
class UUIManagerSubsystem : public UGameInstanceSubsystem
{
    // ViewModel 获取（按需创建 + 缓存）
    UPlayerViewModel* GetLocalViewModel(bool& bIsValid);
    UAttributeViewModel* GetViewModelByPawn(APawn* InPawn);

    // 异步推送 Widget 到指定 Stack
    void PushSoftWidgetToStackAsync(FGameplayTag WidgetStackTag,
                                    FGameplayTag WidgetTag,
                                    TFunction<void(EAsyncPushWidgetState, UWidgetActivatableBase*)> Callback);

    // 确认弹窗
    void PushConfirmScreenToModalStackAsync(...);

    // 输入模式切换（静态辅助）
    static void ExistedSetInputMode(APlayerController* PC, ECommonInputMode Mode, ...);
};
```

**设计特点：**
- ViewModel 使用 `TMap<APawn*, UAttributeViewModel*>` 按需缓存，避免重复创建
- Widget 异步加载（`TSoftClassPtr`）+ GameplayTag 索引
- 模板函数 `TryGetViewModelByActor<T>()` 支持类型安全的 ViewModel 获取

#### 3.3.2 ActivatableWidgetStack — Widget 栈管理

基于 `UCommonActivatableWidgetContainerBase` 扩展的自定义 Widget 栈：

```cpp
class UKitsuneActivatableWidgetStack : public UCommonActivatableWidgetContainerBase
{
    void PopWidget();     // 弹出栈顶 Widget
    void ClearStack();    // 清空整个栈

    // 重写：Widget 添加时自动激活
    virtual void OnWidgetAddedToList(UCommonActivatableWidget& AddedWidget) override;
};
```

**四个 Widget 栈：**

| Stack Tag | 用途 |
|-----------|------|
| `UI.WidgetStack.ModalStack` | 模态弹窗栈（确认框等） |
| `UI.WidgetStack.GameMenu` | 游戏内菜单栈 |
| `UI.WidgetStack.HudStack` | HUD 覆盖层栈 |
| `UI.WidgetStack.FrontendStack` | 前端/主菜单栈 |

#### 3.3.3 MVVM ViewModel 层

```cpp
// ViewModel 基类
class UViewModelBase : public UObject { ... };

// 属性 ViewModel — 捕获 GAS 属性变化并广播给 UI
class UAttributeViewModel : public UViewModelBase
{
    // 捕获的属性值
    float Health, MaxHealth;
    float Stamina, MaxStamina;

    // 变化委托
    FOnAttributeChangedDelegate OnHealthPercentChanged;
    FOnAttributeChangedDelegate OnStaminaPercentChanged;
};

// 玩家 ViewModel — 物品/交互相关
class UPlayerViewModel : public UAttributeViewModel { ... };

// 敌人 ViewModel — 敌人血条等
class UEnemyViewModel : public UAttributeViewModel { ... };
```

**数据流：** GAS Attribute 变化 → ViewModel 属性绑定回调 → UI Widget 绑定刷新

#### 3.3.4 组件化 UI 部件

| 组件 | 基类 | 功能 |
|------|------|------|
| `KitsuneCommonListView` | `UCommonListView` | 支持 Entry 映射的自定义列表 |
| `KitsuneCommonTileView` | `UCommonTileView` | 菜单磁贴网格 |
| `KitsuneTabListWidgetBase` | `UCommonTabListWidgetBase` | 标签页切换 |
| `KitsuneCommonButtonBase` | `UCommonButtonBase` | 带描述更新的通用按钮 |
| `KitsuneComboBoxString` | `UComboBoxString` | 选项设置下拉框 |

**Entry 映射系统：** `UListEntryMapping` DataAsset 将不同的 DataObject 类型映射到对应的 Entry Widget 类，实现列表项的动态类型匹配渲染。

#### 3.3.5 异步 Widget 推送（AsyncAction）

项目实现了两个蓝图异步节点，支持在蓝图中优雅地处理 Widget 的异步加载和推送：

```cpp
// 异步推送软引用 Widget 到指定 Stack（支持回调）
UAsyncActionPushSoftWidget* PushSoftWidgetToStackAsync(...);

// 异步推送确认弹窗到 ModalStack（支持按钮回调）
UAsyncActionPushConfirmScreen* PushConfirmScreenToModalStackAsync(...);
```

**解决的问题：** UE 的 Widget 蓝图类通常使用软引用（`TSoftClassPtr`）避免常驻内存。异步节点封装了 `StreamableManager.RequestAsyncLoad` 的复杂性，在蓝图中以线性节点流的方式处理加载完成、Push 完成、用户操作回调等异步流程。

#### 3.3.6 ListDataObject 抽象层

自定义了一套数据对象抽象，将不同类型的设置值（字符串、枚举重映射、集合）统一封装为 `UListDataObjectBase` 子类，使其能够被 `CommonListView` 统一渲染：

| DataObject 类 | 用途 |
|--------------|------|
| `ListDataObjectString` | 字符串选项（如分辨率） |
| `ListDataObjectRemap` | 按键重映射 |
| `ListDataObjectValue` | 数值选项（如音量滑块） |
| `ListDataObjectCollection` | 子选项集合 |

结合 `OptionsDataRegistry` 运行时注册机制，实现了类似 UE 内置设置面板的动态选项生成。

---

### 3.4 战斗系统

#### 3.4.1 战斗组件层次

```
UKitsuneExtensionComponent (自定义 ActorComponent 扩展基类)
  ├── UKitsuneCombatComponent (抽象战斗基类)
  │     ├── UPlayerCombatComponent (玩家战斗逻辑)
  │     └── UEnemyKitsuneCombatComponent (敌人战斗逻辑)
  └── UInteractionComponent (交互组件)
```

**UKitsuneCombatComponent 核心功能：**

- **武器管理：** `CarriedWeapons` 数组（Replicated），支持多武器携带
- **武器切换：** `SetCurrentWeapon(FGameplayTag)` 通过 GameplayTag 切换武器并赋予对应技能
- **碰撞检测：** `ToggleWeaponCollision(bool)` 开关武器碰撞盒用于攻击判定
- **命中回调：** `OnHitTargetActor` / `OnPulledFromTargetActor` 虚拟函数子类重写

#### 3.4.2 武器系统

```cpp
// 武器基类继承自物品基类
class AWeaponBase : public AItemBase
{
    FGameplayTag WeaponTag;        // 武器标识 Tag
    UBoxComponent* ItemOverlapBox; // 攻击碰撞盒

    // 装备/卸载到角色
    void EquipWeaponToCharacter(ACharacterBase* TargetCharacter) const;
    void UnequipWeaponFromCharacter(ACharacterBase* TargetCharacter) const;

    // 赋予武器初始技能到 ASC
    void GiveWeaponInitialAbilityToASC(UAbilitySystemComponent* TargetASC) const;

    // 重叠委托（用于命中检测）
    FOnWeaponOverlapChangedDelegate WeaponBeginOverlap;
    FOnWeaponOverlapChangedDelegate WeaponEndOverlap;
};
```

**武器数据驱动：** `UDataAssetStartDataWeapon` (DataAsset) 定义武器的初始技能、GameplayEffect 等数据，支持不同的武器蓝图子类（如 `AWeaponSword`）。

---

### 3.5 AI 系统

#### AI Controller

```cpp
class AKitsuneAIController : public AAIController
{
    // 团队态度判定（实现 IGenericTeamAgentInterface）
    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

    // 感知更新回调
    void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    // AI 感知组件 + 视觉配置
    UAISenseConfig_Sight* SenseConfig_Sight;
    UAIPerceptionComponent* EnemyPerceptionComponent;

    // Detour Crowd Avoidance（人群避让）
    bool bEnableDetourCrowdAvoidance;
    float CollisionQueryRange;
    int32 DetourCrowdAvoidanceQuality;
};
```

**自定义 BT 节点：**
- `BTTask_RotateToFaceTarget` — 旋转面向目标
- `BTService_OrientToTargetActor` — 持续朝向目标

---

### 3.6 角色系统

#### 类继承层次

```
ACharacter
  └── ACharacterBase (implements IAbilitySystemInterface, IPawnCombatInterface)
        ├── AKitsuneCharacter (玩家角色, implements IPawnInteractInterface)
        └── AEnemyCharacter (敌人角色)
```

#### CharacterBase 核心功能

```cpp
class ACharacterBase : public ACharacter
{
    UAbilitySystemComponent* AbilitySystemComponent;    // ASC
    UAttributeSet* AttributeSet;                        // 属性集
    UMotionWarpingComponent* MotionWarpingComponent;    // 动画位移适配
    UDataAssetStartDataBase* InitialInfoData;           // DataAsset 初始化数据
    int32 CharacterLevel;                               // 角色等级
};
```

**初始化流程：** `InitAbilityInfo()` → 从 DataAsset 读取默认 Attribute / GameplayEffect / GameplayAbility → 赋予 ASC

**KitsuneCharacter（玩家）：** 额外包含 `UPlayerCombatComponent` 和 `UInteractionComponent`

**EnemyCharacter（敌人）：** 额外包含 `UEnemyKitsuneCombatComponent`、`UWidgetComponent`（头顶血条）、`UEnemyViewModel`

---

### 3.7 输入系统

```cpp
class UKitsuneInputComponent : public UEnhancedInputComponent
{
    template<class UserObject, typename CallBackFunc>
    void BindAbilityInputAction(UKitsuneInputConfig* InputConfig,
                                UserObject* ObjectContext,
                                CallBackFunc CallBackPressed,
                                CallBackFunc CallBackReleased);
};
```

**输入流程：**
1. `UKitsuneInputConfig` (DataAsset) 存储 `TMap<UInputAction*, FGameplayTag>` 映射
2. `BindAbilityInputAction` 遍历该映射自动绑定 Pressed/Released 回调
3. 回调中调用 `ASC->OnAbilityInputPressed(InputTag)` 激活对应 GA

**已定义的输入 Tag：**
| InputTag | 绑定动作 |
|----------|---------|
| `InputTag.Weapon.Light` | 武器轻攻击 |
| `InputTag.Weapon.Heavy` | 武器重攻击 |
| `InputTag.Weapon.Unequip` | 卸载武器 |
| `InputTag.Hero.Jump` | 跳跃 |
| `InputTag.Hero.Dodge` | 闪避 |

**PlayerController 输入管理：** `AKitsunePlayerController` 管理两套 Input Mapping Context（IMC）：
- `IMC_GAS_Skills` — 战斗技能输入（轻/重攻击、闪避等）
- `IMC_Gameplay` — 通用操作输入（移动、视角、跳跃、交互）

---

### 3.8 交互系统

```cpp
class UInteractionComponent : public UKitsuneExtensionComponent
{
    // Server RPC：添加/移除可交互物品
    void AddInteractableItem(UInventoryItemInstance* ItemInstance);
    void RemoveInteractableItem(UInventoryItemInstance* ItemInstance);

    // 获取背包系统引用
    UInventorySystem* GetInventorySystem();

    // Client RPC：物品重叠变化通知
    void ItemOverlapChange(UInventoryItemInstance* ItemInstance, EItemInstanceAction Action);
};
```

**交互流程：**
1. 武器/物品进入角色交互范围 → 触发 `AddInteractableItem`（Server RPC）
2. 背包系统接收物品 → UI 更新
3. 物品离开 → `RemoveInteractableItem` → UI 移除

**碰撞通道：** 自定义了 `InteractionTrace` Channel 和 `InteractableItem`/`InteractableCharacter` Profile，实现专门的交互射线检测。

#### EffectActor — 通用效果触发器

`AEffectActor` 是一个通用的 GameplayEffect 应用器，放置在场景中，角色进入重叠区域时自动施加 GE：

- **支持策略：** Instant（瞬间）/ Duration（持续）/ Infinite（无限）三种 GameplayEffect 类型
- **应用/移除策略：** 可配置 OnOverlap / OnEndOverlap 时分别应用或移除效果
- **应用目标：** 支持按 ActorTag 过滤目标
- **典型用例：** 血瓶（Instant GE 回复生命）、毒雾区域（Duration GE 持续扣血）、Buff 光环（Infinite GE）

---

### 3.9 动画系统

#### AnimInstance 层次

```
UAnimInstance
  ├── UKitsuneAnimInstance (动画基类)
  ├── UHeroAnimInstance (玩家动画实例)
  └── ULinkedLayerAnimInstance (Linked Anim Layer 支持)
```

- 使用 **Motion Warping** 插件实现攻击动画的位移自适应
- 通过 **Linked Anim Layer** 实现动画逻辑的模块化分层
- AnimInstance 直接访问 ASC 获取 GameplayTag 状态，驱动动画状态机

---

### 3.10 GameplayTag 体系

项目使用 Native GameplayTags（`UE_DECLARE_GAMEPLAY_TAG_EXTERN`），分层管理所有 Tag：

| 命名空间 | 用途 | 示例 |
|---------|------|------|
| `UI.WidgetStack.*` | Widget 栈标识 | `UI.WidgetStack.ModalStack` |
| `UI.Widget.*` | Widget 界面标识 | `UI.Widget.BackBagScreen` |
| `UI.ListEntryPurpose.*` | 列表项用途分类 | `UI.ListEntryPurpose.Interact` |
| `UI.CategoryDisplay.*` | 分类显示配置 | `UI.CategoryDisplay.Inventory.Item` |
| `InputTag.*` | 输入动作标记 | `InputTag.Weapon.Light` |
| `Player.Ability.*` | 玩家技能 Tag | `Player.Ability.Equip.Sword` |
| `Player.Event.*` | 玩家事件 Tag | `Player.Event.Weapon.Equip.Sword` |
| `Player.Status.*` | 玩家状态 Tag | `Player.Status.Dodging` |
| `Enemy.*` | 敌人相关 Tag | `Enemy.Status.Strafing` |
| `Shared.*` | 玩家敌人公用 Tag | `Shared.Event.MeleeHit` |

---

### 3.11 配置与数据驱动

#### DeveloperSettings

```cpp
// FrontendDeveloperSettings.h — 在 Project Settings 中直接编辑
class UFrontendDeveloperSettings : public UDeveloperSettings
{
    // Widget Tag → Widget 类的软引用映射
    TMap<FGameplayTag, TSoftClassPtr<UWidgetActivatableBase>> FrontendWidgetMap;

    // 分类 Tag → 分类配置（槽位/锁定/费用等）
    TMap<FGameplayTag, FCategoryInfoGroup> CategoryDisplayGroups;
};
```

#### DataAsset 体系

| DataAsset | 用途 |
|-----------|------|
| `DataAssetStartDataBase` | 角色初始属性/技能/GE 配置 |
| `DataAssetStartDataPlayer` | 玩家特化初始数据 |
| `DataAssetStartDataEnemy` | 敌人特化初始数据 |
| `DataAssetStartDataWeapon` | 武器初始技能配置 |
| `KitsuneInputConfig` | 输入映射配置（InputAction → GameplayTag） |
| `UListEntryMapping` | 列表数据对象 → Entry Widget 映射 |

#### 游戏用户设置

`UKitsuneGameUserSettings` 继承自 `UGameUserSettings`，注册为项目的用户设置类，增加运行时配置项：游戏难度（`CurrentGameDifficulty`）、辅助瞄准（`CurrentAutoAim`）、自动保存（`CurrentAutoSave`）。`FGameSettingsSaveHelper` 提供基于函数路径字符串的动态设置读写能力，用于设置 UI 的通用化绑定。

#### Asset Manager

`AssetManager` 已配置 `ItemDefinition` 为 Primary Asset Type，物品定义可通过 `StreamableManager` 异步按需加载，支持大型项目的资产管理优化。

---

## 4. 项目文件结构

```
Kitsune/
├── Kitsune.uproject                  # UE 项目文件 (UE 5.6)
├── Config/
│   ├── DefaultEngine.ini             # 引擎配置（渲染/输入/碰撞等）
│   ├── DefaultGame.ini               # 游戏配置（UI映射/分类/AssetManager）
│   ├── DefaultInput.ini              # 输入映射
│   ├── DefaultGameplayTags.ini       # GameplayTag 注册
│   └── DefaultEditor.ini             # 编辑器设置
├── Source/
│   └── Kitsune/
│       ├── Kitsune.h / Kitsune.cpp   # 模块入口
│       ├── Public/                   # 头文件（~80 个）
│       │   ├── AbilitySyetem/        # GAS（ASC/AttributeSet/GA/ExecCalc）
│       │   ├── Actor/                # Actor 类（武器/物品/可交互物）
│       │   ├── AI/                   # AI（Controller/BTTask/BTService）
│       │   ├── Characters/           # 角色（Base/Player/Enemy + AnimInstance）
│       │   ├── Component/            # 组件（Combat/Interaction/Extension）
│       │   ├── FrontendSettings/     # DeveloperSettings
│       │   ├── FrontendTypes/        # 结构体/枚举定义
│       │   ├── FunctionLibrary/      # 蓝图函数库
│       │   ├── Game/                 # GameMode/PlayerState
│       │   ├── GameSettings/         # 游戏用户设置
│       │   ├── GameplayTag/          # Native GameplayTag 声明
│       │   ├── Input/                # 输入配置/组件
│       │   ├── Interfaces/           # 接口（PawnCombat/PawnInteract）
│       │   ├── Inventory/            # 物品系统（Definition/Instance/Trait/System）
│       │   ├── Player/               # PlayerController
│       │   └── UI/                   # UI（HUD/Widget/ViewModel/DataObject/Stack）
│       ├── Private/                  # 实现文件（与 Public —对应）
│       └── DevLog.md                 # 开发日志
└── Content/                          # 蓝图/资源
    └── BluePrints/
        ├── Actor/Items/Interact/     # 可交互物品蓝图
        ├── Game/                     # GameMode 蓝图
        ├── Maps/                     # 关卡
        ├── Table/                    # DataTable（输入/控制器）
        └── UI/Widget/               # UI Widget 蓝图
```

### 代码统计

| 类别 | 数量 |
|------|------|
| C++ 头文件 (.h) | ~80 |
| C++ 实现文件 (.cpp) | ~80 |
| 总系统模块数 | 10+ |

---

## 5. 技术栈与依赖

| 技术/框架 | 版本/来源 | 用途 |
|-----------|----------|------|
| Unreal Engine | 5.6 | 游戏引擎 |
| Gameplay Ability System | UE 内置 | 技能/属性/效果框架 |
| CommonUI | UE 内置 | 跨平台 UI 框架 |
| Enhanced Input | UE 内置 | 增强输入系统 |
| Motion Warping | UE 内置 | 动画位移自适应 |
| Behavior Tree | UE 内置 | AI 行为决策 |
| SwitchLanguage | 商城插件 | 运行时多语言切换 |
| FlatNodes | 商城插件 | 蓝图节点可视化优化 |

### 渲染特性

- 图形 API：DirectX 12（SM6）+ Vulkan（Linux SM6）
- 全局光照：Lumen（软件追踪）
- 虚拟阴影贴图：启用
- Nanite：启用
- 抗锯齿：TAA Upsampling（MSAA 4x）

---

## 6. 开发历程

项目自 2025 年 8 月启动，历时约 1 年，持续活跃开发至今：

| 时间 | 阶段 | 主要工作 |
|------|------|---------|
| **2025.08** | 项目搭建 | 角色类、GAS 基础框架、生命值 Demo、输入系统 |
| **2025.09** | 核心系统 | 属性初始化流程、ViewModel 基类、CommonUI ListView 架构、按键重映射、武器与数据解耦、DataAsset 驱动的角色启动 |
| **2025.10** | 战斗基础 | 伤害计算公式（ExecCalc）、受击反应、死亡效果、敌人 AI 基础 |
| **2025.11** | UI 打磨 | 敌人/玩家血条 UI、ViewModel 优化 |
| **2025.12** | AI 模块 | 行为树、AIController、BTTask、BTService，补充项目文档 |
| **2026.02** | 交互与输入 | 交互提示系统、输入优化、ViewModel 缓存机制 |
| **2026.03** | 菜单系统 | DataTable 驱动的菜单、TileView 升级为 ScrollBox+WrapBox |
| **2026.04–08** | UI 架构重构 | 菜单/背包重构、自定义 ActivatableWidgetStack（Push/Pop/缓存复用）、界面布局优化、Pop 接口完善 |

**当前状态：** 活跃开发中，最新工作集中在 UI 框架的持续优化和物品交互功能的完善。
