/*
 * IIC_device.h
 *
 *  Created on: 2015Äê6ÔÂ11ÈÕ
 *      Author: yu
 */

#ifndef IIC_DEVICE_H_
#define IIC_DEVICE_H_

#include "MSP430F5529_conf.h"

void iic_init(uint8_t SLAVE_ADDRESS);
void iic_WriteMode(void);
void iic_ReadMode(void);
void iic_writebyte(uint8_t reg_address,uint8_t write_number,uint8_t *data_to_write);
void iic_readbyte(uint8_t reg_address,uint8_t read_number);

#endif /* IIC_DEVICE_H_ */
