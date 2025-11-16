#include "8052.h"

typedef unsigned char u8;
typedef unsigned int u16;

#define KEY3 P3_2
#define LED1 P2_1


void delay_10us(u16 ten_us)
{
while(ten_us--);
}
//外部中断0
void exit0_init(void){
    IT0=1;//跳变沿触发方式（下降沿）
    EX0=1;//打开 INT0 的中断允许
    EA=1;//打开总中断
}

//SDCC编译器使用__interrupt()来替代interrupt处理中断
void exit0(void) __interrupt(0){
        delay_10us(1000);//消抖
        //if(KEY3==0)//再次判断 K3 键是否按下
        while(KEY3 == 0){
            LED1= 0;//LED1 状态翻转
        //delay_10us(100000);
        }
        
    }

void main(){
    exit0_init();
    while(1){
        LED1 = 1;
    }
   
}

