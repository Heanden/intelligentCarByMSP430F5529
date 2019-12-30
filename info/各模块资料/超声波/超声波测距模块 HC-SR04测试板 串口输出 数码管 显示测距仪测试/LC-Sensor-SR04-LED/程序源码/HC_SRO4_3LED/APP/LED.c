#include "stm8s_gpio.h"
#include "LED.h"
#include "delay.h"
#include "UART1.h"

/*数码管引脚连接：段选：A--PD3,B--PA3,C--PB4,D--PA2,E--PD4,F--PC7,G--PB5,DP-PD2    位选：D1--PA1,D2--PC6,D3--PC5    */

extern u16 Ultrasonic_Count;       //计数ECHO高电平持续时间
extern u16 Distance;              //检测距离


/****数码管I/O口初始化****/

void Led_Init(void)
{
   GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_OUT_PP_HIGH_FAST);    //段码A，段选，初始化LED1的管脚的模式为：Output push-pull, low level, 10MHz
   GPIO_Init(GPIOA,GPIO_PIN_3,GPIO_MODE_OUT_PP_HIGH_FAST);    //段码B
   GPIO_Init(GPIOB,GPIO_PIN_4,GPIO_MODE_OUT_PP_HIGH_FAST);    //段码C
   GPIO_Init(GPIOA,GPIO_PIN_2,GPIO_MODE_OUT_PP_HIGH_FAST);    //段码D
   GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_OUT_PP_HIGH_FAST);    //段码E
   GPIO_Init(GPIOC,GPIO_PIN_7,GPIO_MODE_OUT_PP_HIGH_FAST);    //段码F
   GPIO_Init(GPIOB,GPIO_PIN_5,GPIO_MODE_OUT_PP_HIGH_FAST);    //段码G
   GPIO_Init(GPIOD,GPIO_PIN_2,GPIO_MODE_OUT_PP_HIGH_FAST);    //PC6数码管DP控制引脚
   
   GPIO_Init(GPIOA,GPIO_PIN_1,GPIO_MODE_OUT_PP_LOW_FAST);   //第一位，位选，初始化LED1的管脚的模式为：Output push-pull, low level, 10MHz           
   GPIO_Init(GPIOC,GPIO_PIN_6,GPIO_MODE_OUT_PP_LOW_FAST);   //第二位
   GPIO_Init(GPIOC,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_FAST);   //第三位

}



/*****数码管显示子程序*****/

