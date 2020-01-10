#include <msp430f5529.h>
unsigned int k = 0;
unsigned int LastCCR1; //��һ�θߵ�ƽ������ʱ�䣬�;��������
unsigned int dat[20];  //ͨ�����鱸�ݲɼ���������

void HC_SR04Init();
void HC_SR04Start();
void TimeGetInit();

void main()
{
    HC_SR04Init(); //��ʼ������ģ��HC_SR04
    TimeGetInit(); //��ʼ�������������ţ�ģʽ��
    _EINT();
    while (1)
    {
        HC_SR04Start();
        LPM0;
    }
}

/*��ʼ��HC_SR04Init()ģ��*/
void HC_SR04Init()
{
    P1DIR |= BIT3;
}
/*��ʼ��������*/
void HC_SR04Start()
{
    P1OUT |= BIT3;
    _delay_cycles(12); //���ٳ���10us�ĸߵ�ƽ
    P1OUT &= ~BIT3;
}

/*��ʼ������ģʽ����ȡʱ��*/
void TimeGetInit()
{
    P1DIR &= ~BIT2;
    P1SEL |= BIT2;

    TA0CTL = TASSEL_2 + ID_0 + MC_2 + TACLR;
    TA0CCTL1 = CM_1 + SCS + CAP + CCIE + CCIS_0;
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
        LastCCR1 = TA0CCR1; //��¼���ϴ�CCR2��ֵ
        TA0CCTL1 &= ~CM_1;  //�������ز���
        TA0CCTL1 |= CM_2;   //��Ϊ�½��ز���
        times++;
    }
    if (times == 0)
    {
        if (i < 20) //�Ѳɼ�����20�ε�ֵ���ŵ�������
        {
            dat[i] = TA0CCR1 - LastCCR1;
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
            for (i = 2; i < 18; i++)
            {
                t = t + dat[i];
            }
            k = t / 16;
            i = 0;
        }
        TA0CCTL1 &= ~CM_2; //����½��ش���
        TA0CCTL1 |= CM_1;  //��Ϊ�����ش���
        times++;           //�ı�times��ֵ
    }
    times &= 0x01;      //times>1ʱ��0
    LPM0_EXIT;          //�˳��͹���ģʽ
    TA0CCTL1 &= ~CCIFG; //����ж���Ӧ��־
    _EINT();            //���ж�
}
