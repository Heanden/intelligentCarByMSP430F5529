#include "stm8s_tim1.h"
#include "Timer2.h"


u16 Ultrasonic_Count;        //计数ECHO高电平持续时间

/********定时器2初始化********/

void Timer2_Init(void)
{
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_1,320);         //定时器设置16MHz的计数频率，1/16 US的分辨率 ,每20us中断一次,因为距离最小单位为1cm,所以计数分辨率应该小于或等于1/0.034=29.4us
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  TIM2_Cmd(DISABLE); 
  __enable_interrupt();
}


/*******定时器2中断服务程序*******/


#pragma vector=0xF                    //TIM2计数溢出中断的中断号为0xF

__interrupt void TIM2_UPD_OVF_TRG_BRK_IRQHandler(void)
{
  //if(Ultrasonic_Count==1176)   Ultrasonic_Count=0;    最远检测距离不超过4m,得: 400*2/0.034/20=1176
  Ultrasonic_Count++;
  TIM2_ClearITPendingBit(TIM2_IT_UPDATE);     //清除标志位，否则不能跳出中断 
}