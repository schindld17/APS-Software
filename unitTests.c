//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	unitTests.c
//
//	TITLE: Unit/Sub-System Test Functions
//
//	DESCRIPTION: Basic unit tests used to determine specific system functionality
//
//
//
//
//
//###########################################################################
//	Author: Dylan Schindler
//	Creation Date: Mar 26, 2016 
//	Release Date:
//###########################################################################

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "F2837xS_device.h"
#include "APS_GlobalPrototypes.h"


//*************************************************************************************************************************
//NAME: basicFuctionalityTest
//
//DESC: Initialize GPIO pins 12/13 for LEDs on Launchpad and toggle them to demonstrate system initlization and program
// 		linking has completed correctly.
//
//DATE: 26 March 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void basicFuctionalityTest(void)
{
	int loopCount = 0;

	//Initialize GPIOs for the LEDs and turn them off
	    EALLOW;
	    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
	    GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;
	    GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;
	    GpioDataRegs.GPACLEAR.bit.GPIO13 = 1;
	    EDIS;

	    //Toggle LEDS
	    EALLOW;
	    for(loopCount = 0;loopCount < 100; loopCount++)
	    {
	        GpioDataRegs.GPATOGGLE.bit.GPIO12 = 1;
	        DELAY_US(50000);
	        GpioDataRegs.GPATOGGLE.bit.GPIO13 = 1;
	        DELAY_US(50000);
	    }//END FOR

}//END FUNCTION



//###########################################################################

