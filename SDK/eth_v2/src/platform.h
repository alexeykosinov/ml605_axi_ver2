#ifndef __PLATFORM_H_
#define __PLATFORM_H_

#define STDOUT_IS_UARTLITE

#define PLATFORM_EMAC_BASEADDR          XPAR_AXI_ETHERNET_0_BASEADDR

#define PLATFORM_TIMER_BASEADDR         XPAR_AXI_TIMER_0_BASEADDR
#define PLATFORM_TIMER_INTERRUPT_INTR   XPAR_MICROBLAZE_0_INTC_AXI_TIMER_0_INTERRUPT_INTR
#define PLATFORM_TIMER_INTERRUPT_MASK   (1 << XPAR_MICROBLAZE_0_INTC_AXI_TIMER_0_INTERRUPT_INTR)

void init_platform();
void cleanup_platform();

#endif
