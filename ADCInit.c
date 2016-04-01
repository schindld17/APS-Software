//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	ADCInit.c
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
//	Creation Date: Mar 26, 2016 
//	Release Date:
//###########################################################################

#include "F2837xS_Adc_defines.h"
#include "F2837xS_device.h"
#include "F2837xS_Examples.h"



//###########################################################################

void ADCInit(void)
{
	EALLOW;
	//Configure all ADC Modules for 12-bit single-ended mode
	AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE );
	AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE );
	AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE );
	AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE );
	EDIS;

	//Initialize ADC modules to be used with 12V input line (Block R on System-Block Diagram).
	//This line corresponds to the Hydrogen Fuel Cell input line.
	//ADCINC4 will be used to measure the voltage on this line and ADCINA1 will be used
	// to measure the current on this line.


	//Initialize ADC modules to be used with 20V input line (Block N on System-Block Diagram).
	//This line corresponds to the Solar line.
	//ADCINC2 will be used to measure the voltage on this line and ADCINA0 will be used
	// to measure the current on this line.

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize ADC modules to be used with AC input line (Block B on System-Block Diagram).
	//This line corresponds to the AC input line.
	//ADCINB2 will be used to measure the voltage on this line and ADCINB1 will be used
	// to measure the current on this line.
	EALLOW;

	//Set ADCCLK Prescale to divide input SYSCLK by 4 (193.75 MHz / 4.0 ~ 48.5 MHz).
	AdcbRegs.ADCCTL2.bit.PRESCALE = 0x6;

	//Set Interrupt Pulse Position to late.
	AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 0x1;

	//Power up ADC module.
	AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 0x1;

	//Delay for 1ms to allow ADC module to power up.
	DELAY_US(1000);

	//SOC0 will convert pin ADCINB2.
	AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0x2;

	//SOC0 sample window is 3 SYSCLOCK cycles (~15ns).
	AdcbRegs.ADCSOC0CTL.bit.ACQPS = 0x2;

	//SOC0 will convert pin ADCINB1.
	AdcbRegs.ADCSOC1CTL.bit.CHSEL = 0x1;

	//SOC1 sample window is 3 SYSCLOCK cycles (~15ns).
	AdcbRegs.ADCSOC1CTL.bit.ACQPS = 0x2;

#ifdef _TEST
	//End of conversion on SOC0 will set INT1 flag.
	AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 0x0;

	//Enable interrupt flag
	AdcbRegs.ADCINTSEL1N2.bit.INT1E = 0x1;

	//Clear INT1 interrupt flag
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 0x1;
#endif

	EDIS;

	/////////////////////////////////////////////////////////////////////////////////////////







}//END FUNCTION
