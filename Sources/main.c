/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "I2C.h"

int read = 0;
unsigned char data_sent=0;

int main(void)
{
	
	
	cfg_i2c_c0();
	
	
	for(;;) 
	{	 
		if(!data_sent)
		{
			
		write_I2C(0,0x38);
		
		read_I2C(0);
		data_sent=1;
		}
	}
	
	return 0;
}
