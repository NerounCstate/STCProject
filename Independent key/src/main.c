#include "8052.h"

typedef unsigned int u16;
typedef unsigned char u8;

#define K1 P3_1
#define K2 P3_2
#define K3 P3_3
#define K4 P3_4

#define K1P 1
#define K2P 2
#define K3P 3
#define K4P 4
#define KUP 0

#define BEEP P2_5

#define LED1 P2_0

void delay10us(u16 ten_ms){
    while(ten_ms--);
}

//按键检测
//mode1持续扫描
//static可记录上一次的值而非重新赋值
void beep(u8 t){
    while(t){
        BEEP = ! BEEP;
        delay10us(100);
        t--;
    }
}
u8 key_scan(u8 mode){
    //key = 1表示未按下
    static u8 key = 1;
    if(mode){
        //如果检测到按下
        if(key == 1 && (K1 == 0 || K2 == 0 || K3 == 0 || K4 == 0)) {
            key = 0;
            delay10us(100);
            if(K1 == 0) { beep(10); return K1P; }
            else if(K2 == 0) { beep(10); return K2P; }
            else if(K3 == 0) { beep(10); return K3P; }
            else if(K4 == 0) { beep(10); return K4P; }
        }else if(K1 == 1 && K2 == 1 && K3 == 1 && K4 == 1){
            key = 1;
        }
    }
    return KUP;
}
void main(){
    
    u8 key;
    while(1){
        key = key_scan(1);
        if(key != KUP){
            LED1 = !LED1;
        }
    }
    
}