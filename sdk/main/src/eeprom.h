#ifndef __EEPROM_H_
#define __EEPROM_H_

#include "xparameters.h"
#include "xiic.h"

//#define PAGE_SIZE 16
//typedef u8 addr_t;

unsigned eeprom_write_byte(u8 address, u8 *buffer_p, u16 byte_count, u8 offset_addr, u16 page_size, u8 eeprom_iic_addr);
unsigned eeprom_read_byte(u8 address, u8 *buffer_p, u16 byte_count, u8 eeprom_iic_addr);
unsigned eeprom_test(u8 eeprom_iic_addr, u16 page_size);

#endif
