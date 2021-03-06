![GitHub Logoa](/icon/favicon128_128.ico)
# Super Measuring Tool
这是一个依照地图提供的比列尺，进行测距的工具。

#Alpha0.1.2版本修改：
> 网盘链接: [https://pan.baidu.com/s/1UPebgpCKgbbOMLIh86Eqvg](https://pan.baidu.com/s/1UPebgpCKgbbOMLIh86Eqvg)


# 关于Super Measuring Tool的介绍（以下简称SMT）
这是第一版，也有很大可能就是最后一版，只要有需要将持续改进；
## 标尺
- 1  标尺调节： 鼠标滚轮可以调节标尺， 按住Shift，可以进行微调；
- 2  标尺作用： 标尺的长度决定了计算的准确性，现在版本指定标尺是100m的标尺，必须与游戏地图中地图100m标尺长度一样才准确；
## 密位
- 1 密位作用： 这个为squad的迫击炮密位，如果是其他游戏，可自行测定密位，然后在配置表中修改`config/Mil.ini`；
## 距离
- 1 距离作用：显示线段长度，按照标尺的定义，计算出的游戏中的距离；
## 方位
- 1 方位意义：显示了所画出的线段与正北的方向的顺时针旋转夹角， 与squad游戏中的方向相同；

## 其他操作介绍
- 1    Up   （ ↑ ） 键：可以在SMT与游戏之间自由切换；
- 2 Down （ ↓ ） 键：与Up键相似， 但可以隐藏STM在屏幕上的图形，如果还想使用，必须再次按下；
- 3   Left   （←） 键：关闭SMT，建议用词关闭SMT工具；
## 多余介绍（SMT中关于方位、密位的计算方法）
- 1 方位计算 ：按住鼠标左键（起始点）， 拖动鼠标（终点），期间形成一条直线，可看做向量v1，从起始点往正北方发出射线，取模100，得到向量v2，夹角是通过v1 、v2向量夹角计算公式计算出来的；
- 2 密位计算 ：密位是通过微积分的思想的出来的，在`config/Mil.ini`中记录了手动测得的散点密位，将这些散点画在xy坐标轴上，将相邻两点求出一次函数，将计算得到的距离 D 带入所在的距离区间的一次函数，将得到密位近似值。所以如果想精确密位，那么在`config/Mil.ini`中记录越多正确值，那么得到的密位越精确。（就靠你们了，我觉得25m的精度，求出的密位误差已经不超过2m了。）

# 使用方法详解
## 矫正标尺
- 1 按照100米的比例尺去矫正标尺, 图左图测得103米，实际只有100米，就需要滑动滚轮，调整标尺，由于调整幅度小，所以按住Shift进行微调，调整后为右图
![标尺矫正示意图](/MarkdownResources/标尺示意图.png)
- 2 关于标尺矫正，还有一种：拉出一段900m的距离，但如左图测得是526m，现在矫正标尺到测定距离像是为900m（如右图），现在标尺的比列就是游戏中900m的标尺。这样就可以在大图上进行测距。
![标尺矫正示意图2](/MarkdownResources/标尺矫正2.png)
- 3 与玩家常用的 SquadMc 对比 ，误差值在接受范围内。
![对比示意图](/MarkdownResources/对比示意图.png)

# 教学完成，可以自己玩了

> GitHub: [SMT_Alpha0.1.0.zip](https://github.com/CppToGo/SquadPlugin/files/3558628/SMT_Alpha0.1.0.zip)  （特慢）  
> 下载源码: [SMT_Alpha0.1.0.zip](https://github.com/CppToGo/SquadPlugin/releases)  （慢）  
> 百度网盘: [https://pan.baidu.com/s/1UPebgpCKgbbOMLIh86Eqvg](https://pan.baidu.com/s/1UPebgpCKgbbOMLIh86Eqvg) （这个比较快）  
