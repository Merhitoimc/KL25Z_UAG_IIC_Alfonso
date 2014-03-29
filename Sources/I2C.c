/*
 * I2C.c
 *
 *  Created on: Mar 22, 2014
 *      Author: Jorge Armando Hdez
 */

#include "derivative.h"
#include "I2C.h"

int data = 0;

unsigned char I2C_CycleRead(unsigned char byte_pending);

void cfg_i2c_c0(void)
{
	// TURN ON THE CLOCK FOR I2C CHANEL 0 
	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
	// TURN ON THE CLOCK FOR PORTB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	//I2C PORT I2C0_SCL
	PORTB_PCR3 = (PORT_PCR_MUX(2)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK);                                  
	//I2C PORT I2C0_SDA
	PORTB_PCR2 = (PORT_PCR_MUX(2)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK);
	
	//	I2C CONFIGURATION
	//Frequency Divider for the clock, ICR = b000001 and MULT = b00 (which means MUL = 1) 
	I2C0_F = I2C_F_ICR(0x1A);
	
	//Configuration Register 1, selected I2C enable, transmit mode on.
	I2C0_C1 |= I2C_C1_IICEN_MASK ;
	
	//Configuration Register 2, NO SLAVE MODE, so no need to configurate this register.
	I2C0_C2 = 0x00;
	
	//Status Register, interrupt pending 
	I2C0_S |= I2C_S_IICIF_MASK;
}

void write_I2C(unsigned int reg, unsigned char data)
{
	unsigned char address;
	
	if(reg<256)
	{
		address = 0xA0;
	}
	else
	{
		address = 0xA2;
		reg -= 255;
	}
	
	I2C0_C1 |= I2C_C1_TX_MASK;
		
	I2C0_C1 |= I2C_C1_MST_MASK;
	while ((I2C0_S & I2C_S_BUSY_MASK)==0);
	
	while ((I2C0_S & I2C_S_TCF_MASK)==0);
	I2C0_D = address; // PHY ADD
	while ((I2C0_S & I2C_S_IICIF_MASK) == 0x00);
	I2C0_S |= I2C_S_IICIF_MASK;
		
	while ((I2C0_S & I2C_S_TCF_MASK)==0);
	I2C0_D = reg; // MEM ADD
	while ((I2C0_S & I2C_S_IICIF_MASK) == 0x00);
	I2C0_S |= I2C_S_IICIF_MASK;
	
	while ((I2C0_S & I2C_S_TCF_MASK)==0);
	I2C0_D = data; // DATA
	while ((I2C0_S & I2C_S_IICIF_MASK) == 0x00);
	I2C0_S |= I2C_S_IICIF_MASK;
	
	I2C0_C1 &= ~I2C_C1_MST_MASK;
	
}

void read_I2C(unsigned char read_mem_address)
{
	
	I2C0_C1 |= I2C_C1_TX_MASK;
	
	I2C0_C1 |= I2C_C1_MST_MASK;
	while ((I2C0_S & I2C_S_BUSY_MASK)==0);
	
	while ((I2C0_S & I2C_S_TCF_MASK)==0);
	I2C0_D = 0xA0; // PHY ADD
	while ((I2C0_S & I2C_S_IICIF_MASK) == 0x00);
	I2C0_S |= I2C_S_IICIF_MASK;
	
	while ((I2C0_S & I2C_S_TCF_MASK)==0);
	I2C0_D = read_mem_address; // MEM ADD
	while ((I2C0_S & I2C_S_IICIF_MASK) == 0x00);
	I2C0_S |= I2C_S_IICIF_MASK;
	
	I2C0_C1 |= I2C_C1_RSTA_MASK;
	while ((I2C0_S & I2C_S_BUSY_MASK)==0);
	
	while ((I2C0_S & I2C_S_TCF_MASK)==0);
	I2C0_D = 0xA0+1; // PHY ADD
	while ((I2C0_S & I2C_S_IICIF_MASK) == 0x00);
	I2C0_S |= I2C_S_IICIF_MASK;
	
	data = I2C_CycleRead(0);
	
	I2C0_C1 &= ~I2C_C1_MST_MASK;
	
	while ((I2C0_S & I2C_S_BUSY_MASK) == 0x00);
}
unsigned char I2C_CycleRead(unsigned char byte_pending)
{
	unsigned char byte_read=0;
	
	while ((I2C0_S & I2C_S_TCF_MASK) == 0);
	I2C0_C1 &= ~I2C_C1_TX_MASK ; 
	if(byte_pending==1) {I2C0_C1 &= ~I2C_C1_TXAK_MASK ;}
	if(byte_pending==0) {I2C0_C1 |=  I2C_C1_TXAK_MASK ;}
	byte_read = I2C0_D;
	while ((I2C0_S & I2C_S_IICIF_MASK) == 0);
	while ((I2C0_S & I2C_S_TCF_MASK) == 0);
	//I2C0_C1 |= I2C_C1_TX_MASK ; 
	byte_read = I2C0_D;
	while ((I2C0_S & I2C_S_IICIF_MASK) == 0);
	while ((I2C0_S & I2C_S_TCF_MASK) == 0);
	I2C0_S |= I2C_S_IICIF_MASK ;

	return byte_read;
}
