#include <8052.h>
#include <math.h>
#define LED P2

void delay(unsigned int ms){
    unsigned int i;
    for(i=0;i<ms;i++);
}
void main(){
    while(1){
        for(int i=0;i<8;i++){
            //流水灯移位
            //括号内第一位数字控制的是从第几个灯开始操作，操作到i。
            //      a<<b表示把a的二进制表示向左移动b位，左移时右侧补零（没有想右移操作）
            //8个LED用int（00000000）表示，哪一位是0哪个LED就会亮
            //LED = ~(0x01<<i);//1是00000001，每次把1向左移动i位，取反。但这个板子是倒过来的，也就是1是10000000向右移位。可以把板子旋转180度
            LED = ~(1<<i);
            delay(50000);
        }
    }
}
