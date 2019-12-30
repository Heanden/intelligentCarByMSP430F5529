#include "stm8s.h"
#include "stm8s_clk.h"
#include "stm8s_gpio.h"
#include "LED.h"
#include "delay.h"
#include "Timer1.h"
#include "Timer2.h"
#include "HC_SR04.h"
#include "UART1.h"

extern u16 Ultrasonic_Count;        //计数ECHO高电平持续时间

void main(void)
{
   
   CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);       //设置内部16M高速时钟为主时钟       
  
   Led_Init();         //数码管初始化
   Timer1_Init();      //定时器1初始化，定时器1用于数码管显示
   Timer2_Init();      //定时器2初始化，定时器2用于计算ECHO高电平持续时间
   HC_SR04_Init();     //HC-SR04初始化
   Uart1_Init();       //串口初始化
       
   while(1)
   {
    while(!ECHO);          //当ECHO为低电平时等待
    TIM2_Cmd(ENABLE);      //ECHO变高后开启定时器计数
    while(ECHO);           //等待ECHO变低
    TIM2_Cmd(DISABLE);     //ECHO变低后停止定时器计数
    Distance_Count();      //计算距离
    Uart_Send();           //串口发送距离数据
    Ultrasonic_Count=0;    //清零，等待下一测距周期到来
    Delayms(500);          //延时500ms
   }

}



