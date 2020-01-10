#ifndef __PAJ7620U2_IIC_H
#define __PAJ7620U2_IIC_H

#include "sys.h"

#define GS_SDA_IN()               \
    {                             \
        GPIOB->CRH &= 0XFFFF0FFF; \
        GPIOB->CRH |= 8 << 12;    \
    }
#define GS_SDA_OUT()              \
    {                             \
        GPIOB->CRH &= 0XFFFF0FFF; \
        GPIOB->CRH |= 3 << 12;    \
    }

//IO²Ù×÷º¯Êý
#define GS_IIC_SCL PBout(10) //SCL
#define GS_IIC_SDA PBout(11) //SDA
#define GS_READ_SDA PBin(11) //ÊäÈëSDA

u8 GS_Write_Byte(u8 REG_Address, u8 REG_data);
u8 GS_Read_Byte(u8 REG_Address);
u8 GS_Read_nByte(u8 REG_Address, u16 len, u8 *buf);
void GS_i2c_init(void);
void GS_WakeUp(void);

#endif
