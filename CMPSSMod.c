//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	CMPSSMod.c
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
//	Creation Date: Apr 6, 2016 
//	Release Date:
//###########################################################################

#include "F2837xS_device.h"
#include "F2837xS_Examples.h"
#include "APS_GlobalDefs.h"

//*************************************************************************************************************************
//NAME: InitCMPSS
//
//DESC:
//
//DATE: 5 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void CMPSSInit(void)
{

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize CMPSS to be used with Solar Input Voltage (Block N on System-Block Diagram).
	//ADCINC2 will be used as the input pin.
	EALLOW;
#ifdef _LAUNCH
	///////////CMPSS2 Positive Values//////////////////////////////////////////
		//Enable CMPSS
		Cmpss2Regs.COMPCTL.bit.COMPDACE = 0x1;
		//Use VDDA as reference for DAC
		Cmpss2Regs.COMPDACCTL.bit.SELREF = 0x0;
		////////////CMPSS High//////////////////////
		//Set Negative signal for CMPSSH to come from internal DAC
		Cmpss2Regs.COMPCTL.bit.COMPHSOURCE = 0x0;
		Cmpss2Regs.COMPCTL.bit.COMPHINV = 0x0;
	#ifdef _NO_VOLT_TEST
		//Set DAC to midpoint for reference
		Cmpss2Regs.DACHVALS.bit.DACVAL = 2048;
	#else
		//Set DAC to midpoint for reference
		Cmpss2Regs.DACHVALS.bit.DACVAL = CMPSS_SOL_V;
	#endif
		//Asynch output feeds for CTRIPH
		Cmpss2Regs.COMPCTL.bit.CTRIPHSEL = 0x0;

		EDIS;
#else
	///////////CMPSS6 Positive Values//////////////////////////////////////////
		//Enable CMPSS
		Cmpss6Regs.COMPCTL.bit.COMPDACE = 0x1;
		//Use VDDA as reference for DAC
		Cmpss6Regs.COMPDACCTL.bit.SELREF = 0x0;
		////////////CMPSS High//////////////////////
		//Set Negative signal for CMPSSH to come from internal DAC
		Cmpss6Regs.COMPCTL.bit.COMPHSOURCE = 0x0;
		Cmpss6Regs.COMPCTL.bit.COMPHINV = 0x0;
	#ifdef _NO_VOLT_TEST
		//Set DAC to midpoint for reference
		Cmpss6Regs.DACHVALS.bit.DACVAL = 2048;
	#else
		//Set DAC to midpoint for reference
		Cmpss6Regs.DACHVALS.bit.DACVAL = CMPSS_SOL_V;
	#endif
		//Asynch output feeds for CTRIPH
		Cmpss6Regs.COMPCTL.bit.CTRIPHSEL = 0x0;

		EDIS;
#endif

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize CMPSS to be used with AC Input Voltage (Block B on System-Block Diagram).
	//ADCINB2 will be used as the input pin.
	EALLOW;

	///////////CMPSS3 Positive Values//////////////////////////////////////////
	//Enable CMPSS
	Cmpss3Regs.COMPCTL.bit.COMPDACE = 0x1;
	//Use VDDA as reference for DAC
	Cmpss3Regs.COMPDACCTL.bit.SELREF = 0x0;
	////////////CMPSS High//////////////////////
	//Set Negative signal for CMPSSH to come from internal DAC
	Cmpss3Regs.COMPCTL.bit.COMPHSOURCE = 0x0;
	Cmpss3Regs.COMPCTL.bit.COMPHINV = 0x0;
#ifdef _NO_VOLT_TEST
	//Set DAC to midpoint for reference
	Cmpss3Regs.DACHVALS.bit.DACVAL = 2048;
#else
	//Set DAC to midpoint for reference
	Cmpss3Regs.DACHVALS.bit.DACVAL = CMPSS_AC_V;
#endif
	//Asynch output feeds for CTRIPH
	Cmpss3Regs.COMPCTL.bit.CTRIPHSEL = 0x0;

	EDIS;

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize CMPSS to be used with Cap Line (Block L on System-Block Diagram).
	//ADCIND2 will be used as the input pin.
	EALLOW;
#ifdef _LAUNCH
	///////////CMPSS8 Positive Values//////////////////////////////////////////
	//Enable CMPSS
	Cmpss4Regs.COMPCTL.bit.COMPDACE = 0x1;
	//Use VDDA as reference for DAC
	Cmpss4Regs.COMPDACCTL.bit.SELREF = 0x0;
	////////////CMPSS High//////////////////////
	//Set Negative signal for CMPSSH to come from internal DAC
	Cmpss4Regs.COMPCTL.bit.COMPHSOURCE = 0x0;
	Cmpss4Regs.COMPCTL.bit.COMPHINV = 0x0;
#ifdef _NO_VOLT_TEST
	//Set DAC to midpoint for reference
	Cmpss4Regs.DACHVALS.bit.DACVAL = 2048;
#else
	//Set DAC to midpoint for reference
	Cmpss4Regs.DACHVALS.bit.DACVAL = CMPSS_CAP_H;
#endif
	//Asynch output feeds for CTRIPH
	Cmpss4Regs.COMPCTL.bit.CTRIPHSEL = 0x0;
	////////////CMPSS Low//////////////////////
	//Set Negative signal for CMPSSL to come from internal DAC
	Cmpss4Regs.COMPCTL.bit.COMPLSOURCE = 0x0;
	Cmpss4Regs.COMPCTL.bit.COMPLINV = 0x0;
#ifdef _NO_VOLT_TEST
	//Set DAC to midpoint for reference
	Cmpss4Regs.DACLVALS.bit.DACVAL = 100;
#else
	//Set DAC to midpoint for reference
	Cmpss4Regs.DACLVALS.bit.DACVAL = CMPSS_CAP_L;
#endif
	//Asynch output feeds for CTRIPH and CTRIPOUTH
	Cmpss4Regs.COMPCTL.bit.CTRIPLSEL = 0x0;

	EDIS;
#else
	///////////CMPSS8 Positive Values//////////////////////////////////////////
	//Enable CMPSS
	Cmpss8Regs.COMPCTL.bit.COMPDACE = 0x1;
	//Use VDDA as reference for DAC
	Cmpss8Regs.COMPDACCTL.bit.SELREF = 0x0;
	////////////CMPSS High//////////////////////
	//Set Negative signal for CMPSSH to come from internal DAC
	Cmpss8Regs.COMPCTL.bit.COMPHSOURCE = 0x0;
	Cmpss8Regs.COMPCTL.bit.COMPHINV = 0x0;
#ifdef _NO_VOLT_TEST
	//Set DAC to midpoint for reference
	Cmpss8Regs.DACHVALS.bit.DACVAL = 2048;
#else
	//Set DAC to midpoint for reference
	Cmpss8Regs.DACHVALS.bit.DACVAL = CMPSS_CAP_H;
#endif
	//Asynch output feeds for CTRIPH
	Cmpss8Regs.COMPCTL.bit.CTRIPHSEL = 0x0;
	////////////CMPSS Low//////////////////////
	//Set Negative signal for CMPSSL to come from internal DAC
	Cmpss8Regs.COMPCTL.bit.COMPLSOURCE = 0x0;
	Cmpss8Regs.COMPCTL.bit.COMPLINV = 0x0;
#ifdef _NO_VOLT_TEST
	//Set DAC to midpoint for reference
	Cmpss8Regs.DACLVALS.bit.DACVAL = 100;
#else
	//Set DAC to midpoint for reference
	Cmpss8Regs.DACLVALS.bit.DACVAL = CMPSS_CAP_L;
#endif
	//Asynch output feeds for CTRIPH and CTRIPOUTH
	Cmpss8Regs.COMPCTL.bit.CTRIPLSEL = 0x0;

	EDIS;
#endif


	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize CMPSS to be used with 5V Output Line (Block E on System-Block Diagram).
	//ADCINA2 will be used as the input pin.
	EALLOW;

	///////////CMPSS1 Positive Values//////////////////////////////////////////
	//Enable CMPSS
	Cmpss1Regs.COMPCTL.bit.COMPDACE = 0x1;
	//Use VDDA as reference for DAC
	Cmpss1Regs.COMPDACCTL.bit.SELREF = 0x0;
	////////////CMPSS High//////////////////////
	//Set Negative signal for CMPSSH to come from internal DAC
	Cmpss1Regs.COMPCTL.bit.COMPHSOURCE = 0x0;
	Cmpss1Regs.COMPCTL.bit.COMPHINV = 0x0;
#ifdef _NO_VOLT_TEST
	//Set DAC to midpoint for reference
	Cmpss1Regs.DACHVALS.bit.DACVAL = 2048;
#else
	//Set DAC to midpoint for reference
	Cmpss1Regs.DACHVALS.bit.DACVAL = CMPSS_FIVE;
#endif
	//Asynch output feeds for CTRIPH
	Cmpss1Regs.COMPCTL.bit.CTRIPHSEL = 0x0;

	EDIS;

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize CMPSS to be used with 12V Output Line (Block F on System-Block Diagram).
	//ADCINA4 will be used as the input pin.
	EALLOW;

	///////////CMPSS2 Positive Values//////////////////////////////////////////
	//Enable CMPSS
	Cmpss2Regs.COMPCTL.bit.COMPDACE = 0x1;
	//Use VDDA as reference for DAC
	Cmpss2Regs.COMPDACCTL.bit.SELREF = 0x0;
	////////////CMPSS High//////////////////////
	//Set Negative signal for CMPSSH to come from internal DAC
	Cmpss2Regs.COMPCTL.bit.COMPHSOURCE = 0x0;
	Cmpss2Regs.COMPCTL.bit.COMPHINV = 0x0;
#ifdef _NO_VOLT_TEST
	//Set DAC to midpoint for reference
	Cmpss2Regs.DACHVALS.bit.DACVAL = 2048;
#else
	//Set DAC to midpoint for reference
	Cmpss2Regs.DACHVALS.bit.DACVAL = CMPSS_TWELVE;
#endif
	//Asynch output feeds for CTRIPH
	Cmpss2Regs.COMPCTL.bit.CTRIPHSEL = 0x0;

	EDIS;

}//END FUNCTION


