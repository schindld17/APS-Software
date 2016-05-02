//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	ADCMod.c
//
//	TITLE: ADC Peripheral Support Functions
//
//	DESCRIPTION: Multiple Support functions for the ADC peripherals on-board.
//  Functions include an intialization function to set the proper state of
//  peripherals on system start. Function to sample desired ADC and peform
//  averaging. Funtion to convert raw result supplied from ADC result register
//  to proper scale.
//
//###########################################################################
//	Author: Dylan Schindler
//	Creation Date: 	Mar 26, 2016
//	Release Date: 	Apr 27, 2016
//###########################################################################

#include "F2837xS_Adc_defines.h"
#include "F2837xS_device.h"
#include "F2837xS_Examples.h"
#include "APS_GlobalDefs.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//The number of samples that are taken to create the average
#define SAMPLEAVG 100


//****************************************************************************
//NAME: ADCInit
//
//DESC: Initialize all ADC modules (A,B,C,D) to 12-bit singled ended mode with
//		clocks scaled to 48.5 MHZ. Below the ADC pins used within the system
//		are listed along with the basic configuration used
//
//CHANNEL CONFIG (System Function: Pin/Start-of-Conversion,Post Processing
//						block, interrupt flag)
//		Hydrogen Fuel Cell Input Voltage Measurement: 	ADCINC4/SOC0/PPB1/INT1
//		Hydrogen Fuel Cell Input Current Measurement: 	ADCINA1/SOC0/PPB1/INT1
//		Solar Input Voltage Measurement:			  	ADCINC2/SOC1/PPB2/INT2
//			NOTE: If _LAUNCH is defined then			ADCINA4/SOC3/PPB4/INT4
//		Solar Input Current Measurement: 				ADCINA0/SOC1/PPB2/INT2
//		AC Input Voltage Measurement:					ADCINB2/SOC0/PPB1/INT1
//		AC Input Current Measurement:					ADCINB1/SOC1/PPB2/INT2
//		Capacitor Voltage Measurement:					ADCIND2/SOC0/PPB1/INT1
//			NOTE: If _LAUNCH is defined then 			ADCIN14/SOC3/PPB3/INT3
//		Capacitor Current Measurement:					ADCIND4/SOC1/PPB2/INT2
//		Five Volt Output Current Measurement:			ADCINA2/SOC2/PPB3/INT3
//		Twelve Volt Output Current Measurement:			ADCINA4/SOC3/PPB4/INT4
//
//DATE: 26 March 2016
//
//AUTHOR: Dylan Schindler
//*****************************************************************************

void ADCInit(void)
{
	EALLOW;
	//Configure all ADC Modules for 12-bit single-ended mode
	AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE );
	AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE );
	AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE );
	AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE );
	EDIS;

	EALLOW;
	/////////////ADCA Module Level Values/////////////////////////////////////
	//Set ADCCLK Prescale to divide input SYSCLK by 4 (193.75 MHz / 4.0 ~ 48.5 MHz).
	AdcaRegs.ADCCTL2.bit.PRESCALE = 0x6;

	//Set Interrupt Pulse Position to late.
	AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 0x1;

	//Power up ADC module.
	AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 0x1;

	//Delay for 1ms to allow ADC module to power up.
	DELAY_US(1000);

	/////////////ADCB Module Level Values/////////////////////////////////////
	//Set ADCCLK Prescale to divide input SYSCLK by 4 (193.75 MHz / 4.0 ~ 48.5 MHz).
	AdcbRegs.ADCCTL2.bit.PRESCALE = 0x6;

	//Set Interrupt Pulse Position to late.
	AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 0x1;

	//Power up ADC module.
	AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 0x1;

	//Delay for 1ms to allow ADC module to power up.
	DELAY_US(1000);

	/////////////ADCC Module Level Values/////////////////////////////////////
	//Set ADCCLK Prescale to divide input SYSCLK by 4 (193.75 MHz / 4.0 ~ 48.5 MHz).
	AdccRegs.ADCCTL2.bit.PRESCALE = 0x6;

	//Set Interrupt Pulse Position to late.
	AdccRegs.ADCCTL1.bit.INTPULSEPOS = 0x1;

	//Power up ADC module.
	AdccRegs.ADCCTL1.bit.ADCPWDNZ = 0x1;

	//Delay for 1ms to allow ADC module to power up.
	DELAY_US(1000);

	/////////////ADCD Module Level Values/////////////////////////////////////
	//Set ADCCLK Prescale to divide input SYSCLK by 4 (193.75 MHz / 4.0 ~ 48.5 MHz).
	AdcdRegs.ADCCTL2.bit.PRESCALE = 0x6;

	//Set Interrupt Pulse Position to late.
	AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 0x1;

	//Power up ADC module.
	AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 0x1;

	//Delay for 1ms to allow ADC module to power up.
	DELAY_US(1000);

	EDIS;

	//Initialize ADC channels to be used with 12V input line (Block R on System-Block Diagram).
	//This line corresponds to the Hydrogen Fuel Cell input line.
	//ADCINC4 will be used to measure the voltage on this line and ADCINA1 will be used
	// to measure the current on this line.
