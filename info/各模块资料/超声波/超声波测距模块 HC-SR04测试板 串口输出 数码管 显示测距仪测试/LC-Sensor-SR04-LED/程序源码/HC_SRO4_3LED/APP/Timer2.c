#include "stm8s_tim1.h"
#include "Timer2.h"


u16 Ultrasonic_Count;        //����ECHO�ߵ�ƽ����ʱ��

/********��ʱ��2��ʼ��********/

void Timer2_Init(void)
{
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_1,320);         //��ʱ������16MHz�ļ���Ƶ�ʣ�1/16 US�ķֱ��� ,ÿ20us�ж�һ��,��Ϊ������С��λΪ1cm,���Լ����ֱ���Ӧ��С�ڻ����1/0.034=29.4us
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  TIM2_Cmd(DISABLE); 
  __enable_interrupt();
}


/*******��ʱ��2�жϷ������*******/


#pragma vector=0xF                    //TIM2��������жϵ��жϺ�Ϊ0xF

__interrupt void TIM2_UPD_OVF_TRG_BRK_IRQHandler(void)
{
  //if(Ultrasonic_Count==1176)   Ultrasonic_Count=0;    ��Զ�����벻����4m,��: 400*2/0.034/20=1176
  Ultrasonic_Count++;
  TIM2_ClearITPendingBit(TIM2_IT_UPDATE);     //�����־λ�������������ж� 
}