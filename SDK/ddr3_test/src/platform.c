#include "xparameters.h"
#include "xil_cache.h"
#include "platform.h"
#include "mb_interface.h"

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

void init_uart(){

}


void
init_platform() {
    microblaze_enable_interrupts();
    enable_caches();
    init_uart();
}

void cleanup_platform(){
    disable_caches();
}
