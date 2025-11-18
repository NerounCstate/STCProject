#ifndef __KEY_H__
#define __KEY_H__ 
#include "intrins.h"

// 定义独立按键控制脚
#define KEY1 P3_1;
#define KEY2 P3_0;
//#define KEY3 P3_2;
#define KEY4 P3_3;

// 使用宏定义独立按键按下的键值
#define KEY1_PRESS 1
#define KEY2_PRESS 2
#define KEY3_PRESS 3
#define KEY4_PRESS 4
#define KEY_UNPRESS 0

// 模式定义
#define MODE_START_PAGE 0
#define MODE_TEMPERATURE 1
#define MODE_HARUHIKAGE 2
#define MODE_3 3
#define MODE_4 4

#endif