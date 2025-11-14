#include <8052.h>

#define LED P2
#define LED1 P2_0
#define LED2 P2_1
#define LED3 P2_2
#define LED4 P2_3
#define LED5 P2_4
#define LED6 P2_5
#define LED7 P2_6
#define LED8 P2_7

typedef unsigned int u16;
typedef unsigned char u8;
// LED闪烁
void delay_10us(u16 ten_us) //@11.0592MHz
{
  while (ten_us--)
    ;
}
void main() {

  while (1) {
    LED1 = 0;
    delay_10us(90000);
    LED1 = 1;
    delay_10us(90000);
  }
}