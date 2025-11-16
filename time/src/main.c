
#include "8052.h"
typedef unsigned char u8;
typedef unsigned int u16;

#define LED1 P2_0
/*
每个定时器有 4 个控制位：
GATE：门控位
    0：由 TR0 启动定时器
    1：需要 INT0 引脚为高 + TR0=1 才能启动（用于外部控制）
C/T：选择是“定时器”还是“计数器”
    0：定时器模式（对内部时钟计数，常用）
    1：计数器模式（对外部引脚 T0/P3.4 的脉冲计数）
M1、M0：工作模式选择
    M1	M0	模式	说明
    0	0	方式0	13位定时器（不常用）
    0	1	方式1	✅ 16位定时器（最常用）
    1	0	方式2	8位自动重载（常用于串口通信）
    1	1	方式3	拆分模式（很少用）

设置 Timer0 为 16 位定时器模式
TMOD = 0x01; （0001）
高 4 位为 0 → Timer1 不影响
低 4 位 0001 → GATE=0, C/T=0, M1=0, M0=1 → 16位定时器模式
*/
void delay_10us(u16 ten_us){
    while(ten_us--);
}

//time0也是一个中断，要写中断入口
void time0_init(){
    TMOD|=0X01;//选择为定时器 0 模式，工作方式 1
    //TH0是高八位，TL0是低八位。两个连在一起是一个16位定时器
    //计时器是靠溢出发送中断信号来计时，
    //16位（0～65535）-> 65535μs。计时50ms->50000μs。65535-50000=15535->0x3CB0(TH0=0x3C,TL0=0xB0)
    TH0=0XFC; 
    TL0=0X67;
    ET0=1;//打开定时器 0 中断允许
    EA=1;//打开总中断
    TR0=1;//打开定时器
}
void time0() __interrupt(1){
    static u16 i;
    TH0=0XFC; 
    TL0=0X67;
    i++;
    if(i == 5000){
        i =0;
        LED1 = !LED1;
    }
    
}

void main(){
    time0_init();
    while(1){

    }
}
