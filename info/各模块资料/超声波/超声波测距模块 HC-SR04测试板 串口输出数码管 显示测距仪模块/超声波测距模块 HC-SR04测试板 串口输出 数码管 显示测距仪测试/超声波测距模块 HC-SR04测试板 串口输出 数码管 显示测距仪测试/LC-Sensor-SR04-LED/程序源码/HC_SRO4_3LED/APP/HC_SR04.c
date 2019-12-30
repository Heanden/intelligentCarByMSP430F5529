#include "stm8s_gpio.h"
#include "delay.h"
#include "HC_SR04.h"
#include "Timer2.h"

extern u16 Ultrasonic_Count;      //计数ECHO高电平持续时间
u16 Distance;                    //检测距离

/***超声波引脚连接： ECHO--PC3,TRIG--PC4  ***/

/********ECHO和TRIG引脚I/O初始化******/

void HC_SR04_Init(void)                      
{  
   GPIO_Init(GPIOC,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_FAST);
   GPIO_Init(GPIOC,GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT);  
}

/***触发信号，用于启动HC-SR04***/

void Trig(void)
{
  GPIO_WriteLow(GPIOC,GPIO_PIN_4);       //先拉低TRIG脚
  Delayus();                             //延时约8us
  GPIO_WriteHigh(GPIOC,GPIO_PIN_4);      //再拉高，以触发HC-SR04
  Delayus_Trig();                        //延时约34uS(至少要延时10us)
  GPIO_WriteLow(GPIOC,GPIO_PIN_4);       //再拉低
}      

/*****计算距离*****/

void Distance_Count(void)
{
  float Distance_temp;
  Distance_temp=Ultrasonic_Count*0.34*1.03;    //超声波传播速度：340m/s=34000cm/s=0.034cm/us, Distance_temp=(Ultrasonic_Count*0.034*20/2)*1.03,其中乘1.03是为了修正误差加进去的
  Distance=Distance_temp;                      //取得Distance_temp的整数部分
}
  




