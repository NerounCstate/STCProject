#include "8052.h"


// 数据类型定义
typedef unsigned char u8;
typedef unsigned int u16;

// 端口定义
#define DS18B20_PORT P3_7    // DS18B20数据线
#define LSA P2_2             // 数码管位选A
#define LSB P2_3             // 数码管位选B
#define LSC P2_4             // 数码管位选C
#define SMG_A_DP_PORT P0     // 数码管段选
#define KEY1 P3_0            // 按键1
#define KEY2 P3_1            // 按键2
#define KEY3 P3_2            // 按键3
#define KEY4 P3_3            // 按键4
#define BEEP P1_0            // 蜂鸣器
#define LED1 P1_1            // LED1
#define LED2 P1_2            // LED2
#define LED3 P1_3            // LED3
#define LED4 P1_4            // LED4
#define LED_MATRIX_DATA P0   // LED点阵数据
#define LED_MATRIX_ROW P2    // LED点阵行控制

// 共阴极数码管段码
u8 gsmg_code[18] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, // 0-7
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, // 8-F
    0x40  // 16: 负号
};

// LED点阵显示图案
u8  led_ice[8] = {    // 冰雪图案
    0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x7E, 0x3C, 0x18
};
u8 led_fire[8] = {   // 火焰图案
    0x3C, 0x7E, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00
};
u8 led_sun[8] = {    // 太阳图案
    0x3C, 0x42, 0xA5, 0x81, 0x81, 0xA5, 0x42, 0x3C
};

// 全局变量
volatile u8 display_mode = 0;      // 0:温度 1:时钟 2:设置
volatile u8 hour = 12, minute = 0, second = 0; // 时钟变量
volatile u8 setting_item = 0;      // 0:小时 1:分钟
volatile int temp_value = 250;     // 温度值(25.0℃)
volatile u8 temp_alarm = 30;       // 温度报警阈值(30℃)
volatile u8 pwm_duty = 50;         // PWM占空比(50%)
volatile u8 second_count = 0;      // 秒计数器
volatile u8 key_pressed = 0;       // 按键状态
volatile u8 led_pattern = 0;       // LED图案选择
volatile u8 alarm_flag = 0;        // 报警标志

// 10us延时函数
void delay_10us(u16 ten_us) {
    while(ten_us--);
}

// 毫秒延时
void delay_ms(u16 ms) {
    u16 i;
    for(i = 0; i < ms; i++)
        delay_10us(100); // 100 * 10us = 1ms
}

// 动态数码管显示
void smg_display(u8 dat[], u8 pos) {
    u8 i = 0;
    u8 pos_temp = pos - 1;
    for(i = pos_temp; i < 8; i++) {
        switch(i) { // 位选
            case 0: LSC = 1; LSB = 1; LSA = 1; break;
            case 1: LSC = 1; LSB = 1; LSA = 0; break;
            case 2: LSC = 1; LSB = 0; LSA = 1; break;
            case 3: LSC = 1; LSB = 0; LSA = 0; break;
            case 4: LSC = 0; LSB = 1; LSA = 1; break;
            case 5: LSC = 0; LSB = 1; LSA = 0; break;
            case 6: LSC = 0; LSB = 0; LSA = 1; break;
            case 7: LSC = 0; LSB = 0; LSA = 0; break;
        }
        SMG_A_DP_PORT = dat[i - pos_temp]; // 传送段选数据
        delay_10us(40); // 减少延时，减轻闪烁
        SMG_A_DP_PORT = 0x00; // 消影
    }
}

// DS18B20复位
void ds18b20_reset(void) {
    DS18B20_PORT = 0; // 拉低DQ
    delay_10us(75);   // 拉低750us
    DS18B20_PORT = 1; // DQ=1
    delay_10us(2);    // 20US
}

// 检测DS18B20是否存在
u8 ds18b20_check(void) {
    u8 time_temp = 0;
    while(DS18B20_PORT && time_temp < 20) { // 等待DQ为低电平
        time_temp++;
        delay_10us(1);
    }
    if(time_temp >= 20) return 1; // 超时返回1
    else time_temp = 0;
    
    while((!DS18B20_PORT) && time_temp < 20) { // 等待DQ为高电平
        time_temp++;
        delay_10us(1);
    }
    if(time_temp >= 20) return 1; // 超时返回1
    return 0;
}

