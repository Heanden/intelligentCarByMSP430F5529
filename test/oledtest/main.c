#include "msp430f5529.h"
#include "include.h"
#include "bmp.h"
#include "LQ12864.h"
unsigned int k = 0;
unsigned int LastCCR1; //��һ�θߵ�ƽ������ʱ�䣬�;��������
unsigned int dat[20];  //ͨ�����鱸�ݲɼ���������
unsigned char fnum[7];
float fdis;
unsigned int idis;
void HC_SR04Init();
void HC_SR04Start();
void TimeGetInit();

/*��ʼ��HC_SR04Init()ģ��*/
void HC_SR04Init()
{
    P1DIR |= BIT2;
}
/*��ʼ��������*/
void HC_SR04Start()
{
    P1OUT |= BIT2;
    _delay_cycles(12); //���ٳ���10us�ĸߵ�ƽ
    P1OUT &= ~BIT2;
}

/*��ʼ������ģʽ����ȡʱ��*/
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
    _DINT(); //���ж�
    static unsigned char times = 1;
    static unsigned char i = 0;
    unsigned char j;
    unsigned int t;

    if (times == 1)
    {
        LastCCR1 = TA0CCR2; //��¼���ϴ�CCR2��ֵ
        TA0CCTL2 &= ~CM_1;  //�������ز���
        TA0CCTL2 |= CM_2;   //��Ϊ�½��ز���
        times++;
    }
    if (times == 0)
    {
        if (i < 20) //�Ѳɼ�����20�ε�ֵ���ŵ�������
        {
            dat[i] = TA0CCR2 - LastCCR1;
            ;
            i++;
        }
        if (i >= 20) //Ϊ�˷�ֹ������Ѳɼ�������ͷ��β��ֵȥ����ȡ�м��ƽ��ֵ
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
        TA0CCTL2 &= ~CM_2; //����½��ش���
        TA0CCTL2 |= CM_1;  //��Ϊ�����ش���
        times++;           //�ı�times��ֵ
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
    times &= 0x01;      //times>1ʱ��0
    LPM0_EXIT;          //�˳��͹���ģʽ
    TA0CCTL2 &= ~CCIFG; //����ж���Ӧ��־
    _EINT();            //���ж�
}
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; //�رտ��Ź�
    LCD_Init();               //��ʼ��
    LCD_P14x16Str(0,6, "�·���־���");
}
