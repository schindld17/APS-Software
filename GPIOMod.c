//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	GPIOMod.c
//
//	TITLE: GPIO Support Functions
//
//	DESCRIPTION: Multiple support functions for the GPIO pins on-board.
//	Functions include an initliazation function to set the proper states
//	of the GPIO pins used by the system. A function to turn a GPIO pin
//	connected to a load switch circuit on or off. A function to check the
//	status of a GPIO pin connected to a load switch circuit to test if it
//	is currently active.
//
//	NOTE: There needs to be a function added to allow 8 GPIO pins to be used
//	as basic communication pins to the external camera systems. There also
//	needs to be a function added to allow for 1 GPIO pin to be used to turn
//	on or off the hydrogen fuel cell (Logic high will communicate to the
//	hydrogen fuel cell that it needs to turn on, logic low will communicate
//	to the fuel cell to turn off and remain off).
//
//###########################################################################
//	Author: Dylan Schindler
//	Creation Date: 	Apr 06, 2016
//	Release Date: 	May 02, 2016
//###########################################################################

#include "F2837xS_device.h"
#include "APS_GlobalDefs.h"
#include "APS_GlobalPrototypes.h"


//***************************************************************************
//NAME: GPIOInit
//
//DESC: Initialize all GPIO pins that will be used by the system as either
//		output or inputs and set the default values of the pins.
//
//NOTE: Currently function only initializes pins that will be connected to
//		the load switch circuits within the system. If the program is being
//		run on a Launchpad then different GPIO pins will need to be used than
//		those defined in the system pin out diagram.
//
//DATE: 6 April 2016
//
//AUTHOR: Dylan Schindler
//***************************************************************************
void GPIOInit(void)
{
	/////////////Hydrogen Load Switch (Block S)/////////////////////////////////
	EALLOW;
#ifdef _LAUNCH
	//Initialize GPIO Pin 90 as output
	GpioCtrlRegs.GPCDIR.bit.GPIO90 = 1;
	//Turn on GPIO Pin 90 (Open Load Switch)
	LoadSwitch((Load_Switch)HYDRO_LOAD, LOAD_OPEN);
#else
	//Initialize GPIO Pin 44 as output
	GpioCtrlRegs.GPBDIR.bit.GPIO44 = 1;
	//Turn on GPIO Pin 44 (Open Load Switch)
	GpioDataRegs.GPBSET.bit.GPIO44 = LOAD_OPEN;
#endif
	EDIS;
	/////////////Solar Load Switch (Block M)////////////////////////////////////
	EALLOW;
#ifdef _LAUNCH
	//Initialize GPIO Pin 89 as output
	GpioCtrlRegs.GPCDIR.bit.GPIO89 = 1;
	//Turn on GPIO Pin 89 (Open Load Switch)
	LoadSwitch((Load_Switch)SOL_LOAD, LOAD_OPEN);
#else
	//Initialize GPIO Pin 45 as output
	GpioCtrlRegs.GPBDIR.bit.GPIO45 = 1;
	//Turn on GPIO Pin 45 (Open Load Switch)
	GpioDataRegs.GPBSET.bit.GPIO45 = LOAD_OPEN;
#endif
	EDIS;
	/////////////AC Load Switch (Block C)///////////////////////////////////////
	EALLOW;
#ifdef _LAUNCH
	//Initialize GPIO Pin 71 as output
	GpioCtrlRegs.GPCDIR.bit.GPIO71 = 1;
	//Turn on GPIO Pin 71 (Open Load Switch)
	LoadSwitch((Load_Switch)AC_LOAD, LOAD_OPEN);
#else
	//Initialize GPIO Pin 48 as output
	GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;
	//Turn on GPIO Pin 48 (Open Load Switch)
	GpioDataRegs.GPBSET.bit.GPIO48 = LOAD_OPEN;
#endif
	EDIS;
	/////////////5V Output Load Switch (Block D)/////////////////////////////////
	EALLOW;
#ifdef _LAUNCH
	//Initialize GPIO Pin 41 as output
	GpioCtrlRegs.GPBDIR.bit.GPIO41 = 1;
	//Turn on GPIO Pin 41 (Open Load Switch)
	LoadSwitch((Load_Switch)FIVE_LOAD, LOAD_OPEN);
#else
	//Initialize GPIO Pin 47 as output
	GpioCtrlRegs.GPBDIR.bit.GPIO47 = 1;
	//Turn on GPIO Pin 47 (Open Load Switch)
	GpioDataRegs.GPBSET.bit.GPIO47 = LOAD_OPEN;
#endif
	EDIS;
	/////////////12V Output Load Switch (Block T)/////////////////////////////////
	EALLOW;
#ifdef _LAUNCH
	//Initialize GPIO Pin 60 as output
	GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;
	//Turn on GPIO Pin 60 (Open Load Switch)
	LoadSwitch((Load_Switch)TWELVE_LOAD, LOAD_OPEN);
#else
	//Initialize GPIO Pin 67 as output
	GpioCtrlRegs.GPCDIR.bit.GPIO67 = 1;
	//Turn on GPIO Pin 67 (Open Load Switch)
	GpioDataRegs.GPCSET.bit.GPIO67 = LOAD_OPEN;
#endif

	EDIS;

}//END FUNCTION