// DS18B20写一个字节
void ds18b20_write_byte(u8 dat) {
    u8 i, j;
    for(j = 0; j < 8; j++) {
        DS18B20_PORT = 0; // 拉低总线
        delay_10us(1);    // 等待10us
        DS18B20_PORT = dat & 0x01; // 写数据
        delay_10us(6);    // 延时60us
        DS18B20_PORT = 1; // 释放总线
        dat >>= 1;        // 右移一位
    }
}

// DS18B20读一个字节
u8 ds18b20_read_byte(void) {
    u8 i, j, dat = 0;
    for(j = 0; j < 8; j++) {
        DS18B20_PORT = 0; // 拉低总线
        delay_10us(1);    // 延时10us
        DS18B20_PORT = 1; // 释放总线
        delay_10us(1);    // 等待10us
        dat >>= 1;
        if(DS18B20_PORT) dat |= 0x80; // 读数据
        delay_10us(6);    // 延时60us
    }
    return dat;
}

// 获取温度
float ds18b20_read_temperture(void) {
    u8 dath = 0;
    u8 datl = 0;
    u16 value = 0;
    
    ds18b20_reset(); // 复位
    ds18b20_check();
    ds18b20_write_byte(0xcc); // SKIP ROM
    ds18b20_write_byte(0x44); // 转换命令
    
    delay_10us(75000); // 等待转换完成(750ms)
    
    ds18b20_reset(); // 复位
    ds18b20_check();
    ds18b20_write_byte(0xcc); // SKIP ROM
    ds18b20_write_byte(0xbe); // 读存储器
    datl = ds18b20_read_byte(); // 低字节
    dath = ds18b20_read_byte(); // 高字节
    value = (dath << 8) + datl; // 合并为16位数据
    
    if((value & 0xf800) == 0xf800) { // 负温度
        value = (~value) + 1; // 取反加1
        return value * (-0.0625); // 乘以精度
    }
    else { // 正温度
        return value * 0.0625;
    }
}

// LED点阵显示
void led_matrix_display(u8 *pattern) {
    u8 i;
    u8 row_data = 0x01;
    
    for(i = 0; i < 8; i++) {
        // 设置行
        LSA = (row_data & 0x01) ? 1 : 0;
        LSB = (row_data & 0x02) ? 1 : 0;
        LSC = (row_data & 0x04) ? 1 : 0;
        
        // 设置列数据
        SMG_A_DP_PORT = ~pattern[i]; // 取反，因为共阳
        
        delay_10us(30); // 每行显示300us
        
        row_data <<= 1; // 移到下一行
    }
}

// 按键扫描
u8 key_scan(void) {
    u8 key_val = 0;
    
    if(KEY1 == 0) key_val = 1;
    else if(KEY2 == 0) key_val = 2;
    else if(KEY3 == 0) key_val = 3;
    else if(KEY4 == 0) key_val = 4;
    
    if(key_val != 0) {
        delay_ms(10); // 消抖
        switch(key_val) {
            case 1: if(KEY1 == 0) return 1; break;
            case 2: if(KEY2 == 0) return 2; break;
            case 3: if(KEY3 == 0) return 3; break;
            case 4: if(KEY4 == 0) return 4; break;
        }
    }
    return 0;
}

// 蜂鸣器报警
void beep_alarm(u8 times) {
    u8 i;
    for(i = 0; i < times; i++) {
        BEEP = 1;
        delay_ms(100);
        BEEP = 0;
        delay_ms(100);
    }
}

// PWM控制LED亮度
void pwm_control(void) {
    static u8 count = 0;
    
    count++;
    if(count < pwm_duty) {
        LED4 = 1; // LED亮
    } else {
        LED4 = 0; // LED灭
    }
    
    if(count >= 100) count = 0; // 周期100
}

