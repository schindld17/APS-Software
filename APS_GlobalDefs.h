/*
 * APS_GlobalDefs.h
 *
 *  Created on: Apr 3, 2016
 *      Author: schindld
 */

#ifndef APS_GLOBALDEFS_H_
#define APS_GLOBALDEFS_H_

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

#define LOAD_OPEN 1
#define LOAD_CLOSED 2


#endif /* APS_GLOBALDEFS_H_ */
