#include "xparameters.h"
#include "xil_cache.h"
#include "platform.h"
#include "mb_interface.h"
#include "xintc.h"
#include "xil_exception.h"
#include "lwip/tcp.h"
#include "arch/cc.h"
#include "xtmrctr_l.h"
#include "xuartlite.h"
#include "xuartlite_l.h"

#define MHZ 				(66)
#define TIMER_TLR 			(25000000*((float)MHZ/100))

static XIntc intc;
static XUartLite uartlite;

static volatile int TotalReceivedCount;

void SendHandler(void *CallBackRef, unsigned int EventData);
void RecvHandler(void *CallBackRef, unsigned int EventData);

u8* ReceiveBufferPtr = &ReceiveBuffer[0];

//static XUartLite_Stats uart_stats;



void enable_caches() {
#ifdef XPAR_MICROBLAZE_USE_ICACHE
    Xil_ICacheEnable();
#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE
    Xil_DCacheEnable();
#endif
}

void disable_caches() {
    Xil_DCacheDisable();
    Xil_ICacheDisable();
}

void timer_callback(){
	/* we need to call tcp_fasttmr & tcp_slowtmr at intervals specified by lwIP.
	 * It is not important that the timing is absoluetly accurate.
	 */
	static int odd = 1;
	tcp_fasttmr();
	odd = !odd;
	if (odd) tcp_slowtmr();
}

void xadapter_timer_handler(void *p) {
	timer_callback();

	/* Load timer, clear interrupt bit */
	XTmrCtr_SetControlStatusReg(PLATFORM_TIMER_BASEADDR, 0, XTC_CSR_INT_OCCURED_MASK | XTC_CSR_LOAD_MASK);
	XTmrCtr_SetControlStatusReg(PLATFORM_TIMER_BASEADDR, 0, XTC_CSR_ENABLE_TMR_MASK | XTC_CSR_ENABLE_INT_MASK | XTC_CSR_AUTO_RELOAD_MASK | XTC_CSR_DOWN_COUNT_MASK);
	XIntc_AckIntr(XPAR_INTC_0_BASEADDR, PLATFORM_TIMER_INTERRUPT_MASK);
}


void microblaze_setup_interrupts() {
	XIntc *intcp;
	intcp = &intc;

	XUartLite *xuartlitep;
	xuartlitep = &uartlite;

	XIntc_Initialize(intcp, XPAR_INTC_0_DEVICE_ID);


	XIntc_Connect(intcp, XPAR_INTC_0_UARTLITE_1_VEC_ID, (XInterruptHandler)XUartLite_InterruptHandler, (void *)xuartlitep);

	XIntc_Start(intcp, XIN_REAL_MODE);

	/* Start the interrupt controller */
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);

	microblaze_register_handler((XInterruptHandler)XIntc_InterruptHandler, intcp);

    /* Setup timer */ 
	/* Set the number of cycles the timer counts before interrupting */
	/* 100 Mhz clock => .01us for 1 clk tick. For 100ms, 10000000 clk ticks need to elapse  */
	XTmrCtr_SetLoadReg(PLATFORM_TIMER_BASEADDR, 0, TIMER_TLR);
	/* Reset the timers, and clear interrupts */
	XTmrCtr_SetControlStatusReg(PLATFORM_TIMER_BASEADDR, 0, XTC_CSR_INT_OCCURED_MASK | XTC_CSR_LOAD_MASK);
	/* Start the timers */
	XTmrCtr_SetControlStatusReg(PLATFORM_TIMER_BASEADDR, 0, XTC_CSR_ENABLE_TMR_MASK | XTC_CSR_ENABLE_INT_MASK | XTC_CSR_AUTO_RELOAD_MASK | XTC_CSR_DOWN_COUNT_MASK);
	/* Register Timer handler */
	XIntc_RegisterHandler(XPAR_INTC_0_BASEADDR, PLATFORM_TIMER_INTERRUPT_INTR,(XInterruptHandler)xadapter_timer_handler, 0);

	XIntc_Enable(intcp, PLATFORM_TIMER_INTERRUPT_INTR);
	XIntc_Enable(intcp, XPAR_INTC_0_AXIETHERNET_0_VEC_ID);
	XIntc_Enable(intcp, XPAR_INTC_0_UARTLITE_1_VEC_ID);

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XIntc_InterruptHandler, intcp);
	Xil_ExceptionEnable();

}

void enable_uart() {
	XUartLite_Initialize(&uartlite, XPAR_UARTLITE_1_DEVICE_ID);
	XUartLite_SetSendHandler(&uartlite, SendHandler, &uartlite);
	XUartLite_SetRecvHandler(&uartlite, RecvHandler, &uartlite);
	XUartLite_EnableInterrupt(&uartlite);
}

void init_platform() {
    enable_caches();
	enable_uart(uartlite);
    microblaze_setup_interrupts();
    microblaze_enable_interrupts();
}

void cleanup_platform(){
    disable_caches();
}

void SendHandler(void *CallBackRef, unsigned int EventData) { }

void RecvHandler(void *CallBackRef, unsigned int EventData) {

	TotalReceivedCount = EventData;
	if(EventData == RX_BUFF_SIZE) {
		uart_receive_flag = 1;
	}

}
