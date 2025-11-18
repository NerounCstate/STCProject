#ifndef __INTRINS_H__
#define __INTRINS_H__

#include <8052.h>

// 为SDCC/PlatformIO环境正确定义bit类型
#ifndef bit
typedef unsigned char bit;
#endif

// 使用正确的函数原型声明
extern void          _nop(void);
#define __nop _nop
extern bit           _testbit_(bit b);
extern unsigned char _cror_(unsigned char val, unsigned char n);
extern unsigned int  _iror_(unsigned int val,  unsigned char n);
extern unsigned long _lror_(unsigned long val, unsigned char n);
extern unsigned char _crol_(unsigned char val, unsigned char n);
extern unsigned int  _irol_(unsigned int val,  unsigned char n);
extern unsigned long _lrol_(unsigned long val, unsigned char n);
extern unsigned char _chkfloat_(float f);
extern void          _push_(unsigned char sfr);
extern void          _pop_(unsigned char sfr);

#endif