#include "delay.h"
#include "stm8s_clk.h"

void Delayus(void)
{
asm("nop"); //һ��asm("nop")��������ʾ�������Դ���100ns������˵���������ʵ��2.35us��
asm("nop");
asm("nop");
asm("nop");
}

void Delayus_Trig(void)       //��ʱԼ34uS,���ڴ���������ģ��
{
  u8 i;
  for(i=0;i<100;i++)   asm("nop");
}

void Delayms(unsigned int time)
{
unsigned int i;
while(time--)
for(i=884;i>0;i--)  //�Ƽ�ֵi=900,�޸�Ϊ884��׼ȷ�о�
Delayus();
}