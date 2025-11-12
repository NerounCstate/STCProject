//ZZH告诉我如果让一个灯持续闪烁的同时当按下按键另一个灯反转状态，最好用中断。
#include "8052.h"

typedef unsigned char u8;
typedef unsigned int u16;

#define LED0 P2_0
#define LED1 P2_1
#define KEY1 P3_1
#define KEY3 P3_2

void delay_10us(u16 ten_us){
    while(ten_us--){
    };
}

void exit_init(){
    EA = 1;
    EX0 = 1;
    IT0 = 1;
}
void exit() __interrupt(0){
        LED1 =! LED1;
}
//LED0规律闪烁
void led0s(){
    LED0 =! LED0;
    delay_10us(5000);
}

void main(){
    exit_init();
    LED1 = 0;
    LED0 = 0;
    while(1){
        led0s();
        if(KEY3 == 0){
            exit_init();
        }
    }
    
}