#ifndef __KEY_MATRIX_H__
#define __KEY_MATRIX_H__

#include "public.h"

// 使用宏定义矩阵按键控制口
#define KEY_MATRIX_PORT P1

// 函数声明
u8 key_matrix_scan(void);

#endif