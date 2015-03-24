#ifndef _EEPROM_FUN_H_
#define _EEPROM_FUN_H_


/*********************************************************************
 * Function: 	 EEPROM_write(unsigned int address, unsigned char data);

  ********************************************************************/


void EEPROM_write(unsigned int address, unsigned char data);


/*********************************************************************
 * Function: 	 EEPROM_read(unsigned int address)
 *
  ********************************************************************/


unsigned char EEPROM_read(unsigned int address);

#endif

