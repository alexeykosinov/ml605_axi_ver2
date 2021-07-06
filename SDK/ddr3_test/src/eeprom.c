#include "eeprom.h"

unsigned eeprom_write_byte(addr_t address, u8 *buffer_p, u16 byte_count, u8 offset_addr, u8 eeprom_iic_addr) {

	volatile unsigned sent_byte_count;
	volatile unsigned ack_byte_count;

	u8 write_buffer[sizeof(address) + PAGE_SIZE];
	u32 cntl_reg;

	if (sizeof(addr_t) == 2) {
		write_buffer[0] = (u8)(address >> 8);
		write_buffer[1] = (u8)(address);
	} else if (sizeof(addr_t) == 1) {
		write_buffer[0] = (u8)(address);
		eeprom_iic_addr |= (offset_addr >> 8) & 0x7;
	}

	int i;
	for (i = 0; i < byte_count; i++) {
		write_buffer[sizeof(address) + i] = buffer_p[i];
	}

	do {
		sent_byte_count = XIic_Send(XPAR_IIC_0_BASEADDR, eeprom_iic_addr, (u8 *)&address, sizeof(address), XIIC_STOP);
		if (sent_byte_count != sizeof(address)) {
			cntl_reg = XIic_ReadReg(XPAR_IIC_0_BASEADDR, XIIC_CR_REG_OFFSET);
			XIic_WriteReg(XPAR_IIC_0_BASEADDR, XIIC_CR_REG_OFFSET, cntl_reg | XIIC_CR_TX_FIFO_RESET_MASK);
			XIic_WriteReg(XPAR_IIC_0_BASEADDR, XIIC_CR_REG_OFFSET, XIIC_CR_ENABLE_DEVICE_MASK);
		}
	} while (sent_byte_count != sizeof(address));

	sent_byte_count = XIic_Send(XPAR_IIC_0_BASEADDR, eeprom_iic_addr, write_buffer, sizeof(address) + PAGE_SIZE, XIIC_STOP);

	do {
		ack_byte_count = XIic_Send(XPAR_IIC_0_BASEADDR, eeprom_iic_addr, (u8 *)&address, sizeof(address), XIIC_STOP);
		if (ack_byte_count != sizeof(address)) {
			cntl_reg = XIic_ReadReg(XPAR_IIC_0_BASEADDR, XIIC_CR_REG_OFFSET);
			XIic_WriteReg(XPAR_IIC_0_BASEADDR, XIIC_CR_REG_OFFSET, cntl_reg | XIIC_CR_TX_FIFO_RESET_MASK);
			XIic_WriteReg(XPAR_IIC_0_BASEADDR, XIIC_CR_REG_OFFSET, XIIC_CR_ENABLE_DEVICE_MASK);
		}
	} while (ack_byte_count != sizeof(address));

	return sent_byte_count - sizeof(address);
}

unsigned eeprom_read_byte(addr_t address, u8 *buffer_p, u16 byte_count, u8 eeprom_iic_addr) {

	volatile unsigned received_byte_count;

	u16 status_reg;
	u32 cntl_reg;

	do {
		status_reg = XIic_ReadReg(XPAR_IIC_0_BASEADDR, XIIC_SR_REG_OFFSET);
		if(!(status_reg & XIIC_SR_BUS_BUSY_MASK)) {
			received_byte_count = XIic_Send(XPAR_IIC_0_BASEADDR, eeprom_iic_addr, (u8 *)&address, sizeof(address), XIIC_STOP);

			if (received_byte_count != sizeof(address)) {
				cntl_reg = XIic_ReadReg(XPAR_IIC_0_BASEADDR, XIIC_CR_REG_OFFSET);
				XIic_WriteReg(XPAR_IIC_0_BASEADDR, XIIC_CR_REG_OFFSET, cntl_reg | XIIC_CR_TX_FIFO_RESET_MASK);
				XIic_WriteReg(XPAR_IIC_0_BASEADDR, XIIC_CR_REG_OFFSET, XIIC_CR_ENABLE_DEVICE_MASK);
			}
		}
	} while (received_byte_count != sizeof(address));

	received_byte_count = XIic_Recv(XPAR_IIC_0_BASEADDR, eeprom_iic_addr, buffer_p, byte_count, XIIC_STOP);
	return received_byte_count;
}
