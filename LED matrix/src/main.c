#include "8051.h"

//srclk移位寄存器输入
//rclk存储寄存器输入
//ser串行数据输入
#define SER P3_4
#define RCLK P3_5
#define SRCLK P3_6

typedef unsigned char  u8;
typedef unsigned int  u16;

#define LEDDZ P0

u8 gled_row[8]={0x38,0x7C,0x7E,0x3F,0x3F,0x7E,0x7C,0x38};//LED 点阵显示数字 0 的行数据
u8 gled_col[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};//LED 点阵显示数字 0 的列数据

void delay_10us(u16 ten_us)
{
    while(ten_us--);
}

void delay_ms(u16 ms)
{
    u16 i,j;
    for(i=ms;i>0;i--)
    for(j=110;j>0;j--);
}

void hc595_write(u8 dat)
{ 
    u8 i=0;
    for(i=0;i<8;i++)//循环 8 次即可将一个字节写入移位寄存器中
    {   
        //dat>>7可以把8位二进制最高位（最左侧）移动到最低位赋值给SER。而SER的硬件只能读取最低位的数据
        SER=dat>>7;//优先传输一个字节中的高位
        //把dat中的最低位放在最高位等待下一次写入，即下一个SER=dat>>7
        dat<<=1;//将低位移动到高位
        SRCLK=0;
        delay_10us(1);
        //周期电压上升沿时会写入
        SRCLK=1;
        delay_10us(1);//移位寄存器时钟上升沿将端口数据送入寄存器中
    }
    //好了孩子，我现在要启动储存寄存器了
    RCLK=0;
    delay_10us(1);
    RCLK=1;
}
void main(){
    u8 i=0;
    //LEDDZ=0x7f;//将 LED 点阵列全部设置为 0，即 LED 阴极为低电平
    while(1){
        for(i=0;i<8;i++){
            LEDDZ=gled_col[i];//传送列选数据
            hc595_write(gled_row[i]);//传送行选数据
            delay_10us(100);//延时一段时间，等待显示稳定
            hc595_write(0x00);//消影
        }
    }
    
}