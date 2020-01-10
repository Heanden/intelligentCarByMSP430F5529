/*
 * @Date: 2020-01-05 19:54:25
 * @LastEditors  : Gavin Chen
 * @Github: https://github.com/Heanden
 * @课程所用程序: https://eiet.xyz
 * @个人项目: https://eiet.xyz/ITEM/
 * @LastEditTime : 2020-01-07 23:33:03
 */
#include <msp430f5529.h>
#include <oppwm.h>
#define CPU_CLOCK 1000000 //é»˜è®¤ä¸»é¢‘1Mhz
#define delay_ms(x) __delay_cycles((x)*CPU_CLOCK / 1000);
#define delay_us(x) __delay_cycles((x)*CPU_CLOCK / 1000000);
unsigned int x = 1;
unsigned int y = 1;

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    P1OUT &= ~BIT0;
    P1DIR |= BIT0;
    P4OUT &= ~BIT7;
    P4DIR |= BIT7;

    P1REN |= BIT1;
    P1OUT |= BIT1;
    P1IE |= BIT1;
    P1IES |= BIT1;
    P1IFG &= ~BIT1;

    P2REN |= BIT1;
    P2OUT |= BIT1;
    P2IE |= BIT1;
    P2IES |= BIT1;
    P2IFG &= ~BIT1;

    timer_init();
    __bis_SR_register(LPM3_bits + GIE);
}
#pragma vector = PORT1_VECTOR
__interrupt void Port_(void)
{
    switch (x)
    {
    case 1:
        fillup();
        x = 2;
        break;
    case 2:
        filldown();
        x = 3;
        break;
    case 3:
        stop();
        x = 1;
        break;
    default:
        stop();
        x = 1;
    }
    P1IFG &= ~BIT1;
}

#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void)
{
    switch (y)
    {
    case 1:
        turnrctl(50);
        P1OUT &= ~BIT0;
        P4OUT |= BIT7;
        y = 2;
        break;
    case 2:
        rotateL();
        P4OUT &= ~BIT7;
        P1OUT |= BIT0;
        y = 1;
        break;
    default:
        stop();
        y = 1;
    }
    P2IFG &= ~BIT1;
}
