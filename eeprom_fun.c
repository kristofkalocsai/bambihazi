#include "eeprom_fun.h"
#include <avr/io.h>




/*********************************************************************
 * Function: 	 EEPROM_write(unsigned int address, unsigned char data);

  ********************************************************************/

void EEPROM_write(unsigned int address, unsigned char data)
{

	/* Wait for completion of previous write or bootloader operation*/
	while((EECR & (1<<EEWE)) && (SPMCSR & (1<<SPMEN)))
		;

	/* Set up address and data registers */
	EEAR = address;
	EEDR = data;
	


	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);


}


/*********************************************************************
 * Function: 	 EEPROM_read(unsigned int address)
 *
  ********************************************************************/

unsigned char EEPROM_read(unsigned int address)
{

	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE))
		;
	
	/* Set up address register */
	EEAR = address;
	
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	
	/* Return data from data register */
	return EEDR;

}

void EEPROM_write16(unsigned int address, signed int data)
{
	unsigned char dataLow = 0, dataHigh = 0;

	
	dataHigh =	(data & 0xFF00) >> 8;
	dataLow  = 	(data & 0x00FF);

	EEPROM_write( address, dataLow);
	EEPROM_write( (address + 1), dataHigh);


}

signed int EEPROM_read16(unsigned int address)
{
	unsigned char dataLow = 0,dataHigh = 0;
	unsigned int data = 0;

	dataLow = EEPROM_read( address );
	dataHigh = EEPROM_read( address + 1);

	data = dataHigh;
	data <<= 8;
	data += dataLow;

	return data;


}


