# Kitsune - UE5 模块化战斗框架原型

> 基于 **Unreal Engine 5.6** 的 C++ 战斗框架原型。
> 本项目的核心目标并非制作一个内容完整的游戏，而是**工程实践**：构建一套数据驱动、高度模块化、符合生产级开发规范的客户端架构范本。

## 项目概述

项目聚焦 UE5 的几套核心框架：`Gameplay Ability System (GAS)`、自实现的 `MVVM` 数据驱动 UI、基于接口的交互系统、组件化的背包与存档，以及行为树驱动的 AI。

所有系统的设计都以 **可配置、可扩展、低耦合** 为首要原则：能力形态由继承层级表达、内容分类由 `GameplayTag` / `DataAsset` 表达、UI 与逻辑之间只通过 ViewModel 的委托通信。

> 当前状态：GAS 能力父类分层已完成（`Charged` 完整实现，`Channeled` / `Toggle` / `Passive` 为骨架）；背包系统已完成组件化重构；UI 层已切换为 ViewModel 注入式绑定。

---

## ✨ 核心模块

### 1. 分层的能力框架（GAS）

`UKitsuneGameplayAbility` 是**唯一基类**，承载所有能力共用的一切：UI 数据（图标 / 键位图标 / 显示名 / 优先级 / 冷却 Tag）、`AbilityTypeTag`、玩家/战斗组件便捷函数、伤害 `GameplayEffectSpec` 构造、MotionWarping。

在此之上按**终止语义**分支出 6 个语义父类（不再用 `IsA` 判定 UI，改用 `AbilityTypeTag`）：

| 父类 | 触发源 | 花费/冷却结算 | 谁负责 End | 蓝图钩子 |
|---|---|---|---|---|
| `UKitsuneInstantAbility` | 输入 / 代码 | 激活时 | **框架写死**（契约=无异步） | `K2_OnInstantExecute` |
| `UKitsuneInputAbility` | 输入 | 派生自己调 | **派生**（蒙太奇 Task 三回调） | `K2_OnPressed` / `K2_OnReleased` |
| `UKitsuneChargedAbility` | 输入 | **松手且达标才结算** | 框架 | `K2_OnChargeBegin` / `K2_OnCharged` / `K2_OnChargeCancelled` |
| `UKitsuneChanneledAbility` | 输入 | 按下即结算 | 框架（松开时） | *待实现* |
| `UKitsuneToggleAbility` | 输入 | 第一次按下结算 | 框架（再按一次） | *待实现* |
| `UKitsuneEventAbility` | `AbilityTriggers` / `SendGameplayEvent` | 激活时 | **派生** | 引擎的 `Event ActivateAbilityFromEvent` |
| `UKitsunePassiveAbility` | 授予（`OnGiven`） | 无 | 永不（常驻） | *待实现* |

**钩子机制统一为「方式二」**：C++ 侧是普通 `virtual On*`（蓝图里看不到），蓝图侧是 `K2_On*`（`BlueprintImplementableEvent`，可选实现、永远会被调用）。因此蓝图**不需要任何 `Call Parent`**，也不用担心「重写了却忘了调父级」。

选型规则、行为契约与决策树见本地设计文档 `Kitsune/GAS能力框架重构/00_总览与执行顺序.md`（该目录仅本地保留，未纳入仓库）。

### 2. AbilitySystemComponent 与输入路由

`UKitsuneAbilitySystemComponent` 把「输入事件」映射到「能力实例」：

```
InputTag 按下 → 未激活 → TryActivateAbility（激活）
              → 已激活 → AbilitySpecInputPressed（转发，支撑蓄力/引导/开关）
InputTag 松开 → 已激活 → AbilitySpecInputReleased（转发，支撑「松手结算」）
```

能力的增删不再由 ASC 反向调用 UI，而是广播 `OnAbilityNumChanged`，由 `PlayerViewModel` 订阅后重建技能栏数据 —— ASC 因此不依赖任何 UI 类型。

### 3. 数据驱动 UI（MVVM）

- `UViewModelBase`：`GetViewModel<T>(Controller, Pawn)` 创建实例，`NativeInitialize(APawn*)` 完成初始化，并通过 `BindAbilitySystemComponent` / `BindCombatComponent` / `BindInventorySystemComponent` 三个虚函数把组件能力注入派生 ViewModel（不再持有 `OwningController` / `OwningPawn`）。
- `UAttributeViewModel`：把 `AttributeSet` 的属性变化回调收敛成 `BlueprintAssignable` 委托（血量 / 体力 / 精力 / 受击）。
- `UPlayerViewModel`：技能的增删、背包物品变化、当前武器切换全部由组件委托驱动，维护「镜像数据」并广播给 UI。
- `UUIManagerSubsystem` 管理 Widget 生命周期；`UListDataObject` 系列把任意数据源抽象为可被 `CommonListView` 绑定的列表项（按键重映射、选项设置、背包格子都走这套）。

### 4. 背包与物品

