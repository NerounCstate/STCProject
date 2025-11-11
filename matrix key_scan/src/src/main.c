//说实话，这个不难，就是有点复杂。不想往下写了。
//说一下主要思路：一列是0，检查每一行，如果有行也是0，key_value是行和列都是0的这个位置。以此类推。
#include <8052.h>

typedef unsigned char u8;
typedef unsigned int u16;

#define KEY_MATRIX_PORT P1
#define SMG P0

u8 gsmg_code[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
void delay10us(u16 ten_us)
{
    while(ten_us--){

    }
}

void matrix_scan(){
    //P10-P13是列，P14-P17是行
    //行1 行2 行3 行4 列1 列2 列3 列4
    // 0   0   0   0  0  0   0   0
    u8 key_value = 0;
    KEY_MATRIX_PORT = 0xf7;
    if(KEY_MATRIX_PORT!=0xf7)//判断第一列按键是否按下
{
delay10us(1000);//消抖
switch(KEY_MATRIX_PORT)//保存第一列按键按下后的键值
{
case 0x77: key_value=1;break;
case 0xb7: key_value=5;break;
case 0xd7: key_value=9;break;
case 0xe7: key_value=13;break;
}
}
while(KEY_MATRIX_PORT!=0xf7);//等待按键松开

}
void main(){

}