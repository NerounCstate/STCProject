#include"8052.h"
typedef unsigned char  u8;
typedef unsigned int  u16;
#define DC P1_0
#define DC_run_time 5000

void delay_ms(u16 ms)
{
u16 i,j;
for(i=ms;i>0;i--)
for(j=110;j>0;j--);
}

void main()
{
DC=1;
delay_ms(DC_run_time);
DC=0;
while(1)
{

}
}