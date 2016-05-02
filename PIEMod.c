//#############################################################################
//	PROJECT: APS-Software
//
//	FILE:	PIEMod.c
//
//	TITLE: Peripheral Interrupt Service Routines
//
//	DESCRIPTION: Multiple ISRs (Interrupt Service Routines) that are used with
//	the perhipheral interrupts to provide system control and event logging
//	capabilites. Funtions and ISRs included are an initialization function
//	that initializes and configures the peripheral interrupt architecture. An
//	ISR that increments the current time within the system by one second and
//	generates a watchdog event at specified intervals. An ISR that services
// 	the enhanced pulse width modulation (ePWM) peripheral connected to the
//	AC power input soure when a voltage spike has occurred on that power line.
//	An ISR that services the ePWM peripheral connected to the Solar panal
// 	power input source when a voltage spike has occurred on that power line.
//	An ISR that services the ePWM peripheral connected to the Hydrogen Fuel
//	Cell power input source when a votlage spike has coccured on that power
//	line.
//
//	NOTE: Numerous ISRs are not included in this revision of the software and
//	need to be added as necessary
//
//#############################################################################
//	Author: Dylan Schindler
//	Creation Date: 	Apr 10, 2016
//	Release Date:	May 02, 2016
//#############################################################################

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <file.h>
#include "F2837xS_device.h"
#include "APS_GlobalPrototypes.h"
#include "APS_GlobalDefs.h"
#include "F2837xS_Examples.h"
#include "F2837xS_Pie_defines.h"


