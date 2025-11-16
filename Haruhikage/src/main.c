#include<8052.h>
#define BEEP P2_5
#define LED1 P2_0
#define LED2 P2_1
#define LED3 P2_2
#define LED4 P2_3
#define LED5 P2_4
#define LED6 P2_5
#define LED7 P2_6
#define LED8 P2_7
#define key P3_1
#define SMG_DP P0
void delay10us(int ten_us){
while(ten_us--);
}

// 定义音符
void dou(int i) { // B音
while(i--) {
BEEP = !BEEP;
delay10us(176);
LED1 = 0;
}
LED1 = 1;
BEEP = 0;
}

void rei(int i) { // C#音
while(i--) {
BEEP = !BEEP;
delay10us(157);
LED2 = 0;
}
LED2 = 1;
BEEP = 0;
}

void mi(int i) { // D音
while(i--) {
BEEP = !BEEP;
delay10us(140);
LED3 = 0;
}
LED3 = 1;
BEEP = 0;
}

void fa(int i) { // E音
while(i--) {
BEEP = !BEEP;
delay10us(132);
LED4 = 0;
}
LED4 = 1;
BEEP = 0;
}

void sol(int i) { // F#音
while(i--) {
BEEP = !BEEP;
delay10us(118);
LED5 = 0;
}
LED5 = 1;
BEEP = 0;
}

void la(int i) { // G音
while(i--) {
BEEP = !BEEP;
delay10us(108);
//tm的LED6与蜂鸣器共用，只要这里用LED6就会出错音！！！
//LED6 = 0;
}
//LED6 = 1;
BEEP = 0;
}

void si(int i) { // A音
while(i--) {
BEEP = !BEEP;
delay10us(93);
LED7 = 0;
}
LED7 = 1;
BEEP = 0;
}
//额外两个高音
void hi1(int i) { 
while(i--) {
BEEP = !BEEP;
delay10us(87);
LED7 = 0;
}
LED7 = 1;
BEEP = 0;
}
void hi2(int i) { 
while(i--) {
BEEP = !BEEP;
delay10us(88);
LED7 = 0;
}
LED7 = 1;
BEEP = 0;
}
void play_music(){
// 前奏
for(int j=0; j<4; j++){
mi(380);delay10us(500);
rei(180); delay10us(500);
dou(380); delay10us(500);
rei(180); delay10us(500);
mi(380); delay10us(500);
fa(100); delay10us(500);
mi(100); delay10us(500);
rei(380); delay10us(200000);
}
delay10us(5000);
// 第一段歌词部分
dou(80); delay10us(500);
rei(80); delay10us(500);
mi(220); delay10us(500);
mi(220); delay10us(500);
rei(220); delay10us(500);
fa(220); delay10us(500);
mi(220); delay10us(500);
rei(220); delay10us(500);
rei(220); delay10us(500);
rei(220); delay10us(500);
dou(220); delay10us(500);
fa(220); delay10us(500);
mi(220); delay10us(500);
rei(220); delay10us(500);
rei(260); delay10us(500);
dou(80); delay10us(500);
rei(80); delay10us(500);
mi(600); delay10us(50000);
// 间奏部分
mi(220); delay10us(500);
sol(220); delay10us(500);
hi1(220); delay10us(500);
si(600); delay10us(500);
hi1(220); delay10us(500);
si(600); delay10us(500);
hi1(180); delay10us(500);
si(180); delay10us(500);
la(180); delay10us(500);
sol(350); delay10us(5000);
// 第二段歌词部分
sol(200); delay10us(500);
rei(200); delay10us(500);
fa(200); delay10us(500);
fa(200); delay10us(500);
mi(200); delay10us(500);
mi(200); delay10us(500);
mi(250); delay10us(10000);
dou(200); delay10us(500);
fa(180); delay10us(500);
mi(200); delay10us(500);
rei(200); delay10us(500);
mi(380); delay10us(500);
sol(200); delay10us(500);
dou(240); delay10us(500);
}

void main(){
int gsmg_code[5]={0x3f,0x06,0x5b,0x4f};
SMG_DP = gsmg_code[3];
if(key == 0){
delay10us(2000); 
if(key == 0){
//数码管3倒数
for(int i = 3;i>=0; i--){
SMG_DP = gsmg_code[i];
delay10us(50000);
}
play_music();
}
}
}