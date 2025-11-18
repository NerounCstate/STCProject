#include<8052.h>
#include "public.h"
#include "led8.h"
#include "beep.h"
#include "smg.h"
#include "key.h"

// 函数声明
void dou(int i);
void rei(int i);
void mi(int i);
void fa(int i);
void sol(int i);
void la(int i);
void si(int i);
void hi1(int i);
void hi2(int i);
void play_music(void);  // 添加函数声明
void play_music_interruptible(void);  // 添加可中断的音乐播放函数声明

// 全局变量，用于控制音乐播放
extern unsigned char current_mode;

// 定义音符
void dou(int i) { // B音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(176);
        LED1 = 0;
    }
    LED1 = 1;
    BEEP = 0;
}

void rei(int i) { // C#音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(157);
        LED2 = 0;
    }
    LED2 = 1;
    BEEP = 0;
}

void mi(int i) { // D音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(140);
        LED3 = 0;
    }
    LED3 = 1;
    BEEP = 0;
}

void fa(int i) { // E音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(132);
        LED4 = 0;
    }
    LED4 = 1;
    BEEP = 0;
}

void sol(int i) { // F#音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(118);
        LED5 = 0;
    }
    LED5 = 1;
    BEEP = 0;
}

void la(int i) { // G音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(108);
        //tm的LED6与蜂鸣器共用，只要这里用LED6就会出错音！！！
        //LED6 = 0;
    }
    //LED6 = 1;
    BEEP = 0;
}

void si(int i) { // A音
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(93);
        LED7 = 0;
    }
    LED7 = 1;
    BEEP = 0;
}
//额外两个高音
void hi1(int i) { 
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(87);
        LED7 = 0;
    }
    LED7 = 1;
    BEEP = 0;
}
void hi2(int i) { 
    while(i-- && current_mode == MODE_HARUHIKAGE) {
        BEEP = !BEEP;
        delay_10us(88);
        LED7 = 0;
    }
    LED7 = 1;
    BEEP = 0;
}
void play(){
// 前奏
for(int j=0; j<4 && current_mode == MODE_HARUHIKAGE; j++){
mi(380);delay_10us(500);
rei(180); delay_10us(500);
dou(380); delay_10us(500);
rei(180); delay_10us(500);
mi(380); delay_10us(500);
fa(100); delay_10us(500);
mi(100); delay_10us(500);
rei(380); delay_10us(200000);
}
delay_10us(5000);
// 第一段歌词部分
dou(80); delay_10us(500);
rei(80); delay_10us(500);
mi(220); delay_10us(500);
mi(220); delay_10us(500);
rei(220); delay_10us(500);
fa(220); delay_10us(500);
mi(220); delay_10us(500);
rei(220); delay_10us(500);
rei(220); delay_10us(500);
rei(220); delay_10us(500);
dou(220); delay_10us(500);
fa(220); delay_10us(500);
mi(220); delay_10us(500);
rei(220); delay_10us(500);
rei(260); delay_10us(500);
dou(80); delay_10us(500);
rei(80); delay_10us(500);
mi(600); delay_10us(50000);
// 间奏部分
mi(220); delay_10us(500);
sol(220); delay_10us(500);
hi1(220); delay_10us(500);
si(600); delay_10us(500);
hi1(220); delay_10us(500);
si(600); delay_10us(500);
hi1(180); delay_10us(500);
si(180); delay_10us(500);
la(180); delay_10us(500);
sol(350); delay_10us(5000);
// 第二段歌词部分
sol(200); delay_10us(500);
rei(200); delay_10us(500);
fa(200); delay_10us(500);
fa(200); delay_10us(500);
mi(200); delay_10us(500);
mi(200); delay_10us(500);
mi(250); delay_10us(10000);
dou(200); delay_10us(500);
fa(180); delay_10us(500);
mi(200); delay_10us(500);
rei(200); delay_10us(500);
mi(380); delay_10us(500);
sol(200); delay_10us(500);
dou(240); delay_10us(500);
}

void haruhikage(){
int gsmg_code[5]={0x3f,0x06,0x5b,0x4f};
SMG_DP = gsmg_code[3];
if(key == 0){
delay_10us(2000); 
if(key == 0){
//数码管3倒数
for(int i = 3;i>=0; i--){
SMG_DP = gsmg_code[i];
delay_10us(50000);
}
play();
}
}
}