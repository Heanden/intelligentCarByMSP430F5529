#include "msp430f5529.h"
#include "include.h"

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; //关闭看门狗
    LCD_Init();               //初始化
    Draw_LQLogo();
    while (1)
    {
    }
}
