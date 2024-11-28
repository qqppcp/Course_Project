基于ue5官方的First Person示例，实现一个游戏demo，本课程作业为实现以下功能:
一、物件规则:
1.射击命中方块，获得积分X分
2.方块被子弹命中后，缩放为Y倍，再次被命中后销毁
二、游戏流程：
1.游戏开始时随机N个方块成为“重要目标”，射击命中后获得双倍积分
2.游戏开始后限时T秒，时间到后游戏结算，打印日志输出每个玩家获得的积分和所有玩家获得的总积分
三、附加题，Nice to have
1.利用UMG制作结算UI替代日志打印
2.支持多人联机

### 1.Attribute Component

构造一个继承自Actor Component的FPSDemoAttributeComponent类，