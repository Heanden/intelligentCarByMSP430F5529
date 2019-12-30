#include "delay.h"
#include "stm8s_clk.h"

void Delayus(void)
{
asm("nop"); //一个asm("nop")函数经过示波器测试代表100ns（网上说法，我这边实测2.35us）
asm("nop");
asm("nop");
asm("nop");
}

void Delayus_Trig(void)       //延时约34uS,用于触发超声波模块
{
  u8 i;
  for(i=0;i<100;i++)   asm("nop");
}

void Delayms(unsigned int time)
{
unsigned int i;
while(time--)
for(i=884;i>0;i--)  //推荐值i=900,修改为884更准确感觉
Delayus();
}