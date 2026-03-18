

#### DevLog - 2026.03.15 21:00

##### *本次完成*
- 

##### 遇到问题
- 出现错误回退

##### 下次计划
- 

---


#### DevLog - 2026.03.18 13:43

##### *本次完成*
- 添加了可点击CommonUserWidget
- 添加了菜单条目的简单交互效果
- 为之前的图标按钮优化了交互显示效果

##### 遇到问题
- ViewModel里的Level还是测试用的20，需要监听角色的等级并进行绑定

##### 下次计划
- 搭建背包界面：将InventoryScreen Push，回退后Pop正常显示
- 在交互系统中存储InventorySystem，在ViewModel中存储更新，在背包界面创建时获取到Item数组信息进行显示，以及背包系统的添加删除等接口
- 考虑是否需要扩展CommonActivatableStack，将Pop出来的Widget做一个暂存
- 将Z键回退继续绑定一下

---
