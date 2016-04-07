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
	//Initialize CMPSS to be used with Solar Input (Block N on System-Block Diagram).
	//ADCINC2 will be used as the input pin.
	EALLOW;

	///////////CMPSS6 Positive Values//////////////////////////////////////////
	//Enable CMPSS
	Cmpss6Regs.COMPCTL.bit.COMPDACE = 0x1;
	//Set Negative signal for CMPSS to come from internal DAC
	Cmpss6Regs.COMPCTL.bit.COMPLSOURCE = 0x1;
	//Use VDDA as reference for DAC
	Cmpss6Regs.COMPDACCTL.bit.SELREF = 0x0;
	//Set DAC to midpoint for reference
	Cmpss6Regs.DACHVALS.bit.DACVAL = 2048;
	//Asynch output feeds for CTRIPH and CTRIPOUTH
	Cmpss6Regs.COMPCTL.bit.CTRIPHSEL = 0x0;
	Cmpss6Regs.COMPCTL.bit.CTRIPOUTHSEL = 0x0;
	//Configure OUTPUTXBAR1 to be CTRIPOUT6H
	OutputXbarRegs.OUTPUT1MUX0TO15CFG.bit.MUX10 = 0x0;

	EDIS;

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize CMPSS to be used with AC Input (Block B on System-Block Diagram).
	//ADCINB2 will be used as the input pin.
	EALLOW;

	///////////CMPSS3 Positive Values//////////////////////////////////////////
	//Enable CMPSS
	Cmpss3Regs.COMPCTL.bit.COMPDACE = 0x1;
	//Set Negative signal for CMPSS to come from internal DAC
	Cmpss3Regs.COMPCTL.bit.COMPLSOURCE = 0x1;
	//Use VDDA as reference for DAC
	Cmpss3Regs.COMPDACCTL.bit.SELREF = 0x0;
	//Set DAC to midpoint for reference
	Cmpss3Regs.DACHVALS.bit.DACVAL = 2048;
	//Asynch output feeds for CTRIPH and CTRIPOUTH
	Cmpss3Regs.COMPCTL.bit.CTRIPHSEL = 0x0;
	Cmpss3Regs.COMPCTL.bit.CTRIPOUTHSEL = 0x0;
	//Configure OUTPUTXBAR2 to be CTRIPOUT3H
	OutputXbarRegs.OUTPUT2MUX0TO15CFG.bit.MUX4 = 0x0;

	EDIS;

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize CMPSS to be used with Cap Line (Block L on System-Block Diagram).
	//ADCIND2 will be used as the input pin.
	EALLOW;

	///////////CMPSS8 Positive Values//////////////////////////////////////////
	//Enable CMPSS
	Cmpss8Regs.COMPCTL.bit.COMPDACE = 0x1;
	//Set Negative signal for CMPSS to come from internal DAC
	Cmpss8Regs.COMPCTL.bit.COMPLSOURCE = 0x1;
	//Use VDDA as reference for DAC
	Cmpss8Regs.COMPDACCTL.bit.SELREF = 0x0;
	//Set DAC to midpoint for reference
	Cmpss8Regs.DACHVALS.bit.DACVAL = 2048;
	//Asynch output feeds for CTRIPH and CTRIPOUTH
	Cmpss8Regs.COMPCTL.bit.CTRIPHSEL = 0x0;
	Cmpss8Regs.COMPCTL.bit.CTRIPOUTHSEL = 0x0;
	//Configure OUTPUTXBAR3 to be CTRIPOUT8H
	OutputXbarRegs.OUTPUT3MUX0TO15CFG.bit.MUX14 = 0x0;

	EDIS;

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize CMPSS to be used with 5V Output Line (Block E on System-Block Diagram).
	//ADCINA2 will be used as the input pin.
	EALLOW;

	///////////CMPSS1 Positive Values//////////////////////////////////////////
	//Enable CMPSS
	Cmpss1Regs.COMPCTL.bit.COMPDACE = 0x1;
	//Set Negative signal for CMPSS to come from internal DAC
	Cmpss1Regs.COMPCTL.bit.COMPLSOURCE = 0x1;
	//Use VDDA as reference for DAC
	Cmpss1Regs.COMPDACCTL.bit.SELREF = 0x0;
	//Set DAC to midpoint for reference
	Cmpss1Regs.DACHVALS.bit.DACVAL = 2048;
	//Asynch output feeds for CTRIPH and CTRIPOUTH
	Cmpss1Regs.COMPCTL.bit.CTRIPHSEL = 0x0;
	Cmpss1Regs.COMPCTL.bit.CTRIPOUTHSEL = 0x0;
	//Configure OUTPUTXBAR4 to be CTRIPOUT1H
	OutputXbarRegs.OUTPUT4MUX0TO15CFG.bit.MUX0 = 0x0;

	EDIS;

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize CMPSS to be used with 12V Output Line (Block F on System-Block Diagram).
	//ADCINA4 will be used as the input pin.
	EALLOW;

	///////////CMPSS2 Positive Values//////////////////////////////////////////
	//Enable CMPSS
	Cmpss2Regs.COMPCTL.bit.COMPDACE = 0x1;
	//Set Negative signal for CMPSS to come from internal DAC
	Cmpss2Regs.COMPCTL.bit.COMPLSOURCE = 0x1;
	//Use VDDA as reference for DAC
	Cmpss2Regs.COMPDACCTL.bit.SELREF = 0x0;
	//Set DAC to midpoint for reference
	Cmpss2Regs.DACHVALS.bit.DACVAL = 2048;
	//Asynch output feeds for CTRIPH and CTRIPOUTH
	Cmpss2Regs.COMPCTL.bit.CTRIPHSEL = 0x0;
	Cmpss2Regs.COMPCTL.bit.CTRIPOUTHSEL = 0x0;
	//Configure OUTPUTXBAR5 to be CTRIPOUT2H
	OutputXbarRegs.OUTPUT5MUX0TO15CFG.bit.MUX2 = 0x0;

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
	/////////////////////////////////////////////////////////////////////////////////
	//Initialize CMPSS to be used with AC Input (Block B on System-Block Diagram).
	//ADCINB2 will be used as the input pin.
	EALLOW;

	///////////Epwm2  Values////////////////////////////////////////////////////////
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0x0;
	//Configure EPWM to run at SYSCLK
	ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0x0;
	EPwm2Regs.TBCTL.bit.CLKDIV = 0x0;
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0x0;
	//Initialize Dummy Values
	EPwm2Regs.TBCTR = 0;
	EPwm2Regs.TBPRD = 0xFFFF;
	//Configure EPWM1 to output high on Trip Zone A Trip
	EPwm2Regs.TZCTL.bit.TZA = 0x1;
	//Configure Digital Comparator Trip Zone Output A (DCA) Event 1 to be Trip 4
	EPwm2Regs.TZDCSEL.bit.DCAEVT1 = 0x2;
	EPwm2Regs.DCTRIPSEL.bit.DCAHCOMPSEL = 0xF;
	EPwm2Regs.DCAHTRIPSEL.bit.TRIPINPUT4 = 0x1;
	//Configure DCA as One-Shot-Trip
	EPwm2Regs.TZSEL.bit.DCAEVT1 = 0x1;
	//Configure DCA to be unfiltered and asynch
	EPwm2Regs.DCACTL.bit.EVT1SRCSEL = 0x0;
	EPwm2Regs.DCACTL.bit.EVT1FRCSYNCSEL = 0x1;
	CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0x1;
	//Configure Trip 4 to be Comparator High
	EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX4 = 0x0;
	//Enable Trip 4 Mux for Output
	EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX4 = 0x0;
    //Enable Trip 4 Mux for Output
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX4   = 0x1;
	//Clear trip flags
	EPwm2Regs.TZCLR.bit.OST = 0x1;
	EPwm2Regs.TZCLR.bit.INT = 0x1;
	//Enable DCA interrupt
	EPwm2Regs.TZEINT.bit.OST = 0x1;
	//Enable PWM
	EPwm2Regs.TBCTL.bit.CTRMODE = 0x0;

	EDIS;

}//END FUNCTION

//###########################################################################