// 定时器0初始化 - 10ms定时
void timer0_init(void) {
    TMOD &= 0xF0; // 清除T0配置
    TMOD |= 0x01; // 16位定时器模式
    TH0 = 0xD8;   // 10ms重载值(11.0592MHz)
    TL0 = 0xF0;
    ET0 = 1;      // 使能T0中断
    TR0 = 1;      // 启动T0
}

// 定时器1初始化 - 1ms定时，用于PWM
void timer1_init(void) {
    TMOD &= 0x0F; // 清除T1配置
    TMOD |= 0x10; // 16位定时器模式
    TH1 = 0xFC;   // 1ms重载值(11.0592MHz)
    TL1 = 0x66;
    ET1 = 1;      // 使能T1中断
    TR1 = 1;      // 启动T1
}

// 外部中断0初始化
void exti0_init(void) {
    IT0 = 1;      // 下降沿触发
    EX0 = 1;      // 使能外部中断0
    EA = 1;       // 使能全局中断
}

// 定时器0中断服务函数 - 10ms
void timer0_isr(void) __interrupt(1){
    TH0 = 0xD8;   // 重载初值
    TL0 = 0xF0;
    
    second_count++;
    if(second_count >= 100) { // 100 * 10ms = 1s
        second_count = 0;
        second++;
        if(second >= 60) {
            second = 0;
            minute++;
            if(minute >= 60) {
                minute = 0;
                hour++;
                if(hour >= 24) hour = 0;
            }
        }
    }
    
    // 温度报警检查
    if(temp_value > temp_alarm * 10 && !alarm_flag) {
        alarm_flag = 1;
        BEEP = 1; // 持续报警
        LED1 = 1; // LED闪烁
    } else if(temp_value <= temp_alarm * 10) {
        alarm_flag = 0;
        BEEP = 0;
        LED1 = 0;
    }
}

// 定时器1中断服务函数 - 1ms
void timer1_isr(void) __interrupt( 3 ){
    TH1 = 0xFC;   // 重载初值
    TL1 = 0x66;
    
    pwm_control(); // PWM控制
    
    // LED2闪烁指示系统运行
    static u8 led2_count = 0;
    led2_count++;
    if(led2_count >= 200) { // 200ms
        led2_count = 0;
        LED2 = !LED2;
    }
}

// 外部中断0服务函数 - 紧急模式
void exti0_isr(void) __interrupt (0 ){
    // 切换到紧急报警模式
    display_mode = 2; // 设置模式
    setting_item = 2; // 报警阈值
    beep_alarm(3);    // 3次蜂鸣
}

// 显示温度
void display_temperature(void) {
    u8 temp_buf[5];
    int display_temp = temp_value;
    
    if(display_temp < 0) { // 负温度
        display_temp = -display_temp;
        temp_buf[0] = 0x40; // 负号
    } else {
        temp_buf[0] = 0x00; // 不显示
    }
    
    temp_buf[1] = gsmg_code[display_temp / 1000]; // 百位
    temp_buf[2] = gsmg_code[display_temp % 1000 / 100]; // 十位
    temp_buf[3] = gsmg_code[display_temp % 1000 % 100 / 10] | 0x80; // 个位+小数点
    temp_buf[4] = gsmg_code[display_temp % 10]; // 小数点后一位
    
    smg_display(temp_buf, 4);
    
    // 根据温度选择LED图案
    if(temp_value > 280) led_pattern = 1; // 热
    else if(temp_value < 200) led_pattern = 0; // 冷
    else led_pattern = 2; // 适中
}

// 显示时钟
void display_clock(void) {
    u8 clock_buf[6];
    
    clock_buf[0] = gsmg_code[hour / 10]; // 小时十位
    clock_buf[1] = gsmg_code[hour % 10]; // 小时个位
    clock_buf[2] = gsmg_code[10]; // 显示"-"作为分隔符
    clock_buf[3] = gsmg_code[minute / 10]; // 分钟十位
    clock_buf[4] = gsmg_code[minute % 10]; // 分钟个位
    
    smg_display(clock_buf, 3);
    
    // LED3表示秒
    if(second % 2 == 0) LED3 = 1;
    else LED3 = 0;
}