if(HYDRO_INPUT_VOLTAGE_COMPONENT)
{
	EALLOW;
	///////////ADCINC4/SOC0 Values//////////////////////////////////////////
	//SOC0 will convert pin ADCINC4.
	AdccRegs.ADCSOC0CTL.bit.CHSEL = 0x4;
	//SOC0 sample window is 26 SYSCLOCK cycles (~82ns).
	AdccRegs.ADCSOC0CTL.bit.ACQPS = 0x22;
	//Associate SOC0 with Post Processing Block (PPB) 1
	AdccRegs.ADCPPB1CONFIG.bit.CONFIG = 0x0;
#ifdef _NO_VOLT_TEST
	//Set SOC0 PPB offset correction to about .1V (ADCOut - 124)
	AdccRegs.ADCPPB1OFFREF = 0x7C;
#else

#endif
	//End of conversion on SOC0 will set INT1 flag.
	AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0x0;
	//Enable interrupt flag
	AdccRegs.ADCINTSEL1N2.bit.INT1E = 0x1;
	//Clear INT1 interrupt flag
	AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 0x1;
	EDIS;
}//END IF

if(HYDRO_INPUT_CURRENT_COMPONENT)
{
	EALLOW;
	///////////ADCINA1/SOC0 Values//////////////////////////////////////////
	//SOC0 will convert pin ADCINA1.
	AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0x1;
	//SOC0 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcaRegs.ADCSOC0CTL.bit.ACQPS = 0x22;
	//Associate SOC0 with Post Processing Block (PPB) 1
	AdcaRegs.ADCPPB1CONFIG.bit.CONFIG = 0x0;
#ifdef _NO_VOLT_TEST
	//Set SOC0 PPB offset correction to about .1V (ADCOut - 124)
	AdcaRegs.ADCPPB1OFFREF = 0x7C;
#else

#endif
	//End of conversion on SOC0 will set INT1 flag.
	AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0x0;
	//Enable interrupt flag
	AdcaRegs.ADCINTSEL1N2.bit.INT1E = 0x1;
	//Clear INT1 interrupt flag
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 0x1;
	EDIS;
}//END IF

	//Initialize ADC channels to be used with 20V input line (Block N on System-Block Diagram).
	//This line corresponds to the Solar input line.
	//ADCINC2 will be used to measure the voltage on this line and ADCINA0 will be used
	// to measure the current on this line.

if(SOLAR_INPUT_VOLTAGE_COMPONENT)
{
	EALLOW;
	///////////ADCINC2/SOC1 Values//////////////////////////////////////////
#ifndef _LAUNCH

	//SOC1 will convert pin ADCINC2.
	AdccRegs.ADCSOC1CTL.bit.CHSEL = 0x2;
	//SOC0 sample window is 26 SYSCLOCK cycles (~82ns).
	AdccRegs.ADCSOC1CTL.bit.ACQPS = 0x22;
	//Associate SOC1 with Post Processing Block (PPB) 2
	AdccRegs.ADCPPB2CONFIG.bit.CONFIG = 0x1;
#ifdef _NO_VOLT_TEST
	//Set SOC1 PPB offset correction to about .1V (ADCOut - 124)
	AdccRegs.ADCPPB2OFFREF = 0x7C;
#else

#endif
	//End of conversion on SOC1 will set INT2 flag.
	AdccRegs.ADCINTSEL1N2.bit.INT2SEL = 0x1;
	//Enable interrupt flag
	AdccRegs.ADCINTSEL1N2.bit.INT2E = 0x1;
	//Clear INT2 interrupt flag
	AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 0x1;

#else
	EALLOW;
	///////////ADCINA4/SOC3 Values//////////////////////////////////////////
	//SOC3 will convert pin ADCINA4.
	AdcaRegs.ADCSOC3CTL.bit.CHSEL = 0x4;
	//SOC3 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcaRegs.ADCSOC3CTL.bit.ACQPS = 0x22;
	//Associate SOC3 with Post Processing Block (PPB) 4
	AdcaRegs.ADCPPB4CONFIG.bit.CONFIG = 0x3;
#ifdef _NO_VOLT_TEST
	//Set SOC2 PPB offset correction to about .1V (ADCOut - 124)
	AdcaRegs.ADCPPB3OFFREF = 0x7C;
#else
	//Set SOC3 PPB offset correction to about .1V (ADCOut - 124)
	AdcaRegs.ADCPPB4OFFREF = 0x7C;
#endif
	//End of conversion on SOC3 will set INT4 flag.
	AdcaRegs.ADCINTSEL3N4.bit.INT4SEL = 0x3;
	//Enable interrupt flag
	AdcaRegs.ADCINTSEL3N4.bit.INT4E = 0x1;
	//Clear INT2 interrupt flag
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 0x1;
	EDIS;
#endif
}//END IF

