/*
 * APS_GlobalPrototypes.h
 *
 *  Created on: Mar 24, 2016
 *      Author: schindld
 */
#include "APS_GlobalDefs.h"
#include "F2837xS_device.h"


#ifndef APS_GLOBALPROTOTYPES_H_
#define APS_GLOBALPROTOTYPES_H_

#define CPU_RATE   5.00L   // for a 200MHz CPU clock speed (SYSCLKOUT)

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
void WatchDogDisable(void);
void InitFlashAPS(int bank);
void initSysPLL(int);
void initPeripheralClocks(void);
void GPIO_EnableUnbondedIOPullups(void);
char* convertADC(int16_t, ADC_Selection, char*);
int16_t sampleADC(ADC_Selection ADCModule);
void ADCInit(void);
void CMPSSInit(void);
void EPWMInit(void);
void GPIOInit(void);
void LoadSwitch(Load_Switch, int);
void APSPieInit(void);
interrupt void epwm2_isr(void);
interrupt void realclock_isr(void);
interrupt void epwm1_isr(void);
interrupt void epwm3_isr(void);
void systemBoot(void);
void setTime(void);
int addSeconds(int);
void getTime(char* returnString);
void voltageCurrent_Event(SPIKETYPE);
void appendTime_EventCode(char*, int);
void watchdog_Event();
int LoadSwitch_Status(Load_Switch loadSwitch);
void boot_Event();
void critical_Power_Event();




#endif /* APS_GLOBALPROTOTYPES_H_ */

