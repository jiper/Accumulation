USER--用户文件：主要保存main.c以及有关中段实现的文件
CORE--核心文件：内核文件，启动文件，芯片头文件，System文件。其中启动文件和System文件根据根据实际情况稍作修改，主要是堆栈设置和主频设置
LIB--外设底层驱动
OnBoardDriver--板级驱动文件，主要是对底层驱动的封装，供上层使用
Algorithm--算法文件
FILE--存放文档