if(SOLAR_INPUT_CURRENT_COMPONENT)
{
	EALLOW;
	///////////ADCINA0/SOC1 Values//////////////////////////////////////////
	//SOC1 will convert pin ADCINA0.
	AdcaRegs.ADCSOC1CTL.bit.CHSEL = 0x0;
	//SOC1 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcaRegs.ADCSOC1CTL.bit.ACQPS = 0x22;
	//Associate SOC1 with Post Processing Block (PPB) 2
	AdcaRegs.ADCPPB2CONFIG.bit.CONFIG = 0x1;
#ifdef _NO_VOLT_TEST
	//Set SOC1 PPB offset correction to about .1V (ADCOut - 124)
	AdcaRegs.ADCPPB2OFFREF = 0x7C;
#else

#endif
	//End of conversion on SOC1 will set INT2 flag.
	AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = 0x1;
	//Enable interrupt flag
	AdcaRegs.ADCINTSEL1N2.bit.INT2E = 0x1;
	//Clear INT2 interrupt flag
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 0x1;
	EDIS;
}//END IF

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize ADC channels to be used with AC input line (Block B on System-Block Diagram).
	//This line corresponds to the AC input line.
	//ADCINB2 will be used to measure the voltage on this line and ADCINB1 will be used
	// to measure the current on this line.
if(AC_INPUT_VOLTAGE_COMPONENT)
{
	EALLOW;
	///////////ADCINB2/SOC0 Values//////////////////////////////////////////
	//SOC0 will convert pin ADCINB2.
	AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0x2;
	//SOC0 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcbRegs.ADCSOC0CTL.bit.ACQPS = 0x22;
	//Associate SOC0 with Post Processing Block (PPB) 1
	AdcbRegs.ADCPPB1CONFIG.bit.CONFIG = 0x0;
#ifdef _NO_VOLT_TEST
	//Set SOC1 PPB offset correction to about .1V (ADCOut - 124)
	AdcaRegs.ADCPPB2OFFREF = 0x7C;
#else
	//Set SOC0 PPB offset correction to about .1V (ADCOut - 124)
	AdcbRegs.ADCPPB1OFFREF = 0x0E;
#endif
	//End of conversion on SOC0 will set INT1 flag.
	AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 0x0;
	//Enable interrupt flag
	AdcbRegs.ADCINTSEL1N2.bit.INT1E = 0x1;
	//Clear INT1 interrupt flag
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 0x1;
	EDIS;
}//END IF

if(AC_INPUT_CURRENT_COMPONENT)
{
	EALLOW;
	///////////ADCINB1/SOC1 Values//////////////////////////////////////////
	//SOC1 will convert pin ADCINB1.
	AdcbRegs.ADCSOC1CTL.bit.CHSEL = 0x1;
	//SOC1 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcbRegs.ADCSOC1CTL.bit.ACQPS = 0x22;
	//Associate SOC1 with Post Processing Block (PPB) 2
	AdcbRegs.ADCPPB2CONFIG.bit.CONFIG = 0x1;
#ifdef _NO_VOLT_TEST
	//Set SOC1 PPB offset correction to about .1V (ADCOut - 124)
	AdcbRegs.ADCPPB2OFFREF = 0x7C;
#else

#endif
	//End of conversion on SOC1 will set INT2 flag.
	AdcbRegs.ADCINTSEL1N2.bit.INT2SEL = 0x1;
	//Enable interrupt flag
	AdcbRegs.ADCINTSEL1N2.bit.INT2E = 0x1;
	//Clear INT2 interrupt flag
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 0x1;
	EDIS;
}//END IF

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize ADC channels to be used with Cap line (Block L on System-Block Diagram).
	//This line corresponds to the line ahead of the Cap Bank.
	//ADCIND2 will be used to measure the voltage on this line and ADCIND4 will be used
	// to measure the current on this line.
