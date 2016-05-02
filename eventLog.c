//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	eventLog.c
//
//	TITLE: Event Logging Support Functions
//
//	DESCRIPTION: Multiple support functions for all event logging features of
//  the APS. Functions include a function to append a timestamp and event code
//  to a character buffer. A function to provide the bootup event to a character
//  buffer. A function to provide the voltage/current spike event to a character
//  buffer. A function to provide the watchdog event to a character buffer. A
//  function to provide the critical power loss event to a character buffer. Each
//  of these events are described within the APS Software Functional Design
//  specification.
//
//  NOTE: In this current version of the file all included functions only provide
//  a character buffer that is output through the SCI (UART). These functions do
//  note provide the capability to save the event data to onboard Flash.
//
//###########################################################################
//	Author: Dylan Schindler
//	Creation Date: Apr 19, 2016 
//	Release Date:  May 02, 2016
//###########################################################################
#include "APS_GlobalDefs.h"
#include "APS_GlobalPrototypes.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


//***************************************************************************
//NAME: appendTime_EventCode
//
//DESC: Append a timestamp and event code to the provided character buffer.
//		the timestamp is obtained by using the getTime function contained
//		within RealClockMod.c. The provided eventID int is typically a EVENT_ID
//      enumeration provided in APS_GlobalDefs.h
//
//DATE: 19 April 2016
//
//AUTHOR: Dylan Schindler
//****************************************************************************
void appendTime_EventCode(char* returnString, int eventID)
{
	char timeString[TIMESTRINGSIZE];

//If the system is currently being used in demo mode a more verbose event code
//code can be returned for easier readability
#ifdef _DEMO
	char eventCode[14];
#else
	char eventCode[1];
#endif
	int eventCodeStringSize = sizeof(eventCode)/sizeof(eventCode[0]);

#ifdef _DEMO
	sprintf(eventCode," Event Code: %c",((char)eventID + 48));
#else
	sprintf(eventCode," %c",((char)eventID + 48));
#endif

	//Use getTime(char*) function defined in RealClockMod.c to get current time
	getTime(timeString);

	//Move the returned time into the provided character buffer and append the event
	//code string.
	memmove(returnString, timeString, TIMESTRINGSIZE - 1);
	strncat(returnString, eventCode, eventCodeStringSize);

}//END FUNCTION


