#include "stm8s_gpio.h"
#include "delay.h"
#include "HC_SR04.h"
#include "Timer2.h"

extern u16 Ultrasonic_Count;      //����ECHO�ߵ�ƽ����ʱ��
u16 Distance;                    //������

/***�������������ӣ� ECHO--PC3,TRIG--PC4  ***/

/********ECHO��TRIG����I/O��ʼ��******/

void HC_SR04_Init(void)                      
{  
   GPIO_Init(GPIOC,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);
   GPIO_Init(GPIOC,GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT);  
}

/***�����źţ���������HC-SR04***/

void Trig(void)
{
  GPIO_WriteLow(GPIOC,GPIO_PIN_4);       //������TRIG��
  Delayus();                             //��ʱԼ8us
  GPIO_WriteHigh(GPIOC,GPIO_PIN_4);      //�����ߣ��Դ���HC-SR04
  Delayus_Trig();                        //��ʱԼ34uS(����Ҫ��ʱ10us)
  GPIO_WriteLow(GPIOC,GPIO_PIN_4);       //������
}      

/*****�������*****/

void Distance_Count(void)
{
  float Distance_temp;
  Distance_temp=Ultrasonic_Count*0.34*1.03;    //�����������ٶȣ�340m/s=34000cm/s=0.034cm/us, Distance_temp=(Ultrasonic_Count*0.034*20/2)*1.03,���г�1.03��Ϊ���������ӽ�ȥ��
  Distance=Distance_temp;                      //ȡ��Distance_temp����������
}
  




