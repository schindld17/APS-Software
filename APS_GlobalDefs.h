//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	APS_GlobalDefs.h
//
//	TITLE: APS Symbol and Type Definitions
//
//	DESCRIPTION: All symbol, type, enumerated type, and structure definitions
//  for the APS software.
//
//###########################################################################
//	Author: Dylan Schindler
//	Creation Date: 	Apr 03, 2016
//	Release Date: 	Apr 27, 2016
//###########################################################################

#include "stdint.h"

#ifndef APS_GLOBALDEFS_H_
#define APS_GLOBALDEFS_H_

//Symbols to define what components are current connected to the Delfino chip
//and which symbols will be used by the software
//NOTE: If these values are not set correctly the software will be in an undefined
//state and the functioning of the system is not guranteed.
#define AC_INPUT_VOLTAGE_COMPONENT 1
#define AC_INPUT_CURRENT_COMPONENT 0
#define AC_INPUT_LOAD_SWITCH_COMPONENT 1
#define SOLAR_INPUT_VOLTAGE_COMPONENT 0
#define SOLAR_INPUT_CURRENT_COMPONENT 0
#define SOLAR_INPUT_LOAD_SWITCH_COMPONENT 1
#define HYDRO_INPUT_VOLTAGE_COMPONENT 0
#define HYDRO_INPUT_CURRENT_COMPONENT 0
#define HYDRO_INPUT_LOAD_SWITCH_COMPONENT 1
#define CAP_VOLTAGE_COMPONENT 0
#define CAP_CURRENT_COMPONENT 0
#define FIVEV_OUTPUT_CURRENT_COMPONENT 0
#define TWELVEV_OUTPUT_CURRENT_COMPONENT 0
#define TEMP_HUMID_SENSOR_COMPONENT 0

//The size of the character buffer where log files will be stored until they are either
//stored in Flash or output through the SPI
#define FILESIZE 256
//The size of the character buffer where the current time used in log files will be stored
#define TIMESTRINGSIZE 32

//Symbols to define which type of spike event has occurred
#define VOLTAGE_SPIKE_EVENT 0
#define CURRENT_SPIKE_EVENT 1
typedef enum
{
	VOLTAGE_SPIKE,
	CURRENT_SPIKE
} SPIKETYPE;

//Structure to hold the current date and time in a Julian Calendar/24-hr format
typedef struct julianTime_struct
{
	char month[3];
	int day;
	int year;
	int hour;
	int minute;
	int second;

} JULIANTIME;

//Enumeration for the components using the ADC channels
typedef enum
{
	HYDRO_VOLT,
	HYDRO_CUR,
	SOL_VOLT,
	SOL_CUR,
	AC_VOLT,
	AC_CUR,
	CAP_VOLT,
	CAP_CUR,
	FIVEV_CUR,
	TWELVEV_CUR
}ADC_Selection;

//Enumeration for the various load switches used by the system
typedef enum
{
	HYDRO_LOAD,
	SOL_LOAD,
	AC_LOAD,
	FIVE_LOAD,
	TWELVE_LOAD
}Load_Switch;

//Symbols to define what configuration the load switches use (Active High vs. Active Low)
//If load switches are using Active High then set LOAD_OPEN to 1 and LOAD_CLOSED to 0
//These values will be used to set the data for the GPIO pins connected to each Load switch
#define LOAD_OPEN 1
#define LOAD_CLOSED 0

//NOTE: For prodcution these should be global values that can be changed and calibrated as necessary
//Reference Voltage Constants for ADC sampling conversion
#define VREFHI_AC_VOLT 34 //This value set for voltage divder circuit using a 2.2KOhm and a 22KOhm resistor
#define VREFHI_AC_CUR 120 //All following values have not been calibrated or tested
#define VREFHI_SOL_VOLT 120
#define VREFHI_SOL_CUR 120
#define VREFHI_HYDRO_VOLT 120
#define VREFHI_HYDRO_CUR 120
#define VREFHI_CAP_VOLT 120
#define VREFHI_CAP_CUR 120
#define VREFHI_FIVEV_CUR 120
#define VREFHI_TWELVE_CUR 120
#define VDDA 3.3

//Symbols to define what ADC result value is used as the reference for the corresponding CMPSS
#define CMPSS_AC_V 1850 	//This value was set to be about 16V with a voltage divder circuit
							//using a 2.2KOhm and a 22KOhm resistor
#define CMPSS_AC_C 2048		//All following vlaues are set for the midpoint of the voltage range
							//and have not been set to any particular value
#define CMPSS_SOL_V 2048
#define CMPSS_SOL_C 2048
#define CMPSS_CAP_H 2048
#define CMPSS_CAP_L 2048
#define CMPSS_TWELVE 2048
#define CMPSS_FIVE 2048

//Global definitions to be used with values that will store the current date/time upon program download
extern int32_t realtime;
extern char provided_date[11];
extern char provided_time[8];
extern char month[3];
extern char day[2];
extern char year[4];
extern char hour[2];
extern char minute[2];
extern char second[2];
extern JULIANTIME julianTime;

//Enumeration to define the various Event Log Code
typedef enum
{
	VOLTAGE_EVENT_ID,
	CURRENT_EVENT_ID,
	WATCHDOG_ID,
	BOOT_ID,
	CRITICAL_POWER_ID
}EVENT_ID;

//Symbol to define the period of the ePWM1 ISR being used for the Watchdog Timer Event in seconds
#define WATCHDOG_TIME 10

//Symbol to define C28x assembly command to insert a software breakpoint in the current run of the program
#define BREAK asm("      ESTOP0");


#endif /* APS_GLOBALDEFS_H_ */
