#ifndef __HC_SR04_H
#define __HC_SR04_H

#define ECHO GPIO_ReadInputPin(GPIOC,GPIO_PIN_3)

void HC_SR04_Init(void);  

void Trig(void);

void Distance_Count(void);

#endif
