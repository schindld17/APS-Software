//##############################################################################
//	PROJECT: APS-Software
//
//	FILE:	RealClockMod.c
//
//	TITLE: Support Functions for the System Date/Time
//
//	DESCRIPTION: Multiple support funtions to keep the current system date/time
//	to allow for real-time clock capabilities for the system. Functions include
//	a funtion to initialize an enhanced Pulse Width Modulation (ePWM) peripheral
//	that is used to update the current system date and time. A funtion to set
//	the current system date and time on program load. A function to update the
//	current system date and time. And a function to get the current system date
//	and time.
//
//	NOTE: All functions included only work with time stored in the Julian
//	Calendar date format and the 24hr time format. The specifications for the APS
//	state that the time must be output in the UNIX time format. This format should
//	be added and used in the future.
//
//##############################################################################
//	Author: Dylan Schindler
//	Creation Date: 	Apr 13, 2016
//	Release Date:	May 02, 2016
//##############################################################################


#include "F2837xS_device.h"
#include "F2837xS_Examples.h"
#include "file.h"
#include "stdio.h"
#include "string.h"
#include "sci_io.h"
#include "APS_GlobalDefs.h"
#include "APS_GlobalPrototypes.h"


//********************************************************************************
//NAME: InitEPWM
//
//DESC: This function initialize the ePWM12 peripheral to be used as a counter
//		to keep the current date and time. With a period of aproximately 1 second
//		when the period of the peripheral expires an interrupt is expired and the
//		realclock_isr() defined in PIEMod.c is called
//
//DATE: 5 April 2016
//
//AUTHOR: Dylan Schindler
//********************************************************************************
void RClock_ePWMInit(void)
{

	/////////////////////////////////////////////////////////////////////////////
	//Initialize ePWM12 to act as a real-time clock counting cycles up to 1 sec
	//and generating an interrupt to add to current time
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

//*******************************************************************************
//NAME: setTime
//
//DESC: This function uses the provided_date and provided_time character arrays
//		set within the main() function defined in main.c to set the current
//		system date and time.
//
//NOTE: This function only sets the time in a Julian Calendar Date and 24hr time
//		format. The specification calls for the system to output the time in
//		event data in the UNIX time format. The time should be stored in the
//		UNIX time format for simplicity
//
//DATE: 15 April 2016
//
//AUTHOR: Dylan Schindler
//*******************************************************************************
void setTime(void)
{
	//Seperating provided date/time into respective parts
	memcpy(julianTime.month, provided_date, 3);
	memcpy(day, &provided_date[4], 2);
	memcpy(year,&provided_date[7],4);
	memcpy(hour, provided_time, 2);
	memcpy(minute, &provided_time[3], 2);
	memcpy(second, &provided_time[6],2);

	//Converting the values from the character arrays to the appropriate
	//data types contained within the julianTime structure defined in
	//APS_GlobalDefs.h
	julianTime.day = (((int)day[0] - 48) * 10) + ((int)day[1] - 48);
	julianTime.year	= (((int)(year[0]) - 48) * 1000) + (((int)(year[1]) - 48) *
			100)
			+ (((int)(year[2]) - 48) * 10) + ((int)(year[3]) - 48);
	julianTime.hour = (((int)hour[0] - 48) * 10) + ((int)hour[1] - 48);
	julianTime.minute = (((int)minute[0] - 48) * 10) + ((int)minute[1] - 48);
	julianTime.second = (((int)second[0] - 48) *10) + ((int)second[1] - 48);
	//Add an offset to account of program build and load time by calling
	//the addSeconds() function defined in RealClockMod.c. This value
	//needs to be adjusted everytime new source code is added to the program
	addSeconds(56);

}//END FUNCTION

//********************************************************************************
//NAME: addTime
//
//DESC: This functions updates the current system date/time by the amount specified
//		in the seconds parameter. This number is meant to represent the number of
//		seconds that the current system date/time is suppossed to be updated.
//
//DATE: 16 April 2016
//
//AUTHOR: Dylan Schindler
//********************************************************************************
int addSeconds(int seconds)
{
	int newMinutes = 0;
	int newHours = 0;

	julianTime.second = julianTime.second + seconds;

	//If this is true than a minute has passed and the current time must be set
	//accordingly
	if(julianTime.second > 59)
	{
		newMinutes = (julianTime.second / 60);
		julianTime.second = julianTime.second - (newMinutes * 60);
		julianTime.minute = julianTime.minute + newMinutes;
		//If this is true than an hour has passed and the current time must be set
		//accordingly
		if(julianTime.minute > 59)
		{
			newHours = (julianTime.minute / 60);
			julianTime.minute = julianTime.minute - (newHours * 60);
			julianTime.hour = julianTime.hour + newHours;
			//If this is true than a day has passed and the current time must be set
			//accordingly
			if(julianTime.hour > 23)
			{
				julianTime.hour = 0;
				julianTime.day = julianTime.day + 1;
			}//END IF
		}//END IF
	}//END IF

	//NOTE:Logic needs to be added to account for when a week, month, and year has
	//passed.

return 1;
}//END FUNCTION

//********************************************************************************
//NAME: getTime
//
//DESC: This functions returns the current system date/time in some unformatted or
//		formatted character buffer provided to the function.
//
//DATE: 19 April 2016
//
//AUTHOR: Dylan Schindler
//********************************************************************************
void getTime(char* returnString)
{
	//If this symbol is defined then a demonstration is being performed with the
	//system and the time should be returned in a formatted string
#ifdef _DEMO
	sprintf(returnString, "%c%c%c %d, %d %d:%d:%d", julianTime.month[0],
			julianTime.month[1],julianTime.month[2], julianTime.day,
			julianTime.year, julianTime.hour, julianTime.minute, julianTime.second);
#else
	sprintf(returnString, "%c%c%c%d%d%d%d%d", julianTime.month[0],
			julianTime.month[1],julianTime.month[2], julianTime.day,
			julianTime.year, julianTime.hour, julianTime.minute, julianTime.second);
#endif

}//END FUNCTION
//#################################################################################