// 显示设置
void display_setting(void) {
    u8 set_buf[4] = {0};
    
    if(setting_item == 0) { // 设置小时
        set_buf[0] = 0x76; // 显示"H"
        set_buf[1] = gsmg_code[hour / 10];
        set_buf[2] = gsmg_code[hour % 10];
    } 
    else if(setting_item == 1) { // 设置分钟
        set_buf[0] = 0x79; // 显示"n"
        set_buf[1] = gsmg_code[minute / 10];
        set_buf[2] = gsmg_code[minute % 10];
    }
    else if(setting_item == 2) { // 设置报警阈值
        set_buf[0] = 0x71; // 显示"A"
        set_buf[1] = gsmg_code[temp_alarm / 10];
        set_buf[2] = gsmg_code[temp_alarm % 10];
    }
    
    smg_display(set_buf, 4);
}

void main() {
    u8 key_val = 0;
    u8 temp_count = 0;
    
    // 初始化
    BEEP = 0;
    LED1 = LED2 = LED3 = LED4 = 0;
    
    // 初始化外设
    ds18b20_reset();
    timer0_init();  // 10ms定时器
    timer1_init();  // 1ms定时器
    exti0_init();   // 外部中断0
    
    beep_alarm(1);  // 开机提示音
    
    while(1) {
        // 每2秒读取一次温度
        if(++temp_count >= 200) { // 200 * 10ms = 2s
            temp_count = 0;
            temp_value = (int)(ds18b20_read_temperture() * 10); // 保留一位小数
        }
        
        // 按键扫描
        key_val = key_scan();
        if(key_val != key_pressed) {
            key_pressed = key_val;
            
            if(key_val != 0) {
                beep_alarm(1); // 按键提示音
            }
            
            switch(display_mode) {
                case 0: // 温度模式
                    if(key_val == 1) display_mode = 1; // 切换到时钟
                    else if(key_val == 2) { // 进入设置模式
                        display_mode = 2;
                        setting_item = 2; // 默认设置报警阈值
                    }
                    break;
                    
                case 1: // 时钟模式
                    if(key_val == 1) display_mode = 0; // 切换到温度
                    else if(key_val == 2) { // 进入设置模式
                        display_mode = 2;
                        setting_item = 0; // 默认设置小时
                    }
                    break;
                    
                case 2: // 设置模式
                    if(key_val == 4) display_mode = 0; // 退出设置
                    else if(key_val == 1) { // 切换设置项
                        setting_item++;
                        if(setting_item > 2) setting_item = 0;
                    }
                    else if(key_val == 2) { // 增加数值
                        if(setting_item == 0) {
                            hour++;
                            if(hour >= 24) hour = 0;
                        }
                        else if(setting_item == 1) {
                            minute++;
                            if(minute >= 60) minute = 0;
                        }
                        else if(setting_item == 2) {
                            temp_alarm++;
                            if(temp_alarm > 50) temp_alarm = 10; // 限制在50℃
                        }
                    }
                    else if(key_val == 3) { // 减少数值
                        if(setting_item == 0) {
                            if(hour == 0) hour = 23;
                            else hour--;
                        }
                        else if(setting_item == 1) {
                            if(minute == 0) minute = 59;
                            else minute--;
                        }
                        else if(setting_item == 2) {
                            if(temp_alarm <= 10) temp_alarm = 50;
                            else temp_alarm--;
                        }
                    }
                    break;
            }
        }
        
        // LED点阵显示
        if(led_pattern == 0) led_matrix_display((u8*)led_ice);
        else if(led_pattern == 1) led_matrix_display((u8*)led_fire);
        else led_matrix_display((u8*)led_sun);
        
        // 根据模式显示不同内容
        switch(display_mode) {
            case 0: display_temperature(); break;
            case 1: display_clock(); break;
            case 2: display_setting(); break;
        }
        
        // PWM占空比随温度变化
        if(temp_value > 300) pwm_duty = 90; // 高温
        else if(temp_value > 250) pwm_duty = 60; // 中温
        else pwm_duty = 30; // 低温
    }
}