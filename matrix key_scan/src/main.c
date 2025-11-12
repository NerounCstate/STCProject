//说实话，这个不难，就是有点复杂。不想往下写了。
//说一下主要思路：一列是0，检查每一行，如果有行也是0，key_value是行和列都是0的这个位置。以此类推。
#include <8052.h>

typedef unsigned char u8;
typedef unsigned int u16;

#define KEY_MATRIX_PORT P1
#define SMG P0

u8 gsmg_code[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
void delay_10us(u16 ten_us)
{
    while(ten_us--){

    }
}

u8 matrix_rank_scan(){
    //P10-P13是列，P14-P17是行
    //行1 行2 行3 行4 列1 列2 列3 列4
    // 0   0   0   0  0  0   0   0
    u8 key_value = 0;
    KEY_MATRIX_PORT = 0xf7;
    if(KEY_MATRIX_PORT!=0xf7){//判断第一列按键是否按下
        delay_10us(100);//消抖
        switch(KEY_MATRIX_PORT){//保存第一列按键按下后的键值
            case 0x77: key_value=1;break;
            case 0xb7: key_value=5;break;
            case 0xd7: key_value=9;break;
            case 0xe7: key_value=13;break;
        }
    }
    while(KEY_MATRIX_PORT!=0xf7);//等待按键松开
    KEY_MATRIX_PORT=0xfb;//给第二列赋值 0，其余全为 1
    if(KEY_MATRIX_PORT!=0xfb){//判断第二列按键是否按下{
        delay_10us(1000);//消抖
        switch(KEY_MATRIX_PORT){//保存第二列按键按下后的键值
        case 0x7b: key_value=2;break;
        case 0xbb: key_value=6;break;
        case 0xdb: key_value=10;break;
        case 0xeb: key_value=14;break;
        }
    }
    while(KEY_MATRIX_PORT!=0xfb);//等待按键松开
    KEY_MATRIX_PORT=0xfd;//给第三列赋值 0，其余全为 1
    if(KEY_MATRIX_PORT!=0xfd){//判断第三列按键是否按下{
        delay_10us(1000);//消抖
        switch(KEY_MATRIX_PORT){//保存第三列按键按下后的键值
        case 0x7d: key_value=3;break;
        case 0xbd: key_value=7;break;
        case 0xdd: key_value=11;break;
        case 0xed: key_value=15;break;
        }
    }
    while(KEY_MATRIX_PORT!=0xfd);//等待按键松开
    KEY_MATRIX_PORT=0xfe;//给第四列赋值 0，其余全为 1
    if(KEY_MATRIX_PORT!=0xfe){//判断第四列按键是否按下
        delay_10us(1000);//消抖
        switch(KEY_MATRIX_PORT){//保存第四列按键按下后的键值
        case 0x7e: key_value=4;break;
        case 0xbe: key_value=8;break;
        case 0xde: key_value=12;break;
        case 0xee: key_value=16;break;
        }
    }
    while(KEY_MATRIX_PORT!=0xfe);//等待按键松开
    return key_value;
}
//翻转法检测按键（更高效跟简单）
u8 matrix_flip_scan(){ 
    //行为0、列为1:0x0f
    //列为1、行为0:0xf0
    u8 key_value = 0;
    KEY_MATRIX_PORT = 0x0f;
    if(KEY_MATRIX_PORT != 0x0f){
        delay_10us(1000);
        if(KEY_MATRIX_PORT != 0x0f){
            //消除抖动后开始检测
            KEY_MATRIX_PORT = 0x0f;
            switch(KEY_MATRIX_PORT){
                case 0x07:key_value = 1;break;
                case 0x0b:key_value = 2;break;
                case 0x0d:key_value = 3;break;
                case 0x0e:key_value = 4;break;
                //这里的key_value不是键值，而是按键的列数
            }
            //翻转
            KEY_MATRIX_PORT = 0xf0;
            switch(KEY_MATRIX_PORT){
                case 0x70:key_value = key_value ;break;
                case 0xb0:key_value = key_value + 4;break;
                case 0xd0:key_value = key_value + 8;break;
                case 0xe0:key_value = key_value + 12;break;
            }
            while(KEY_MATRIX_PORT!=0xf0);//等待按键松开
        }else{
            key_value = 0;
        }
    }
    return key_value;
}
void main(){
    u8 key=0;
    while(1){
        key=matrix_flip_scan();
    if(key!=0)
    SMG=gsmg_code[key-1];//得到的按键值减 1 换算成数组下标对应 0-F 段码
}
}