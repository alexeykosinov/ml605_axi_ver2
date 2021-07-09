#ifndef __SLEEP_H_
#define __SLEEP_H_

#include "xparameters.h"
#include "xtmrctr_l.h"

#define PROCESSOR_CLOCK_FREQUENCY XPAR_TMRCTR_0_CLOCK_FREQ_HZ

void msleep(unsigned int microseconds);
void usleep(unsigned int useconds);
void sleep(unsigned int seconds);

#endif
