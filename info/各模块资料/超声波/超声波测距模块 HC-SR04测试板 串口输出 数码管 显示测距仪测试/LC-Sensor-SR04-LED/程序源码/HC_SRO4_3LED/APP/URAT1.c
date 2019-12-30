//#include "stm8s_gpio.h"
#include "stm8s_uart1.h"
#include "stm8s_clk.h"
#include "UART1.h"



u8 Uart1_Rx_Buf[60];     //定义接收缓冲数组，用来存放串口接收到的数据，最大可存放60个字节
u8 Uart1_Rx_Sta;        //接收状态标记，接收到0x0D时该变量bit6置1，接收到0x0D（即接收完成）时该变量的 bit7置1，bit0-bit5用来对接收到的非0x0D,0x0A的数据计数,其中0x0D和0x0A分别代表"\r"和"\n"

/*****串口1初始化以及开启串口中断******/

void Uart1_Init(void)          
{
    UART1_DeInit();
    UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1,UART1_PARITY_NO , UART1_SYNCMODE_CLOCK_DISABLE , UART1_MODE_TXRX_ENABLE);   //波特率115200，8位数据位，1位停止位，无奇偶校验，UART1同步时钟关闭，查询方式发送，使能UART1发送和接收
    UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE  );     //开启接收中断                            
    UART1_Cmd(ENABLE );                            //启动UART1
    __enable_interrupt();                          //开启总中断
}

/******发送一个字节******/

void UART1_SendByte(u8 data)
{
  UART1_SendData8((unsigned char)data);                  //调用库函数UART1_SendData8(U8 data)发送一个字节数据
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);     //等待发送完成
}


/*****发送一个字符串*****/

void UART1_SendString(u8 *S)
{

  while(*S)
  {
   UART1_SendByte(*S++);   //一次发送一个字节，直到遇到零字符("\0")
  }
  
}

/***返回一个字符串，即将串口接收到的数据原封不动的返回***/
void UART1_ReturnString(void)
{
   u8 i;
   for(i=0;i<(Uart1_Rx_Sta&0x3f);i++)   //Uart1_Rx_Sta&0x3f的意思是因为变量Uart1_Rx_Sta只有低6位用来计数串口接收到的常规数据（不包括0x0D和0x0A)
     
     UART1_SendByte(Uart1_Rx_Buf[i]);
}



/*****接收一个字节*****/

u8 UART1_ReceiveByte(void)
{
     u8 USART1_RX_BUF; 
     while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);  //读串口状态，等待接收非空，当接收非空即UART1_GetFlagStatus(UART1_FLAG_RXNE)=1时说明可以从串口读取数据了
     USART1_RX_BUF=UART1_ReceiveData8();                     //将读取到的数据赋给变量USART1_RX_BUF
     return  USART1_RX_BUF;                                  //将读取到的数据返回
}


/******中断接收服务函数，规定协议：接收到的字符串必须以0x0D,0x0A结尾*******/

/***接收状态标记Uart1_Rx_Sta，当接收到0x0D时该变量bit6置1，接收到0x0D（即接收完成）时该变量的 bit7置1，bit0-bit5用来对接收到的非0x0D,0x0A的数据计数***/

#pragma vector=0x14

__interrupt void UART1_RX_IRQHandler(void)
{ 
    u8 Res;     //用于存放串口接收到的8位数据
    if(UART1_GetITStatus(UART1_IT_RXNE )!= RESET)    //读取串口状态，接收不为空，即UART1_GetITStatus(UART1_IT_RXNE )=1，说明可以从串口读取接收到的数据了
    {
	Res =UART1_ReceiveData8();                   //将串口接收到的数据赋给变量Res
        
	if(( Uart1_Rx_Sta&0x80)==0)                  //接收未完成，之前未接收到0x0D以及0x0A
	{
	    if( Uart1_Rx_Sta&0x40)                  //之前有收到0xD 
		{
		  if(Res!=0x0a) Uart1_Rx_Sta=0;     //如果这次收到的数据不是0x0A,说明接收出错，将Uart1_Rx_Sta清零，重新接收
		  else  Uart1_Rx_Sta|=0x80;	    //如果这次收到的数据是0x0A,说明接收完成，将Uart1_Rx_Sta的bit7置1
		}
            else                                    //之前没有收到0x0D                             
              {	
                if(Res==0x0d) Uart1_Rx_Sta|=0x40;   //但这次收到了0x0D,将Uart1_Rx_Sta的bit6置1
                
                else                                //这次也没收到0x0D，说明是0x0D和0x0A以外的数据（常规数据）
                  {
                     Uart1_Rx_Buf[Uart1_Rx_Sta&0X3F]=Res ;   //将该1个字节的数据存进Uart1_Rx_Buf数组中（因为Uart1_Rx_Sta的bit7和bit6为标志位，所以需&上0x3F）
                     Uart1_Rx_Sta++;                         //每接收到一个常规数据Uart1_Rx_Sta加1
                     if( Uart1_Rx_Sta>59) Uart1_Rx_Sta=0;   //如果超出最大接收范围，将Uart1_Rx_Sta清零，重新接收
                  }		 
	      }
	}  		
    }
}


