

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