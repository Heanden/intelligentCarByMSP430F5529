#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "lcd.h"
#include "usmart.h"
#include "paj7620u2.h"

/************************************************
 ALIENTEK 战舰V3开发板扩展实验22
 ATK-PAJ7620U2 手势识别模块测试实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
	Stm32_Clock_Init(9);   //系统时钟设置
	uart_init(72, 115200); //串口初始化为115200
	delay_init(72);		   //延时初始化
	KEY_Init();			   //按键初始化
	LED_Init();			   //初始化与LED连接的硬件接口
	LCD_Init();			   //初始化LCD
	usmart_dev.init(72);   //初始化USMART
	POINT_COLOR = RED;	 //设置字体为红色
	LCD_ShowString(30, 50, 200, 16, 16, "WarShip STM32");
	LCD_ShowString(30, 70, 200, 16, 16, "Sensor PAJ7620U2 TEST");
	LCD_ShowString(30, 90, 200, 16, 16, "ATOM@ALIENTEK");
	LCD_ShowString(30, 110, 200, 16, 16, "2017/8/23");
	POINT_COLOR = BLUE;		  //设置字体为蓝色
	while (!paj7620u2_init()) //PAJ7620U2传感器初始化
	{
		printf("PAJ7620U2 Error!!!\r\n");
		LCD_ShowString(30, 140, 200, 16, 16, "PAJ7620U2 Error!!!");
		delay_ms(500);
		LCD_ShowString(30, 140, 200, 16, 16, "                  ");
		delay_ms(500);
		LED0 = !LED0; //DS0闪烁
	}
	printf("PAJ7620U2 OK\r\n");
	LCD_ShowString(30, 140, 200, 16, 16, "PAJ7620U2 OK");
	while (1)
	{
		paj7620u2_sensor_test(); //PAJ7620U2传感器测试
	}
}
