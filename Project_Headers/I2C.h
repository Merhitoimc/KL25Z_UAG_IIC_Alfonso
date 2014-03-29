/*
 * I2C.h
 *
 *  Created on: Mar 26, 2014
 *      Author: Jorge Armando Hdez
 */

#ifndef I2C_H_
#define I2C_H_

void cfg_i2c_c0(void);
void write_I2C(unsigned int reg, unsigned char data);
unsigned char I2C_CycleRead(unsigned char byte_pending);

void read_I2C(unsigned char read_mem_address);

#endif /* I2C_H_ */