//*****************************************************************************
//NAME: APSPieInit
//
//DESC: Initialize currently included peripheral interrupts and enable those
//		interrupts for use within the system
//
//DATE: 10 April 2016
//
//AUTHOR: Dylan Schindler
//******************************************************************************
void APSPieInit(void)
{
	EALLOW;

	//Start ePWM modules
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0x1;

	//Enable ePWM12 interrupts within PIE Vector Table and connect it to the
	//realclock_isr defined within PIEMod.c
	PieVectTable.EPWM12_INT = &realclock_isr;
	PieCtrlRegs.PIEIER3.bit.INTx12 = 0x1;

	//Enable EPWM1 (Solar Input Voltage Comparator) interrupts within the PIE
	//Vector Table if component is connected
	if(SOLAR_INPUT_VOLTAGE_COMPONENT)
	{
		PieVectTable.EPWM1_TZ_INT = &epwm1_isr;
		PieCtrlRegs.PIEIER2.bit.INTx1 = 0x1;
	}
	//Enable EPWM3 (Capcitor Voltage Comparator) interrupts within the PIE
	//Vector Table if component is connected
	if(AC_INPUT_VOLTAGE_COMPONENT)
	{
		PieVectTable.EPWM2_TZ_INT = &epwm2_isr;
		PieCtrlRegs.PIEIER2.bit.INTx2 = 0x1;
	}
	//Enable EPWM2 (AC Input Voltage Comparator) interrupts within the PIE
	//Vector Table if component is connected
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

//****************************************************************************
//NAME: realclock_isr
//
//DESC: Update the current system date/time by 1 second and generate a
//		watchdog event if the period determined by the WATCHDOG_TIME symbol
//		defined in APS_GlobalDefs.h has expired.
//
//DATE: 10 April 2016
//
//AUTHOR: Dylan Schindler
//*****************************************************************************
interrupt void realclock_isr(void)
{
	//Call addSeconds() function defined in RealClockMod.c to update the
	//current system date and time
	addSeconds(1);
	//Update global variable to control the number of times this ISR has been
	//called
	realtime++;
#ifdef _TEST
		realClockTest();

#endif
	//If this is true than the period has expired and the realtime counter
	//needs to be reset and a watchdog event needs to be generated
	if(realtime > WATCHDOG_TIME)
	{
		//Call the watchdog_Event() function defined in eventLog.c
		watchdog_Event();
		realtime = 0;
	}

	//Acknowledge the interrupt and clear necessary flags
	EALLOW;
	EPwm12Regs.ETCLR.bit.INT = 0x1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
	EDIS;
}//END ISR

//*****************************************************************************
//NAME: epwm2_isr
//
//DESC: This ISR is called when the ePWM2 peripheral sets the Digital Compare
//		B Event 1 or Digital Compare A Event 1 flags. These flags are set when
//		the CMPSS peripheral connected to ePWM2 crosses its given threshold.
//		The AC Input Voltage (CMPSS3) (Block B on System-Block Diagram) input
//		power source is used with ePWM2. This ISR should only be active when
//		a voltage spike has been observed or the voltage has returned to nominal
//		values after a spike has occurred. This ISR generates a voltage spike
//		event and switches on or off the AC input power source load switch.
//
//DATE: 17 April 2016
//
//AUTHOR: Dylan Schindler
//*****************************************************************************
interrupt void epwm2_isr(void)
{
	//If the _TEST symbol is defined then a simple test of the functionality of
	//this ISR will be completed when the ISR is called. This symbol is defined
	//within the predefind symbols options for the build configuration of the
	//project.
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
#endif

	//If this is true then the voltage has returned to a nominal value
	if(EPwm2Regs.TZFLG.bit.DCBEVT1 & (EPwm2Regs.TZFLG.bit.DCAEVT1 == 0))
	{
		//If the _TEST symbol, described above, is defined then output
		//through the SCI pins(UART) the value of voltage measured on CMPSS
		//pin where the spike occured.
#ifdef _TEST
        adcSample = sampleADC(AC_VOLT);
    	convertADC(adcSample,AC_VOLT, adcValString);
    	printf(" AC Voltage Back in range - ADC Value = %sV", adcValString);
    	ucChar = 10;
    	putchar(ucChar);
    	ucChar = 13;
    	putchar(ucChar);
#endif
    	//It is safe to open the AC input load switch
    	LoadSwitch(AC_LOAD, LOAD_OPEN);

    	//Enable the overvoltage interrupt and disable the back in range interrupt
    	EALLOW;
		EPwm2Regs.TZEINT.bit.DCBEVT1 = 0x0;
		EPwm2Regs.TZEINT.bit.DCAEVT1 = 0x1;
		EDIS;

	}
	else if(EPwm2Regs.TZFLG.bit.DCAEVT1 & (EPwm2Regs.TZFLG.bit.DCBEVT1 == 0))
	{
		//If the _TEST symbol, described above, is defined then output
		//through the SCI pins(UART) the value of voltage measured on CMPSS
		//pin where the spike occured.
#ifdef _TEST
		adcSample = sampleADC(AC_VOLT);
		convertADC(adcSample,AC_VOLT, adcValString);
		printf(" AC Voltage Out of range (High) -  ADC Value = %sV", adcValString);
		ucChar = 10;
		putchar(ucChar);
		ucChar = 13;
		putchar(ucChar);
#endif
		//Protect the system and turn off the AC input load switch
		LoadSwitch(AC_LOAD, LOAD_CLOSED);

		//Generate a voltage spike event using the voltageCurrent_Event()
		//function defined in eventLog.c
    	voltageCurrent_Event(VOLTAGE_SPIKE);

    	//Enable the the back in range interrupt  and disable the overvoltage
    	//interrupt
		EALLOW;
		EPwm2Regs.TZEINT.bit.DCBEVT1 = 0x1;
		EPwm2Regs.TZEINT.bit.DCAEVT1 = 0x0;
		EDIS;
	}
	else
		asm ("          NOP");

	//Acknowledge the interrupt and clear necessary flags
	EALLOW;
	EPwm2Regs.TZCLR.bit.DCAEVT1 = 0x1;
	EPwm2Regs.TZCLR.bit.DCBEVT1 = 0x1;
	EPwm2Regs.TZCLR.bit.INT = 0x1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
	EDIS;

}//END ISR

//******************************************************************************
//NAME: epwm1_isr
//
//DESC: This ISR is called when the ePWM1 peripheral sets the Digital Compare
//		B Event 1 or Digital Compare A Event 1 flags. These flags are set when
//		the CMPSS peripheral connected to ePWM1 crosses its given threshold.
//		The Solar Input Voltage (CMPSS6) (Block N on System-Block Diagram) input
//		power source is used with ePWM1. This ISR should only be active when
//		a voltage spike has been observed or the voltage has returned to nominal
//		values after a spike has occurred. This ISR generates a voltage spike
//		event and switches on or off the solar input power source load switch.
//
//DATE: 17 April 2016
//
//AUTHOR: Dylan Schindler
//*******************************************************************************
interrupt void epwm1_isr(void)
{
	//If the _TEST symbol is defined then a simple test of the functionality of
	//this ISR will be completed when the ISR is called. This symbol is defined
	//within the predefind symbols options for the build configuration of the
	//project.
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
#endif

	if(EPwm1Regs.TZFLG.bit.DCBEVT1 & (EPwm1Regs.TZFLG.bit.DCAEVT1 == 0))
	{
		//If the _TEST symbol, described above, is defined then output
		//through the SCI pins(UART) the value of voltage measured on CMPSS
		//pin where the spike occured.
#ifdef _TEST
        adcSample = sampleADC(SOL_VOLT);
    	convertADC(adcSample,SOL_VOLT, adcValString);
    	printf(" Solar Voltage Back in range - ADC Value = %sV",
    			adcValString);
    	ucChar = 10;
    	putchar(ucChar);
    	ucChar = 13;
    	putchar(ucChar);
#endif

    	//It is safe to open the solar panel input load switch
    	LoadSwitch((Load_Switch)SOL_LOAD, LOAD_OPEN);

    	//Enable the overvoltage interrupt and disable the back in
    	//range interrupt
    	EALLOW;
		EPwm1Regs.TZEINT.bit.DCBEVT1 = 0x0;
		EPwm1Regs.TZEINT.bit.DCAEVT1 = 0x1;
		EDIS;

	}
	else if(EPwm1Regs.TZFLG.bit.DCAEVT1 & (EPwm1Regs.TZFLG.bit.DCBEVT1 == 0))
	{
		//If the _TEST symbol, described above, is defined then output
		//through the SCI pins(UART) the value of voltage measured on CMPSS
		//pin where the spike occured.
#ifdef _TEST
		adcSample = sampleADC(SOL_VOLT);
		convertADC(adcSample,SOL_VOLT, adcValString);
		printf(" Solar Voltage Out of range (High) -  ADC Value = %sV",
				adcValString);
		ucChar = 10;
		putchar(ucChar);
		ucChar = 13;
		putchar(ucChar);
#endif
		//Protect the system and turn off the solar panel input load switch
		LoadSwitch((Load_Switch)SOL_LOAD, LOAD_CLOSED);

		//Generate a voltage spike event using the voltageCurrent_Event()
		//function defined in eventLog.c
    	voltageCurrent_Event(VOLTAGE_SPIKE);

    	//Enable the the back in range interrupt  and disable the overvoltage
    	//interrupt
		EALLOW;
		EPwm1Regs.TZEINT.bit.DCBEVT1 = 0x1;
		EPwm1Regs.TZEINT.bit.DCAEVT1 = 0x0;
		EDIS;
	}
	else
		asm ("          NOP");

	//Acknowledge the interrupt and clear necessary flags
	EALLOW;
	EPwm1Regs.TZCLR.bit.DCAEVT1 = 0x1;
	EPwm1Regs.TZCLR.bit.DCBEVT1 = 0x1;
	EPwm1Regs.TZCLR.bit.INT = 0x1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
	EDIS;

}//END ISR

//******************************************************************************
//NAME: epwm3_isr
//
//DESC: This ISR is called when the ePWM3 peripheral sets the Digital Compare
//		B Event 1/2 or Digital Compare A Event 1/2 flags. These flags are set
//		when the CMPSS peripheral connected to ePWM3 crosses its given threshold.
//		The Capacitor Voltage (CMPSS8) (Block L on System-Block Diagram)is used
//		with ePWM3. This ISR should only be active when a the capcitor is about
//		to be overcharged or the system has reach a critical power level and the
//		capicitor is outputting a critically low voltage. This ISR generates a
//		either a critical power loss event and switches on all input power sources
//		and puts the system into a low-power state or shuts off all input power
//		sources to let the capcitor drain down to prevent overcharging.
//
//DATE: 17 April 2016
//
//AUTHOR: Dylan Schindler
//*******************************************************************************
interrupt void epwm3_isr(void)
{
	int16_t ACAdc, SolAdc;
	//If the _TEST symbol is defined then a simple test of the functionality of
	//this ISR will be completed when the ISR is called. This symbol is defined
	//within the predefind symbols options for the build configuration of the
	//project.
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
#endif

	//If this is true than the capictor is below the overcharge limit and
	//above the critical power loss voltage
	if((EPwm3Regs.TZFLG.bit.DCBEVT1 & EPwm3Regs.TZFLG.bit.DCBEVT2) &
			((EPwm3Regs.TZFLG.bit.DCAEVT1 == 0) &
					(EPwm3Regs.TZFLG.bit.DCAEVT2 == 0)))
	{
		//If the _TEST symbol, described above, is defined then output
		//through the SCI pins(UART) the value of voltage measured on CMPSS
		//pin where the spike occured.
#ifdef _TEST
        adcSample = sampleADC(CAP_VOLT);
    	convertADC(adcSample,CAP_VOLT, adcValString);
    	printf(" Capacitor Voltage Back in range - ADC Value = %sV",
adcValString);
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
    	//Enable the overcharge and critical power loss interrupts
    	//and disable the back in-range interrupts
    	EALLOW;
		EPwm3Regs.TZEINT.bit.DCBEVT1 = 0x0;
		EPwm3Regs.TZEINT.bit.DCBEVT2 = 0x0;
		EPwm3Regs.TZEINT.bit.DCAEVT1 = 0x1;
		EPwm3Regs.TZEINT.bit.DCAEVT2 = 0x1;
		EDIS;

	}
	//If this is true than the capcitor is about to be in an overcharge
	//situation
	else if(EPwm3Regs.TZFLG.bit.DCAEVT1 & (EPwm3Regs.TZFLG.bit.DCBEVT1 == 0)
			& (EPwm3Regs.TZEINT.bit.DCAEVT1))
	{
		//If the _TEST symbol, described above, is defined then output
		//through the SCI pins(UART) the value of voltage measured on CMPSS
		//pin where the spike occured.
#ifdef _TEST
		adcSample = sampleADC(CAP_VOLT);
		convertADC(adcSample,CAP_VOLT, adcValString);
		printf(" Capacitor Voltage Out of range (High) -  ADC Value = %sV",
				adcValString);
		ucChar = 10;
		putchar(ucChar);
		ucChar = 13;
		putchar(ucChar);
#endif
		//Close all loads to prevent overcharging of the capacitor
		LoadSwitch(AC_LOAD,LOAD_CLOSED);
		LoadSwitch(SOL_LOAD,LOAD_CLOSED);
		LoadSwitch(HYDRO_LOAD,LOAD_CLOSED);

		//Enable all interrupts except for the overcharge interrupt
		EALLOW;
		EPwm3Regs.TZEINT.bit.DCBEVT1 = 0x1;
		EPwm3Regs.TZEINT.bit.DCBEVT2 = 0x1;
		EPwm3Regs.TZEINT.bit.DCAEVT1 = 0x0;
		EPwm3Regs.TZEINT.bit.DCAEVT2 = 0x1;
		EDIS;
	}
	//If this is true than the capcitor is below a certain limit and a
	//critical power loss event needs to be registerd
	else if(EPwm3Regs.TZFLG.bit.DCAEVT2 & (EPwm3Regs.TZFLG.bit.DCBEVT2 == 0)
			& (EPwm3Regs.TZEINT.bit.DCAEVT2))
	{
		//If the _TEST symbol, described above, is defined then output
		//through the SCI pins(UART) the value of voltage measured on CMPSS
		//pin where the spike occured.
#ifdef _TEST
		adcSample = sampleADC(CAP_VOLT);
		convertADC(adcSample,CAP_VOLT, adcValString);
		printf(" Capacitor Voltage Out of range (Low) -  ADC Value = %sV",
				adcValString);
		ucChar = 10;
		putchar(ucChar);
		ucChar = 13;
		putchar(ucChar);
#endif
		//Open all loads in hopes of getting power
		LoadSwitch(AC_LOAD,LOAD_OPEN);
		LoadSwitch(SOL_LOAD,LOAD_OPEN);
		LoadSwitch(HYDRO_LOAD,LOAD_OPEN);

		//Generate a critical power loss event by calling critical_Power_Event()
		//defined in eventLog.c
		critical_Power_Event();

		//NOTE: Need to add hibernation or other low power state

		//Enable all interrupts except for the critical power loss interrupt
		EALLOW;
		EPwm3Regs.TZEINT.bit.DCBEVT1 = 0x1;
		EPwm3Regs.TZEINT.bit.DCBEVT2 = 0x1;
		EPwm3Regs.TZEINT.bit.DCAEVT1 = 0x1;
		EPwm3Regs.TZEINT.bit.DCAEVT2 = 0x0;
		EDIS;
	}
	else
		asm ("          NOP");

	//Acknowledge the interrupt and clear necessary flags
	EALLOW;
	EPwm3Regs.TZCLR.bit.DCAEVT1 = 0x1;
	EPwm3Regs.TZCLR.bit.DCAEVT2 = 0x1;
	EPwm3Regs.TZCLR.bit.DCBEVT1 = 0x1;
	EPwm3Regs.TZCLR.bit.DCBEVT2 = 0x1;
	EPwm3Regs.TZCLR.bit.INT = 0x1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
	EDIS;

}//END ISR

//##############################################################################

