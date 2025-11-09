#include "8052.h"
#define LED P2_0
#define K3 P3_1

void delay100us() {
  //_nop_();
  unsigned char i, j;
  i = 2;
  j = 15;
  do {
    while (--j)
      ;
  } while (--i);
}
void main() {

  while (1) {
    if (K3 == 1) {
      delay100us();
      LED = 0;
    } else {
      LED = 1;
    }
  }
}
