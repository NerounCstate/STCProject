#include <lcd1602.h>
#include <8051.h>
#include "intrins.h"
#include "ds18b20.h"
#include "key.h"
#include "key_matrix.h"
#include "public.h"

#define KEY3 P3_2
// 函数声明
void temperature_display(void);
void haruhikage_main(void);
void mode_3_display(void);
void mode_4_display(void);
void haruhikage_playing(void);


// 全局变量
unsigned char current_mode = MODE_START_PAGE;

// 中断初始化
void exit_init(void) {
    IT0 = 1;  // 外部中断0下降沿触发
    //IT1 = 1;  // 外部中断1下降沿触发
    EX0 = 1;  // 使能外部中断0
    //EX1 = 1;  // 使能外部中断1
    EA = 1;   // 使能总中断
}

// 外部中断1服务函数（处理KEY3触发）
void INT1_ISR() __interrupt(0) {
    delay_10us(1000); 
    if (KEY3 == 0) {
        
        while (KEY3 == 0);  // 等待释放
        
        // 任何时候按下KEY3都返回初始界面
        current_mode = MODE_START_PAGE;
    }
}

void main(void)
{
    unsigned char key_val = 0;
    
    // 初始化
    lcd1602_init();
    ds18b20_init();
    exit_init();
    while(1)
    {
        // 如果在初始界面，检测矩阵按键
        if (current_mode == MODE_START_PAGE) {
            key_val = key_matrix_scan();
            if (key_val != 0) {
                switch(key_val) {
                    case 1:
                        current_mode = MODE_TEMPERATURE;
                        break;
                    case 2:
                        current_mode = MODE_HARUHIKAGE;
                        break;
                    case 3:
                        current_mode = MODE_3;
                        break;
                    case 4:
                        current_mode = MODE_4;
                        break;
                    default:
                        break;
                }
            }
        }
        
        // 根据当前模式执行对应功能
        switch(current_mode)
        {
            case MODE_START_PAGE:
                lcd1602_show_string(0, 0, "Press Key3 Back ");
                lcd1602_show_string(0, 1, "Matrix_SKey 1-4 ");
                break;
                
            case MODE_TEMPERATURE:
                
                lcd1602_show_string(0, 0, "Temperature:    "); 
                temperature_display();
                break;
                
            case MODE_HARUHIKAGE:
                lcd1602_show_string(0, 0, "Press Key1 To   ");
                lcd1602_show_string(0, 1, "Play: Haruhikage");
                haruhikage_playing();
                break;
                
            case MODE_3:
                mode_3_display();
                
                break;
                
            case MODE_4:
                mode_4_display();
                break;
        }
        
        delay_ms(20);
    }
}

void temperature_display(void)
{
    unsigned int counter = 0;
    float temp_value = 0.0;  // 使用float类型接收温度值
    unsigned char temp_buf[16];  // 增大缓冲区
    
    ds18b20_init();  
    
    // 主循环
    while(current_mode == MODE_TEMPERATURE)
    {
        counter++;
        // 每50次循环读取一次温度（约1秒）
        if(counter >= 50)
        {
            counter = 0;
            
            // 读取温度
            temp_value = ds18b20_read_temperature();
            
            // 处理温度显示
            unsigned char index = 0;
            int integer_part;
            int decimal_part;
            
            // 分离整数部分和小数部分
            if(temp_value < 0) {
                temp_buf[index++] = '-';  // 负号
                temp_value = -temp_value;
            }
            
            integer_part = (int)temp_value;
            decimal_part = (int)((temp_value - integer_part) * 10 + 0.5); // 保留一位小数
            
            // 构建显示字符串
            // 处理整数部分
            if(integer_part >= 100)
            {
                temp_buf[index++] = (integer_part/100) + '0';
                integer_part %= 100;
            }
            
            if(integer_part >= 10 || index > 0) // 如果百位有数字或index>0
            {
                temp_buf[index++] = (integer_part/10) + '0';
                integer_part %= 10;
            }
            
            temp_buf[index++] = integer_part + '0';
            
            // 小数点
            temp_buf[index++] = '.';
            
            // 小数位
            temp_buf[index++] = decimal_part + '0';
            
            // 摄氏度符号
            temp_buf[index++] = 'C';
            
            // 字符串结束符
            temp_buf[index] = '\0';
            
            lcd1602_show_string(0, 1, "                "); 
            
            // 显示温度值
            lcd1602_show_string(0, 1, temp_buf);
        }
        
        delay_ms(20); 
    } 
}
void haruhikage_playing(void){
    haruhikage();  // 跳转到Haruhikage.c
}
// 按键3功能预留
void mode_3_display(void)
{
    lcd1602_show_string(0, 0, "Mode 3:        ");
    lcd1602_show_string(0, 1, "ILike Haruhikage");
    
    while(current_mode == MODE_3)
    {
        // 功能3
        delay_ms(100);
    }
}

// 按键4功能预留
void mode_4_display(void)
{
    lcd1602_show_string(0, 0, "Mode 4:        ");
    lcd1602_show_string(0, 1, "Battlefield 6");
    
    while(current_mode == MODE_4)
    {
        // 功能4
        delay_ms(100);
    }
}