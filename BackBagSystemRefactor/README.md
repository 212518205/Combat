# 合入后问题修正清单

这一批文件是**修正版**,对应你项目里已合入的同名文件。每处改动我都用 `// 【修正】` 注释标出来了,对照着改即可。

## 必须改(不改无法测试)

| 文件 | 问题 | 修正 |
|---|---|---|
| `InventorySystemComponent.cpp` | `GetCategoryInfo` 返回**悬垂指针**(指向局部 TMap) | 改成 `static` 缓存,返回指针指向 static 内存 |
| `InventorySystemComponent.cpp` | `GetCapacityByCategoryID` 空指针解引用 | 加 `Info ? ... : 0` 判空 |
| `InventorySystemComponent.cpp` | `UnlockCategorySlots_Implementation` 解锁后不广播 | 末尾补 `CapacityChanged.Broadcast()` |
| `KitsuneSaveSubsystem.cpp` | `SaveGame()` 漏了 `SaveToSlot()` | 末尾补 `CurrentSaveGame->SaveToSlot()`(否则存档不落盘) |
| `KitsuneSaveSubsystem.cpp` | 遍历 `SaveTargets` 不判空 | 加 `if (Target)` |
| `KitsuneSaveSubsystem.h` | 缺 `TScriptInterface` / `ISavableInterface` 的 include | 补 `UObject/ScriptInterface.h` 和 `Interfaces/SavableInterface.h` |
| `FrontendStructTypes.h` | 缺 `FPrimaryAssetId` 的 include | 补 `UObject/PrimaryAssetId.h` |

## 建议改(健壮性,不阻塞)

| 文件 | 问题 | 修正 |
|---|---|---|
| `KitsuneSaveGame.h` | `PlayerLevel` 未初始化(UB) | 给 `= 1` |
| `WidgetBackBagScreen.cpp` | `EmptyCount` 未 clamp,可能为负 | `FMath::Max(0, ...)` |

## 顺手的拼写修正(不影响功能,可选)

- `KitsuneAttributeSet.h` 第 151 行:`Category = "Attribuute | Economy"` → `"Attribute | Economy"`(多打一个 u)。
- `KitsuneAttributeSet.cpp` 第 246 行:`void UKitsuneAttributeSet::OnRep_Gold(const FGameplayAttributeData& OleGold)` → 把 `OleGold` 改成 `OldGold`(声明和实现两处同步改,名字要一致)。

## 改完后怎么测

1. 编译通过(如果还报 `FPrimaryAssetId` 或 `TScriptInterface` 未定义,就是上面两个 include 没加)。
2. 运行,打开背包 → 不崩、格子数正确(初始容量 − 已占用 + 锁格)。
3. 点锁格解锁 → 空格子变多(单人下也能看到,靠 `CapacityChanged.Broadcast()`)。
4. 存档 → 退出 → 重进 → 背包内容、容量都还在(靠 `SaveToSlot()` 真正落盘)。
