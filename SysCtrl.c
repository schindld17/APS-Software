//#######################################################################################################################
//	PROJECT: APS-Software
//
//	FILE:	SysCtrl.c
//
//	TITLE: System Initialization Functions
//
//	DESCRIPTION:	Group of functions used to initlize the system to the
//					the correct state.
//
//########################################################################################################################
//	Author: Dylan Schindler
//	Creation Date: 	Mar 17, 2016
//	Release Date:	May 02, 2016
//########################################################################################################################


#include "F2837xS_device.h"
#include "APS_GlobalPrototypes.h"
#include "F2837xS_Examples.h"

#pragma CODE_SECTION(InitFlash, "ramfuncs");

//*************************************************************************************************************************
//NAME: initSysCtrl
//
//DESC: Initialize the system
//
//DATE: 23 March 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void InitSysCtrl(void)
{
	// Disable Watchdog
	WatchDogDisable();


#ifdef _FLASH
// If true then #define _FLASH has been provided to linker meaning code will be
// to flash memory.

// Copy time critical code and Flash setup code to RAM. This includes function InitFlash()
// The RamfuncsRunstart, RamfuncsLoadStart, and RamfuncsLoadSize are created by the linker.
// These can be found in the device .cmd file.
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);

// Call Flash Initilization to setup flash waitstates. --> This function must reside in RAM.
	InitFlashAPS(0);
	InitFlashAPS(1);
#endif

    //enable pull-ups on unbonded IOs as soon as possible to reduce power consumption.
    GPIO_EnableUnbondedIOPullups();

    EALLOW;
	CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
	CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
	CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
	CpuSysRegs.PCLKCR13.bit.ADC_D = 1;

    //check if device is trimmed
    if(*((Uint16 *)0x5D1B6) == 0x0000){
        //device is not trimmed, apply static calibration values
        AnalogSubsysRegs.ANAREFTRIMA.all = 31709;
        AnalogSubsysRegs.ANAREFTRIMB.all = 31709;
        AnalogSubsysRegs.ANAREFTRIMC.all = 31709;
        AnalogSubsysRegs.ANAREFTRIMD.all = 31709;
    }

	CpuSysRegs.PCLKCR13.bit.ADC_A = 0;
	CpuSysRegs.PCLKCR13.bit.ADC_B = 0;
	CpuSysRegs.PCLKCR13.bit.ADC_C = 0;
	CpuSysRegs.PCLKCR13.bit.ADC_D = 0;
    EDIS;

#ifndef _TEST
	//USB is being used.
	initSysPLL(0);
#endif

	//USB is not being used.
	initSysPLL(0);

	//Initialize peripheral clocks
	initPeripheralClocks();

}

void WatchDogDisable(void)
{
	volatile Uint16 temp;

	//Unlock write protected registers.
	EALLOW;

	//Save current clock configuration.
	temp = WdRegs.WDCR.all & 0x0007;

	//Disable watchdog timer and check bits to ensure a reset isn't forced.
	WdRegs.WDCR.all = 0x0068 | temp;

	//Lock write protected registers.
	EDIS;
}

