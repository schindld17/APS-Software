//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	GPIOMod.c
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
//	Creation Date: Apr 6, 2016 
//	Release Date:
//###########################################################################

#include "F2837xS_device.h"
#include "APS_GlobalDefs.h"
#include "APS_GlobalPrototypes.h"


//*************************************************************************************************************************
//NAME: GPIOInit
//
//DESC:
//
//DATE: 6 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void GPIOInit(void)
{
	EALLOW;

	/////////////Hydrogen Load Switch (Block S)/////////////////////////////////
#ifdef _LAUNCH
	//Initialize GPIO Pin 90 as output
	GpioCtrlRegs.GPCDIR.bit.GPIO90 = 1;
	//Turn on GPIO Pin 90 (Open Load Switch)
	GpioDataRegs.GPCSET.bit.GPIO90 = LOAD_OPEN;
#else
	//Initialize GPIO Pin 44 as output
	GpioCtrlRegs.GPBDIR.bit.GPIO44 = 1;
	//Turn on GPIO Pin 44 (Open Load Switch)
	GpioDataRegs.GPCSET.bit.GPIO44 = LOAD_OPEN;
#endif

	/////////////Solar Load Switch (Block M)////////////////////////////////////
#ifdef _LAUNCH
	//Initialize GPIO Pin 89 as output
	GpioCtrlRegs.GPCDIR.bit.GPIO89 = 1;
	//Turn on GPIO Pin 89 (Open Load Switch)
	GpioDataRegs.GPCSET.bit.GPIO89 = LOAD_OPEN;
#else
	//Initialize GPIO Pin 45 as output
	GpioCtrlRegs.GPBDIR.bit.GPIO45 = 1;
	//Turn on GPIO Pin 45 (Open Load Switch)
	GpioDataRegs.GPCSET.bit.GPIO45 = LOAD_OPEN;
#endif

	/////////////AC Load Switch (Block C)///////////////////////////////////////
#ifdef _LAUNCH
	//Initialize GPIO Pin 71 as output
	GpioCtrlRegs.GPCDIR.bit.GPIO71 = 1;
	//Turn on GPIO Pin 71 (Open Load Switch)
	GpioDataRegs.GPCSET.bit.GPIO71 = LOAD_OPEN;
#else
	//Initialize GPIO Pin 48 as output
	GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;
	//Turn on GPIO Pin 48 (Open Load Switch)
	GpioDataRegs.GPCSET.bit.GPIO48 = LOAD_OPEN;
#endif

	/////////////5V Output Load Switch (Block D)/////////////////////////////////
#ifdef _LAUNCH
	//Initialize GPIO Pin 41 as output
	GpioCtrlRegs.GPBDIR.bit.GPIO41 = 1;
	//Turn on GPIO Pin 41 (Open Load Switch)
	GpioDataRegs.GPBSET.bit.GPIO41 = LOAD_OPEN;
#else
	//Initialize GPIO Pin 47 as output
	GpioCtrlRegs.GPBDIR.bit.GPIO47 = 1;
	//Turn on GPIO Pin 47 (Open Load Switch)
	GpioDataRegs.GPCSET.bit.GPIO47 = LOAD_OPEN;
#endif

	/////////////12V Output Load Switch (Block T)/////////////////////////////////
#ifdef _LAUNCH
	//Initialize GPIO Pin 60 as output
	GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;
	//Turn on GPIO Pin 60 (Open Load Switch)
	GpioDataRegs.GPBSET.bit.GPIO60 = LOAD_OPEN;
#else
	//Initialize GPIO Pin 67 as output
	GpioCtrlRegs.GPCDIR.bit.GPIO67 = 1;
	//Turn on GPIO Pin 67 (Open Load Switch)
	GpioDataRegs.GPCSET.bit.GPIO67 = LOAD_OPEN;
#endif

	EDIS;

}//END FUNCTION

//*************************************************************************************************************************
//NAME: LoadSwitch
//
//DESC:
//
//DATE: 6 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
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
				GpioDataRegs.GPCDAT.bit.GPIO90 = 1;
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
				GpioDataRegs.GPCDAT.bit.GPIO90 = 0;
			 	DELAY_US(5000);
#else
				//Turn GPIO Pin 44 on
				GpioDataRegs.GPBDAT.bit.GPIO44 = 0;
				DELAY_US(5000);
#endif
			}//END ELSE
			break;
	/////////////Solar Load Switch (Block M)/////////////////////////////////
		case SOL_LOAD:
			if(on == 1)
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 89 on
				GpioDataRegs.GPCDAT.bit.GPIO89 = 1;
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
				GpioDataRegs.GPCDAT.bit.GPIO89 = 0;
			 	DELAY_US(5000);
#else
				//Turn GPIO Pin 45 on
				GpioDataRegs.GPBDAT.bit.GPIO45 = 0;
				DELAY_US(5000);
#endif
			}//END ELSE
			break;

	/////////////AC Load Switch (Block C)/////////////////////////////////
		case AC_LOAD:
			if(on == 1)
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 71 on
				GpioDataRegs.GPCDAT.bit.GPIO71 = 1;
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
				GpioDataRegs.GPCDAT.bit.GPIO71 = 0;
			 	DELAY_US(5000);
#else
				//Turn GPIO Pin 48 on
				GpioDataRegs.GPBDAT.bit.GPIO48 = 0;
				DELAY_US(5000);
#endif
			}//END ELSE
			break;
	/////////////5V Output Load Switch (Block D)/////////////////////////////////
		case FIVE_LOAD:
			if(on == 1)
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 41 on
				GpioDataRegs.GPBDAT.bit.GPIO41 = 1;
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
				GpioDataRegs.GPBDAT.bit.GPIO41 = 0;
			 	DELAY_US(5000);
#else
				//Turn GPIO Pin 47 on
				GpioDataRegs.GPBDAT.bit.GPIO47 = 0;
				DELAY_US(5000);
#endif
			}//END ELSE
			break;
	/////////////12V Output Load Switch (Block T)/////////////////////////////////
		case TWELVE_LOAD:
			if(on == 1)
			{
#ifdef _LAUNCH
				//Turn GPIO Pin 60 on
				GpioDataRegs.GPBDAT.bit.GPIO60 = 1;
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
				GpioDataRegs.GPBDAT.bit.GPIO60 = 0;
			 	DELAY_US(5000);
#else
				//Turn GPIO Pin 67 on
				GpioDataRegs.GPCDAT.bit.GPIO67 = 0;
				DELAY_US(5000);
#endif
			}//END ELSE
			break;
	}//END SWITCH

    EDIS;

}//END FUNCTION



//###########################################################################

