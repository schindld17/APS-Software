/*
 * APS_GlobalDefs.h
 *
 *  Created on: Apr 3, 2016
 *      Author: schindld
 */

#ifndef APS_GLOBALDEFS_H_
#define APS_GLOBALDEFS_H_

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
#define CMPSS_AC_H 2048
#define CMPSS_AC_L 2048
#define CMPSS_SOL_H 2048
#define CMPSS_SOL_L 2048
#define CMPSS_CAP_H 2048
#define CMPSS_CAP_L 2048
#define CMPSS_FIVEV_H 2048
#define CMPSS_FIVEV_L 2048
#define CMPSS_TWELVE_H 2048
#define CMPSS_FIVEV_L 2048

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


#endif /* APS_GLOBALDEFS_H_ */
