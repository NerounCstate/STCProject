#include "8052.h"
#define LED P2_0
#define K3 P3_1

void delay10us(u16 i) {
  while(i--)
}
void main() {

  while (1) {
    if (K3 == 1) {
      delay10us(100);
      LED = 0;
    } else {
      LED = 1;
    }
  }
}
