#ifndef __PLATFORM_H_
#define __PLATFORM_H_

#include <xbasic_types.h>

#define STDOUT_IS_UARTLITE
#define RX_BUFF_SIZE   	512


#define PLATFORM_EMAC_BASEADDR          XPAR_AXI_ETHERNET_0_BASEADDR

#define PLATFORM_TIMER_BASEADDR         XPAR_AXI_TIMER_0_BASEADDR
#define PLATFORM_TIMER_INTERRUPT_INTR   XPAR_MICROBLAZE_0_INTC_AXI_TIMER_0_INTERRUPT_INTR
#define PLATFORM_TIMER_INTERRUPT_MASK   (1 << XPAR_MICROBLAZE_0_INTC_AXI_TIMER_0_INTERRUPT_INTR)

void init_platform();
void cleanup_platform();

int uart_receive_flag;

u8 SendBuffer[RX_BUFF_SIZE];
u8 ReceiveBuffer[RX_BUFF_SIZE];

u8 data[RX_BUFF_SIZE];
//u32 receive_count = 0;

#endif
