#include "eeprom_fun.h"
#include <avr/io.h>
#include <stdlib.h>
#include "mcu_avr_atmega128_api.h" 	// MCU API   
#include "dpy_trm_s01.h"			// DPY API



/*********************************************************************
 * Function: 	 EEPROM_write(unsigned int address, unsigned char data);

  ********************************************************************/

void EEPROM_write(unsigned int wr_address, signed char wr_data)
{
	DPY_TRM_S01__LED_3_ON();
	/* Wait for completion of previous write or bootloader operation*/
	while(EECR & (1<<EEWE))
		;

	/* Set up address and data registers */
	EEAR = wr_address;
	EEDR = wr_data;
	
	cli();
	/* Write logical one to EEMWE */
	//EECR |= (1<<EEMWE);
	asm("SBI 0x1C,2\n"
		"SBI 0x1C,1\n");
	/* Start eeprom write by setting EEWE */
	//EECR |= (1<<EEWE);
	sei();
	DPY_TRM_S01__LED_3_OFF();

}


/*********************************************************************
 * Function: 	 EEPROM_read(unsigned int address)
 *
  ********************************************************************/

unsigned char EEPROM_read(unsigned int rd_address)
{
	DPY_TRM_S01__LED_4_ON();
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE))
		;
	
	/* Set up address register */
	EEAR = rd_address;
	
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	
	DPY_TRM_S01__LED_4_OFF();
	/* Return data from data register */
	return EEDR;

}

void EEPROM_write16(unsigned int wr_address16, signed int wr_data16)
{
	volatile unsigned char wr_dataLow = 0, wr_dataHigh = 0;

	
	wr_dataHigh =	(wr_data16 & 0xFF00) >> 8;
	wr_dataLow  = 	(wr_data16 & 0x00FF);

	EEPROM_write( wr_address16, 	wr_dataLow);
	EEPROM_write( wr_address16+1, 	wr_dataHigh);


}

signed int EEPROM_read16(unsigned int rd_address16)
{
	volatile unsigned char rd_dataLow = 0,rd_dataHigh = 0;
	volatile unsigned int rd_data16 = 0;

	rd_dataLow 	= EEPROM_read( rd_address16 	);
	rd_dataHigh = EEPROM_read( rd_address16 + 1	);

	rd_data16 = rd_dataHigh;
	rd_data16 <<= 8;
	rd_data16 = rd_data16 + rd_dataLow;

	return rd_data16;


}


