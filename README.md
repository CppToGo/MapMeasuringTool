# SquadPlugin
这是一个squad的辅助插件，不是外挂。

> GitHub: [SMT_Alpha0.1.0.zip](https://github.com/CppToGo/SquadPlugin/files/3558628/SMT_Alpha0.1.0.zip)  
> 百度网盘: [SMT_Alpha0.1.0.zip](https://pan.baidu.com/s/1bgAbescZWx21xCuwUH6DsA)
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