void Led_Display(u16 Dual,u16 Seg)
{
    if(Dual==1)                                              //打开相应位选
  {
      GPIO_WriteHigh(GPIOA,GPIO_PIN_1);
      GPIO_WriteLow(GPIOC,GPIO_PIN_6); 
      GPIO_WriteLow(GPIOC,GPIO_PIN_5); 
  }
  else if(Dual==2)
  {
      GPIO_WriteLow(GPIOA,GPIO_PIN_1);
      GPIO_WriteHigh(GPIOC,GPIO_PIN_6);
      GPIO_WriteLow(GPIOC,GPIO_PIN_5);
  }
  else if(Dual==3)
  {
     GPIO_WriteLow(GPIOA,GPIO_PIN_1);
     GPIO_WriteLow(GPIOC,GPIO_PIN_6);
     GPIO_WriteHigh(GPIOC,GPIO_PIN_5);
    
  }

  
  
  switch(Seg)                                         //设置段选 
   {
     
   case 0: GPIO_WriteLow(GPIOD,GPIO_PIN_3);          //显示“0”，下面以此类推
           GPIO_WriteLow(GPIOA,GPIO_PIN_3);
           GPIO_WriteLow(GPIOB,GPIO_PIN_4);
           GPIO_WriteLow(GPIOA,GPIO_PIN_2);
           GPIO_WriteLow(GPIOD,GPIO_PIN_4);
           GPIO_WriteLow(GPIOC,GPIO_PIN_7);
           GPIO_WriteHigh(GPIOB,GPIO_PIN_5);
           break;  
           
   case 1: GPIO_WriteHigh(GPIOD,GPIO_PIN_3);
           GPIO_WriteLow(GPIOA,GPIO_PIN_3);
           GPIO_WriteLow(GPIOB,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOA,GPIO_PIN_2);
           GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOC,GPIO_PIN_7);
           GPIO_WriteHigh(GPIOB,GPIO_PIN_5);
           break;
           
   case 2: GPIO_WriteLow(GPIOD,GPIO_PIN_3);
           GPIO_WriteLow(GPIOA,GPIO_PIN_3);
           GPIO_WriteHigh(GPIOB,GPIO_PIN_4);
           GPIO_WriteLow(GPIOA,GPIO_PIN_2);
           GPIO_WriteLow(GPIOD,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOC,GPIO_PIN_7);
           GPIO_WriteLow(GPIOB,GPIO_PIN_5);
           break;
           
   case 3: GPIO_WriteLow(GPIOD,GPIO_PIN_3);
           GPIO_WriteLow(GPIOA,GPIO_PIN_3);
           GPIO_WriteLow(GPIOB,GPIO_PIN_4);
           GPIO_WriteLow(GPIOA,GPIO_PIN_2);
           GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOC,GPIO_PIN_7);
           GPIO_WriteLow(GPIOB,GPIO_PIN_5);
           break;
           
   case 4: GPIO_WriteHigh(GPIOD,GPIO_PIN_3);
           GPIO_WriteLow(GPIOA,GPIO_PIN_3);
           GPIO_WriteLow(GPIOB,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOA,GPIO_PIN_2);
           GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
           GPIO_WriteLow(GPIOC,GPIO_PIN_7);
           GPIO_WriteLow(GPIOB,GPIO_PIN_5);
           break;
           
   case 5: GPIO_WriteLow(GPIOD,GPIO_PIN_3);
           GPIO_WriteHigh(GPIOA,GPIO_PIN_3);
           GPIO_WriteLow(GPIOB,GPIO_PIN_4);
           GPIO_WriteLow(GPIOA,GPIO_PIN_2);
           GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
           GPIO_WriteLow(GPIOC,GPIO_PIN_7);
           GPIO_WriteLow(GPIOB,GPIO_PIN_5);
           break;
           
   case 6: GPIO_WriteLow(GPIOD,GPIO_PIN_3);
           GPIO_WriteHigh(GPIOA,GPIO_PIN_3);
           GPIO_WriteLow(GPIOB,GPIO_PIN_4);
           GPIO_WriteLow(GPIOA,GPIO_PIN_2);
           GPIO_WriteLow(GPIOD,GPIO_PIN_4);
           GPIO_WriteLow(GPIOC,GPIO_PIN_7);
           GPIO_WriteLow(GPIOB,GPIO_PIN_5);
           break;
           
   case 7: GPIO_WriteLow(GPIOD,GPIO_PIN_3);
           GPIO_WriteLow(GPIOA,GPIO_PIN_3);
           GPIO_WriteLow(GPIOB,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOA,GPIO_PIN_2);
           GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOC,GPIO_PIN_7);
           GPIO_WriteHigh(GPIOB,GPIO_PIN_5);
           break;
           
   case 8: GPIO_WriteLow(GPIOD,GPIO_PIN_3);
           GPIO_WriteLow(GPIOA,GPIO_PIN_3);
           GPIO_WriteLow(GPIOB,GPIO_PIN_4);
           GPIO_WriteLow(GPIOA,GPIO_PIN_2);
           GPIO_WriteLow(GPIOD,GPIO_PIN_4);
           GPIO_WriteLow(GPIOC,GPIO_PIN_7);
           GPIO_WriteLow(GPIOB,GPIO_PIN_5);
           break;
           
   case 9: GPIO_WriteLow(GPIOD,GPIO_PIN_3);
           GPIO_WriteLow(GPIOA,GPIO_PIN_3);
           GPIO_WriteLow(GPIOB,GPIO_PIN_4);
           GPIO_WriteLow(GPIOA,GPIO_PIN_2);
           GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
           GPIO_WriteLow(GPIOC,GPIO_PIN_7);
           GPIO_WriteLow(GPIOB,GPIO_PIN_5);
           break;
           
  case 10: GPIO_WriteLow(GPIOD,GPIO_PIN_3);                //显示”P“
           GPIO_WriteLow(GPIOA,GPIO_PIN_3);
           GPIO_WriteHigh(GPIOB,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOA,GPIO_PIN_2);
           GPIO_WriteLow(GPIOD,GPIO_PIN_4);
           GPIO_WriteLow(GPIOC,GPIO_PIN_7);
           GPIO_WriteLow(GPIOB,GPIO_PIN_5);
           break;
           
  case 11: GPIO_WriteHigh(GPIOD,GPIO_PIN_3);               //显示”-“
           GPIO_WriteHigh(GPIOA,GPIO_PIN_3);
           GPIO_WriteHigh(GPIOB,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOA,GPIO_PIN_2);
           GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOC,GPIO_PIN_7);
           GPIO_WriteLow(GPIOB,GPIO_PIN_5);
           break;
           
   default:GPIO_WriteHigh(GPIOD,GPIO_PIN_3);               //默认情况下为关显示
           GPIO_WriteHigh(GPIOA,GPIO_PIN_3);
           GPIO_WriteHigh(GPIOB,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOA,GPIO_PIN_2);
           GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOC,GPIO_PIN_7);
           GPIO_WriteHigh(GPIOB,GPIO_PIN_5);
           break;
   }
   
 
}


/********数码管显示********/     


/*void Led_Display_All(void)            //数码管已用定时器1中断显示，故注释掉
{  
      
       Led_Display(1,Distance/100%10);                   
       Delayms(5);
       Led_Display(1,12);
       
       Led_Display(2,Distance/10%10);               
       Delayms(5);
       Led_Display(2,12);

       Led_Display(3,Distance%10);              
       Delayms(5);
       Led_Display(3,12);   
}*/


/********串口显示********/

void Uart_Send(void)
{  
  
        UART1_SendString("D:");        
        UART1_SendByte(Distance/100+0x30);           
        UART1_SendByte(Distance/10%10+0x30);
        UART1_SendByte(Distance%10+0x30);
        UART1_SendString("\r\n");       
}
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
   
      
        
    
