#include "stm8s_tim1.h"
#include "Timer1.h"
#include "LED.h"
#include "HC_SR04.h"

u8 LED_count=1,Trig_Count=0;
extern u16 Distance;             //������

/********��ʱ��1��ʼ��********/

void Timer1_Init(void)
{
  TIM1_DeInit();
  TIM1_TimeBaseInit(16,TIM1_COUNTERMODE_UP,5000,0);   //16��Ƶ����1us,5ms�ж�һ��
  TIM1_ARRPreloadConfig(ENABLE);                      //TIM1_ARR�Ĵ�����Ԥװ�ػ���������(0x80)
  TIM1_ITConfig(TIM1_IT_UPDATE , ENABLE);             //�ж�ʹ�ܼĴ���(TIM1_IER)����������ж�(0x01)
  TIM1_Cmd(ENABLE);                                   //���ƼĴ���1(TIM1_CR1)��ʹ�ܼ�������0x01��/������ʱ��1
  rim();                                              //�������жϣ�Ҳ����__enable_interrupt();
}


#pragma vector=0xD             //TIM1��������жϵ��жϺ�Ϊ0xD

__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void)              //�����ÿ5msɨһ��
{ 
  Led_Display(LED_count,12);           //�ȹ���ʾ��Ԥ���������Ӱ
  Trig_Count++;
  if(Trig_Count==160)                 //���չٷ�����800ms����һ��
  {
      Trig_Count=0;
      Trig();
  }
  LED_count++;
  if(LED_count==4)     LED_count=1;                     
  switch(LED_count)                                     
  {
      case 1: Led_Display(1,Distance%1000/100);          //��ʾ��λ
              break;
      case 2: Led_Display(2,Distance%1000%100/10);      //��ʾʮλ
              break;
      case 3: Led_Display(3,Distance%1000%10);         //��ʾ��λ
              break;
  }
  
  TIM1_ClearITPendingBit(TIM1_IT_UPDATE);     //�����־λ�������������ж�
}







                                                     