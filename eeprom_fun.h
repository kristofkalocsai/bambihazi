#ifndef _EEPROM_FUN_H_
#define _EEPROM_FUN_H_


/*********************************************************************
 * Function: 	 EEPROM_write(unsigned int address, unsigned char data);

  ********************************************************************/


void EEPROM_write(unsigned int address, signed char data);


/*********************************************************************
 * Function: 	 EEPROM_read(unsigned int address)
 *
  ********************************************************************/


unsigned char EEPROM_read(unsigned int address);


/*********************************************************************
*
*	Function: EEPROM_write16
*
**********************************************************************/

void EEPROM_write16( unsigned int address, signed int data);


/*********************************************************************

	Function: EEPROM_read16

*********************************************************************/

signed int EEPROM_read16( unsigned int address );



#endif

