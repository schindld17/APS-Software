//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	PIEMod.c
//
//	TITLE:
//
//	DESCRIPTION:
//
//
//
//
//
//###########################################################################
//	Author: Dylan Schindler
//	Creation Date: Apr 10, 2016 
//	Release Date:
//###########################################################################

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <file.h>
#include "F2837xS_device.h"
#include "APS_GlobalPrototypes.h"
#include "F2837xS_Examples.h"
#include "F2837xS_Pie_defines.h"


void APSPieInit(void)
{
	EALLOW;

	//Start ePWM modules
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0x1;

	PieVectTable.EPWM12_INT = &realclock_isr;
	//PieVectTable.EPWM2_TZ_INT = &epwm2_isr;
	EDIS;

   // IER |= M_INT2; //Enable group 2 interrupts
    IER |= M_INT3; //Enable group 3 interrupts

	//PieCtrlRegs.PIEIER2.bit.INTx2 = 0x1;
	PieCtrlRegs.PIEIER3.bit.INTx12 = 0x1;

    // Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
}

interrupt void realclock_isr(void)
{
	//asm ("      ESTOP0");
	addSeconds(1);
#ifdef _TEST
	//realtime = realtime + 1;
	//if(realtime > 29)
	//{
		//realtime = 0;
		realClockTest();
	//}

#endif
	EALLOW;
	EPwm12Regs.ETCLR.bit.INT = 0x1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
	EDIS;
}//END ISR

interrupt void epwm2_isr(void)
{
	unsigned char ucChar;
	int16_t adcSample;
	char adcValString[16];
	//Check if voltage was too high or too low
	//if(EPwm2Regs.TZFLG.bit.DCAEVT1) //Voltage too high -> voltage spike has occurred
		//Close with load switch to prevent circuit damage
		//LoadSwitch(AC_LOAD, LOAD_CLOSED);

#ifdef _TEST
	int firstTime = 0;

	if(firstTime == 0)
	{
		firstTime = 1;
		redirOut();
	}

	EALLOW;
	//Wait for comparator Trip to de-assert
	//if(EPwm2Regs.TZFLG.bit.DCAEVT1)
	while(Cmpss3Regs.COMPSTS.bit.COMPHSTS & Cmpss3Regs.COMPSTS.bit.COMPLSTS);
	asm ("      ESTOP0");
	adcSample = sampleADC(AC_VOLT);
	convertADC(adcSample,AC_VOLT, adcValString);
	printf("ADC Value = %sV", adcValString);

	ucChar = 10;
	putchar(ucChar);

	ucChar = 13;
	putchar(ucChar);

	DELAY_US(10000);
#endif

	//Clear flags
	EALLOW;
	EPwm2Regs.TZCLR.bit.INT = 0x1;
	EPwm2Regs.TZCLR.bit.OST = 0x1;
	EPwm2Regs.TZOSTCLR.bit.DCAEVT1 = 0x1;
	EPwm2Regs.TZCBCCLR.bit.DCAEVT2 = 0x1;

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

	EDIS;
}//END ISR



//###########################################################################

