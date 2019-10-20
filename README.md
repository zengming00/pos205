在keil v4.10开发, 因为没有其它仪器设备检查存在的问题，目前勉强能用，但是遥控距离可能很短，很多年没玩单片机，哪里有问题我也不知道

2016年最后一次提交的版本经过长时间的使用存在开机问题

目前这个版本经过改进能够克隆空调遥控，暂时没有测试电视遥控是否正常，应该问题不大

蓝牙键开启克隆模式，密码2019

0-9，清除，确认，取消 共13个按键可以学习

![主板](https://github.com/zengming00/pos205/raw/master/res/0.png)

拆除多余的芯片，把防拆键焊死，否则电池供电时无法关机

通过串口刷机时，先把boot0与3.3v接通，然后通过usb转串口线连接RX、TX和GND

![刷机1](https://github.com/zengming00/pos205/raw/master/res/1.png)
![刷机2](https://github.com/zengming00/pos205/raw/master/res/2.png)
![刷机3](https://github.com/zengming00/pos205/raw/master/res/3.png)

# 红外接收与发射电路
![红外电路](https://github.com/zengming00/pos205/raw/master/res/4.gif)