//*************************************************************************************************************************
//NAME: InitEPWM
//
//DESC:
//
//DATE: 5 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void EPWMInit(void)
{
	EALLOW;

	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0x0;

	//////////////////////////////////////////////////////////////////////////////////////////////
	//Initialize EPWM1 to be used with Solar Input Voltage (CMPSS6) (Block N on System-Block Diagram).
	//ADCINB2 will be used as the input pin.

	///////////Epwm1  Values/////////////////////////////////////////////////////////////////////
	//Configure EPWM to run at SYSCLOCK/(128*14) ~ 54018Hz.
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0x7;
	EPwm1Regs.TBCTL.bit.CLKDIV = 0x7;
	//Clear the counter
	EPwm1Regs.TBCTR = 0;
	//Set period to be aproximately 1 sec.
	EPwm1Regs.TBPRD = 0xD302;
	//Up-count mode
	EPwm1Regs.TBCTL.bit.CTRMODE = 0x0;
	//Don't load counter from phase register enable
	EPwm1Regs.TBCTL.bit.PHSEN =0x0;
	//Set counter-compare A to shadow mode
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = 0x1;
	//Set counter-compare A load to be when CTR = 0
	EPwm1Regs.CMPCTL.bit.LOADAMODE = 0x0;
#ifdef _LAUNCH
	//Connect Epwm1 with CMPSS2
	EPwmXbarRegs.TRIP5MUXENABLE.bit.MUX2 = 0x1;
	EPwmXbarRegs.TRIP5MUX0TO15CFG.bit.MUX2 = 0x0;
#else
	//Connect Epwm1 with CMPSS6
	EPwmXbarRegs.TRIP5MUXENABLE.bit.MUX10 = 0x1;
	EPwmXbarRegs.TRIP5MUX0TO15CFG.bit.MUX10 = 0x0;
#endif

	//Configure DCA High to be Trip 5
	EPwm1Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 0x4;
	//Configure DCB High to be Trip 5
	EPwm1Regs.DCTRIPSEL.bit.DCBHCOMPSEL = 0x4;
	//Configure DCB to be unfiltered and asynch
	EPwm1Regs.DCBCTL.bit.EVT1SRCSEL = 0x0;
	EPwm1Regs.DCBCTL.bit.EVT1FRCSYNCSEL = 0x1;
	//Configure Digital Comparator Trip Zone Output A Event 1 (DCAEVT1) to be DCAH 1
	EPwm1Regs.TZDCSEL.bit.DCAEVT1 = 0x2;
	//Configure Digital Comparator Trip Zone Output B Event 1 (DCBEVT1) to be DCAH 0
	EPwm1Regs.TZDCSEL.bit.DCBEVT1 = 0x1;
	//Configure DCA to be unfiltered and asynch
	EPwm1Regs.DCACTL.bit.EVT1SRCSEL = 0x0;
	EPwm1Regs.DCACTL.bit.EVT1FRCSYNCSEL = 0x1;
	//Clear trip flags
	EPwm1Regs.TZCLR.bit.INT = 0x1;
	EPwm1Regs.TZCLR.bit.DCAEVT1 = 0x1;
	EPwm1Regs.TZCLR.bit.DCBEVT1 = 0x1;
	//Enable initial interrupts if component is connected
	if(SOLAR_INPUT_VOLTAGE_COMPONENT)
	{
		EPwm1Regs.TZEINT.bit.DCAEVT1 = 0x1;
		EPwm1Regs.TZEINT.bit.DCBEVT1 = 0x0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////
	//Initialize EPWM2 to be used with AC Input Voltage (CMPSS3) (Block B on System-Block Diagram).
	//ADCINB2 will be used as the input pin.

	///////////Epwm2  Values/////////////////////////////////////////////////////////////////////
	//Configure EPWM to run at SYSCLOCK/(128*14) ~ 54018Hz.
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0x7;
	EPwm2Regs.TBCTL.bit.CLKDIV = 0x7;
	//Clear the counter
	EPwm2Regs.TBCTR = 0;
	//Set period to be aproximately 1 sec.
	EPwm2Regs.TBPRD = 0xD302;
	//Up-count mode
	EPwm2Regs.TBCTL.bit.CTRMODE = 0x0;
	//Don't load counter from phase register enable
	EPwm2Regs.TBCTL.bit.PHSEN =0x0;
	//Set counter-compare A to shadow mode
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = 0x1;
	//Set counter-compare A load to be when CTR = 0
	EPwm2Regs.CMPCTL.bit.LOADAMODE = 0x0;
	//Connect Epwm2 with CMPSS3
	EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX4 = 0x1;
	EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX4 = 0x0;
	//Configure DCA High to be Trip 4
	EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 0x3;
	//Configure DCB High to be Trip 4
	EPwm2Regs.DCTRIPSEL.bit.DCBHCOMPSEL = 0x3;
	//Configure DCB to be unfiltered and asynch
	EPwm2Regs.DCBCTL.bit.EVT1SRCSEL = 0x0;
	EPwm2Regs.DCBCTL.bit.EVT1FRCSYNCSEL = 0x1;
	//Configure Digital Comparator Trip Zone Output A Event 1 (DCAEVT1) to be DCAH 1
	EPwm2Regs.TZDCSEL.bit.DCAEVT1 = 0x2;
	//Configure Digital Comparator Trip Zone Output B Event 1 (DCBEVT1) to be DCAH 0
	EPwm2Regs.TZDCSEL.bit.DCBEVT1 = 0x1;
	//Configure DCA to be unfiltered and asynch
	EPwm2Regs.DCACTL.bit.EVT1SRCSEL = 0x0;
	EPwm2Regs.DCACTL.bit.EVT1FRCSYNCSEL = 0x1;
	//Clear trip flags
	EPwm2Regs.TZCLR.bit.INT = 0x1;
	EPwm2Regs.TZCLR.bit.DCAEVT1 = 0x1;
	EPwm2Regs.TZCLR.bit.DCBEVT1 = 0x1;
	//Enable initial interrupts if component is connected
	if(AC_INPUT_VOLTAGE_COMPONENT)
	{
		EPwm2Regs.TZEINT.bit.DCAEVT1 = 0x1;
		EPwm2Regs.TZEINT.bit.DCBEVT1 = 0x0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////
	//Initialize EPWM2 to be used with AC Input Voltage (CMPSS3) (Block B on System-Block Diagram).
	//ADCINB2 will be used as the input pin.

	///////////Epwm2  Values/////////////////////////////////////////////////////////////////////
	//Configure EPWM to run at SYSCLOCK/(128*14) ~ 54018Hz.
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0x7;
	EPwm3Regs.TBCTL.bit.CLKDIV = 0x7;
	//Clear the counter
	EPwm3Regs.TBCTR = 0;
	//Set period to be aproximately 1 sec.
	EPwm3Regs.TBPRD = 0xD302;
	//Up-count mode
	EPwm3Regs.TBCTL.bit.CTRMODE = 0x0;
	//Don't load counter from phase register enable
	EPwm3Regs.TBCTL.bit.PHSEN =0x0;
	//Set counter-compare A to shadow mode
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = 0x1;
	//Set counter-compare A load to be when CTR = 0
	EPwm3Regs.CMPCTL.bit.LOADAMODE = 0x0;
#ifdef _LAUNCH
	//Connect Epwm3 with CMPSS4 High on Trip 7
	EPwmXbarRegs.TRIP7MUXENABLE.bit.MUX6 = 0x1;
	EPwmXbarRegs.TRIP7MUX0TO15CFG.bit.MUX6 = 0x0;
	//Connect Epwm3 with CMPSS4 Low on Trip 8
	EPwmXbarRegs.TRIP8MUXENABLE.bit.MUX7 = 0x1;
	EPwmXbarRegs.TRIP8MUX0TO15CFG.bit.MUX7  = 0x0;
#else
	//Connect Epwm2 with CMPSS8 High on Trip 7
	EPwmXbarRegs.TRIP7MUXENABLE.bit.MUX14 = 0x1;
	EPwmXbarRegs.TRIP7MUX0TO15CFG.bit.MUX14 = 0x0;
	//Connect Epwm3 with CMPSS8 Low on Trip 8
	EPwmXbarRegs.TRIP8MUXENABLE.bit.MUX15 = 0x1;
	EPwmXbarRegs.TRIP8MUX0TO15CFG.bit.MUX15 = 0x0;
#endif
	//Configure DCA High to be Trip 7
	EPwm3Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 0x6;
	//Configure DCB High to be Trip 7
	EPwm3Regs.DCTRIPSEL.bit.DCBHCOMPSEL = 0x6;
	//Configure DCA Low to be Trip 8
	EPwm3Regs.DCTRIPSEL.bit.DCALCOMPSEL = 0x7;
	//Configure DCB Low to be Trip 8
	EPwm3Regs.DCTRIPSEL.bit.DCBLCOMPSEL = 0x7;
	//Configure DCB to be unfiltered and asynch
	EPwm3Regs.DCBCTL.bit.EVT1SRCSEL = 0x0;
	EPwm3Regs.DCBCTL.bit.EVT1FRCSYNCSEL = 0x1;
	EPwm3Regs.DCBCTL.bit.EVT2SRCSEL = 0x0;
	EPwm3Regs.DCBCTL.bit.EVT2FRCSYNCSEL = 0x1;
	//Configure Digital Comparator Trip Zone Output A Event 1 (DCAEVT1) to be DCAH 1
	EPwm3Regs.TZDCSEL.bit.DCAEVT1 = 0x2;
	//Configure Digital Comparator Trip Zone Output A Event 2 (DCAEVT1) to be DCAL 0
	EPwm3Regs.TZDCSEL.bit.DCAEVT2 = 0x3;
	//Configure Digital Comparator Trip Zone Output B Event 1 (DCBEVT1) to be DCAH 0
	EPwm3Regs.TZDCSEL.bit.DCBEVT1 = 0x1;
	//Configure Digital Comparator Trip Zone Output B Event 2 (DCBEVT1) to be DCAL 1
	EPwm3Regs.TZDCSEL.bit.DCBEVT2 = 0x4;
	//Configure DCA to be unfiltered and asynch
	EPwm3Regs.DCACTL.bit.EVT1SRCSEL = 0x0;
	EPwm3Regs.DCACTL.bit.EVT1FRCSYNCSEL = 0x1;
	EPwm3Regs.DCACTL.bit.EVT2SRCSEL = 0x0;
	EPwm3Regs.DCACTL.bit.EVT2FRCSYNCSEL = 0x1;
	//Clear trip flags
	EPwm3Regs.TZCLR.bit.INT = 0x1;
	EPwm3Regs.TZCLR.bit.DCAEVT1 = 0x1;
	EPwm3Regs.TZCLR.bit.DCAEVT2 = 0x1;
	EPwm3Regs.TZCLR.bit.DCBEVT1 = 0x1;
	EPwm3Regs.TZCLR.bit.DCBEVT2 = 0x1;
	//Enable initial interrupts if component is connected
	if(CAP_VOLTAGE_COMPONENT)
	{
		EPwm3Regs.TZEINT.bit.DCAEVT1 = 0x1;
		EPwm3Regs.TZEINT.bit.DCAEVT2 = 0x1;
		EPwm3Regs.TZEINT.bit.DCBEVT1 = 0x0;
		EPwm3Regs.TZEINT.bit.DCBEVT2 = 0x0;
	}

	EDIS;


}//END FUNCTION

//###########################################################################

