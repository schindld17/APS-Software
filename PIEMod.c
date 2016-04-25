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
#include "APS_GlobalDefs.h"
#include "F2837xS_Examples.h"
#include "F2837xS_Pie_defines.h"


void APSPieInit(void)
{
	EALLOW;

	//Start ePWM modules
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0x1;

	PieVectTable.EPWM12_INT = &realclock_isr;
	PieCtrlRegs.PIEIER3.bit.INTx12 = 0x1;

	//Enable EPWM1 (Solar Input Voltage Comparator) if component is connected
	if(SOLAR_INPUT_VOLTAGE_COMPONENT)
	{
		PieVectTable.EPWM1_TZ_INT = &epwm1_isr;
		PieCtrlRegs.PIEIER2.bit.INTx1 = 0x1;
	}
	//Enable EPWM3 (Cap Voltage Comparator) if component is connected
	if(AC_INPUT_VOLTAGE_COMPONENT)
	{
		PieVectTable.EPWM2_TZ_INT = &epwm2_isr;
		PieCtrlRegs.PIEIER2.bit.INTx2 = 0x1;
	}
	//Enable EPWM2 (AC Input Voltage Comparator) if component is connected
	if(CAP_VOLTAGE_COMPONENT)
	{
		PieVectTable.EPWM3_TZ_INT = &epwm3_isr;
		PieCtrlRegs.PIEIER2.bit.INTx3 = 0x1;
	}
	EDIS;

    IER |= M_INT2; //Enable group 2 interrupts
    IER |= M_INT3; //Enable group 3 interrupts

    // Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
}


interrupt void realclock_isr(void)
{
	//asm ("      ESTOP0");
	addSeconds(1);
	realtime++;
#ifdef _TEST
		realClockTest();

#endif
	if(realtime > WATCHDOG_TIME)
	{
		watchdog_Event();
		realtime = 0;
	}

	EALLOW;
	EPwm12Regs.ETCLR.bit.INT = 0x1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
	EDIS;
}//END ISR

interrupt void epwm2_isr(void)
{
#ifdef _TEST
	unsigned char ucChar;
	int16_t adcSample;
	char adcValString[16];

	int firstTime = 0;

	if(firstTime == 0)
	{
		firstTime = 1;
		redirOut();
	}
	//asm ("      ESTOP0");
#endif
	//asm ("      ESTOP0");
	if(EPwm2Regs.TZFLG.bit.DCBEVT1 & (EPwm2Regs.TZFLG.bit.DCAEVT1 == 0))
	{
#ifdef _TEST
        adcSample = sampleADC(AC_VOLT);
    	convertADC(adcSample,AC_VOLT, adcValString);
    	printf(" AC Voltage Back in range - ADC Value = %sV", adcValString);
    	ucChar = 10;
    	putchar(ucChar);
    	ucChar = 13;
    	putchar(ucChar);
#endif
    	LoadSwitch(AC_LOAD, LOAD_OPEN);
    	EALLOW;
		EPwm2Regs.TZEINT.bit.DCBEVT1 = 0x0;
		EPwm2Regs.TZEINT.bit.DCAEVT1 = 0x1;
		EDIS;

	}
	else if(EPwm2Regs.TZFLG.bit.DCAEVT1 & (EPwm2Regs.TZFLG.bit.DCBEVT1 == 0))
	{
#ifdef _TEST
		adcSample = sampleADC(AC_VOLT);
		convertADC(adcSample,AC_VOLT, adcValString);
		printf(" AC Voltage Out of range (High) -  ADC Value = %sV", adcValString);
		ucChar = 10;
		putchar(ucChar);
		ucChar = 13;
		putchar(ucChar);
#endif
		LoadSwitch(AC_LOAD, LOAD_CLOSED);
    	voltageCurrent_Event(VOLTAGE_SPIKE);
		EALLOW;
		EPwm2Regs.TZEINT.bit.DCBEVT1 = 0x1;
		EPwm2Regs.TZEINT.bit.DCAEVT1 = 0x0;
		EDIS;
	}
	else
		asm ("          NOP");

	//Clear flags
	EALLOW;
	EPwm2Regs.TZCLR.bit.DCAEVT1 = 0x1;
	EPwm2Regs.TZCLR.bit.DCBEVT1 = 0x1;
	EPwm2Regs.TZCLR.bit.INT = 0x1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
	EDIS;

}//END ISR


