#include "platform.h"
#include "sleep.h"
#include "eeprom.h"
#include "mpmc.h"
#include "xgpio.h"
#include "xparameters.h"
#include "netif/xadapter.h"

#define EEPROM_ADDRESS 0x54

XGpio GpioOutput;
void GPIO_led_test(Xuint16 GpioWidth);
int EEPROM_test(Xuint32 eeprom_address, Xuint32 page_size);

/* defined by each RAW mode application */
void print_app_header();
int start_application();
int transfer_data();

/* missing declaration in lwIP */
void lwip_init();

static struct netif server_netif;
struct netif *echo_netif;

void print_ip(char *msg, struct ip_addr *ip) {
	xil_printf(msg);
	xil_printf("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip), ip4_addr3(ip), ip4_addr4(ip));
}

void print_ip_settings(struct ip_addr *ip, struct ip_addr *mask, struct ip_addr *gw) {
	print_ip("Board IP: ", ip);
	print_ip("Netmask : ", mask);
	print_ip("Gateway : ", gw);
}


int main() {

	struct ip_addr ipaddr, netmask, gw;

	/* the MAC address of the board. this should be unique per board */
	unsigned char mac_ethernet_address[] =	{ 0x00, 0x0A, 0x35, 0x02, 0x69, 0xD7 };

	echo_netif = &server_netif;

	init_platform();
	print("Hello, asshole!\n\r");

	int i;
	for(i = 0; i < 6; ++i){
		xil_printf("%d ", i);
		msleep(250);
	}
	xil_printf("\n\r");
	xil_printf("UART test done\n\r");

	GPIO_led_test(8);

	xil_printf("EEPROM test done, errors = %d\n\r", EEPROM_test(EEPROM_ADDRESS, 16));


	u16 DeviceId = 0;
	u32 TotalErrors = 0;

	TotalErrors = MpmcMemTestExample(DeviceId);

	if (TotalErrors) {
		xil_printf("DDR test finished with error\r\n");
	} else {
		xil_printf("DDR test finished successfully\r\n");
	}


	/* ETHERNET */
	/* initliaze IP addresses to be used */
	IP4_ADDR(&ipaddr,  10, 11,   0, 153);
	IP4_ADDR(&netmask, 255, 255, 255,  0);
	IP4_ADDR(&gw,      10, 11,   0,  40);

	print_app_header();
	print_ip_settings(&ipaddr, &netmask, &gw);

	lwip_init();

  	/* Add network interface to the netif_list, and set it as default */
	if (!xemac_add(echo_netif, &ipaddr, &netmask, &gw, mac_ethernet_address, PLATFORM_EMAC_BASEADDR)) {
		xil_printf("Error adding N/W interface\n\r");
		return -1;
	}
	netif_set_default(echo_netif);

	/* specify that the network if is up */
	netif_set_up(echo_netif);

	/* start the application (web server, rxtest, txtest, etc..) */
	start_application();

	/* receive and process packets */
	while (1) {
		xemacif_input(echo_netif);
		transfer_data();
	}
  


	/* never reached */
	cleanup_platform();
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
	int err = 0;

	u8 write_buffer[PAGE_SIZE];
	u8 read_buffer[PAGE_SIZE];

	addr_t address = 125;

	for (k = 0; k < PAGE_SIZE; k++) {
		write_buffer[k] = 0x55;
		read_buffer[k] = 0;
	}

	bytes_written = eeprom_write_byte(address, write_buffer, page_size, 0, eeprom_address);
	if (bytes_written != page_size) return XST_FAILURE;

	bytes_read = eeprom_read_byte(address, read_buffer, page_size, eeprom_address);
	if (bytes_read != page_size) return XST_FAILURE;

	for (j = 0; j < bytes_read; ++j){
		if (read_buffer[j] != write_buffer[j]) {
			xil_printf("Error: read 0x%x != write 0x%x\r\n", read_buffer[j], write_buffer[j]);
			err++;
		}
	}
	return err;
}






