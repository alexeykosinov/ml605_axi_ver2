#include "mpmc.h"


int MpmcMemTestExample(Xuint16 DeviceId) {

	XV6_Ddrx_Config *CfgPtr;
	Xuint32 MemError = 0;
	int Status;

	CfgPtr = XV6_Ddrx_LookupConfig(DeviceId);
	if (CfgPtr == XNULL) return XST_FAILURE;

	Status = XV6_Ddrx_CfgInitialize(&Mpmc, CfgPtr, CfgPtr->MemBaseAddress);
	if (Status != XST_SUCCESS) return XST_FAILURE;

	xil_printf("Multi-Port Memory Controller Memory Test\r\n");
	MemError = MpmcMemTest(&Mpmc, 0x00200000);

	return MemError;
}

Xuint32 MpmcMemTest(XV6_Ddrx *Mpmc, Xuint32 offset_address) {
	Xuint32 MyAdr;
	Xuint32 StartAdr;
	Xuint32 MemSize;
	Xuint32 Range;
	Xuint32 Error = 0;
	Xuint32 DataRead;

	StartAdr = Mpmc->Config.MemBaseAddress + offset_address;
	MemSize  = Mpmc->Config.MemHighAddress - Mpmc->Config.MemBaseAddress + 1;
	Range    = MemSize - offset_address;

	xil_printf("Testing address range 0x%08X-0x%08X.\r\n", StartAdr, Mpmc->Config.MemHighAddress);

	xil_printf("\tWriting...\r\n");
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) WR_WORD(MyAdr, 0x00000000);

	xil_printf("\tReading...\r\n");

	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		RD_WORD(MyAdr, DataRead);
		if (DataRead != 0x00000000) {
			Error++;
			xil_printf("ERROR #%d: Address = 0x%08x, Data Expected was 0x00000000, Data Received was 0x%08x\r\n", Error, MyAdr, DataRead);
		}
	}

	return Error;
}

/*
int MpmcMemTestExample(Xuint16 DeviceId) {

	XMpmc_Config *CfgPtr;
	Xuint32 MemError = 0;
	int Status;

	CfgPtr = XMpmc_LookupConfig(DeviceId);
	if (CfgPtr == XNULL) return XST_FAILURE;

	Status = XMpmc_CfgInitialize(&Mpmc, CfgPtr, CfgPtr->BaseAddress);
	if (Status != XST_SUCCESS) return XST_FAILURE;

	xil_printf("Multi-Port Memory Controller Memory Test\r\n");
	MemError = MpmcMemTest(&Mpmc, 0x00200000);

	return MemError;
}

Xuint32 MpmcMemTest(XMpmc *Mpmc, Xuint32 offset_address) {
	Xuint32 MyAdr;
	Xuint32 StartAdr;
	Xuint32 MemSize;
	Xuint32 Range;
	Xuint32 Error = 0;
	Xuint32 DataRead;
	
	StartAdr = Mpmc->ConfigPtr.MemBaseAddress + offset_address;
	MemSize  = Mpmc->ConfigPtr.MemHighAddress - Mpmc->ConfigPtr.MemBaseAddress + 1;
	Range    = MemSize - offset_address;

	xil_printf("Testing address range 0x%08X-0x%08X.\r\n", StartAdr, Mpmc->ConfigPtr.MemHighAddress);

	xil_printf("\tWriting...\r\n");
	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) WR_WORD(MyAdr, 0x00000000);

	xil_printf("\tReading...\r\n");

	for (MyAdr = StartAdr; MyAdr < StartAdr + Range; MyAdr += 4) {
		RD_WORD(MyAdr, DataRead);
		if (DataRead != 0x00000000) {
			Error++;
			xil_printf("ERROR #%d: Address = 0x%08x, Data Expected was 0x00000000, Data Received was 0x%08x\r\n", Error, MyAdr, DataRead);
		}
	}

	return Error;
}
*/
