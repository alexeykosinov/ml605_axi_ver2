#include "platform.h"
#include "sleep.h"
#include "eeprom.h"
#include "mpmc.h"
#include "xgpio.h"
#include "xparameters.h"


#define EEPROM_ADDRESS 0x54

XGpio GpioOutput;
void GPIO_led_test(Xuint16 GpioWidth);

int EEPROM_test(Xuint32 eeprom_address, Xuint32 page_size);




int main() {

	init_platform();
	print("Hello, asshole!\n\r");

	int i;
	for(i = 0; i < 6; ++i){
		xil_printf("%d ", i);
		msleep(250);
	}
	xil_printf("\n\r");
	xil_printf("UART test done\n\r");

	xil_printf("GPIO led test start\n\r");
	GPIO_led_test(8);
	xil_printf("GPIO led test done\n\r");

	xil_printf("EEPROM test start\n\r");
	EEPROM_test(EEPROM_ADDRESS, 16);
	xil_printf("EEPROM test done\n\r");



	u16 DeviceId = 0;
	u32 TotalErrors = 0;

	TotalErrors = MpmcMemTestExample(DeviceId);

	if (TotalErrors) {
		xil_printf("DDR test finished with error\r\n");
	} else {
		xil_printf("DDR test finished successfully\r\n");
	}

	return 0;
}

void GPIO_led_test(Xuint16 GpioWidth) {

  Xuint16 LedBit;
  Xuint16 LedLoop;

  int numTimes = 6;

  XGpio_Initialize(&GpioOutput, 0);				// Initialization
  XGpio_SetDataDirection(&GpioOutput, 1, 0x0); 	// Set the direction for all signals to be outputs
  XGpio_DiscreteWrite(&GpioOutput, 1, 0x0); 	// Set the GPIO outputs to low

  while (numTimes > 0) {
      for (LedBit = 0x0; LedBit < GpioWidth; LedBit++) {
          for (LedLoop = 0; LedLoop < 1; LedLoop++) {
            XGpio_DiscreteWrite(&GpioOutput, 1, 1 << LedBit); // Set the GPIO Output to High
            msleep(50);
            XGpio_DiscreteClear(&GpioOutput, 1, 1 << LedBit); //  Clear the GPIO Output
          }
      }
      numTimes--;
  }
}

int EEPROM_test(Xuint32 eeprom_address, Xuint32 page_size){

	/* EEPROM TEST*/
	unsigned bytes_written;
	unsigned bytes_read;
	unsigned k,j;


	u8 write_buffer[PAGE_SIZE];
	u8 read_buffer[PAGE_SIZE];

	addr_t address = 125;

	for (k = 0; k < PAGE_SIZE; k++) {
		write_buffer[k] = k*2;
		read_buffer[k] = 0;
	}

	bytes_written = eeprom_write_byte(address, write_buffer, page_size, 0, eeprom_address);
	if (bytes_written != page_size) return XST_FAILURE;

	bytes_read = eeprom_read_byte(address, read_buffer, page_size, eeprom_address);
	if (bytes_read != page_size) return XST_FAILURE;

	xil_printf("EEPROM read data:\r\n");
	for (j = 0; j < bytes_read; ++j){
		xil_printf("%d : 0x%x\r\n", j, read_buffer[j]);
	}
	return XST_SUCCESS;
}






