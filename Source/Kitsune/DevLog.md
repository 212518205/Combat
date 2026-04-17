

#### DevLog - 2026.03.23 12:52

##### *本次完成*
- 将ItemInstance里的的UItemDefinition的TSubclassOf替换为UPrimaryAsset*，InteractPrompt的Entry根据Trait生成

##### 遇到问题
- 使用位标记的枚举无法在蓝图正常显示

##### 下次计划
- 

---


#### DevLog - 2026.03.24 22:07

##### *本次完成*
- 元数据说明符写错，修改过后可以正常显示
```
//UENUM(meta = (Bitflags, UseEnumValueAsMaskValueInEditor = "true"))
//UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true")) 
```

##### 遇到问题
- 

##### 下次计划
- 
---

#### DevLog - 2026.03.30 12:16

##### *本次完成*
- 添加库存系统、堆叠特征，添加Item函数

##### 遇到问题
- 

##### 下次计划
- 设计广播方式，UI与Add、Remove等函数触发的变化绑定

---


#### DevLog - 2026.04.17 13:45

##### *本次完成*
- 添加拾取功能、支持InteractListView的F键拾取并添加到背包
- 将VS+Resharper组合替换为Rider
- 更新.gitignore文件

##### 遇到问题
- 

##### 下次计划
- 添加背包界面以及背包物品的读取与显示

---