//******************************************************************************
//NAME: voltageCurrent_Event
//
//DESC: Create a new character buffer containing the necessary information for a
//		voltage or current spike event. Necessary information includes a timestamp,
//		the event code, and the measured current or voltage on each active input
//		power source. A SPIKITYPE enumeration constant is provided as an argument to
//		the function to determine if the event was a current spike or a voltage spike.
//
//DATE: 19 April 2016
//
//AUTHOR: Dylan Schindler
//******************************************************************************
void voltageCurrent_Event(SPIKETYPE eventType)
{
	//If the system is being used in demonstration mode then these variables are
	//needed to properly output data through the SCI (UART)
#ifdef _DEMO
	unsigned char ucChar;
	int firstTime = 0;
#endif

	//Character buffer to contain the overall event data
	char* newFile;
	int16_t adcSample_AC, adcSample_Solar, adcSample_Hydro;
	char adcValString_AC[16], adcValString_Solar[16], adcValString_Hydro[16];

	//If the system is being used in demonstration mode than more verbose ADC
	//strings will be used for readability
#ifdef _DEMO
	char adcFinalVal_AC[48], adcFinalVal_Sol[48], adcFinalVal_Hydro[48];
#endif

	//Allocate space for the event data based upon the FILESIZE symbol defined in
	//APS_GlobalDefs.h
	newFile = malloc((size_t)(sizeof(char)*(FILESIZE + 1)));

	if(eventType == VOLTAGE_SPIKE)
	{
		//Append a timestamp and the event code to the event data buffer
		appendTime_EventCode(newFile,VOLTAGE_SPIKE_EVENT);

		//Use the ADC peripherals connected to the AC input power source, the
		//solar panel inpuut power source, and the hydrogen fuel cell input
		//power source to measure the voltage provided by each line
		adcSample_AC = sampleADC(AC_VOLT);
    	convertADC(adcSample_AC,AC_VOLT,adcValString_AC);

    	adcSample_Solar = sampleADC(SOL_VOLT);
    	convertADC(adcSample_Solar,SOL_VOLT,adcValString_Solar);

    	adcSample_Hydro = sampleADC(HYDRO_VOLT);
    	convertADC(adcSample_Hydro,HYDRO_VOLT,adcValString_Hydro);

    	//If the system is being used in demonstration mode then append a verbose
    	//message, for readability, to the event data with the measured voltages on
    	//each input power source line
#ifdef _DEMO
    	sprintf(adcFinalVal_AC, " AC Input Voltage:%sV", adcValString_AC);
    	sprintf(adcFinalVal_Sol, " Solar Input Voltage:%sV", adcValString_Solar);
    	sprintf(adcFinalVal_Hydro, " Hydro Input Voltage:%sV", adcValString_Hydro);
		strncat(newFile, adcFinalVal_AC, sizeof(adcFinalVal_AC)/sizeof(adcFinalVal_AC[0]));
		strncat(newFile, adcFinalVal_Sol, sizeof(adcFinalVal_Sol)/sizeof(adcFinalVal_Sol[0]));
		strncat(newFile, adcFinalVal_Hydro, sizeof(adcFinalVal_Hydro)/sizeof(adcFinalVal_Hydro[0]));
#else
		strncat(newFile, adcValString_AC, sizeof(adcValString_AC)/sizeof(adcValString_AC[0]));
		strncat(newFile, adcValString_Solar, sizeof(adcValString_Solar)/sizeof(adcValString_Solar[0]));
		strncat(newFile, adcValString_Hydro, sizeof(adcValString_Hydro)/sizeof(adcValString_Hydro[0]));
#endif

	}
	else
	{
		appendTime_EventCode(newFile,CURRENT_SPIKE_EVENT);
		//NOTE: Need to add current spike logic here
	}

	//If the system is being used in demonstration mode then the event data will be output as a
	//formatted string through the SCI (UART) to a connected station (Use PuTTY with a serial
	//line with a baud rate of 115200 to view the data)
#ifdef _DEMO

	if(firstTime == 0)
	{
	    // For this example, only init the pins for the SCI-A port.
	    EALLOW;
	    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 1;
	    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 1;
	    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
	    GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
	    EDIS;
	    //Redirect standard character buffer from terminal to the SCI
		redirOut();
		firstTime = 1;
	}
	printf("%s",newFile);

    ucChar = 10;
    putchar(ucChar);
    ucChar = 13;
    putchar(ucChar);
#else
    //NOTE: Need to add saving to Flash

#endif
    //Release memory allocated to event data
	free(newFile);
}//END FUNCTION