if(CAP_VOLTAGE_COMPONENT)
{
#ifdef _LAUNCH
	EALLOW;
	///////////ADCIN14/SOC3 Values//////////////////////////////////////////
	//SOC0 will convert pin ADCIN14.
	AdcbRegs.ADCSOC3CTL.bit.CHSEL = 0xE;
	//SOC0 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcbRegs.ADCSOC3CTL.bit.ACQPS = 0x22;
	//Associate SOC4 with Post Processing Block (PPB) 3
	AdcbRegs.ADCPPB3CONFIG.bit.CONFIG = 0x3;
#ifdef _NO_VOLT_TEST
	//Set SOC1 PPB offset correction to about .1V (ADCOut - 124)
	AdcbRegs.ADCPPB2OFFREF = 0x7C;
#else

#endif
	//End of conversion on SOC3 will set INT3 flag.
	AdcbRegs.ADCINTSEL3N4.bit.INT3SEL = 0x3;
	//Enable interrupt flag
	AdcbRegs.ADCINTSEL3N4.bit.INT3E = 0x1;
	//Clear INT3 interrupt flag
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT3 = 0x1;
	EDIS;
#else
	EALLOW;
	///////////ADCIND2/SOC0 Values//////////////////////////////////////////
	//SOC0 will convert pin ADCIND2.
	AdcdRegs.ADCSOC0CTL.bit.CHSEL = 0x2;
	//SOC0 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcdRegs.ADCSOC0CTL.bit.ACQPS = 0x22;
	//Associate SOC0 with Post Processing Block (PPB) 1
	AdcdRegs.ADCPPB1CONFIG.bit.CONFIG = 0x0;
#ifdef _NO_VOLT_TEST
	//Set SOC1 PPB offset correction to about .1V (ADCOut - 124)
	AdcbRegs.ADCPPB2OFFREF = 0x7C;
#else

#endif
	//End of conversion on SOC0 will set INT1 flag.
	AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = 0x0;
	//Enable interrupt flag
	AdcdRegs.ADCINTSEL1N2.bit.INT1E = 0x1;
	//Clear INT1 interrupt flag
	AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 0x1;
	EDIS;
#endif
}//END IF

if(CAP_CURRENT_COMPONENT)
{
	EALLOW;
	///////////ADCIND4/SOC1 Values//////////////////////////////////////////
	//SOC1 will convert pin ADCIND4.
	AdcdRegs.ADCSOC1CTL.bit.CHSEL = 0x4;
	//SOC1 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcdRegs.ADCSOC1CTL.bit.ACQPS = 0x22;
	//Associate SOC1 with Post Processing Block (PPB) 2
	AdcdRegs.ADCPPB2CONFIG.bit.CONFIG = 0x1;
#ifdef _NO_VOLT_TEST
	//Set SOC1 PPB offset correction to about .1V (ADCOut - 124)
	AdcdRegs.ADCPPB2OFFREF = 0x7C;
#else

#endif
	//End of conversion on SOC1 will set INT2 flag.
	AdcdRegs.ADCINTSEL1N2.bit.INT2SEL = 0x1;
	//Enable interrupt flag
	AdcdRegs.ADCINTSEL1N2.bit.INT2E = 0x1;
	//Clear INT2 interrupt flag
	AdcdRegs.ADCINTFLGCLR.bit.ADCINT2 = 0x1;
	EDIS;
}//END IF

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize ADC channels to be used with 5V output line (Block E on System-Block Diagram).
	//ADCINA2 will be used to measure the current on this line.
