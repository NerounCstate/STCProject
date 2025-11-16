#include<8052.h>
#define SMG_DP P0
#define BEEP P2_5

void delay10us(int _ms){
    while(_ms--);
}
void main(){
    int gsmg_code[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
    while(1){
        for(int i = 0; i<16;i++){
            SMG_DP = gsmg_code[i];
            int n = 100;
            while(n--){
                BEEP =! BEEP;
                delay10us(160-i*7);
            }
            

            delay10us(50000);
    }
    }
    
    
}