//******************************************************************************
//NAME: LoadSwitch
//
//DESC: Set a GPIO pin connected to a load switch as either logic high or low.
//		The pin that is set is determined by the provided Load_Switch
//		enumeration constant while the value that the pin will be set to is
//		set by the int on parameter. If on equals 1 then the pin will be set to
//		logic high. If on does not equal 1 then the pin will be set to logic low.
//		Depending upon the configuration of the circuit (active high vs. active
//		low) this will either turn the load switch on or off. Normally this
//		function is used with the LOAD_OPEN or LOAD_CLOSED symbols defined within
//		APS_GlobalDefs.h to set the load switch to simplify active high vs. active
//		low logic.
//
//NOTE: If the program is being run on a Launchpad then different GPIO pins will
//		need to be used than those defined in the system pin out diagram.
//
//DATE: 6 April 2016
//
//AUTHOR: Dylan Schindler
//******************************************************************************
void LoadSwitch(Load_Switch loadSwitch, int on)
{
	EALLOW;

	switch (loadSwitch)
	{
	/////////////Hydrogen Load Switch (Block S)/////////////////////////////////
		case HYDRO_LOAD:
			if(on == 1)
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 90 on
				GpioDataRegs.GPCSET.bit.GPIO90 = 1;
				DELAY_US(5000);
#else
				//Turn GPIO Pin 44 on
				GpioDataRegs.GPBDAT.bit.GPIO44 = 1;
				DELAY_US(5000);
#endif
			}//END IF
			else
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 90 off
				GpioDataRegs.GPCCLEAR.bit.GPIO90 = 1;
			 	DELAY_US(5000);
#else
				//Turn GPIO Pin 44 on
				GpioDataRegs.GPBDAT.bit.GPIO44 = 1;
				DELAY_US(5000);
#endif
			}//END ELSE
			break;
	/////////////Solar Load Switch (Block M)///////////////////////////////////
		case SOL_LOAD:
			if(on == 1)
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 89 on
				GpioDataRegs.GPCSET.bit.GPIO89 = 1;
				DELAY_US(5000);
#else
				//Turn GPIO Pin 45 on
				GpioDataRegs.GPBDAT.bit.GPIO45 = 1;
				DELAY_US(5000);
#endif
			}//END IF
			else
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 89 off
				GpioDataRegs.GPCCLEAR.bit.GPIO89 = 1;
			 	DELAY_US(5000);
#else
				//Turn GPIO Pin 45 on
				GpioDataRegs.GPBDAT.bit.GPIO45 = 1;
				DELAY_US(5000);
#endif
			}//END ELSE
			break;

	/////////////AC Load Switch (Block C)/////////////////////////////////////
		case AC_LOAD:
			if(on == 1)
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 71 on
				GpioDataRegs.GPCSET.bit.GPIO71 = 1;
				DELAY_US(5000);
#else
				//Turn GPIO Pin 48 on
				GpioDataRegs.GPBDAT.bit.GPIO48 = 1;
				DELAY_US(5000);
#endif
			}//END IF
			else
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 71 off
				GpioDataRegs.GPCCLEAR.bit.GPIO71 = 1;
			 	DELAY_US(5000);
#else
				//Turn GPIO Pin 48 on
				GpioDataRegs.GPBDAT.bit.GPIO48 = 1;
				DELAY_US(5000);
#endif
			}//END ELSE
			break;
	/////////////5V Output Load Switch (Block D)/////////////////////////////
		case FIVE_LOAD:
			if(on == 1)
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 41 on
				GpioDataRegs.GPBSET.bit.GPIO41 = 1;
				DELAY_US(5000);
