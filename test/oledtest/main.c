#include "msp430f5529.h"
#include "include.h"

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; //�رտ��Ź�
    LCD_Init();               //��ʼ��
    Draw_LQLogo();
    while (1)
    {
    }
}
