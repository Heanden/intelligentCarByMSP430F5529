#include "main.h"
/**************************************************************************
作者：Mini Balance 
淘宝店铺：http://shop114407458.taobao.com/
接线说明如下：
GNG---GND
VCC---3.3V~5.0V
SCL---PA5
SDA---PA6
RES---PA7
DC---PA3
函数功能：主函数 初始化系统和外设
作    者:平衡小车之家
**************************************************************************/
int main(void)
{
	SystemInit();                   //=====系统初始化
	delay_init(72);                 //=====延时函数
	OLED_Init();	                  //=====初始化OLED 模拟SPI 
  while(1)
  {

		oled_show(); //===显示屏打开
		
	}	
}

