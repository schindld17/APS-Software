//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	eventLog.c
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
//	Creation Date: Apr 19, 2016 
//	Release Date:
//###########################################################################
#include "APS_GlobalDefs.h"
#include "APS_GlobalPrototypes.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


//*************************************************************************************************************************
//NAME: voltageCurrent_Event
//
//DESC:
//
//DATE: 19 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void appendTime_EventCode(char* returnString, int eventID)
{
	char timeString[TIMESTRINGSIZE];
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

	getTime(timeString);

	memmove(returnString, timeString, TIMESTRINGSIZE - 1);

	strncat(returnString, eventCode, eventCodeStringSize);


}//END FUNCTION


//*************************************************************************************************************************
//NAME: voltageCurrent_Event
//
//DESC:
//
//DATE: 19 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void voltageCurrent_Event(SPIKETYPE eventType)
{
#ifdef _DEMO
	unsigned char ucChar;
	int firstTime = 0;
#endif

	char* newFile;
	int16_t adcSample_AC, adcSample_Solar, adcSample_Hydro;
	char adcValString_AC[16], adcValString_Solar[16], adcValString_Hydro[16];
#ifdef _DEMO
	char adcFinalVal_AC[48], adcFinalVal_Sol[48], adcFinalVal_Hydro[48];
#endif

	newFile = malloc((size_t)(sizeof(char)*(FILESIZE + 1)));

	if(eventType == VOLTAGE_SPIKE)
	{
		appendTime_EventCode(newFile,VOLTAGE_SPIKE_EVENT);

		adcSample_AC = sampleADC(AC_VOLT);
    	convertADC(adcSample_AC,AC_VOLT,adcValString_AC);

    	adcSample_Solar = sampleADC(SOL_VOLT);
    	convertADC(adcSample_Solar,SOL_VOLT,adcValString_Solar);

    	adcSample_Hydro = sampleADC(HYDRO_VOLT);
    	convertADC(adcSample_Hydro,HYDRO_VOLT,adcValString_Hydro);

#ifdef _DEMO
    	sprintf(adcFinalVal_AC, " AC Input Voltage:%sV", adcValString_AC);
    	sprintf(adcFinalVal_Sol, " Solar Input Voltage:%sV", adcValString_Solar);
    	sprintf(adcFinalVal_Hydro, " Hydro Input Voltage:%sV", adcValString_Hydro);
		strncat(newFile, adcFinalVal_AC, sizeof(adcFinalVal_AC)/sizeof(adcFinalVal_AC[0]));
		strncat(newFile, adcFinalVal_Sol, sizeof(adcFinalVal_Sol)/sizeof(adcFinalVal_Sol[0]));
		strncat(newFile, adcFinalVal_Hydro, sizeof(adcFinalVal_Hydro)/sizeof(adcFinalVal_Hydro[0]));
#else
		strncat(newFile, adcFinalVal_AC, sizeof(adcValString_AC)/sizeof(adcFinalVal_AC[0]));
		strncat(newFile, adcFinalVal_Sol, sizeof(adcValString_Solar)/sizeof(adcFinalVal_Sol[0]));
		strncat(newFile, adcFinalVal_Hydro, sizeof(adcValString_Hydro)/sizeof(adcFinalVal_Hydro[0]));
#endif

	}
	else
	{
		appendTime_EventCode(newFile,CURRENT_SPIKE_EVENT);
	}

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
#else
    //NOTE: Need to add saving to Flash

#endif

	free(newFile);
}//END FUNCTION

//*************************************************************************************************************************
//NAME: watchdog_Event
//
//DESC:
//
//DATE: 24 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void watchdog_Event()
{
#ifdef _DEMO
	unsigned char ucChar;
	int firstTime;
#endif

	char* newFile;
	int16_t adcSample_AC, adcSample_Solar, adcSample_Hydro;
	char adcValString_AC[16], adcValString_Solar[16], adcValString_Hydro[16];
#ifdef _DEMO
	char adcFinalVal_AC[48], adcFinalVal_Sol[48], adcFinalVal_Hydro[48], loadSwitch_AC[24], loadSwitch_Sol[24], loadSwitch_Hydro[24];
#else
	char loadSwitch_AC[1], loadSwitch_Sol[1], loadSwitch_Hydro[1];
#endif

//	asm ("      ESTOP0");

#ifdef _DEMO
	firstTime = 0;
#endif
	newFile = malloc((size_t)(sizeof(char)*(FILESIZE + 1)));

	appendTime_EventCode(newFile,WATCHDOG_ID);

	//NOTE: Need to add measured Load

	//NOTE: Need to add measure relative humidity

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

	strncat(newFile, loadSwitch_AC, sizeof(loadSwitch_AC)/sizeof(loadSwitch_AC[0]));
	strncat(newFile, loadSwitch_Sol, sizeof(loadSwitch_Sol)/sizeof(loadSwitch_Sol[0]));
	strncat(newFile, loadSwitch_Hydro, sizeof(loadSwitch_Hydro)/sizeof(loadSwitch_Hydro[0]));


	adcSample_AC = sampleADC(AC_VOLT);
    convertADC(adcSample_AC,AC_VOLT,adcValString_AC);

    adcSample_Solar = sampleADC(SOL_VOLT);
    convertADC(adcSample_Solar,SOL_VOLT,adcValString_Solar);

    adcSample_Hydro = sampleADC(HYDRO_VOLT);
    convertADC(adcSample_Hydro,HYDRO_VOLT,adcValString_Hydro);

#ifdef _DEMO
    sprintf(adcFinalVal_AC, " AC Input Voltage:%sV", adcValString_AC);
    sprintf(adcFinalVal_Sol, " Solar Input Voltage:%sV", adcValString_Solar);
    sprintf(adcFinalVal_Hydro, " Hydro Input Voltage:%sV", adcValString_Hydro);
	strncat(newFile, adcFinalVal_AC, sizeof(adcFinalVal_AC)/sizeof(adcFinalVal_AC[0]));
	strncat(newFile, adcFinalVal_Sol, sizeof(adcFinalVal_Sol)/sizeof(adcFinalVal_Sol[0]));
	strncat(newFile, adcFinalVal_Hydro, sizeof(adcFinalVal_Hydro)/sizeof(adcFinalVal_Hydro[0]));
#else
	strncat(newFile, adcFinalVal_AC, sizeof(adcValString_AC)/sizeof(adcFinalVal_AC[0]));
	strncat(newFile, adcFinalVal_Sol, sizeof(adcValString_Solar)/sizeof(adcFinalVal_Sol[0]));
	strncat(newFile, adcFinalVal_Hydro, sizeof(adcValString_Hydro)/sizeof(adcFinalVal_Hydro[0]));
#endif



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
//DESC:
//
//DATE: 24 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void boot_Event()
{
#ifdef _DEMO
	unsigned char ucChar;
	int firstTime;
#endif

	char* newFile;

#ifdef _DEMO
	firstTime = 0;
#endif
	newFile = malloc((size_t)(sizeof(char)*(FILESIZE + 1)));

	appendTime_EventCode(newFile,BOOT_ID);

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
//DESC:
//
//DATE: 24 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void critical_Power_Event()
{
#ifdef _DEMO
	unsigned char ucChar;
	int firstTime;
#endif

	char* newFile;

#ifdef _DEMO
	firstTime = 0;
#endif
	newFile = malloc((size_t)(sizeof(char)*(FILESIZE + 1)));

	appendTime_EventCode(newFile,CRITICAL_POWER_ID);

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