if(FIVEV_OUTPUT_CURRENT_COMPONENT)
{
	EALLOW;
	///////////ADCINA2/SOC2 Values//////////////////////////////////////////
	//SOC2 will convert pin ADCINA2.
	AdcaRegs.ADCSOC2CTL.bit.CHSEL = 0x2;
	//SOC2 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcaRegs.ADCSOC2CTL.bit.ACQPS = 0x22;
	//Associate SOC2 with Post Processing Block (PPB) 3
	AdcaRegs.ADCPPB3CONFIG.bit.CONFIG = 0x2;
#ifdef _NO_VOLT_TEST
	//Set SOC2 PPB offset correction to about .1V (ADCOut - 124)
	AdcaRegs.ADCPPB3OFFREF = 0x7C;
#else

#endif
	//End of conversion on SOC2 will set INT3 flag.
	AdcaRegs.ADCINTSEL3N4.bit.INT3SEL = 0x2;
	//Enable interrupt flag
	AdcaRegs.ADCINTSEL3N4.bit.INT3E = 0x1;
	//Clear INT3 interrupt flag
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 0x1;
	EDIS;
}//END IF

#ifndef _LAUNCH
	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize ADC channels to be used with 12V output line (Block F on System-Block Diagram).
	//ADCINA4 will be used to measure the current on this line.
if(TWELVEV_OUTPUT_CURRENT_COMPONENT)
{
	EALLOW;
	///////////ADCINA4/SOC3 Values//////////////////////////////////////////
	//SOC3 will convert pin ADCINA4.
	AdcaRegs.ADCSOC3CTL.bit.CHSEL = 0x4;
	//SOC3 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcaRegs.ADCSOC3CTL.bit.ACQPS = 0x22;
	//Associate SOC3 with Post Processing Block (PPB) 4
	AdcaRegs.ADCPPB4CONFIG.bit.CONFIG = 0x3;
#ifdef _NO_VOLT_TEST
	//Set SOC2 PPB offset correction to about .1V (ADCOut - 124)
	AdcaRegs.ADCPPB3OFFREF = 0x7C;
#else
	//Set SOC3 PPB offset correction to about .1V (ADCOut - 124)
	AdcaRegs.ADCPPB4OFFREF = 0x7C;
#endif
	//End of conversion on SOC3 will set INT4 flag.
	AdcaRegs.ADCINTSEL3N4.bit.INT4SEL = 0x3;
	//Enable interrupt flag
	AdcaRegs.ADCINTSEL3N4.bit.INT4E = 0x1;
	//Clear INT2 interrupt flag
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 0x1;
	EDIS;
}//END IF
#endif



}//END FUNCTION

//*************************************************************************************************************************
//NAME: sampleADC
//
//DESC: Function to create a sample average of the requested adc by polling the same SOC multiple times
//
//DATE: 3 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************

