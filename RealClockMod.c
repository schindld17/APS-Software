//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	RealClockMod.c
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
//	Creation Date: Apr 13, 2016 
//	Release Date:
//###########################################################################\


#include "F2837xS_device.h"
#include "F2837xS_Examples.h"
#include "APS_GlobalDefs.h"
#include "APS_GlobalPrototypes.h"


//*************************************************************************************************************************
//NAME: InitEPWM
//
//DESC:
//
//DATE: 5 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void RClock_ePWMInit(void)
{

	/////////////////////////////////////////////////////////////////////////////////
	//Initialize ePWM12 to act as a real-time clock counting cycles up to 1 sec and generating an interrupt to add to
	//	current time
	EALLOW;

	//Set clock to SYSCLOCK/(128*14) ~ 54018Hz.
	EPwm12Regs.TBCTL.bit.HSPCLKDIV = 0x7;
	EPwm12Regs.TBCTL.bit.CLKDIV = 0x7;
	//Set period to be aproximately 1 sec.
	EPwm12Regs.TBPRD = 0xD302;
	//No Phase loaded
	EPwm12Regs.TBPHS.bit.TBPHS = 0x0;
	//Clear the counter
	EPwm12Regs.TBCTR = 0x0;
	//Up-count mode
	EPwm12Regs.TBCTL.bit.CTRMODE = 0x0;
	//Don't load counter from phase register enable
	EPwm12Regs.TBCTL.bit.PHSEN =0x0;
	//Set clock to SYSCLOCK/96
	EPwm12Regs.TBCTL.bit.HSPCLKDIV = 0x7;
	EPwm12Regs.TBCTL.bit.CLKDIV = 0x7;
	//Set counter-compare A to shadow mode
	EPwm12Regs.CMPCTL.bit.SHDWAMODE = 0x1;
	//Set counter-compare A load to be when CTR = 0
	EPwm12Regs.CMPCTL.bit.LOADAMODE = 0x0;
	// Interrupt where we will change the Compare Values
	EPwm12Regs.ETPS.bit.INTPSSEL = 0x1;
	EPwm12Regs.ETSEL.bit.INTSEL = 0x2;        // Select INT on Zero event
	EPwm12Regs.ETSEL.bit.INTEN = 0x1;                   // Enable INT
	EPwm12Regs.ETINTPS.bit.INTCNT2 = 0x2;
	EPwm12Regs.ETINTPS.bit.INTPRD2 = 0x1;
	EDIS;
}//END FUNCTION

//*************************************************************************************************************************
//NAME: setTime
//
//DESC:
//
//DATE: 15 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void setTime(void)
{
	//Seperating provided date/time into respective parts
	memcpy(julianTime.month, provided_date, 3);
	memcpy(day, &provided_date[4], 2);
	memcpy(year,&provided_date[7],4);
	memcpy(hour, provided_time, 2);
	memcpy(minute, &provided_time[3], 2);
	memcpy(second, &provided_time[6],2);

	julianTime.day = (((int)day[0] - 48) * 10) + ((int)day[1] - 48);
	julianTime.year	= (((int)(year[0]) - 48) * 1000) + (((int)(year[1]) - 48) * 100) + (((int)(year[2]) - 48) * 10) + ((int)(year[3]) - 48);
	julianTime.hour = (((int)hour[0] - 48) * 10) + ((int)hour[1] - 48);
	julianTime.minute = (((int)minute[0] - 48) * 10) + ((int)minute[1] - 48);
	julianTime.second = (((int)second[0] - 48) *10) + ((int)second[1] - 48);
	addSeconds(97);



}//END FUNCTION

//*************************************************************************************************************************
//NAME: addTime
//
//DESC:
//
//DATE: 16 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
int addSeconds(int seconds)
{
	julianTime.second = julianTime.second + seconds;

	if(julianTime.second > 59)
	{
		julianTime.second = 0;
		julianTime.minute = julianTime.minute + 1;

		if(julianTime.minute > 59)
		{
			julianTime.minute = 0;
			julianTime.hour = julianTime.hour + 1;

			if(julianTime.hour > 23)
			{
				julianTime.hour = 0;
				julianTime.day = julianTime.day + 1;
			}//END IF
		}//END IF
	}//END IF
return 1;
}//END FUNCTION





//###########################################################################