interrupt void epwm1_isr(void)
{
#ifdef _TEST
	unsigned char ucChar;
	int16_t adcSample;
	char adcValString[16];

	int firstTime = 0;

	if(firstTime == 0)
	{
		firstTime = 1;
		redirOut();
	}
	//asm ("      ESTOP0");
#endif
	//asm ("      ESTOP0");

	if(EPwm1Regs.TZFLG.bit.DCBEVT1 & (EPwm1Regs.TZFLG.bit.DCAEVT1 == 0))
	{
#ifdef _TEST
        adcSample = sampleADC(SOL_VOLT);
    	convertADC(adcSample,SOL_VOLT, adcValString);
    	printf(" Solar Voltage Back in range - ADC Value = %sV", adcValString);
    	ucChar = 10;
    	putchar(ucChar);
    	ucChar = 13;
    	putchar(ucChar);
#endif
    	LoadSwitch((Load_Switch)SOL_LOAD, LOAD_OPEN);
    	EALLOW;
		EPwm1Regs.TZEINT.bit.DCBEVT1 = 0x0;
		EPwm1Regs.TZEINT.bit.DCAEVT1 = 0x1;
		EDIS;

	}
	else if(EPwm1Regs.TZFLG.bit.DCAEVT1 & (EPwm1Regs.TZFLG.bit.DCBEVT1 == 0))
	{
#ifdef _TEST
		adcSample = sampleADC(SOL_VOLT);
		convertADC(adcSample,SOL_VOLT, adcValString);
		printf(" Solar Voltage Out of range (High) -  ADC Value = %sV", adcValString);
		ucChar = 10;
		putchar(ucChar);
		ucChar = 13;
		putchar(ucChar);
#endif
		LoadSwitch((Load_Switch)SOL_LOAD, LOAD_CLOSED);
    	voltageCurrent_Event(VOLTAGE_SPIKE);
		EALLOW;
		EPwm1Regs.TZEINT.bit.DCBEVT1 = 0x1;
		EPwm1Regs.TZEINT.bit.DCAEVT1 = 0x0;
		EDIS;
	}
	else
		asm ("          NOP");

	//Clear flags
	EALLOW;
	EPwm1Regs.TZCLR.bit.DCAEVT1 = 0x1;
	EPwm1Regs.TZCLR.bit.DCBEVT1 = 0x1;
	EPwm1Regs.TZCLR.bit.INT = 0x1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
	EDIS;

}//END ISR