int16_t sampleADC(ADC_Selection ADCModule)
{
	int32_t sample = 0, sampleCount = 0, finalVal = 0;

	EALLOW;
	//Sample the same SOC multiple times to create a time average
	for(sampleCount = 0; sampleCount < SAMPLEAVG; sampleCount++)
	{
		//Select the registers/SOC to poll based on provided enum value
		switch (ADCModule)
			{
				case HYDRO_VOLT://ADCINC4/SOC0/PPB1/INT1
					if(HYDRO_INPUT_VOLTAGE_COMPONENT)
					{
						//Force start of conversion on SOC1
						AdccRegs.ADCSOCFRC1.bit.SOC0 = 0x01;

						//Wait for end of conversion.
						while(AdccRegs.ADCINTFLG.bit.ADCINT1 == 0){}  //Wait for ADCINT1
						AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;        //Clear ADCINT1

						//Get ADC sample result from SOC1
						sample = AdcaResultRegs.ADCPPB1RESULT.bit.PPBRESULT + sample;
					}//END IF
					else
						sample = 0;
					break;
				case HYDRO_CUR://ADCINA1/SOC0/PPB1/INT2
					if(HYDRO_INPUT_CURRENT_COMPONENT)
					{
						//Force start of conversion on SOC0
						AdcaRegs.ADCSOCFRC1.bit.SOC0 = 0x01;

						//Wait for end of conversion.
						while(AdcaRegs.ADCINTFLG.bit.ADCINT2 == 0){}  //Wait for ADCINT2
						AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;        //Clear ADCINT2

						//Get ADC sample result from SOC0
						sample = AdcaResultRegs.ADCPPB1RESULT.bit.PPBRESULT + sample;
					}//END IF
					else
						sample = 0;
					break;
#ifndef _LAUNCH
				case SOL_VOLT://ADCINC2/SOC1/PPB2/INT2
					if(SOLAR_INPUT_VOLTAGE_COMPONENT)
					{
						//Force start of conversion on SOC1
						AdccRegs.ADCSOCFRC1.bit.SOC1 = 0x01;

						//Wait for end of conversion.
						while(AdccRegs.ADCINTFLG.bit.ADCINT2 == 0){}  //Wait for ADCINT2
						AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;        //Clear ADCINT2

						//Get ADC sample result from SOC1
						sample = AdccResultRegs.ADCPPB2RESULT.bit.PPBRESULT + sample;
					}
					else
						sample = 0;
					break;
#else
				case SOL_VOLT://ADCINA4/SOC2/PPB3/INT3
					if(SOLAR_INPUT_VOLTAGE_COMPONENT)
					{
						//Force start of conversion on SOC3
						AdcaRegs.ADCSOCFRC1.bit.SOC3 = 0x01;

						//Wait for end of conversion.
						while(AdcaRegs.ADCINTFLG.bit.ADCINT4 == 0){}  //Wait for ADCINT4
						AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 1;        //Clear ADCINT4

						//Get ADC sample result from SOC3
						sample = AdcaResultRegs.ADCPPB4RESULT.bit.PPBRESULT + sample;
					}//END IF
					else
						sample = 0;
					break;
#endif

				case SOL_CUR://ADCINA0/SOC1/PPB2/INT2
					if(SOLAR_INPUT_CURRENT_COMPONENT)
					{
						//Force start of conversion on SOC0
						AdcaRegs.ADCSOCFRC1.bit.SOC1 = 0x01;

						//Wait for end of conversion.
						while(AdcaRegs.ADCINTFLG.bit.ADCINT2 == 0){}  //Wait for ADCINT2
						AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;        //Clear ADCINT2

						//Get ADC sample result from SOC0
						sample = AdcaResultRegs.ADCPPB2RESULT.bit.PPBRESULT + sample;
					}//END IF
					else
						sample = 0;
					break;
				case AC_VOLT://ADCINB2/SOC0/PPB1/INT1
					if(AC_INPUT_VOLTAGE_COMPONENT)
					{
						//Force start of conversion on SOC0
						AdcbRegs.ADCSOCFRC1.bit.SOC0 = 0x01;

						//Wait for end of conversion.
						while(AdcbRegs.ADCINTFLG.bit.ADCINT1 == 0){}  //Wait for ADCINT1
						AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;        //Clear ADCINT1

						//Get ADC sample result from SOC0
						sample = AdcbResultRegs.ADCPPB1RESULT.bit.PPBRESULT + sample;
					}//END IF
					else
						sample = 0;
					break;
				case AC_CUR://ADCINB1/SOC1/PPB2/INT2
					if(AC_INPUT_CURRENT_COMPONENT)
					{
						//Force start of conversion on SOC1
						AdcbRegs.ADCSOCFRC1.bit.SOC1 = 0x01;

						//Wait for end of conversion.
						while(AdcbRegs.ADCINTFLG.bit.ADCINT2 == 0){}  //Wait for ADCINT2
						AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;        //Clear ADCINT2

						//Get ADC sample result from SOC1
						sample = AdcbResultRegs.ADCPPB2RESULT.bit.PPBRESULT + sample;
					}//END IF
					else
						sample = 0;
					break;
#ifdef _LAUNCH
				case CAP_VOLT://ADCINB14/SOC3/PPB3/INT3
					if(CAP_VOLTAGE_COMPONENT)
					{
						//Force start of conversion on SOC3
						AdcdRegs.ADCSOCFRC1.bit.SOC3 = 0x01;

						//Wait for end of conversion.
						while(AdcdRegs.ADCINTFLG.bit.ADCINT3 == 0){}  //Wait for ADCINT1
						AdcdRegs.ADCINTFLGCLR.bit.ADCINT3 = 1;        //Clear ADCINT1

						//Get ADC sample result from SOC3
						sample = AdcdResultRegs.ADCPPB3RESULT.bit.PPBRESULT + sample;
					}//END IF
					else
						sample = 0;
					break;
#else
				case CAP_VOLT://ADCIND2/SOC0/PPB1/INT1
					if(CAP_CURRENT_COMPONENT)
					{
						//Force start of conversion on SOC0
						AdcdRegs.ADCSOCFRC1.bit.SOC0 = 0x01;

						//Wait for end of conversion.
						while(AdcdRegs.ADCINTFLG.bit.ADCINT1 == 0){}  //Wait for ADCINT1
						AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;        //Clear ADCINT1

						//Get ADC sample result from SOC0
						sample = AdcdResultRegs.ADCPPB1RESULT.bit.PPBRESULT + sample;
					}//END IF
					else
						sample = 0;
					break;
#endif

				case CAP_CUR://ADCIND4/SOC1/PPB2/INT2
					if(CAP_CURRENT_COMPONENT)
					{
						//Force start of conversion on SOC1
						AdcdRegs.ADCSOCFRC1.bit.SOC1 = 0x01;

						//Wait for end of conversion.
						while(AdcdRegs.ADCINTFLG.bit.ADCINT2 == 0){}  //Wait for ADCINT2
						AdcdRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;        //Clear ADCINT2

						//Get ADC sample result from SOC1
						sample = AdcdResultRegs.ADCPPB2RESULT.bit.PPBRESULT + sample;
					}//END IF
					else
						sample = 0;
					break;
				case FIVEV_CUR://ADCINA2/SOC2/PPB3/INT3
					if(FIVEV_OUTPUT_CURRENT_COMPONENT)
					{
						//Force start of conversion on SOC2
						AdcaRegs.ADCSOCFRC1.bit.SOC2 = 0x01;

						//Wait for end of conversion.
						while(AdcaRegs.ADCINTFLG.bit.ADCINT3 == 0){}  //Wait for ADCINT3
						AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1;        //Clear ADCINT3

						//Get ADC sample result from SOC2
						sample = AdcaResultRegs.ADCPPB3RESULT.bit.PPBRESULT + sample;
					}//END IF
					else
						sample = 0;
					break;
				case TWELVEV_CUR://ADCINA4/SOC3/PPB/INT4
					if(TWELVEV_OUTPUT_CURRENT_COMPONENT)
					{
						//Force start of conversion on SOC3
						AdcaRegs.ADCSOCFRC1.bit.SOC3 = 0x01;

						//Wait for end of conversion.
						while(AdcaRegs.ADCINTFLG.bit.ADCINT4 == 0){}  //Wait for ADCINT4
						AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 1;        //Clear ADCINT4

						//Get ADC sample result from SOC3
						sample = AdcaResultRegs.ADCPPB4RESULT.bit.PPBRESULT + sample;
					}//END IF
					else
						sample = 0;
					break;
			}//END SWITCH
	}//END FOR
	//Set the final value by calculating a sample average
	finalVal = (int16_t)(sample/SAMPLEAVG);

    EDIS;

    //If an overflow has occurred or some other unknown error set the value to zero
    if(finalVal < 0)
    	finalVal = 0;

    return(finalVal);

}//END FUNCTION


