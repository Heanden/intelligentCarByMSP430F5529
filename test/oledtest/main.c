#include "msp430f5529.h"
#include "include.h"
#include "bmp.h"
#include "LQ12864.h"
unsigned int k = 0;
unsigned int LastCCR1; //上一次高电平持续的时间，和距离正相关
unsigned int dat[20];  //通过数组备份采集到的数据
unsigned char fnum[7];
float fdis;
unsigned int idis;
void HC_SR04Init();
void HC_SR04Start();
void TimeGetInit();

/*初始化HC_SR04Init()模块*/
void HC_SR04Init()
{
    P1DIR |= BIT2;
}
/*开始超声发送*/
void HC_SR04Start()
{
    P1OUT |= BIT2;
    _delay_cycles(12); //至少持续10us的高电平
    P1OUT &= ~BIT2;
}

/*初始化捕获模式，获取时间*/
void TimeGetInit()
{
    P1DIR &= ~BIT3;
    P1SEL |= BIT3;

    TA0CTL = TASSEL_2 + ID_0 + MC_2 + TACLR;
    TA0CCTL2 = CM_1 + SCS + CAP + CCIE + CCIS_0;
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
        LastCCR1 = TA0CCR2; //记录下上次CCR2的值
        TA0CCTL2 &= ~CM_1;  //清上升沿捕获
        TA0CCTL2 |= CM_2;   //改为下降沿捕获
        times++;
    }
    if (times == 0)
    {
        if (i < 20) //把采集到的20次的值都放到数组中
        {
            dat[i] = TA0CCR2 - LastCCR1;
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
            for (i = 7; i < 12; i++)
            {
                t = t + dat[i];
            }
            k = t / 5;
            i = 0;
        }
        TA0CCTL2 &= ~CM_2; //清除下降沿触发
        TA0CCTL2 |= CM_1;  //变为上升沿触发
        times++;           //改变times的值
    }

    if (k > 2000)
    {
        fdis = (float)k * 0.01652 - 29.1403;
    }
    else
    {
        fdis = (float)k * 0.01795 + 25.3464;
    }
    idis = (int)(fdis*100);
    fnum[0] = idis/1000+48;
    fnum[1] = idis%1000/100+48;
    fnum[2] = '.';
    fnum[3] = idis%1000%100/10+48;
    fnum[4] = idis%1000%100%10+48;
    fnum[5] = 'c';
    fnum[6] = 'm';
    LCD_P8x16Str(1, 1, fnum);
    times &= 0x01;      //times>1时清0
    LPM0_EXIT;          //退出低功耗模式
    TA0CCTL2 &= ~CCIFG; //清除中断响应标志
    _EINT();            //开中断
}
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; //关闭看门狗
    LCD_Init();               //初始化
    LCD_P14x16Str(0,6, "陈飞鸢林钧铭");
}
