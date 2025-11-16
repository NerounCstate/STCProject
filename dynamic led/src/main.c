#include "8052.h"
#define SMG1 P0_0
#define SMG2 P0_1
#define SMG3 P0_2
#define SMG4 P0_3
#define SMG5 P0_4
#define SMG6 P0_5
#define SMG7 P0_6
#define SMG8 P0_7
#define SMG_DP P0
#define LSA P2_2
#define LSB P2_3
#define LSC P2_4


char gsmg_code[17] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71,0x00};

void delay10us(int ten_us){
    while(ten_us--){
    }
}
//位控制器，三位二进制共8个数码管，每次控制一个
void smg_display(void){
    for(int i = 0; i < 8; i++){
        switch(i){
            case 0:LSC = 1; LSB = 1; LSA = 1;break;
            case 1:LSC = 1; LSB = 1; LSA = 0;break;
            case 2:LSC = 1; LSB = 0; LSA = 1;break;
            case 3:LSC = 1; LSB = 0; LSA = 0;break;
            case 4:LSC = 0; LSB = 1; LSA = 1;break;
            case 5:LSC = 0; LSB = 1; LSA = 0;break;
            case 6:LSC = 0; LSB = 0; LSA = 1;break;
            case 7:LSC = 0; LSB = 0; LSA = 0;break;
        }
        SMG_DP = gsmg_code[i];
        delay10us(10);
        SMG_DP = 0x00;
    }
}

void main()
{
    smg_display();

                 
}