#else
				//Turn GPIO Pin 47 on
				GpioDataRegs.GPBDAT.bit.GPIO47 = 1;
				DELAY_US(5000);
#endif
			}//END IF
			else
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 41 off
				GpioDataRegs.GPBCLEAR.bit.GPIO41 = 1;
			 	DELAY_US(5000);
#else
				//Turn GPIO Pin 47 on
				GpioDataRegs.GPBDAT.bit.GPIO47 = 1;
				DELAY_US(5000);
#endif
			}//END ELSE
			break;
	/////////////12V Output Load Switch (Block T)////////////////////////////
		case TWELVE_LOAD:
			if(on == 1)
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 60 on
				GpioDataRegs.GPBSET.bit.GPIO60 = 1;
				DELAY_US(5000);
#else
				//Turn GPIO Pin 67 on
				GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
				DELAY_US(5000);
#endif
			}//END IF
			else
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 60 off
				GpioDataRegs.GPBCLEAR.bit.GPIO60 = 1;
			 	DELAY_US(5000);
#else
				//Turn GPIO Pin 67 on
				GpioDataRegs.GPCDAT.bit.GPIO67 = 1;
				DELAY_US(5000);
#endif
			}//END ELSE
			break;
	}//END SWITCH

    EDIS;

}//END FUNCTION

//****************************************************************************
//NAME: LoadSwitch_Status
//
//DESC: Check whether or not a GPIO pin connected to a load switch circuit is
//		set to the value that means the load switch is open (the power line
//		is being used). A Load_Switch enumeration constant is provided as a
//		parameter to the function to determine which GPIO pin to check. The
//		value of the pin is compared against the value of the LOAD_OPEN symbol
//		defined in APS_GlobalDefs.h
//
//NOTE: If a the program is being run on a Launchpad then different GPIO pins will
//		need to be used than those defined in the system pin out diagram.
//
//DATE: 24 April 2016
//
//AUTHOR: Dylan Schindler
//****************************************************************************
int LoadSwitch_Status(Load_Switch loadSwitch)
{
	EALLOW;

	//Check to see if desired load switch is open or closed
	switch (loadSwitch)
	{
	/////////////Hydrogen Load Switch (Block S)/////////////////////////////////
		case HYDRO_LOAD:
#ifdef _LAUNCH
			if(GpioDataRegs.GPCDAT.bit.GPIO90 == LOAD_OPEN)
				return 1;
			else
				return 0;
#else
			if(GpioDataRegs.GPBDAT.bit.GPIO44 == LOAD_OPEN)
				return 1;
			else
				return 0;
#endif

	/////////////Solar Load Switch (Block M)/////////////////////////////////
		case SOL_LOAD:

#ifdef _LAUNCH
			if(GpioDataRegs.GPCDAT.bit.GPIO89 == LOAD_OPEN)
				return 1;
			else
				return 0;
#else
			if(GpioDataRegs.GPBDAT.bit.GPIO45 == LOAD_OPEN)
				return 1;
			else
				return 0;
#endif

	/////////////AC Load Switch (Block C)/////////////////////////////////
		case AC_LOAD:

#ifdef _LAUNCH
			if(GpioDataRegs.GPCDAT.bit.GPIO71 == LOAD_OPEN)
				return 1;
			else
				return 0;
#else
			if(GpioDataRegs.GPBDAT.bit.GPIO48 == LOAD_OPEN)
				return 1;
			else
				return 0;
#endif

	/////////////5V Output Load Switch (Block D)/////////////////////////////////
		case FIVE_LOAD:
#ifdef _LAUNCH
			if(GpioDataRegs.GPBDAT.bit.GPIO41 == LOAD_OPEN)
				return 1;
			else
				return 0;
#else
			if(GpioDataRegs.GPBDAT.bit.GPIO47 == LOAD_OPEN)
				return 1;
			else
				return 0;
#endif

	/////////////12V Output Load Switch (Block T)/////////////////////////////////
		case TWELVE_LOAD:

#ifdef _LAUNCH
			if(GpioDataRegs.GPBDAT.bit.GPIO60 == LOAD_OPEN)
				return 1;
			else
				return 0;
#else
			if(GpioDataRegs.GPCDAT.bit.GPIO67 == LOAD_OPEN)
				return 1;
			else
				return 0;
#endif
		default:
			return 0;

	}//END SWITCH

}


//###########################################################################