//*************************************************************************************************************************
//NAME: initFlash
//
//DESC: Initialize all both flash banks depending on if system is in test mode or production mode
//
//DATE: 23 March 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void InitFlashAPS(int bank)
{

	volatile struct FLASH_CTRL_REGS *Flash;
	volatile struct FLASH_ECC_REGS *FlashECC;

	EALLOW;
	//Programmatically choosing which Flash bank will be used during initilization.
	if (bank == 1)
	{
		Flash = &Flash1CtrlRegs;
		FlashECC = &Flash1EccRegs;
	}//END IF
	else
	{
		Flash = &Flash0CtrlRegs;
		FlashECC = &Flash0EccRegs;
	}//END ELSE

	//Set Flash Bank power up delay.
	(*Flash).FBAC.bit.VREADST=0x14;

	//On reset Flash bank and pump are in sleep mode.
	//A Flash access will power up the bank and pump automatically.
	//After a Flash access, bank and pump go to low power mode (configurable in FBFALLBACK/FPAC1 registers)-
	// if there is no further access to flash.

	//Power up the Flash Bank and Pump and set fall back mode as active.
	(*Flash).FPAC1.bit.PMPPWR = 0x1;
	(*Flash).FBFALLBACK.bit.BNKPWR0 = 0x3;

	//Disable data Cache and prefecth mechanism to change flash wait state.
	(*Flash).FRD_INTF_CTRL.bit.DATA_CACHE_EN = 0;
	(*Flash).FRD_INTF_CTRL.bit.PREFETCH_EN = 0;

	//Set Flash wait state according to 200MHz clock frequency.
	(*Flash).FRDCNTL.bit.RWAIT = 0x3;

	//If Flash bank 1 is being initialized then turn on data cache and prefetch to improve performance
	//Flash bank 1 will be used to store log files
	if(bank == 1)
	{
		(*Flash).FRD_INTF_CTRL.bit.DATA_CACHE_EN = 1;
		(*Flash).FRD_INTF_CTRL.bit.PREFETCH_EN = 1;
	}//END IF

	//If system is not in test mode then Flash 0 will also be used for log files so data cache and prefetch should
	// be turned on to improve performance.
#ifndef _TEST
	if(bank == 0)
	{
		(*Flash).FRD_INTF_CTRL.bit.DATA_CACHE_EN = 1;
		(*Flash).FRD_INTF_CTRL.bit.PREFETCH_EN = 1;
	}//END IF
#endif


	//Flush pipline to ensure all writes to registers has been completed.
	__asm(" RPT #7 || NOP" );

	//Protect write-protected registers.
	EDIS;

}//END FUNCTION
//*************************************************************************************************************************
//NAME: initSysPLL
//
//DESC: Initialize all system PLL and aux. PLL if in use.
//
//DATE: 23 March 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void initSysPLL(int usbActive)
{
	//Open write protected registres
	EALLOW;

	//Check to see if USB is being used, meaning the auxilarly clock has to be set up to use an external crystal oscillator.
	if(usbActive == 1)
	{
		//////////////////First set up system PLL

		//Check to see if correct settings  are already selected
		//Settings to check include the clock source, the PLL Integer Multiplier, the PLL Fractional Multiplier.
		// and the PLL Divider
		if((ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL == 0x1) &&
			(ClkCfgRegs.SYSPLLMULT.bit.IMULT == IMULT_9) &&
			(ClkCfgRegs.SYSPLLMULT.bit.FMULT == FMULT_0pt75) &&
			(ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV == PLLCLK_BY_2))
		{
			return;
		}//END IF

		//Turn off Internal Oscillator 2.
		ClkCfgRegs.CLKSRCCTL1.bit.INTOSC2OFF = 0x1;

		//Ensure that the External Oscillator is turned on.
		// Default state is turned on.
		ClkCfgRegs.CLKSRCCTL1.bit.XTALOFF = 0;

		//Set clock source to External Oscillator.
		ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 0x0;

		//Bypass the system PLL and set PLL divider to 1.
		//This will minimize the increase in current drawn when enabling the PLL.
		ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 0x0;
		ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = PLLCLK_BY_1;

		//Check to see if system PLL multipliers have been set, if not then set them.
		if(ClkCfgRegs.SYSPLLMULT.bit.IMULT != IMULT_9 || ClkCfgRegs.SYSPLLMULT.bit.FMULT != FMULT_0pt75)
		{
			int loopCount = 0;

			//Loop through procedure twice to ensure registers have been set.
			for(loopCount = 0; loopCount < 2; loopCount++)
			{
				Uint32 temp_pllMult = ClkCfgRegs.SYSPLLMULT.all;

				//Set the integer multiplier to 9 (0x9) and fractional to .75 (0x3).
				//This gives a PLL Raw Clock of 390 MHz within acceptable range. The CPU frequency will be 195 MHz to be
				// within 3% of desired 200 MHz.
				ClkCfgRegs.SYSPLLMULT.all = (temp_pllMult | 0x309 );

				//Wait for SYSPLL to lock.
				//This will take 16 microsec plus 1024 OSCCLK cycles.
				while(ClkCfgRegs.SYSPLLSTS.bit.LOCKS != 1)
				{
					//Do nothing.
				}//END WHILE
			}//END FOR
		}//END IF

		//Connect PLL output clock to system.
		ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 0x1;

		//Set final system clock divider to 2.
		ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = PLLCLK_BY_2;

		//Small 100 cycle delay.
		asm(" RPT #100 || NOP");

		////////////////Second setup auxiliary PLL

		//Set aux. clock source to use external oscillator
		ClkCfgRegs.CLKSRCCTL2.bit.AUXOSCCLKSRCSEL = 0x1;

		//Bypass the aux. PLL and set PLL divider to 1.
		//This will minimize the increase in current drawn when enabling the PLL.
		ClkCfgRegs.AUXPLLCTL1.bit.PLLCLKEN = 0x0;
		ClkCfgRegs.AUXCLKDIVSEL.bit.AUXPLLDIV= PLLCLK_BY_1;

		//Check to see if auxiliary PLL multipliers have been set, if not then set them.
		if(ClkCfgRegs.AUXPLLMULT.bit.IMULT != IMULT_1 || ClkCfgRegs.AUXPLLMULT.bit.FMULT != FMULT_0pt75)
		{
			Uint32 temp_auxPllMult = ClkCfgRegs.AUXPLLMULT.all;

			//Set the integer multipler to 1 (0x1) and fractional to .5 (0x2).
			//This gives an auxiliary PLL Raw Clock of 60 MHz.
			ClkCfgRegs.AUXPLLMULT.all = (temp_auxPllMult | (0x201));

			//Wait for AUXPLL to lock.
			//This will take 16 microsec plus 1024 AUXOSCCLK cycles.
			while(ClkCfgRegs.AUXPLLSTS.bit.LOCKS != 1)
			{
				//Do nothing.
			}//END WHILE
		}//END IF

		//Connect PLL output clock to aux.
		ClkCfgRegs.AUXPLLCTL1.bit.PLLCLKEN = 0x1;

		//Set final aux. clock divider to 1.
		ClkCfgRegs.AUXCLKDIVSEL.bit.AUXPLLDIV = PLLCLK_BY_1;

		//Small 100 cycle delay.
		asm(" RPT #100 || NOP");

		EDIS;

	}//END IF
	else
	{
		//Check to see if correct settings are already selected
		//Settings to check include the clock source, the PLL Integer Multiplier, the PLL Fractional Multiplier.
		// and the PLL Divider
		if((ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL == 0x0) &&
			(ClkCfgRegs.SYSPLLMULT.bit.IMULT == IMULT_38) &&
			(ClkCfgRegs.SYSPLLMULT.bit.FMULT == FMULT_0pt75) &&
			(ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV == PLLCLK_BY_2))
		{
			return;
		}//END IF

		//Turn off External Clock source.
		ClkCfgRegs.CLKSRCCTL1.bit.XTALOFF = 0x1;

		//Ensure that Internal Oscillator 2 is turned on.
		// Default state is turned on.
		ClkCfgRegs.CLKSRCCTL1.bit.INTOSC2OFF = 0x0;

		//Set clock source to Internal Oscillator 2.
		ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 0x0;

		//Bypass the system PLL and set PLL divider to 1.
		//This will minimize the increase in current drawn when enabling the PLL.
		ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 0x0;
		ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = PLLCLK_BY_1;

		//Check to see if PLL multipliers have been set, if not then set them.
		if(ClkCfgRegs.SYSPLLMULT.bit.IMULT != IMULT_38 || ClkCfgRegs.SYSPLLMULT.bit.FMULT != FMULT_0pt75)
		{
			int loopCount = 0;

			//Loop through procedure twice to ensure registers have been set.
			for(loopCount = 0; loopCount < 2; loopCount++)
			{
				Uint32 temp_pllMult = ClkCfgRegs.SYSPLLMULT.all;

				//Set the integer multiplier to 38 (0x26) and fractional to .75 (0x3).
				//This gives a PLL Raw Clock of 387.5 MHz within acceptable range. The CPU frequency will be 193.75 MHz to be
				// within 3% of desired 200 MHz.
				ClkCfgRegs.SYSPLLMULT.bit.IMULT = IMULT_38;
				ClkCfgRegs.SYSPLLMULT.bit.FMULT = FMULT_0pt75;
				//= (temp_pllMult | 0x326 );

				//Wait for SYSPLL to lock.
				//This will take 16 microsec plus 1024 OSCCLK cycles.
				while(ClkCfgRegs.SYSPLLSTS.bit.LOCKS != 1)
				{
					//Do nothing.
				}
			}//END FOR
		}//END IF

		//Connect PLL output clock to system.
		ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 0x1;

		//Set final system clock divider to 2.
		ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = PLLCLK_BY_2;

		//Set Low-Speed Peripheral Clock (LSPCLK) divider to 4 (~ 48.5 MHz)
		ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 0x2;

		//Small 100 cycle delay.
		asm(" RPT #100 || NOP");


	}//END ELSE

	//Lock write protected registers
	EDIS;

}//END FUNCTION

