#ifndef _LCD1602_H
#define _LCD1602_H

#include <public.h> 

// LCD1602数据口4位和8位定义，若为1，则为LCD1602四位数据口驱动，反之为8位
#define LCD1602_4OR8_DATA_INTERFACE 0  // 默认使用8位数据口LCD1602

// 管脚定义 - 保持原始连接，但改为PlatformIO兼容格式
// 原始定义: sbit LCD1602_RS=P2^6; sbit LCD1602_RW=P2^5; sbit LCD1602_E=P2^7;
#define LCD1602_RS P2_6  // 数据命令选择 (原始P2.6)
#define LCD1602_RW P2_5  // 读写选择 (原始P2.5) 
#define LCD1602_E  P2_7  // 使能信号 (原始P2.7)
#define LCD1602_DATAPORT P0  // 宏定义LCD1602数据端口
void lcd1602_write_cmd(u8 cmd);  // 添加这一行
void lcd1602_write_data(u8 dat);


// 函数声明
void lcd1602_init(void);
void lcd1602_clear(void);
void lcd1602_show_string(u8 x, u8 y, u8 *str);

#endif