interrupt void epwm3_isr(void)
{
	int16_t ACAdc, SolAdc;

#ifdef _TEST
	unsigned char ucChar;
	int16_t adcSample;
	char adcValString[16];

	int firstTime = 0;

	if(firstTime == 0)
	{
		firstTime = 1;
		redirOut();
	}
	asm ("      ESTOP0");
#endif


	if((EPwm3Regs.TZFLG.bit.DCBEVT1 & EPwm3Regs.TZFLG.bit.DCBEVT2) & ((EPwm3Regs.TZFLG.bit.DCAEVT1 == 0) & (EPwm3Regs.TZFLG.bit.DCAEVT2 == 0)))
	{
		//asm ("      ESTOP0");
#ifdef _TEST
        adcSample = sampleADC(CAP_VOLT);
    	convertADC(adcSample,CAP_VOLT, adcValString);
    	printf(" Capacitor Voltage Back in range - ADC Value = %sV", adcValString);
    	ucChar = 10;
    	putchar(ucChar);
    	ucChar = 13;
    	putchar(ucChar);
#endif

    	//Check to see if AC Input or Solar Input has power
    	ACAdc = sampleADC(AC_VOLT);
    	SolAdc = sampleADC(SOL_VOLT);
    	//If Solar has power turn off other loads and use only Solar
    	if(SolAdc > 100)
    	{
    		LoadSwitch((Load_Switch)SOL_LOAD, LOAD_OPEN);
    		LoadSwitch((Load_Switch)AC_LOAD, LOAD_CLOSED);
    		LoadSwitch((Load_Switch)HYDRO_LOAD, LOAD_CLOSED);
    	}
    	else
    	{
    		LoadSwitch((Load_Switch)SOL_LOAD, LOAD_CLOSED);
    		LoadSwitch((Load_Switch)AC_LOAD, LOAD_OPEN);
    		LoadSwitch((Load_Switch)HYDRO_LOAD, LOAD_CLOSED);
    	}
    	EALLOW;
		EPwm3Regs.TZEINT.bit.DCBEVT1 = 0x0;
		EPwm3Regs.TZEINT.bit.DCBEVT2 = 0x0;
		EPwm3Regs.TZEINT.bit.DCAEVT1 = 0x1;
		EPwm3Regs.TZEINT.bit.DCAEVT2 = 0x1;
		EDIS;

	}
	else if(EPwm3Regs.TZFLG.bit.DCAEVT1 & (EPwm3Regs.TZFLG.bit.DCBEVT1 == 0) & (EPwm3Regs.TZEINT.bit.DCAEVT1))
	{
		//asm ("      ESTOP0");
#ifdef _TEST
		adcSample = sampleADC(CAP_VOLT);
		convertADC(adcSample,CAP_VOLT, adcValString);
		printf(" Capacitor Voltage Out of range (High) -  ADC Value = %sV", adcValString);
		ucChar = 10;
		putchar(ucChar);
		ucChar = 13;
		putchar(ucChar);
#endif
		//Close all loads to prevent overcharging of the capacitor
		LoadSwitch(AC_LOAD,LOAD_CLOSED);
		LoadSwitch(SOL_LOAD,LOAD_CLOSED);
		LoadSwitch(HYDRO_LOAD,LOAD_CLOSED);

		EALLOW;
		EPwm3Regs.TZEINT.bit.DCBEVT1 = 0x1;
		EPwm3Regs.TZEINT.bit.DCBEVT2 = 0x1;
		EPwm3Regs.TZEINT.bit.DCAEVT1 = 0x0;
		EPwm3Regs.TZEINT.bit.DCAEVT2 = 0x1;
		EDIS;
	}
	else if(EPwm3Regs.TZFLG.bit.DCAEVT2 & (EPwm3Regs.TZFLG.bit.DCBEVT2 == 0) & (EPwm3Regs.TZEINT.bit.DCAEVT2))
	{
		//asm ("      ESTOP0");
#ifdef _TEST
		adcSample = sampleADC(CAP_VOLT);
		convertADC(adcSample,CAP_VOLT, adcValString);
		printf(" Capacitor Voltage Out of range (Low) -  ADC Value = %sV", adcValString);
		ucChar = 10;
		putchar(ucChar);
		ucChar = 13;
		putchar(ucChar);
#endif
		//Open all loads in hopes of getting power
		LoadSwitch(AC_LOAD,LOAD_OPEN);
		LoadSwitch(SOL_LOAD,LOAD_OPEN);
		LoadSwitch(HYDRO_LOAD,LOAD_OPEN);

		critical_Power_Event();
		//NOTE: Need to add hibernation state

		EALLOW;
		EPwm3Regs.TZEINT.bit.DCBEVT1 = 0x1;
		EPwm3Regs.TZEINT.bit.DCBEVT2 = 0x1;
		EPwm3Regs.TZEINT.bit.DCAEVT1 = 0x1;
		EPwm3Regs.TZEINT.bit.DCAEVT2 = 0x0;
		EDIS;
	}
	else
		asm ("          NOP");

	//Clear flags
	EALLOW;
	EPwm3Regs.TZCLR.bit.DCAEVT1 = 0x1;
	EPwm3Regs.TZCLR.bit.DCAEVT2 = 0x1;
	EPwm3Regs.TZCLR.bit.DCBEVT1 = 0x1;
	EPwm3Regs.TZCLR.bit.DCBEVT2 = 0x1;
	EPwm3Regs.TZCLR.bit.INT = 0x1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
	EDIS;

}//END ISR

//###########################################################################

