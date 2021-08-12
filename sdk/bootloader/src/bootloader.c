#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blconfig.h"
#include "portab.h"
#include "errors.h"
#include "srec.h"

/* Defines */
#define CR       13

static uint8_t load_exec ();
static uint8_t flash_get_srec_line (uint8_t *buf);
extern void init_stdout();

extern int srec_line;
extern void outbyte(char c); 

/* Data structures */
static srec_info_t srinfo;
static uint8_t sr_buf[SREC_MAX_BYTES];
static uint8_t sr_data_buf[SREC_DATA_MAX_BYTES];

static uint8_t *flbuf;

void _interrupt_handler() {}
void _exception_handler() {}
void _hw_exception_handler() {}


int main() {
    uint8_t ret;

    flbuf = (uint8_t*)FLASH_IMAGE_BASEADDR;
    ret = load_exec();

    return ret;
}

static uint8_t load_exec() {
    uint8_t ret;
    void (*laddr)();
    int8_t done = 0;
    
    srinfo.sr_data = sr_data_buf;
    
    while (!done) {
        if ((ret = flash_get_srec_line (sr_buf)) != 0) 
            return ret;

        if ((ret = decode_srec_line (sr_buf, &srinfo)) != 0)
            return ret;
        
        switch (srinfo.type) {
            case SREC_TYPE_0:
                break;
            case SREC_TYPE_1:
            case SREC_TYPE_2:
            case SREC_TYPE_3:
                memcpy ((void*)srinfo.addr, (void*)srinfo.sr_data, srinfo.dlen);
                break;
            case SREC_TYPE_5:
                break;
            case SREC_TYPE_7:
            case SREC_TYPE_8:
            case SREC_TYPE_9:
                laddr = (void (*)())srinfo.addr;
                done = 1;
                ret = 0;
                break;
        }
    }

    (*laddr)();                 
  
    /* We will be dead at this point */
    return 0;
}

static uint8_t flash_get_srec_line (uint8_t *buf) {
    uint8_t c;
    int count = 0;

    while (1) {
        c  = *flbuf++;
        if (c == 0xD) {   
            /* Eat up the 0xA too */
            c = *flbuf++; 
            return 0;
        }
        
        *buf++ = c;
        count++;
        if (count > SREC_MAX_BYTES) return LD_SREC_LINE_ERROR;
    }
}
