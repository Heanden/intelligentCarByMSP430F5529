#include "stm8s.h"
#include "stm8s_clk.h"
#include "stm8s_gpio.h"
#include "LED.h"
#include "delay.h"
#include "Timer1.h"
#include "Timer2.h"
#include "HC_SR04.h"
#include "UART1.h"

extern u16 Ultrasonic_Count;        //����ECHO�ߵ�ƽ����ʱ��

void main(void)
{
   
   CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);       //�����ڲ�16M����ʱ��Ϊ��ʱ��       
  
   Led_Init();         //����ܳ�ʼ��
   Timer1_Init();      //��ʱ��1��ʼ������ʱ��1�����������ʾ
   Timer2_Init();      //��ʱ��2��ʼ������ʱ��2���ڼ���ECHO�ߵ�ƽ����ʱ��
   HC_SR04_Init();     //HC-SR04��ʼ��
   Uart1_Init();       //���ڳ�ʼ��
       
   while(1)
   {
    while(!ECHO);          //��ECHOΪ�͵�ƽʱ�ȴ�
    TIM2_Cmd(ENABLE);      //ECHO��ߺ�����ʱ������
    while(ECHO);           //�ȴ�ECHO���
    TIM2_Cmd(DISABLE);     //ECHO��ͺ�ֹͣ��ʱ������
    Distance_Count();      //�������
    Uart_Send();           //���ڷ��;�������
    Ultrasonic_Count=0;    //���㣬�ȴ���һ������ڵ���
    Delayms(500);          //��ʱ500ms
   }

}



