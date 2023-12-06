# StopModeTickless 功能说明
1. 创建一个线程，每间隔13s醒来喂狗，其余时间进入STOP2模式，醒来的时间大概只有9ms。
2. 由于我的测试板子上有几个LDO有一些漏电流，进入STOP2后电流大概在12uA（STOP2理论能达到1uA）。
3. 主要关注lpm_bsp.c/FreeRTOSConfig.h两个文件即可。
