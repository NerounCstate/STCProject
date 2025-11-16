#include "8052.h"

typedef unsingned char u8;
typedef unsingned int u16;

#define DS18B20 P3_7

void delay_10us(u16 n){
    while(n--);
}

void smg_display(u8 dat[],u8 pos)
{
u8 i=0;
u8 pos_temp=pos-1;
for(i=pos_temp;i<8;i++)
{
switch(i)//位选
{
case 0: LSC=1;LSB=1;LSA=1;break;
case 1: LSC=1;LSB=1;LSA=0;break;
case 2: LSC=1;LSB=0;LSA=1;break;
case 3: LSC=1;LSB=0;LSA=0;break;
case 4: LSC=0;LSB=1;LSA=1;break;
case 5: LSC=0;LSB=1;LSA=0;break;
case 6: LSC=0;LSB=0;LSA=1;break;
case 7: LSC=0;LSB=0;LSA=0;break;
}
SMG_A_DP_PORT=dat[i-pos_temp];//传送段选数据
delay_10us(100);//延时一段时间，等待显示稳定
SMG_A_DP_PORT=0x00;//消音
}
}

u8 ds18b20_init(){
    ds18b20_reset();
    return ds18b20_check();
}

void ds18b20_reset(void)
{
DS18B20_PORT=0; //拉低 DQ
delay_10us(75); 
DS18B20_PORT=1; //DQ=1
delay_10us(2); 
}

void ds18b20_write_bit(u8 bit;u8 pin){
    DS18B20_PORT=0;
}