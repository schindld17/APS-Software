/*
 * APS_GlobalDefs.h
 *
 *  Created on: Apr 3, 2016
 *      Author: schindld
 */

#include "stdint.h"

#ifndef APS_GLOBALDEFS_H_
#define APS_GLOBALDEFS_H_

#define AC_INPUT_VOLTAGE_COMPONENT 0
#define AC_INPUT_CURRENT_COMPONENT 0
#define AC_INPUT_LOAD_SWITCH_COMPONENT 1
#define SOLAR_INPUT_VOLTAGE_COMPONENT 1
#define SOLAR_INPUT_CURRENT_COMPONENT 0
#define SOLAR_INPUT_LOAD_SWITCH_COMPONENT 1
#define HYDRO_INPUT_VOLTAGE_COMPONENT 0
#define HYDRO_INPUT_CURRENT_COMPONENT 0
#define HYDRO_INPUT_LOAD_SWITCH_COMPONENT 1
#define CAP_VOLTAGE_COMPONENT 1
#define CAP_CURRENT_COMPONENT 0
#define FIVEV_OUTPUT_CURRENT_COMPONENT 0
#define TWELVEV_OUTPUT_CURRENT_COMPONENT 0
#define TEMP_HUMID_SENSOR_COMPONENT 0

#define FILESIZE 256
#define TIMESTRINGSIZE 32

#define VOLTAGE_SPIKE_EVENT 0
#define CURRENT_SPIKE_EVENT 1

typedef enum
{
	VOLTAGE_SPIKE,
	CURRENT_SPIKE
} SPIKETYPE;

typedef struct julianTime_struct
{
	char month[3];
	int day;
	int year;
	int hour;
	int minute;
	int second;

} JULIANTIME;

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

typedef enum
{
	HYDRO_LOAD,
	SOL_LOAD,
	AC_LOAD,
	FIVE_LOAD,
	TWELVE_LOAD
}Load_Switch;

#define LOAD_OPEN 0
#define LOAD_CLOSED 1

//NOTE: For prodcution these should be global values that can be changed and calibrated as necessary

//Reference Voltage Constants for ADC sampling conversion [found in ADCMod.C - convertADC()]
#define VREFHI_AC_VOLT 120
#define VREFHI_AC_CUR 120
#define VREFHI_SOL_VOLT 120
#define VREFHI_SOL_CUR 120
#define VREFHI_HYDRO_VOLT 120
#define VREFHI_HYDRO_CUR 120
#define VREFHI_CAP_VOLT 120
#define VREFHI_CAP_CUR 120
#define VREFHI_FIVEV_CUR 120
#define VREFHI_TWELVE_CUR 120
#define VDDA 3.3

//CMPSS Constants
#define CMPSS_AC_V 2048
#define CMPSS_AC_C 2048
#define CMPSS_SOL_V 2048
#define CMPSS_SOL_C 2048
#define CMPSS_CAP_H 2048
#define CMPSS_CAP_L 2048
#define CMPSS_TWELVE 2048
#define CMPSS_FIVE 2048

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

typedef enum
{
	VOLTAGE_EVENT_ID,
	CURRENT_EVENT_ID,
	WATCHDOG_ID,
	BOOT_ID,
	CRITICAL_POWER_ID
}EVENT_ID;

#define WATCHDOG_TIME 30


#endif /* APS_GLOBALDEFS_H_ */
