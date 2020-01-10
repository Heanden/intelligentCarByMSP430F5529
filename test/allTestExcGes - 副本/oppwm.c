#include "oppwm.h"
#include "msp430f5529.h"
#define CPU_CLOCK 1000000 //默认主频1Mhz
#define delay_ms(x) __delay_cycles((x)*CPU_CLOCK / 1000);
#define delay_us(x) __delay_cycles((x)*CPU_CLOCK / 1000000);

void timer_init(void)
{
    P1DIR |= BIT4 + BIT5;
    P1SEL |= BIT4 + BIT5;

    P2DIR |= BIT4 + BIT5;
    P2SEL |= BIT4 + BIT5;

    TA0CTL = TASSEL_2 + MC_1 + TACLR;
    TA2CTL = TASSEL_2 + MC_1 + TACLR;
}

void TA2pwm_ctl(unsigned int R2,unsigned int R1)//<512
{
    TA2CCR0 = 512 - 1;
    TA2CCTL2 = OUTMOD_7;
    TA2CCTL1 = OUTMOD_7;
    TA2CCR2 = R2;//AIN1（P2.5）TA2.2输出R2/512占空比的pwm
    TA2CCR1 = R1;//AIN2（P2.4）TA2.1输出R1/512占空比的pwm
}

void TA0pwm_ctl(unsigned int R4,unsigned int R3)//<512
{
    TA0CCTL4 = OUTMOD_7;
    TA0CCTL3 = OUTMOD_7;
    TA0CCR0 = 512 - 1;
    TA0CCR4 = R4;//BIN1（P1.5）TA0.4输出R2/512占空比的pwm
    TA0CCR3 = R3;//BIN2（P1.4）TA0.3输出R1/512占空比的pwm
}

void fillup(void)
{
    TA0pwm_ctl(0,511);
    TA2pwm_ctl(0,511);
}

void filldown(void)
{
    TA0pwm_ctl(511,0);
    TA2pwm_ctl(511,0);
}

void rotateL(void)
{
    TA0pwm_ctl(511,0);
    TA2pwm_ctl(0,511);
}

void rotateR(void)
{
    TA0pwm_ctl(0,511);
    TA2pwm_ctl(511,0);

}


void upctl(float perc)
{
    perc = 100 - perc;
    perc /= (float)100;
    perc *= (float)512;
    TA0pwm_ctl((int)perc,511);
    TA2pwm_ctl((int)perc,511);
}

void downctl(float perc)
{
    perc = 100 - perc;
    perc /= (float)100;
    perc *= (float)512;
    TA0pwm_ctl(511,(int)perc);
    TA2pwm_ctl(511,(int)perc);
}

void stop(void)
{
    TA0pwm_ctl(0,0);
    TA2pwm_ctl(0,0);
}

void turnrctl(float perc)
{
    perc = 100 - perc;
    perc /= (float)100;
    perc *= (float)512;

    TA0pwm_ctl(256,(int)perc);
    TA2pwm_ctl(0,0);
}

void turnlctl(float perc)
{
    perc = 100 - perc;
    perc /= (float)100;
    perc *= (float)512;

    TA2pwm_ctl(256,(int)perc);
    TA0pwm_ctl(0,0);
}