//*************************************************************************************************************************
//NAME: watchdog_Event
//
//DESC: Create a new character buffer containing the necessary information for a
//		watchdog event. Necessary information includes a timestamp,the event code,
//		the measured voltage and current of each active input, the measured load
//		on the system, the current relative humidity within the system enclosure,
//		and the input power sources currently in use.
//
//DATE: 24 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void watchdog_Event()
{
	//If the system is being used in demonstration mode then these variables are
	//needed to properly output data through the SCI (UART)
#ifdef _DEMO
	unsigned char ucChar;
	int firstTime;
#endif

	//Character buffer to contain the overall event data
	char* newFile;
	int16_t adcSample_AC, adcSample_Solar, adcSample_Hydro;
	char adcValString_AC[16], adcValString_Solar[16], adcValString_Hydro[16];

	//If the system is being used in demonstration mode than more verbose ADC
	//and load switch statements will be used for readability
#ifdef _DEMO
	char adcFinalVal_AC[48], adcFinalVal_Sol[48], adcFinalVal_Hydro[48], loadSwitch_AC[24], loadSwitch_Sol[24], loadSwitch_Hydro[24];
#else
	char loadSwitch_AC[1], loadSwitch_Sol[1], loadSwitch_Hydro[1];
#endif


#ifdef _DEMO
	firstTime = 0;
#endif

	//Allocate space for the event data based upon the FILESIZE symbol defined in
	//APS_GlobalDefs.h
	newFile = malloc((size_t)(sizeof(char)*(FILESIZE + 1)));

	//Append a timestamp and the event code to the event data buffer
	appendTime_EventCode(newFile,WATCHDOG_ID);

	//NOTE: Need to add measured Load

	//NOTE: Need to add measure relative humidity

	//If the AC input power load switch component is in use (configured within
	//APS_GlobalDefs.h) then append the load switch status (1 = on, 0 = off)
	if(AC_INPUT_LOAD_SWITCH_COMPONENT)
	{
#ifdef _DEMO
		sprintf(loadSwitch_AC," AC Load Switch:%d",LoadSwitch_Status(AC_LOAD));
#else
		sprintf(loadSwitch_AC,"%d",LoadSwitch_Status(AC_LOAD));
#endif
	}//END IF
	else
	{
#ifdef _DEMO
		sprintf(loadSwitch_AC," AC Load Switch:%d",0);
#else
		sprintf(loadSwitch_AC,"%d",0);
#endif
	}//END ELSE

	//If the Solar panel input power load switch component is in use
	//(configured within APS_GlobalDefs.h) then append the load switch
	//status (1 = on, 0 = off)
	if(SOLAR_INPUT_LOAD_SWITCH_COMPONENT)
	{
#ifdef _DEMO
		sprintf(loadSwitch_Sol," Solar Load Switch:%d",LoadSwitch_Status(SOL_LOAD));
#else
		sprintf(loadSwitch_Sol,"%d",LoadSwitch_Status(SOL_LOAD));
#endif
	}//END IF
	else
	{
#ifdef _DEMO
		sprintf(loadSwitch_Sol," Solar Load Switch:%d",0);
#else
		sprintf(loadSwitch_Sol,"%d",0);
#endif
	}//END ELSE

	//If the hydrogen fuel cell input power load switch component
	//is in use  (configured within APS_GlobalDefs.h) then append
	//the load switch status (1 = on, 0 = off)
	if(HYDRO_INPUT_LOAD_SWITCH_COMPONENT)
	{
#ifdef _DEMO
		sprintf(loadSwitch_Hydro," Hydro Load Switch:%d",LoadSwitch_Status(HYDRO_LOAD));
#else
		sprintf(loadSwitch_Hydro,"%d",LoadSwitch_Status(HYDRO_LOAD));
#endif
	}//END IF
	else
	{
#ifdef _DEMO
		sprintf(loadSwitch_Hydro," Hydro Load Switch:%d",0);
#else
		sprintf(loadSwitch_Hydro,"%d",0);
#endif
	}//END ELSE

	//Apppend the load witch statuses to the event data character buffer
	strncat(newFile, loadSwitch_AC, sizeof(loadSwitch_AC)/sizeof(loadSwitch_AC[0]));
	strncat(newFile, loadSwitch_Sol, sizeof(loadSwitch_Sol)/sizeof(loadSwitch_Sol[0]));
	strncat(newFile, loadSwitch_Hydro, sizeof(loadSwitch_Hydro)/sizeof(loadSwitch_Hydro[0]));


	//Use the ADC peripherals connected to the AC input power source, the
	//solar panel inpuut power source, and the hydrogen fuel cell input
	//power source to measure the voltage provided by each line
	adcSample_AC = sampleADC(AC_VOLT);
    convertADC(adcSample_AC,AC_VOLT,adcValString_AC);

    adcSample_Solar = sampleADC(SOL_VOLT);
    convertADC(adcSample_Solar,SOL_VOLT,adcValString_Solar);

    adcSample_Hydro = sampleADC(HYDRO_VOLT);
    convertADC(adcSample_Hydro,HYDRO_VOLT,adcValString_Hydro);

	//If the system is being used in demonstration mode then append a verbose
	//message, for readability, to the event data with the measured voltages on
	//each input power source line
#ifdef _DEMO
    sprintf(adcFinalVal_AC, " AC Input Voltage:%sV", adcValString_AC);
    sprintf(adcFinalVal_Sol, " Solar Input Voltage:%sV", adcValString_Solar);
    sprintf(adcFinalVal_Hydro, " Hydro Input Voltage:%sV", adcValString_Hydro);
	strncat(newFile, adcFinalVal_AC, sizeof(adcFinalVal_AC)/sizeof(adcFinalVal_AC[0]));
	strncat(newFile, adcFinalVal_Sol, sizeof(adcFinalVal_Sol)/sizeof(adcFinalVal_Sol[0]));
	strncat(newFile, adcFinalVal_Hydro, sizeof(adcFinalVal_Hydro)/sizeof(adcFinalVal_Hydro[0]));
#else
	strncat(newFile, adcValString_AC, sizeof(adcValString_AC)/sizeof(adcValString_AC[0]));
	strncat(newFile, adcValString_Solar, sizeof(adcValString_Solar)/sizeof(adcValString_Solar[0]));
	strncat(newFile, adcValString_Hydro, sizeof(adcValString_Hydro)/sizeof(adcValString_Hydro[0]));
#endif


	//If the system is being used in demonstration mode then the event data will be output as a
	//formatted string through the SCI (UART) to a connected station (Use PuTTY with a serial
	//line with a baud rate of 115200 to view the data)
#ifdef _DEMO

	if(firstTime == 0)
	{
	    // For this example, only init the pins for the SCI-A port.
	    EALLOW;
	    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 1;
	    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 1;
	    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
	    GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
	    EDIS;

		redirOut();
		firstTime = 1;
	}
	printf("%s",newFile);

    ucChar = 10;
    putchar(ucChar);
    ucChar = 13;
    putchar(ucChar);

#endif

	free(newFile);
}//END FUNCTION

