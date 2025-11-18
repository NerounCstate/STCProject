#include "8052.h"

typedef unsigned char u8;
typedef unsigned int u16;

#define DS18B20_PORT P3_7
#define LSA P2_2
#define LSB P2_3
#define LSC P2_4
#define SMG_A_DP_PORT P0

// 全局段码表数组声明（需要定义实际值）
u8 gsmg_code[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f}; // 0~9的段码

void delay_10us(u16 n) {
    while(n--);
}

void smg_display(u8 dat[], u8 pos) {
    u8 i = 0;
    u8 pos_temp = pos - 1;
    for(i = pos_temp; i < 8; i++) {
        switch(i) {  // 位选
            case 0: LSC = 1; LSB = 1; LSA = 1; break;
            case 1: LSC = 1; LSB = 1; LSA = 0; break;
            case 2: LSC = 1; LSB = 0; LSA = 1; break;
            case 3: LSC = 1; LSB = 0; LSA = 0; break;
            case 4: LSC = 0; LSB = 1; LSA = 1; break;
            case 5: LSC = 0; LSB = 1; LSA = 0; break;
            case 6: LSC = 0; LSB = 0; LSA = 1; break;
            case 7: LSC = 0; LSB = 0; LSA = 0; break;
        }
        SMG_A_DP_PORT = dat[i - pos_temp];  // 传送段选数据
        delay_10us(100);                   // 延时一段时间，等待显示稳定
        SMG_A_DP_PORT = 0x00;              // 消音
    }
}

u8 ds18b20_init() {
    ds18b20_reset();
    return ds18b20_check();
}

void ds18b20_reset(void) {
    DS18B20_PORT = 0;   // 拉低 DQ
    delay_10us(75);
    DS18B20_PORT = 1;   // DQ=1
    delay_10us(2);
}

u8 ds18b20_check(void) {
    u8 time_temp = 0;
    while(DS18B20_PORT && time_temp < 20) {  // 等待 DQ 为低电平
        time_temp++;
        delay_10us(1);
    }
    if(time_temp >= 20)
        return 1;  // 如果超时则强制返回 1
    else
        time_temp = 0;
    
    while((!DS18B20_PORT) && time_temp < 20) {  // 等待 DQ 为高电平
        time_temp++;
        delay_10us(1);
    }
    if(time_temp >= 20)
        return 1;  // 如果超时则强制返回 1
    
    return 0;
}

void ds18b20_write_byte(u8 dat) {
    u8 i, j;
    for(j = 0; j < 8; j++) {
        DS18B20_PORT = 0;         // 拉低 DQ
        DS18B20_PORT = dat & 0x01; // 写入数据
        dat >>= 1;
        delay_10us(8);
        DS18B20_PORT = 1;         // 拉高 DQ
    }
}

u8 ds18b20_read_byte(void) {
    u8 i, j, dat = 0;
    for(i = 0; i < 8; i++) {
        DS18B20_PORT = 0;     // 拉低 DQ
        dat >>= 1;
        DS18B20_PORT = 1;     // 拉高 DQ
        if(DS18B20_PORT)
            dat |= 0x80;
        delay_10us(8);
    }
    return dat;
}

float ds18b20_read_temperture(void) {
    float temp;
    u8 dath = 0;
    u8 datl = 0;
    u16 value = 0;
    
    ds18b20_start();      // 开始转换
    ds18b20_reset();      // 复位
    ds18b20_check();
    ds18b20_write_byte(0xcc);  // SKIP ROM
    ds18b20_write_byte(0xbe);  // 读存储器
    datl = ds18b20_read_byte(); // 低字节
    dath = ds18b20_read_byte(); // 高字节
    value = (dath << 8) + datl; // 合并为 16 位数据
    
    if((value & 0xf800) == 0xf800) {  // 判断符号位，负温度
        value = (~value) + 1;         // 数据取反再加 1
        temp = value * (-0.0625);     // 乘以精度
    } else {
        temp = value * 0.0625;        // 正温度
    }
    
    return temp;
}

void ds18b20_start(void) {
    ds18b20_reset();             // 复位
    ds18b20_check();             // 检查 DS18B20
    ds18b20_write_byte(0xcc);    // SKIP ROM
    ds18b20_write_byte(0x44);    // 转换命令
}

void main() {
    u8 i = 0;
    float temp_value;            // 应该使用 float 类型
    u8 temp_buf[5];
    
    ds18b20_init();              // 初始化 DS18B20
    
    while(1) {
        i++;
        if(i % 50 == 0) {        // 间隔一段时间读取温度值，间隔时间要大于温度传感器转换温度时间
            temp_value = ds18b20_read_temperture() * 10;  // 保留温度值小数后一位
            
            if(temp_value < 0) {  // 负温度
                temp_value = -temp_value;
                temp_buf[0] = 0x40;  // 显示负号
            } else {
                temp_buf[0] = 0x00;  // 不显示
            }
            
            temp_buf[1] = gsmg_code[(int)(temp_value / 1000)];           // 百位
            temp_buf[2] = gsmg_code[(int)(temp_value % 1000 / 100)];     // 十位
            temp_buf[3] = gsmg_code[(int)(temp_value % 1000 % 100 / 10)] | 0x80;  // 个位+小数点
            temp_buf[4] = gsmg_code[(int)(temp_value % 1000 % 100 % 10)];        // 小数点后一位
            
            smg_display(temp_buf, 4);
            
            i = 0;  // 重置计数器
        }
    }
}