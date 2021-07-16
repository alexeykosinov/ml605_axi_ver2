#ifndef __MPMC_H_
#define __MPMC_H_

#include "xbasic_types.h"
#include "xstatus.h"
#include "xv6_ddrx.h"

typedef volatile Xuint32 *U32Ptr;

#define WR_WORD(ADDR, DATA) (*(U32Ptr)(ADDR) = (DATA))
#define RD_WORD(ADDR, DATA) ((DATA) = *(U32Ptr)(ADDR))

static XV6_Ddrx Mpmc;

Xuint32 MpmcMemTest(XV6_Ddrx *Mpmc, Xuint32 offset_address);
int MpmcMemTestExample(Xuint16 DeviceId);

#endif
