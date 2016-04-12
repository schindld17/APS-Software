//###########################################################################
//
// FILE:	main.c
//
// TITLE:	Main System File
//
// DESCRIPTION:
//
//
//
//
//
//###########################################################################
// Author: Dylan Schindler
// Release Date:
//###########################################################################
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "F2837xS_device.h"
#include "APS_GlobalPrototypes.h"


void InitSysCtrl(void);

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


    //Initialize GPIO pins
    GPIOInit();

    //Initialize ADC Module
    ADCInit();

    //Initialize Comparator Module
    CMPSSInit();
    EPWMInit();

    initsci();

#ifndef _INT_ON
    // Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
#else
    //Initialize custom PIE Interrupt Mappings
    APSPieInit();
#endif

#ifdef _INT_TEST
    while(1)
    {
        asm ("          NOP");
    }
#endif


#ifdef _BASICTEST

while(1)
{
	sciTestwComp(AC_VOLT);

}
#endif

}
