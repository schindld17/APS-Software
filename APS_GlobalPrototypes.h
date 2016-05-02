//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	APS_GlobalPrototypes.h
//
//	TITLE: APS Function Prototypes
//
//	DESCRIPTION: All function prototypes used within the APS software
//
//###########################################################################
//	Author: Dylan Schindler
//	Creation Date: 	Mar 24, 2016
//	Release Date: 	Apr 27, 2016
//###########################################################################

#include "APS_GlobalDefs.h"
#include "F2837xS_device.h"


#ifndef APS_GLOBALPROTOTYPES_H_
#define APS_GLOBALPROTOTYPES_H_

#define CPU_RATE   5.00L   // for a 200MHz CPU clock speed (SYSCLKOUT)

//Fucntion provided by TI to insert a number of NOPs
extern void F28x_usDelay(long LoopCount);
// DO NOT MODIFY THIS LINE.
#define DELAY_US(A)  F28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)

//---------------------------------------------------------------------------
// External symbols created by the linker cmd file
// DSP28 examples will use these to relocate code from one LOAD location
// in Flash to a different RUN location in internal
// RAM
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;
extern Uint16 RamfuncsRunEnd;
extern Uint16 RamfuncsRunSize;


//System Initilization Function Prototypes
//Function Prototype 								Where Function can be found
void WatchDogDisable(void);							//SysCtrl.c
void InitFlashAPS(int bank);						//SysCtrl.c
void initSysPLL(int);								//SysCtrl.c
void initPeripheralClocks(void);					//SysCtrl.c
void GPIO_EnableUnbondedIOPullups(void);			//SysCtrl.c
char* convertADC(int16_t, ADC_Selection, char*);	//ADCMod.c
int16_t sampleADC(ADC_Selection ADCModule);			//ADCMod.c
void ADCInit(void);									//ADCMod.c
void CMPSSInit(void);								//CMPSSMod.c
void EPWMInit(void);								//CMPSSMod.c
void GPIOInit(void);								//GPIOMod.c
void LoadSwitch(Load_Switch, int);					//GPIPMod.c
void APSPieInit(void);								//PIEMod.c
interrupt void epwm2_isr(void);						//PIEMod.c
interrupt void realclock_isr(void);					//PIEMod.c
interrupt void epwm1_isr(void);						//PIEMod.c
interrupt void epwm3_isr(void);						//PIEMod.c
void systemBoot(void);								//main.c
void setTime(void);									//RealClockMod.c
int addSeconds(int);								//RealClockMod.c
void getTime(char* returnString);					//RealClockMod.c
void voltageCurrent_Event(SPIKETYPE);				//eventLog.c
void appendTime_EventCode(char*, int);				//eventLog.c
void watchdog_Event();								//eventLog.c
int LoadSwitch_Status(Load_Switch loadSwitch);		//GPIOMod.c
void boot_Event();									//eventLog.c
void critical_Power_Event();						//eventLog.c


#endif /* APS_GLOBALPROTOTYPES_H_ */

