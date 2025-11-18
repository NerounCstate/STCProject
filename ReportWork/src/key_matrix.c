#include "key_matrix.h"

// 共阴极数码管显示0~F的段码数据
u8 gsmg_code[17] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
                    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

/*******************************************************************************
* 函 数 名       : key_matrix_scan
* 函数功能		 : 使用行列式扫描方法，检测矩阵按键是否按下，按下则返回对应键值
* 输    入       : 无
* 输    出    	 : key_value：1-16，对应S1-S16键，
				   0：按键未按下
*******************************************************************************/
u8 key_matrix_scan(void)
{
	u8 key_value = 0;

	KEY_MATRIX_PORT = 0xf7; // 给第一列赋值0，其余全为1
	if (KEY_MATRIX_PORT != 0xf7) // 判断第一列按键是否按下
	{
		delay_10us(1000); // 消抖
		switch (KEY_MATRIX_PORT) // 保存第一列按键按下后的键值
		{
		case 0x77:
			key_value = 1;
			break;
		case 0xb7:
			key_value = 5;
			break;
		case 0xd7:
			key_value = 9;
			break;
		case 0xe7:
			key_value = 13;
			break;
		}
	}
	while (KEY_MATRIX_PORT != 0xf7); // 等待按键松开

	KEY_MATRIX_PORT = 0xfb; // 给第二列赋值0，其余全为1
	if (KEY_MATRIX_PORT != 0xfb) // 判断第二列按键是否按下
	{
		delay_10us(1000); // 消抖
		switch (KEY_MATRIX_PORT) // 保存第二列按键按下后的键值
		{
		case 0x7b:
			key_value = 2;
			break;
		case 0xbb:
			key_value = 6;
			break;
		case 0xdb:
			key_value = 10;
			break;
		case 0xeb:
			key_value = 14;
			break;
		}
	}
	while (KEY_MATRIX_PORT != 0xfb); // 等待按键松开

	KEY_MATRIX_PORT = 0xfd; // 给第三列赋值0，其余全为1
	if (KEY_MATRIX_PORT != 0xfd) // 判断第三列按键是否按下
	{
		delay_10us(1000); // 消抖
		switch (KEY_MATRIX_PORT) // 保存第三列按键按下后的键值
		{
		case 0x7d:
			key_value = 3;
			break;
		case 0xbd:
			key_value = 7;
			break;
		case 0xdd:
			key_value = 11;
			break;
		case 0xed:
			key_value = 15;
			break;
		}
	}
	while (KEY_MATRIX_PORT != 0xfd); // 等待按键松开

	KEY_MATRIX_PORT = 0xfe; // 给第四列赋值0，其余全为1
	if (KEY_MATRIX_PORT != 0xfe) // 判断第四列按键是否按下
	{
		delay_10us(1000); // 消抖
		switch (KEY_MATRIX_PORT) // 保存第四列按键按下后的键值
		{
		case 0x7e:
			key_value = 4;
			break;
		case 0xbe:
			key_value = 8;
			break;
		case 0xde:
			key_value = 12;
			break;
		case 0xee:
			key_value = 16;
			break;
		}
	}
	while (KEY_MATRIX_PORT != 0xfe); // 等待按键松开

	return key_value;
}