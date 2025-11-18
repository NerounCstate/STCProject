#ifndef _ds18b20_H
#define _ds18b20_H

#include "public.h"

//管脚定义
#define DS18B20_PORT P3_7	//DS18B20数据口定义


//函数声明
u8 ds18b20_init(void);
float ds18b20_read_temperature(void);

#endif