//*************************************************************************************************************************
//NAME: initPeripheralClocks
//
//DESC: Initialize all peripheral clocks to be used by the system
//
//DATE: 23 March 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void initPeripheralClocks(void)
{
	//Open write protected registers.
	EALLOW;

	//Turn on all ADC Modules.
	CpuSysRegs.PCLKCR13.bit.ADC_A = 0x1;
	CpuSysRegs.PCLKCR13.bit.ADC_B = 0x1;
	CpuSysRegs.PCLKCR13.bit.ADC_C = 0x1;
	CpuSysRegs.PCLKCR13.bit.ADC_D = 0x1;

	//Turn on I2CA/ Turn off I2CB.
	CpuSysRegs.PCLKCR9.bit.I2C_A = 0x1;
	CpuSysRegs.PCLKCR9.bit.I2C_B = 0x1;

	//Turn on USB/ turn off all other pins in PCLKCR11.
	CpuSysRegs.PCLKCR11.bit.USB_A = 0x1;
	CpuSysRegs.PCLKCR11.bit.McBSP_A = 0x0;
	CpuSysRegs.PCLKCR11.bit.McBSP_B = 0x0;

	//Turn on Comparator Modules///////////////////////////////
	//If Five Volt Output Current component is connected turn on corresponding CMPSS clock gate
	if(FIVEV_OUTPUT_CURRENT_COMPONENT)
	{
		CpuSysRegs.PCLKCR14.bit.CMPSS1 = 0x1;
	}
	else
		CpuSysRegs.PCLKCR14.bit.CMPSS1 = 0x0;

#ifndef _LAUNCH
	if(TWELVEV_OUTPUT_CURRENT_COMPONENT)
	{
		CpuSysRegs.PCLKCR14.bit.CMPSS2 = 0x1;
	}
	else
		CpuSysRegs.PCLKCR14.bit.CMPSS2 = 0x0;
#endif

	if(AC_INPUT_VOLTAGE_COMPONENT)
	{
		CpuSysRegs.PCLKCR14.bit.CMPSS3 = 0x1;
	}
	else
		CpuSysRegs.PCLKCR14.bit.CMPSS3 = 0x0;

#ifdef _LAUNCH
	if(CAP_VOLTAGE_COMPONENT)
	{
		CpuSysRegs.PCLKCR14.bit.CMPSS4 = 0x1;
	}
	else
		CpuSysRegs.PCLKCR14.bit.CMPSS4 = 0x0;

#else
	if(CAP_VOLTAGE_COMPONENT)
	{
		CpuSysRegs.PCLKCR14.bit.CMPSS8 = 0x1;
	}
	else
		CpuSysRegs.PCLKCR14.bit.CMPSS8 = 0x0;
	#endif

#ifdef _LAUNCH
	if(SOLAR_INPUT_VOLTAGE_COMPONENT)
	{
		CpuSysRegs.PCLKCR14.bit.CMPSS2 = 0x1;
	}
	else
		CpuSysRegs.PCLKCR14.bit.CMPSS2 = 0x0;

#else
	if(SOLAR_INPUT_VOLTAGE_COMPONENT)
	{
		CpuSysRegs.PCLKCR14.bit.CMPSS6 = 0x1;
	}
	else
		CpuSysRegs.PCLKCR14.bit.CMPSS6 = 0x0;
	#endif

	CpuSysRegs.PCLKCR14.bit.CMPSS5 = 0x0;
	CpuSysRegs.PCLKCR14.bit.CMPSS7 = 0x0;

	/////NOTE
	//Need to add in initilization gate and init for USB.

	//Turn on ePWM Modules//////////////////////////////
	//If Solar Voltage component is connected turn on corresponding EPWM clock gate
	if(SOLAR_INPUT_VOLTAGE_COMPONENT)
	{
		CpuSysRegs.PCLKCR2.bit.EPWM1 = 0x1;
	}
	else
		CpuSysRegs.PCLKCR2.bit.EPWM1 = 0x0;

	if(AC_INPUT_VOLTAGE_COMPONENT)
	{
		CpuSysRegs.PCLKCR2.bit.EPWM2 = 0x1;
	}
	else
		CpuSysRegs.PCLKCR2.bit.EPWM2 = 0x0;

	if(CAP_VOLTAGE_COMPONENT)
	{
		CpuSysRegs.PCLKCR2.bit.EPWM3 = 0x1;
	}
	else
		CpuSysRegs.PCLKCR2.bit.EPWM3 = 0x0;

	CpuSysRegs.PCLKCR2.bit.EPWM12 = 0x1;
	//Set EPWM modules clocks to be PLLSYS/2
	ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0x1;


	//Turn off all other peripherals.
	CpuSysRegs.PCLKCR0.all = 0;
	CpuSysRegs.PCLKCR1.all = 0;
	CpuSysRegs.PCLKCR3.all = 0;
	CpuSysRegs.PCLKCR4.all = 0;
	CpuSysRegs.PCLKCR6.all = 0;
#ifdef _DEMO
	CpuSysRegs.PCLKCR7.bit.SCI_A = 0x1;
	CpuSysRegs.PCLKCR7.bit.SCI_B = 0x1;
	CpuSysRegs.PCLKCR7.bit.SCI_C = 0x1;
	CpuSysRegs.PCLKCR7.bit.SCI_D = 0x1;

	CpuSysRegs.PCLKCR10.bit.CAN_A = 1;
	CpuSysRegs.PCLKCR10.bit.CAN_B = 1;

#else
	CpuSysRegs.PCLKCR7.all = 0;
#endif
	CpuSysRegs.PCLKCR10.all = 0;
	CpuSysRegs.PCLKCR12.all = 0;
	CpuSysRegs.PCLKCR16.all = 0;

	//Close write protected registers.
	EDIS;
}



//#######################################################################################################################

