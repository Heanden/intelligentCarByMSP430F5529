//#include "stm8s_gpio.h"
#include "stm8s_uart1.h"
#include "stm8s_clk.h"
#include "UART1.h"



u8 Uart1_Rx_Buf[60];     //������ջ������飬������Ŵ��ڽ��յ������ݣ����ɴ��60���ֽ�
u8 Uart1_Rx_Sta;        //����״̬��ǣ����յ�0x0Dʱ�ñ���bit6��1�����յ�0x0D����������ɣ�ʱ�ñ����� bit7��1��bit0-bit5�����Խ��յ��ķ�0x0D,0x0A�����ݼ���,����0x0D��0x0A�ֱ����"\r"��"\n"

/*****����1��ʼ���Լ����������ж�******/

void Uart1_Init(void)          
{
    UART1_DeInit();
    UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1,UART1_PARITY_NO , UART1_SYNCMODE_CLOCK_DISABLE , UART1_MODE_TXRX_ENABLE);   //������115200��8λ����λ��1λֹͣλ������żУ�飬UART1ͬ��ʱ�ӹرգ���ѯ��ʽ���ͣ�ʹ��UART1���ͺͽ���
    UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE  );     //���������ж�                            
    UART1_Cmd(ENABLE );                            //����UART1
    __enable_interrupt();                          //�������ж�
}

/******����һ���ֽ�******/

void UART1_SendByte(u8 data)
{
  UART1_SendData8((unsigned char)data);                  //���ÿ⺯��UART1_SendData8(U8 data)����һ���ֽ�����
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);     //�ȴ��������
}


/*****����һ���ַ���*****/

void UART1_SendString(u8 *S)
{

  while(*S)
  {
   UART1_SendByte(*S++);   //һ�η���һ���ֽڣ�ֱ���������ַ�("\0")
  }
  
}

/***����һ���ַ������������ڽ��յ�������ԭ�ⲻ���ķ���***/
void UART1_ReturnString(void)
{
   u8 i;
   for(i=0;i<(Uart1_Rx_Sta&0x3f);i++)   //Uart1_Rx_Sta&0x3f����˼����Ϊ����Uart1_Rx_Staֻ�е�6λ�����������ڽ��յ��ĳ������ݣ�������0x0D��0x0A)
     
     UART1_SendByte(Uart1_Rx_Buf[i]);
}



/*****����һ���ֽ�*****/

u8 UART1_ReceiveByte(void)
{
     u8 USART1_RX_BUF; 
     while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);  //������״̬���ȴ����շǿգ������շǿռ�UART1_GetFlagStatus(UART1_FLAG_RXNE)=1ʱ˵�����ԴӴ��ڶ�ȡ������
     USART1_RX_BUF=UART1_ReceiveData8();                     //����ȡ�������ݸ�������USART1_RX_BUF
     return  USART1_RX_BUF;                                  //����ȡ�������ݷ���
}


/******�жϽ��շ��������涨Э�飺���յ����ַ���������0x0D,0x0A��β*******/

/***����״̬���Uart1_Rx_Sta�������յ�0x0Dʱ�ñ���bit6��1�����յ�0x0D����������ɣ�ʱ�ñ����� bit7��1��bit0-bit5�����Խ��յ��ķ�0x0D,0x0A�����ݼ���***/

#pragma vector=0x14

__interrupt void UART1_RX_IRQHandler(void)
{ 
    u8 Res;     //���ڴ�Ŵ��ڽ��յ���8λ����
    if(UART1_GetITStatus(UART1_IT_RXNE )!= RESET)    //��ȡ����״̬�����ղ�Ϊ�գ���UART1_GetITStatus(UART1_IT_RXNE )=1��˵�����ԴӴ��ڶ�ȡ���յ���������
    {
	Res =UART1_ReceiveData8();                   //�����ڽ��յ������ݸ�������Res
        
	if(( Uart1_Rx_Sta&0x80)==0)                  //����δ��ɣ�֮ǰδ���յ�0x0D�Լ�0x0A
	{
	    if( Uart1_Rx_Sta&0x40)                  //֮ǰ���յ�0xD 
		{
		  if(Res!=0x0a) Uart1_Rx_Sta=0;     //�������յ������ݲ���0x0A,˵�����ճ�����Uart1_Rx_Sta���㣬���½���
		  else  Uart1_Rx_Sta|=0x80;	    //�������յ���������0x0A,˵��������ɣ���Uart1_Rx_Sta��bit7��1
		}
            else                                    //֮ǰû���յ�0x0D                             
              {	
                if(Res==0x0d) Uart1_Rx_Sta|=0x40;   //������յ���0x0D,��Uart1_Rx_Sta��bit6��1
                
                else                                //���Ҳû�յ�0x0D��˵����0x0D��0x0A��������ݣ��������ݣ�
                  {
                     Uart1_Rx_Buf[Uart1_Rx_Sta&0X3F]=Res ;   //����1���ֽڵ����ݴ��Uart1_Rx_Buf�����У���ΪUart1_Rx_Sta��bit7��bit6Ϊ��־λ��������&��0x3F��
                     Uart1_Rx_Sta++;                         //ÿ���յ�һ����������Uart1_Rx_Sta��1
                     if( Uart1_Rx_Sta>59) Uart1_Rx_Sta=0;   //������������շ�Χ����Uart1_Rx_Sta���㣬���½���
                  }		 
	      }
	}  		
    }
}


