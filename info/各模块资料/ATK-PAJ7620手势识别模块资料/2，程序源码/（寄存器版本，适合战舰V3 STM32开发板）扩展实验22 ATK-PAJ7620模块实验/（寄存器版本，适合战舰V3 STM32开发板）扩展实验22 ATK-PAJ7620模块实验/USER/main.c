#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "lcd.h"
#include "usmart.h"
#include "paj7620u2.h"

/************************************************
 ALIENTEK ս��V3��������չʵ��22
 ATK-PAJ7620U2 ����ʶ��ģ�����ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{
	Stm32_Clock_Init(9);   //ϵͳʱ������
	uart_init(72, 115200); //���ڳ�ʼ��Ϊ115200
	delay_init(72);		   //��ʱ��ʼ��
	KEY_Init();			   //������ʼ��
	LED_Init();			   //��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   //��ʼ��LCD
	usmart_dev.init(72);   //��ʼ��USMART
	POINT_COLOR = RED;	 //��������Ϊ��ɫ
	LCD_ShowString(30, 50, 200, 16, 16, "WarShip STM32");
	LCD_ShowString(30, 70, 200, 16, 16, "Sensor PAJ7620U2 TEST");
	LCD_ShowString(30, 90, 200, 16, 16, "ATOM@ALIENTEK");
	LCD_ShowString(30, 110, 200, 16, 16, "2017/8/23");
	POINT_COLOR = BLUE;		  //��������Ϊ��ɫ
	while (!paj7620u2_init()) //PAJ7620U2��������ʼ��
	{
		printf("PAJ7620U2 Error!!!\r\n");
		LCD_ShowString(30, 140, 200, 16, 16, "PAJ7620U2 Error!!!");
		delay_ms(500);
		LCD_ShowString(30, 140, 200, 16, 16, "                  ");
		delay_ms(500);
		LED0 = !LED0; //DS0��˸
	}
	printf("PAJ7620U2 OK\r\n");
	LCD_ShowString(30, 140, 200, 16, 16, "PAJ7620U2 OK");
	while (1)
	{
		paj7620u2_sensor_test(); //PAJ7620U2����������
	}
}
