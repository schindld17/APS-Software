//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	SysCtrl.c
//
//	TITLE: System Initialization Functions
//
//	DESCRIPTION:
//
//
//
//
//
//###########################################################################
//	Author: Dylan Schindler
//	Creation Date: Mar 17, 2016 
//	Release Date:
//###########################################################################

#pragma CODE_SECTION(InitFlash, "ramfuncs");
#include <F2837xS_flash.h>


void InitSysCtrl(void)
{
	// Unlock write-protected registers
	EALLOW;
	// Disable Watchdog
	WatchDogDisable();


#ifdef _FLASH
// If true then #define _FLASH has been provided to linker meaning code will be
// to flash memory.

// Copy time critical code and Flash setup code to RAM. This includes function InitFlash()
// The RamfuncsRunstart, RamfuncsLoadStart, and RamfuncsLoadSize are created by the linker.
// These can be found in the device .cmd file.
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize)

// Call Flash Initilization to setup flash waitstates. --> This function must reside in RAM

#endif



}

void WatchDogDisable(void)
{
	volatile Uint16 temp;
	EALLOW;
	//Save current clock configuration
	temp = WdRegs.WDCR.all & 0x0007;
	//Disable watchdog timer and check bits to ensure a reset isn't forced
	WdRegs.WDCR.all = 0x0068 | temp;
	EDIS;

}

void InitFlash(void)
{
	EALLOW;
	if(Test)
	{
		//Set Flash Bank power up delay
		Flash0CtrlRegs.FBAC.bit.VREADST=0x14;

		//On reset Flash bank and pump are in sleep mode
	    //A Flash access will power up the bank and pump automatically
	    //After a Flash access, bank and pump go to low power mode (configurable in FBFALLBACK/FPAC1 registers)-
	    //if there is no further access to flash

		//Power up the Flash Bank and Pump and set fall back mode as active
		Flash0CtrlRegs.FPAC1.bit.PMPPWR = 0x1;
		Flash0CtrlRegs.FBFALLBACK.bit.BNKPWR0 = 0x3;

		//Disable data Cache and prefect mechanism to change flash wait state
		Flash0CtrlRegs.FRD_INTF_CTRL.bit.DATA_CACHE_EN = 0;
		Flash0CtrlRegs.F


	}



}



//###########################################################################

