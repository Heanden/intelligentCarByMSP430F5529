/*
 * @Date: 2020-01-05 19:54:25
 * @LastEditors  : Gavin Chen
 * @Github: https://github.com/Heanden
 * @课程所用程序: https://eiet.xyz
 * @个人项目: https://eiet.xyz/ITEM/
 * @LastEditTime : 2020-01-09 20:59:20
 */

#include "bmp.h"
#include <msp430f5529.h>
#include <oppwm.h>
char key = 0;
int i;
#define CPU_CLOCK 1000000 //é»˜è¤ä¸»é¢‘1Mhz
#define delay_ms(x) __delay_cycles((x)*CPU_CLOCK / 1000);
#define delay_us(x) __delay_cycles((x)*CPU_CLOCK / 1000000);
unsigned int x = 0;
unsigned int y = 0;
unsigned char sspeed[5] = "000%";
unsigned char dir[6]="- s -";
#include "include.h"
#include "LQ12864.h"
unsigned int k = 0;
unsigned int LastCCR1; //上一次高电平持续的时间，和距离正相关
unsigned int dat[20];  //通过数组备份采集到的数据
unsigned char fnum[7];
unsigned int idis;
void HC_SR04Init();
void HC_SR04Start();
void TimeGetInit();

/*初始化HC_SR04Init()模块*/
void HC_SR04Init()
{
    P1DIR |= BIT3;
}
/*开始超声发送*/
void HC_SR04Start()
{
    P1OUT |= BIT3;
    __delay_cycles(10); //至少持续10us的高电平
    P1OUT &= ~BIT3;
}

/*初始化捕获模式，获取时间*/
void TimeGetInit()
{
    P1DIR &= ~BIT2;
    P1SEL |= BIT2;

    TA0CTL = TASSEL_2 + ID_0 + MC_2 + TACLR;
    TA0CCTL1 = CM_1 + SCS + CAP + CCIE + CCIS_0;
}

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

    LCD_Init(); //初始化
    TimeGetInit(); //初始化计数捕获引脚，模式等
    HC_SR04Init(); //初始化超声模块HC_SR04

    while (1)
    {
        _EINT();
        LCD_P6x8Str(64, 0, "dir");
        //delay_ms(1);
        LCD_P6x8Str(64, 2, dir);
        //delay_ms(1);
        LCD_P6x8Str(64, 4, "speed");
        //delay_ms(1);
        LCD_P6x8Str(64, 6, sspeed);
        //delay_ms(1);
        HC_SR04Start();
        LPM0;
    }
}
#pragma vector = PORT1_VECTOR //方向
__interrupt void Port_(void)
{
    _DINT();
    key = P1IN & P1IFG;
    delay_ms(2);
    if (key == (P1IN & BIT1))
    {
        LCD_Fill(0x00);
        timer_init();
        switch (x)
        {
        case 1: //up
            x = 2;
            dir[2] = 85;
            break;
        case 2: //down
            x = 3;
            dir[2] = 68;
            break;
        case 3: //r
            x = 4;
            dir[2] = 82;
            break;
        case 4: //l
            x = 0;
            dir[2] = 73;
            break;
        case 0: //s
            x = 1;
            dir[2] = 83;
            break;
        default:
            dir[2] = 83;
            x = 0;
        }
        carctl(x, y);
    }
    _EINT();
    P1IFG &= ~BIT1;
}

#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void)
{
    _DINT();
    key = P2IN & P2IFG;
    delay_ms(2);
    if (key == (P2IN & BIT1))
    {
        LCD_Fill(0x00);
        timer_init();
        switch (y)
        {
        case 1: //20
            sspeed[1] =50;
            y = 2;
            break;
        case 2: //40
            sspeed[1] = 52;
            y = 3;
            break;
        case 3: //60
            y = 4;
            sspeed[1] = 54;
            break;
        case 4: //80
            y = 5;
            sspeed[1] = 56;
            break;
        case 5: //100
            y = 0;
            sspeed[0] =49;
            sspeed[1] =48;
            break;
        case 0: //0
            sspeed[0] =48;
                        sspeed[1] =48;
            y = 1;
            break;
        default: //0
            sspeed[0] =48;
                        sspeed[1] =48;
            y = 0;
        }
        carctl(x, y);
    }
    _EINT();
    P2IFG &= ~BIT1;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
    _DINT(); //关中断
    static unsigned char times = 1;
    static unsigned char i = 0;
    unsigned char j;
    unsigned int t;

    if (times == 1)
    {
        LastCCR1 = TA0CCR1; //记录下上次CCR2的值
        TA0CCTL1 &= ~CM_1;  //清上升沿捕获
        TA0CCTL1 |= CM_2;   //改为下降沿捕获
        times++;
    }
    if (times == 0)
    {
        if (i < 20) //把采集到的20次的值都放到数组中
        {
            dat[i] = TA0CCR1 - LastCCR1;
            ;
            i++;
        }
        if (i >= 20) //为了防止误差，排序把采集到的两头两尾的值去掉，取中间的平均值
        {
            for (i = 0; i < 20; i++)
            {
                for (j = 0; j < 20 - i; j++)
                {
                    if (dat[j] > dat[j + 1])
                    {
                        t = dat[j];
                        dat[j] = dat[j + 1];
                        dat[j + 1] = t;
                    }
                }
            }

            t = 0;
            for (i = 5; i < 15; i++)
            {
                t = t + dat[i];
            }
            k = t / 10;
            i = 0;
        }
        TA0CCTL1 &= ~CM_2; //清除下降沿触发
        TA0CCTL1 |= CM_1;  //变为上升沿触发
        times++;           //改变times的值
    }

    idis = (int)((float)k*0.01*340/2);
    fnum[0] = idis / 1000 + 48;
    fnum[1] = idis % 1000 / 100 + 48;
    fnum[2] = 46;
    fnum[3] = idis % 1000 % 100 / 10 + 48;
    fnum[4] = idis % 1000 % 100 % 10 + 48;
    LCD_P6x8Str(0, 0, fnum);
    times &= 0x01;      //times>1时清0
    LPM0_EXIT;          //退出低功耗模式
    TA0CCTL1 &= ~CCIFG; //清除中断响应标志
    _EINT();            //开中断
}
