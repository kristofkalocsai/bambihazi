#include "eeprom_fun.h"
#include <avr/io.h>




/*********************************************************************
 * Function: 	 EEPROM_write(unsigned int address, unsigned char data);

  ********************************************************************/

void EEPROM_write(unsigned int address, unsigned char data)
{

	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE))
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
