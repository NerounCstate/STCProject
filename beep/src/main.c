#include<8052.h>
#define BEEP P2_5
void delay10us(int ten_us){
    while(ten_us--);
}

void dou(int i){
    while(i--){
        BEEP =! BEEP;
        delay10us(176);
    }
    i = 0;
    BEEP = 0;
}
void rei(int i){
    while(i--){
        BEEP =! BEEP;
        delay10us(157);
    }
    i = 0;
    BEEP = 0;
}
void mi(int i){
    while(i--){
        BEEP =! BEEP;
        delay10us(140);
    }
    i = 0;
    BEEP = 0;
}
void fa(int i){
    while(i--){
        BEEP =! BEEP;
        delay10us(132);
    }
    i = 0;
    BEEP = 0;
}
void sol(int i){
    while(i--){
        BEEP =! BEEP;
        delay10us(118);
    }
    i = 0;
    BEEP = 0;
}
void la(int i){
    while(i--){
        BEEP =! BEEP;
        delay10us(105);
    }
    i = 0;
    BEEP = 0;
}
void si(int i){
    while(i--){
        BEEP =! BEEP;
        delay10us(93);
    }
    i = 0;
    BEEP = 0;
}

void main(){
   dou(500);  // 演奏 do 音符
delay10us(500);
rei(500);  // 演奏 re 音符
delay10us(500);
mi(500);   // 演奏 mi 音符
}