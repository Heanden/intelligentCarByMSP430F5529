#include "stm8s_gpio.h"
#include "LED.h"
#include "delay.h"
#include "UART1.h"

/*������������ӣ���ѡ��A--PD3,B--PA3,C--PB4,D--PA2,E--PD4,F--PC7,G--PB5,DP-PD2    λѡ��D1--PA1,D2--PC6,D3--PC5    */

extern u16 Ultrasonic_Count;       //����ECHO�ߵ�ƽ����ʱ��
extern u16 Distance;              //������


/****�����I/O�ڳ�ʼ��****/

void Led_Init(void)
{
   GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_OUT_PP_HIGH_FAST);    //����A����ѡ����ʼ��LED1�Ĺܽŵ�ģʽΪ��Output push-pull, low level, 10MHz
   GPIO_Init(GPIOA,GPIO_PIN_3,GPIO_MODE_OUT_PP_HIGH_FAST);    //����B
   GPIO_Init(GPIOB,GPIO_PIN_4,GPIO_MODE_OUT_PP_HIGH_FAST);    //����C
   GPIO_Init(GPIOA,GPIO_PIN_2,GPIO_MODE_OUT_PP_HIGH_FAST);    //����D
   GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_OUT_PP_HIGH_FAST);    //����E
   GPIO_Init(GPIOC,GPIO_PIN_7,GPIO_MODE_OUT_PP_HIGH_FAST);    //����F
   GPIO_Init(GPIOB,GPIO_PIN_5,GPIO_MODE_OUT_PP_HIGH_FAST);    //����G
   GPIO_Init(GPIOD,GPIO_PIN_2,GPIO_MODE_OUT_PP_HIGH_FAST);    //PC6�����DP��������
   
   GPIO_Init(GPIOA,GPIO_PIN_1,GPIO_MODE_OUT_PP_LOW_FAST);   //��һλ��λѡ����ʼ��LED1�Ĺܽŵ�ģʽΪ��Output push-pull, low level, 10MHz           
   GPIO_Init(GPIOC,GPIO_PIN_6,GPIO_MODE_OUT_PP_LOW_FAST);   //�ڶ�λ
   GPIO_Init(GPIOC,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_FAST);   //����λ

}



/*****�������ʾ�ӳ���*****/

void Led_Display(u16 Dual,u16 Seg)
{
    if(Dual==1)                                              //����Ӧλѡ
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

  
  
  switch(Seg)                                         //���ö�ѡ 
   {
     
   case 0: GPIO_WriteLow(GPIOD,GPIO_PIN_3);          //��ʾ��0���������Դ�����
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
           
  case 10: GPIO_WriteLow(GPIOD,GPIO_PIN_3);                //��ʾ��P��
           GPIO_WriteLow(GPIOA,GPIO_PIN_3);
           GPIO_WriteHigh(GPIOB,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOA,GPIO_PIN_2);
           GPIO_WriteLow(GPIOD,GPIO_PIN_4);
           GPIO_WriteLow(GPIOC,GPIO_PIN_7);
           GPIO_WriteLow(GPIOB,GPIO_PIN_5);
           break;
           
  case 11: GPIO_WriteHigh(GPIOD,GPIO_PIN_3);               //��ʾ��-��
           GPIO_WriteHigh(GPIOA,GPIO_PIN_3);
           GPIO_WriteHigh(GPIOB,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOA,GPIO_PIN_2);
           GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOC,GPIO_PIN_7);
           GPIO_WriteLow(GPIOB,GPIO_PIN_5);
           break;
           
   default:GPIO_WriteHigh(GPIOD,GPIO_PIN_3);               //Ĭ�������Ϊ����ʾ
           GPIO_WriteHigh(GPIOA,GPIO_PIN_3);
           GPIO_WriteHigh(GPIOB,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOA,GPIO_PIN_2);
           GPIO_WriteHigh(GPIOD,GPIO_PIN_4);
           GPIO_WriteHigh(GPIOC,GPIO_PIN_7);
           GPIO_WriteHigh(GPIOB,GPIO_PIN_5);
           break;
   }
   
 
}


/********�������ʾ********/     


/*void Led_Display_All(void)            //��������ö�ʱ��1�ж���ʾ����ע�͵�
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


/********������ʾ********/

void Uart_Send(void)
{  
  
        UART1_SendString("D:");        
        UART1_SendByte(Distance/100+0x30);           
        UART1_SendByte(Distance/10%10+0x30);
        UART1_SendByte(Distance%10+0x30);
        UART1_SendString("\r\n");       
}
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
   
      
        
    
