# Kitsune-UE5-Framework - UE5 模块化游戏战斗框架原型

## 🎯 项目概述

这是一个基于 Unreal Engine 5.6 的 C++ 战斗框架原型。**本项目的核心目标并非制作一个内容完整的游戏，而是工程实践，旨在构建一个数据驱动、高度模块化、符合生产级开发规范的客户端架构范本。**

项目完整实践了 Unreal Engine 5 的核心框架，包括 **Gameplay Ability System (GAS)** 技能架构、**MVVM (Model-View-ViewModel)** 数据驱动UI、**行为树AI**决策系统以及基于接口的泛化交互系统。所有系统设计均以“可配置”、“可扩展”、“低耦合”为首要原则，为快速的功能迭代与复杂的游戏逻辑管理提供了清晰的工程解决方案。

## 🏗️ 系统架构

> **设计理念**：数据驱动，模块解耦。游戏逻辑与表现分离，系统功能通过数据资产配置，核心模块间通过接口和事件通信。
>
> 

## ✨ 特性与设计理念

- **模块化战斗系统**：基于 **GAS**，将技能、属性、效果彻底组件化。所有技能逻辑（如Player连招、伤害计算）均由可复用的 `GameplayAbility` 和可配置的 `DataAsset` 驱动，脱离硬编码。
- **数据驱动的 UI 框架**：独立实现了 **MVVM 模式**。`ViewModel` 层自动同步角色属性（生命值、能量）、技能状态至UMG界面，实现了业务逻辑与表现层的彻底解耦，使UI开发更高效、更易测试。
- **可扩展的 AI 决策层**：利用 **行为树** 与 **环境查询系统 (EQS)** 构建AI。将行为（如侧移、概率攻击）拆分为独立的 `BTTask` 和 `BTService`，支持通过行为树资产灵活组合复杂行为逻辑。
- **基于接口的交互系统**：通过 `PawnCombatInterface` 等接口定义角色与世界的交互契约。场景中的 `EffectActor`（如血瓶）仅需配置不同的 `GameplayEffect`，即可实现多样化的交互效果，系统扩展性极强。
- **生产级开发规范**：项目结构严格遵循 `Public/Private` 头文件分离，代码模块清晰（`Kitsune.Build.cs` 管理），并实践了输入映射、游戏用户设置保存等完整功能流程。

## 📁 项目结构详解
本项目采用清晰、模块化的C++代码结构，严格遵循Unreal Engine模块规范。以下为主要目录概览：

```
Kitsune
├── Private/Public
│   ├── AbilitySystem/     # GAS核心：技能组件、属性集(AttributeSet)、伤害计算(ExecCalc)
│   ├── Actor/            # 场景交互物基类(EffectActor)与武器系统
│   ├── AI/               # AI控制器、行为树任务(BTTask)与服务(BTService)
│   ├── Characters/       # 角色基类、动画实例及角色数据资产(DataAsset)
│   ├── UI/               # **MVVM框架核心**：ViewModel, DataObject, 控件组件
│   ├── Input/           # 增强输入(Enhanced Input)配置与管理
│   └── GameSettings/    # 游戏设置与存档系统
└── ...
```

详细到文件粒度的完整目录结构，请参阅：[项目源代码结构详情](PROJECT_STRUCTURE.md)。

## 🚀 快速开始

### 环境要求
- **引擎**: Unreal Engine 5.6
- **插件**: Gameplay Ability System, Common UI, Motion Warping 等相关插件
- **IDE**: Visual Studio 2022 或 Rider for Unreal  (个人使用VS 2022 + Resharper插件)

### 构建与运行
1.  克隆仓库：
    ```bash
    git clone https://github.com/212518205/Combat.git
    ```
2.  使用 UE5.6 打开 `Kitsune.uproject`(建议从Visual Studio编译运行)
3.  首次打开将提示编译C++模块，点击“Yes”确认。
4.  编译成功后，在内容浏览器中打开演示关卡(Content/Blueprints/Maps/TestPawn，是的，Pawn是笔误，就这个名字)
5.  点击“运行”即可体验框架原型。

## 🧪 技术演示与截图

本项目侧重于框架逻辑验证。以下演示展示了系统的**数据驱动特性**与**模块化能力**：

1.  **技能系统配置**：在编辑器中直接调整 `DataAsset` 中的伤害值等，运行时立即生效，无需重启游戏。
2.  **AI行为调整**：修改行为树中的概率权重或EQS查询半径，AI的决策逻辑与移动模式实时改变。
3.  **UI数据绑定**：角色受伤后，HUD上的血条通过 `AttributeViewModel` 自动同步更新(Calculate类里做计算，AttributeSet类里有做限制和事件通知)

效果展示：https://b23.tv/vSw7Bco b站BV号：BV1DvBpB3EAt

## 🔧 核心模块深度解析

### 1. Gameplay Ability System (GAS) 技能框架
- **设计**：`KitsuneAbilitySystemComponent` 作为能力系统组件，管理所有 `GameplayAbility` 和 `GameplayEffect`。
- **实现**：`ExecCalcDamageTaken` 展示了自定义伤害计算流程，支持复杂的公式扩展。所有技能参数均由 `DataAsset` 配置。

### 2. MVVM 数据驱动 UI
- **设计**：完全自主实现的MVVM层，包含 `ViewModelBase`、`AttributeViewModel`、`PlayerViewModel`、 等。
- **实现**：`UIManagerSubsystem` 全局管理UI生命周期。`ListDataObject` 系列类将任意数据源（如设置选项）抽象为可被 `CommonListView` 绑定的列表项，实现了按键重映射等复杂UI功能。

### 3. 行为树与AI决策
- **设计**：`KitsuneAIController` 搭载行为树，AI逻辑与控制器逻辑分离。
- **实现**：`BTService_OrientToTargetActor` 等服务提供持续决策信息，`BTTask_RotateToFaceTarget` 等任务执行具体行为，两者均可独立复用。

## 📝 后续规划
- [ ] 编写更详细的模块技术设计文档。
- [ ] 为关键类添加单元测试示例。
- [ ] 实现一个简单的网络同步原型，展示属性与技能的复制。
- [ ] 扩展工具链，开发简易的数据资产编辑器辅助工具。

## 📄 许可证

本项目采用 **MIT 许可证**。这是一种高度宽松的许可证，允许他人自由使用、复制、修改、合并、发布你的代码，唯一要求是保留原作者的版权和许可声明。


---

**欢迎探讨！** 如果你对这个框架的设计或实现有任何疑问或建议，欢迎通过 Issues 或 Pull Requests 参与贡献。