| 层 | 类 | 职责 |
|---|---|---|
| 数据 | `UInventoryItemDefinition` | 静态定义，用 `Instanced` 的 **Trait** 组合能力：`Display` / `Stack` / `Interact` / `Use` / `Trade` |
| 实例 | `UInventoryItemInstance` | 运行时实例：堆叠数、`EInventoryItemFeature` 位掩码、序列化 |
| 容器 | `UInventoryComponent` | 继承 `UKitsuneExtensionComponent` + `ISavableInterface`，FastArray 复制，服务端 RPC `AddItem` / `UnlockCategorySlots`，分类容量与金币消耗 |
| 展示 | `FInventoryCategoryConfig` | 分类名、初始解锁格数、单次解锁数、解锁花费、固定显示的锁格数、排序 |

背包分类容量走 `TArray<FCategoryCapacityEntry>` 并随存档持久化（存档版本号 `4`），归类 UI 通过 `PlayerViewModel` 暴露的委托增量刷新，不再整表重建。

### 5. 交互与战斗

- **交互**：`IPawnInteractInterface` 定义契约，`UInteractionComponent` 负责扫描与转发，`AInteractable` 作为场景物基类，`ListEntryInteract` 在 UI 侧呈现可交互条目。
- **战斗**：`UKitsuneCombatComponent` 为基类，玩家/敌人各自派生；武器由 `AWeaponBase` + `DataAsset` 配置，装备切换通过 `OnCurrentWeaponChanged` 广播通知 UI，而不是直接写 ViewModel。
- **阵营**：`UDataAssetFactionMatrix` 配置阵营矩阵，由 `UKitsuneFactionSubsystem` 在服务端加载，替代早期「比较 TeamID」的判敌方式。

### 6. AI

`AKitsuneAIController` 搭载行为树；`BTTask_RotateToFaceTarget` 等任务负责执行行为，`BTService_OrientToTargetActor` 等服务提供持续决策信息，任务与服务均可独立复用。

### 7. 存档

`UKitsuneSaveSubsystem` 统一读写：玩家存档按 `PlayerUID` 分槽（`MakePlayerSlotName`），全局数据走 `UGlobalSaveGame`，背包等业务对象只要实现 `ISavableInterface` 即可接入。存档带 `SaveVersion`，字段结构调整时通过版本号做迁移。

---

## 📁 目录结构

```
Source/Kitsune/
├── Public|Private/
│   ├── AbilitySystem/     # KitsuneGameplayAbility + 6 个语义父类、AttributeSet、ExecCalc
│   ├── Actor/             # EffectActor / Interactable / Weapon
│   ├── AI/                # AIController、BTTask、BTService
│   ├── AsyncAction/       # 异步蓝图节点（冷却监听、软引用 UI 入栈等）
│   ├── Characters/        # 角色基类、AnimInstance、角色数据资产
│   ├── CommonTypes/       # 跨层枚举与结构体（UI 数据、背包条目等）
│   ├── Component/         # ExtensionComponent / Combat / Interaction / UI(WidgetComponent)
│   ├── FunctionLibrary/   # 蓝图函数库（含 Inventory 相关查询）
│   ├── Game/              # GameMode / GameState / PlayerState / 存档 / Subsystem / 阵营矩阵
│   ├── GameplayTag/       # 原生 GameplayTag 声明与定义
│   ├── GameSettings/      # 游戏设置项
│   ├── Input/             # Enhanced Input 配置与管理
│   ├── Interfaces/        # Lockable / PawnCombat / PawnInteract / Savable
│   ├── Inventory/         # Definition / Instance / Trait / Component
│   ├── Player/            # PlayerController
│   ├── UI/                # ViewModel / Widget / DataObjects / HUD / Interact
│   └── UISettings/        # UI 相关 DeveloperSettings
├── Kitsune.Build.cs
└── DevLog.md
```

内容侧统一放在 `Content/BluePrints/`（能力、角色、UI、地图、输入配置）与 `Content/OutSide`、`Content/OutsideAssets`（外部美术资源）。

---

## 🚀 快速开始

### 环境要求

- **引擎**：Unreal Engine 5.6
- **插件**：Gameplay Abilities、Common UI、Enhanced Input、Motion Warping、Modeling Tools Editor Mode
- **IDE**：Visual Studio 2022 或 Rider for Unreal（个人使用 VS 2022 + ReSharper 转Rider）

### 构建与运行

1. 克隆仓库：

   ```bash
   git clone https://github.com/212518205/Combat.git
   ```

2. 使用 UE 5.6 打开 `Kitsune.uproject`（建议从 IDE 编译后启动）。
3. 首次打开会提示编译 C++ 模块，确认即可。
4. 默认启动关卡为 `Content/BluePrints/Maps/Start`；想直接看战斗原型可打开测试关卡 `Content/BluePrints/Maps/TestPawn`。
5. 运行时默认 GameMode 为 `BP_KitsuneGameModeBase`。

---

## 📄 许可证

本项目采用 **MIT 许可证**：允许自由使用、复制、修改、合并、发布，唯一要求是保留原作者的版权与许可声明。

---

**欢迎探讨！** 对框架设计或实现有疑问、建议，欢迎通过 Issues 或 Pull Requests 参与。
