#include "stm8s_tim1.h"
#include "Timer1.h"
#include "LED.h"
#include "HC_SR04.h"

u8 LED_count=1,Trig_Count=0;
extern u16 Distance;             //检测距离

/********定时器1初始化********/

void Timer1_Init(void)
{
  TIM1_DeInit();
  TIM1_TimeBaseInit(16,TIM1_COUNTERMODE_UP,5000,0);   //16分频，即1us,5ms中断一次
  TIM1_ARRPreloadConfig(ENABLE);                      //TIM1_ARR寄存器由预装载缓冲器缓冲(0x80)
  TIM1_ITConfig(TIM1_IT_UPDATE , ENABLE);             //中断使能寄存器(TIM1_IER)：允许更新中断(0x01)
  TIM1_Cmd(ENABLE);                                   //控制寄存器1(TIM1_CR1)：使能计数器（0x01）/启动定时器1
  rim();                                              //开启总中断，也可用__enable_interrupt();
}


#pragma vector=0xD             //TIM1计数溢出中断的中断号为0xD

__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void)              //数码管每5ms扫一次
{ 
  Led_Display(LED_count,12);           //先关显示，预防数码管重影
  Trig_Count++;
  if(Trig_Count==160)                 //参照官方例程800ms触发一次
  {
      Trig_Count=0;
      Trig();
  }
  LED_count++;
  if(LED_count==4)     LED_count=1;                     
  switch(LED_count)                                     
  {
      case 1: Led_Display(1,Distance%1000/100);          //显示百位
              break;
      case 2: Led_Display(2,Distance%1000%100/10);      //显示十位
              break;
      case 3: Led_Display(3,Distance%1000%10);         //显示个位
              break;
  }
  
  TIM1_ClearITPendingBit(TIM1_IT_UPDATE);     //清除标志位，否则不能跳出中断
}







                                                     