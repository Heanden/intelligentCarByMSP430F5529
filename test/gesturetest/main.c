#include <msp430.h>
#include "delay.h"
#include "iic.h"
#include "paj7620u2.h"

void paj7620u2_selectBank(bank_e bank)
{
	switch (bank)
	{
	case BANK0:
		iic_writebyte(PAJ_REGITER_BANK_SEL, 4, PAJ_BANK0);
		break; //BANK0寄存器区域
	case BANK1:
		iic_writebyte(PAJ_REGITER_BANK_SEL, , 4, PAJ_BANK1);
		break; //BANK1寄存器区域
	}
}
u8 paj7620u2_wakeup(void)
{
	u8 data = 0x0a;
	GS_WakeUp();				 //唤醒PAJ7620U2
	delay_ms(5);				 //唤醒时间>400us
	GS_WakeUp();				 //唤醒PAJ7620U2
	delay_ms(5);				 //唤醒时间>400us
	paj7620u2_selectBank(BANK0); //进入BANK0寄存器区域
	data = GS_Read_Byte(0x00);   //读取状态
	if (data != 0x20)
		return 0; //唤醒失败

	return 1;
}
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
	iic_init(0xE6);
}