//****************************************************************************************
//NAME: convertADC
//
//DESC: Take as input an unsigned 16-bit integer value from the result register of an ADC
//		channel and scale the value to proper size if a voltage step-down circuit is used
//		ahead of the pin. Convert the value to a properly formatted string (ex. 3.0004).
//		NOTE: If the values from the ADC channel result register are being saved to a log
//			  within system flash then this function should not be used. This function is
//			  to be used to allow for a more readable format to be shown to users during
//			  debug or system demo
//
//DATE: 3 April 2016
//
//AUTHOR: Dylan Schindler
//****************************************************************************************

void convertADC(int16_t adcVal, ADC_Selection ADCModule, char* returnString)
{
	float convVal = 0, finalVal = 0, decHalfFloat;
	int intAdcVal = (int)adcVal, intHalf, decHalfInt, moduleConnected = 1;


	//Divide the intput value by the correct resolution (2^12 or 2^16)
	if(AdcbRegs.ADCCTL2.bit.RESOLUTION == 0)
		convVal = (float)intAdcVal / 4096;
	else
		convVal = adcVal/ 65536;

	//Select the correct reference voltage constant
	//These (VREF_) values are defined within APS_GlobalDefs.h
	switch (ADCModule)
		{
			case HYDRO_VOLT://ADCINC4
				//Check to see if the component is in use
				//The value of this symbol is defined in APS_GlobalDefs.h
				if(HYDRO_INPUT_VOLTAGE_COMPONENT)
				{
#ifdef _NO_VOLT_TEST
					//Multiply it by reference voltage ~3.3V (VDDA)
					finalVal = convVal * VDDA;
#else
					//Multipy it by the Hydrogen Input Reference Voltage Value
					finalVal = convVal * VREFHI_HYDRO_VOLT;
#endif
				}//END IF
				else
					moduleConnected = 0;
				break;
			case HYDRO_CUR://ADCINA1
				//Check to see if the component is in use
				//The value of this symbol is defined in APS_GlobalDefs.h
				if(HYDRO_INPUT_CURRENT_COMPONENT)
				{
					//NOTE: ADD Current Conversions
				}//END IF
				//NOTE: ADD Current Conversions
				break;
			case SOL_VOLT://ADCINC2
				//Check to see if the component is in use
				//The value of this symbol is defined in APS_GlobalDefs.h
				if(SOLAR_INPUT_VOLTAGE_COMPONENT)
				{
#ifdef _NO_VOLT_TEST
					//Multiply it by reference voltage ~3.3V (VDDA)
					finalVal = convVal * VDDA;
#else
					//Multiply it by the Solar Input Reference Voltage Value
					finalVal = convVal * VREFHI_SOL_VOLT;
#endif
				}//END IF
				else
					moduleConnected = 0;
				break;
			case SOL_CUR://ADCINA0
				//Check to see if the component is in use
				//The value of this symbol is defined in APS_GlobalDefs.h
				if(SOLAR_INPUT_CURRENT_COMPONENT)
				{
						//NOTE: ADD Current Conversions
				}//END IF
				break;
			case AC_VOLT://ADCINB2
				//Check to see if the component is in use
				//The value of this symbol is defined in APS_GlobalDefs.h
				if(AC_INPUT_VOLTAGE_COMPONENT)
				{
#ifdef _NO_VOLT_TEST
					//Multiply it by reference voltage ~3.3V (VDDA)
					finalVal = convVal * VDDA;
#else
					//Multiply it by the AC Input Reference Voltage Value
					finalVal = convVal * VREFHI_AC_VOLT;
#endif
				}//END IF
				else
					moduleConnected = 0;
				break;
			case AC_CUR://ADCINB1
				//Check to see if the component is in use
				//The value of this symbol is defined in APS_GlobalDefs.h
				if(AC_INPUT_CURRENT_COMPONENT)
				{
					//NOTE: ADD Current Conversions
				}//END IF
				break;
			case CAP_VOLT://ADCIND2
				//Check to see if the component is in use
				//The value of this symbol is defined in APS_GlobalDefs.h
				if(CAP_VOLTAGE_COMPONENT)
				{
#ifdef _NO_VOLT_TEST
					//Multiply it by reference voltage ~3.3V (VDDA)
					finalVal = convVal * VDDA;
#else
					finalVal = convVal * VREFHI_CAP_VOLT;
#endif
				}//END IF
				else
					moduleConnected = 0;
				break;
			case CAP_CUR://ADCIND4
				//Check to see if the component is in use
				//The value of this symbol is defined in APS_GlobalDefs.h
				if(CAP_CURRENT_COMPONENT)
				{
					//NOTE: ADD Current Conversions
				}//END IF
				break;
			case FIVEV_CUR://ADCINA2
				//Check to see if the component is in use
				//The value of this symbol is defined in APS_GlobalDefs.h
				if(FIVEV_OUTPUT_CURRENT_COMPONENT)
				{
						//NOTE: ADD Current Conversions
				}//END IF
				else
					moduleConnected = 0;
				break;
			case TWELVEV_CUR://ADCINA4
				//Check to see if the component is in use
				//The value of this symbol is defined in APS_GlobalDefs.h
				if(TWELVEV_OUTPUT_CURRENT_COMPONENT)
				{
						//NOTE: ADD Current Conversions
				}//END IF
				else
					moduleConnected = 0;
				break;
		}//END SWITCH

	if(moduleConnected)
	{
		//Convert float to integer to obtain integer portion of results
		intHalf = finalVal;
		//Obtain decimal part by removing integer portion
		decHalfFloat = finalVal - intHalf;
		//Scale the decimal portion to be within 4 digits past decimal point
		decHalfFloat = decHalfFloat * 10000;
		//Remove remaining decimal portion digits
		decHalfInt = decHalfFloat;

		//Depending on the size of the decimal portion create a string with the correct number of leading zeros
		if(decHalfInt > 1000)
			sprintf(returnString, "%d.%d",intHalf,decHalfInt);
		else if(decHalfInt < 1000 & decHalfInt > 100)
			sprintf(returnString, "%d.0%d",intHalf,decHalfInt);
		else if(decHalfInt < 100 & decHalfInt > 10)
			sprintf(returnString, "%d.00%d",intHalf,decHalfInt);
		else
			sprintf(returnString, "%d.000%d",intHalf,decHalfInt);
	}
	else
		sprintf(returnString,"N/A");

	//Output final formatted string as IntegerPortion.(LeadingZeros)DecimalPortion
    return;
}//END FUNCTION


//########################################################################################################################
