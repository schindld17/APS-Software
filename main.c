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
#include "F2837xS_device.h"

int main(void)
{
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

    //Initialize GPIOs for the LEDs and turn them off
    EALLOW;
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;
    GpioDataRegs.GPADAT.bit.GPIO12 = 1;
    GpioDataRegs.GPADAT.bit.GPIO13 = 1;
    EDIS;

    // Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM

    //Toggle LEDS
    for(;;)
    {
        GpioDataRegs.GPATOGGLE.bit.GPIO12 = 1;
        GpioDataRegs.GPATOGGLE.bit.GPIO13 = 1;
        DELAY_US(50000);
    }
	
	return 0;
}
