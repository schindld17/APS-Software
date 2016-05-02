//###########################################################################
//
// FILE:	main.c
//
// TITLE:	Main System File
//
// DESCRIPTION: Multiple functions to control the primary system functions.
//				Functions include the main function wherein all system
//				initialization support functions are called and the program
//				operates until an ISR is entered. A function used on boot
//				to determine which input power sources are currently in use.
//
//###########################################################################
// 	Author: Dylan Schindler
//	Creation Date: 	Mar 23, 2016
//	Release Date:	May 02, 2016
//###########################################################################
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "F2837xS_device.h"
#include "APS_GlobalPrototypes.h"

//Declarations of global variables defined within APS_GlobalDefs.h
int32_t realtime = 0;
char provided_date[11] = __DATE__;
char provided_time[8] = __TIME__;
char month[3];
char day[2];
char year[4];
char hour[2];
char minute[2];
char second[2];
JULIANTIME julianTime;


//*****************************************************************************
//NAME: main
//
//DESC: Main program
//
//DATE: 23 March 2016
//
//AUTHOR: Dylan Schindler
//*****************************************************************************
int main(void)
{
    // If running from flash copy RAM only functions to RAM
#ifdef _FLASH

	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif

    InitSysCtrl();

	//Clear all interrupts and initalize PIE vector table.
	//Disable CPU interrupts.
	DINT;

	//Initialize PIE control registers to their default state.
	//The default state is all PIE interrupts disabled and flags
	// are cleared.
	//This function can be found in the F2806x_PieCtrl.c file
	InitPieCtrl();

	//Disable CPU interrupts and clear all CPU interrupt flags
	IER = 0x0000;
	IFR = 0x0000;

    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2806x_DefaultIsr.c.
    // This function is found in F2806x_PieVect.c.
    InitPieVectTable();

    setTime();


    //Initialize GPIO pins by calling GPIOInit() function defined in
    //GPIOMod.c
    GPIOInit();

    //Initialize ADC peripherals by calling ADCInit() function defined
    //in ADCMod.c
    ADCInit();

    //Initialize Comparator/ ePWM peripherals by calling CMPSSInit() and
    //EPWMInit() functions defined in CMPSSMod.c
    CMPSSInit();
    EPWMInit();

    //Initialize SCI to be used for output to local station
    //NOTE: This should be placed inside of a #define statement as this
    //function should not be called if a demonstration is not taking place
    initsci();

    //If these symbols, defined in APS_GlobalDefs.h, are set to true then
    //either component is enabled and in use by the system
if(AC_INPUT_VOLTAGE_COMPONENT | SOLAR_INPUT_VOLTAGE_COMPONENT)
	//Determine which input power source by calling the systemBoot() function
	//defined in main.c
    systemBoot();

	//Initialize the ePWM to be used for the real-time clock by calling the
	//RClock_ePWMInit() function defined in RealClockMod.c
    RClock_ePWMInit();


#ifndef _INT_ON
    // Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
#else
    //Initialize custom PIE Interrupt Mappings
    APSPieInit();
#endif

    //Generate a boot-up event by calling the boot_Event() function defined in
    //eventLog.c
    boot_Event();

#ifdef _INT_TEST
    while(1)
    {
        asm ("          NOP");
    }//END WHILE
#endif

//If this symbol is defined then a unit test is most likely going to be used.
//This symbol is defined within the predefined symbols options for the build
//configuration of the project.
#ifdef _BASICTEST

while(1)
{
	sciTestwADC(AC_VOLT);
}
#endif

}//END FUNCTION

//*****************************************************************************
//NAME: systemBoot
//
//DESC: Function to determine which input power sources are currently in use.
//		This function will give priority to the Solar input power source, so
//		if adequate voltage is coming from this source than all other sources
//		will be turned off to prevent overcharging of the capacitor and to
//		protect the system from any uncessary voltage/current spike conditions.
//
//DATE: 17 April 2016
//
//AUTHOR: Dylan Schindler
//*****************************************************************************
void systemBoot(void)
{
	int16_t ACAdc, SolAdc;

	//Check to see if AC Input or Solar Input has power
	ACAdc = sampleADC(AC_VOLT);
	SolAdc = sampleADC(SOL_VOLT);

	if((ACAdc > 100) | (SolAdc > 100))
	{
		//If Solar has power turn off other loads and use only Solar
		if(SolAdc > 100)
		{
			LoadSwitch((Load_Switch)SOL_LOAD, LOAD_OPEN);
			LoadSwitch((Load_Switch)AC_LOAD, LOAD_CLOSED);
			LoadSwitch((Load_Switch)HYDRO_LOAD, LOAD_CLOSED);
		}//END IF
		else
		{
			LoadSwitch((Load_Switch)SOL_LOAD, LOAD_CLOSED);
			LoadSwitch((Load_Switch)AC_LOAD, LOAD_OPEN);
			LoadSwitch((Load_Switch)HYDRO_LOAD, LOAD_CLOSED);
		}//END ELSE
		return;

	}//END ELSE
	else
		return;

}//END FUNCTION