//*************************************************************************************************************************
//NAME: boot_Event
//
//DESC: Create a new character buffer containing the necessary information for a
//		bootup event. Necessary information includes a timestamp and the event
//		code.
//
//DATE: 24 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void boot_Event()
{
	//If the system is being used in demonstration mode then these variables are
	//needed to properly output data through the SCI (UART)
#ifdef _DEMO
	unsigned char ucChar;
	int firstTime;
#endif

	//Character buffer to contain the overall event data
	char* newFile;

#ifdef _DEMO
	firstTime = 0;
#endif

	//Allocate space for the event data based upon the FILESIZE symbol defined in
	//APS_GlobalDefs.h
	newFile = malloc((size_t)(sizeof(char)*(FILESIZE + 1)));

	//Append a timestamp and the event code to the event data buffer
	appendTime_EventCode(newFile,BOOT_ID);

	//If the system is being used in demonstration mode then the event data will be output as a
	//formatted string through the SCI (UART) to a connected station (Use PuTTY with a serial
	//line with a baud rate of 115200 to view the data)
#ifdef _DEMO

	if(firstTime == 0)
	{
	    // For this example, only init the pins for the SCI-A port.
	    EALLOW;
	    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 1;
	    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 1;
	    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
	    GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
	    EDIS;

		redirOut();
		firstTime = 1;
	}
	printf("%s",newFile);

    ucChar = 10;
    putchar(ucChar);
    ucChar = 13;
    putchar(ucChar);

#endif

	free(newFile);
}//END FUNCTION

//*************************************************************************************************************************
//NAME: critical_Power_Event
//
//DESC: Create a new character buffer containing the necessary information for a
//		critical power loss event. Necessary information includes a timestamp and
//		the event code.
//
//DATE: 24 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void critical_Power_Event()
{
	//If the system is being used in demonstration mode then these variables are
	//needed to properly output data through the SCI (UART)
#ifdef _DEMO
	unsigned char ucChar;
	int firstTime;
#endif

	//Character buffer to contain the overall event data
	char* newFile;

#ifdef _DEMO
	firstTime = 0;
#endif

	//Allocate space for the event data based upon the FILESIZE symbol defined in
	//APS_GlobalDefs.h
	newFile = malloc((size_t)(sizeof(char)*(FILESIZE + 1)));

	//Append a timestamp and the event code to the event data buffer
	appendTime_EventCode(newFile,CRITICAL_POWER_ID);

	//If the system is being used in demonstration mode then the event data will be output as a
	//formatted string through the SCI (UART) to a connected station (Use PuTTY with a serial
	//line with a baud rate of 115200 to view the data)
#ifdef _DEMO

	if(firstTime == 0)
	{
	    // For this example, only init the pins for the SCI-A port.
	    EALLOW;
	    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 1;
	    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 1;
	    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
	    GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
	    EDIS;

		redirOut();
		firstTime = 1;
	}
	printf("%s",newFile);

    ucChar = 10;
    putchar(ucChar);
    ucChar = 13;
    putchar(ucChar);

#endif

	free(newFile);
}


//###########################################################################

