# StopModeTickless 功能说明
1. 每间隔13s醒来喂狗，其余时间进入STOP2模式，醒来的时间大概只有9ms。
2. 由于我的测试板子上有几个LDO有一些漏电流，进入STOP2的电流大概在12uA（STOP2理论能达到1uA）。
