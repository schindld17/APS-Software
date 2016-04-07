//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	ADCMod.c
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
#include "APS_GlobalDefs.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SAMPLEAVG 10
#define VREFHIB 3.3



//*************************************************************************************************************************
//NAME: ADCInit
//
//DESC: Initialize all ADC modules....
//
//DATE: 26 March 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************

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

	//Initialize ADC modules to be used with 12V input line (Block R on System-Block Diagram).
	//This line corresponds to the Hydrogen Fuel Cell input line.
	//ADCINC4 will be used to measure the voltage on this line and ADCINA1 will be used
	// to measure the current on this line.
	EALLOW;

	///////////ADCINC4/SOC0 Values//////////////////////////////////////////

	//SOC0 will convert pin ADCINC4.
	AdccRegs.ADCSOC0CTL.bit.CHSEL = 0x4;
	//SOC0 sample window is 26 SYSCLOCK cycles (~82ns).
	AdccRegs.ADCSOC0CTL.bit.ACQPS = 0x22;
	//Associate SOC0 with Post Processing Block (PPB) 1
	AdccRegs.ADCPPB1CONFIG.bit.CONFIG = 0x0;
	//Set SOC0 PPB offset correction to about .1V (ADCOut - 124)
	AdccRegs.ADCPPB1OFFREF = 0x7C;
	//End of conversion on SOC0 will set INT1 flag.
	AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0x0;
	//Enable interrupt flag
	AdccRegs.ADCINTSEL1N2.bit.INT1E = 0x1;
	//Clear INT1 interrupt flag
	AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 0x1;

	///////////ADCINA1/SOC0 Values//////////////////////////////////////////

	//SOC0 will convert pin ADCINA1.
	AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0x1;
	//SOC0 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcaRegs.ADCSOC0CTL.bit.ACQPS = 0x22;
	//Associate SOC0 with Post Processing Block (PPB) 1
	AdcaRegs.ADCPPB1CONFIG.bit.CONFIG = 0x0;
	//Set SOC0 PPB offset correction to about .1V (ADCOut - 124)
	AdcaRegs.ADCPPB1OFFREF = 0x7C;
	//End of conversion on SOC0 will set INT1 flag.
	AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0x0;
	//Enable interrupt flag
	AdcaRegs.ADCINTSEL1N2.bit.INT1E = 0x1;
	//Clear INT1 interrupt flag
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 0x1;

	EDIS;

	//Initialize ADC modules to be used with 20V input line (Block N on System-Block Diagram).
	//This line corresponds to the Solar input line.
	//ADCINC2 will be used to measure the voltage on this line and ADCINA0 will be used
	// to measure the current on this line.
	EALLOW;

	///////////ADCINC2/SOC1 Values//////////////////////////////////////////

	//SOC1 will convert pin ADCINC2.
	AdccRegs.ADCSOC1CTL.bit.CHSEL = 0x2;
	//SOC0 sample window is 26 SYSCLOCK cycles (~82ns).
	AdccRegs.ADCSOC1CTL.bit.ACQPS = 0x22;
	//Associate SOC1 with Post Processing Block (PPB) 2
	AdccRegs.ADCPPB2CONFIG.bit.CONFIG = 0x1;
	//Set SOC1 PPB offset correction to about .1V (ADCOut - 124)
	AdccRegs.ADCPPB2OFFREF = 0x7C;
	//End of conversion on SOC1 will set INT2 flag.
	AdccRegs.ADCINTSEL1N2.bit.INT2SEL = 0x1;
	//Enable interrupt flag
	AdccRegs.ADCINTSEL1N2.bit.INT2E = 0x1;
	//Clear INT2 interrupt flag
	AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 0x1;

	///////////ADCINA0/SOC1 Values//////////////////////////////////////////

	//SOC1 will convert pin ADCINA0.
	AdcaRegs.ADCSOC1CTL.bit.CHSEL = 0x0;
	//SOC1 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcaRegs.ADCSOC1CTL.bit.ACQPS = 0x22;
	//Associate SOC1 with Post Processing Block (PPB) 2
	AdcaRegs.ADCPPB2CONFIG.bit.CONFIG = 0x1;
	//Set SOC1 PPB offset correction to about .1V (ADCOut - 124)
	AdcaRegs.ADCPPB2OFFREF = 0x7C;
	//End of conversion on SOC1 will set INT2 flag.
	AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = 0x1;
	//Enable interrupt flag
	AdcaRegs.ADCINTSEL1N2.bit.INT2E = 0x1;
	//Clear INT2 interrupt flag
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 0x1;

	EDIS;

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize ADC modules to be used with AC input line (Block B on System-Block Diagram).
	//This line corresponds to the AC input line.
	//ADCINB2 will be used to measure the voltage on this line and ADCINB1 will be used
	// to measure the current on this line.
	EALLOW;

	///////////ADCINB2/SOC0 Values//////////////////////////////////////////

	//SOC0 will convert pin ADCINB2.
	AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0x2;
	//SOC0 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcbRegs.ADCSOC0CTL.bit.ACQPS = 0x22;
	//Associate SOC0 with Post Processing Block (PPB) 1
	AdcbRegs.ADCPPB1CONFIG.bit.CONFIG = 0x0;
	//Set SOC0 PPB offset correction to about .1V (ADCOut - 124)
	AdcbRegs.ADCPPB1OFFREF = 0x7C;
	//End of conversion on SOC0 will set INT1 flag.
	AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 0x0;
	//Enable interrupt flag
	AdcbRegs.ADCINTSEL1N2.bit.INT1E = 0x1;
	//Clear INT1 interrupt flag
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 0x1;

	///////////ADCINB1/SOC1 Values//////////////////////////////////////////

	//SOC1 will convert pin ADCINB1.
	AdcbRegs.ADCSOC1CTL.bit.CHSEL = 0x1;
	//SOC1 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcbRegs.ADCSOC1CTL.bit.ACQPS = 0x22;
	//Associate SOC1 with Post Processing Block (PPB) 2
	AdcbRegs.ADCPPB2CONFIG.bit.CONFIG = 0x1;
	//Set SOC1 PPB offset correction to about .1V (ADCOut - 124)
	AdcbRegs.ADCPPB2OFFREF = 0x7C;
	//End of conversion on SOC1 will set INT2 flag.
	AdcbRegs.ADCINTSEL1N2.bit.INT2SEL = 0x1;
	//Enable interrupt flag
	AdcbRegs.ADCINTSEL1N2.bit.INT2E = 0x1;
	//Clear INT2 interrupt flag
	AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 0x1;

	EDIS;

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize ADC modules to be used with Cap line (Block L on System-Block Diagram).
	//This line corresponds to the line ahead of the Cap Bank.
	//ADCIND2 will be used to measure the voltage on this line and ADCIND4 will be used
	// to measure the current on this line.
	EALLOW;

	///////////ADCIND2/SOC0 Values//////////////////////////////////////////

	//SOC0 will convert pin ADCIND2.
	AdcdRegs.ADCSOC0CTL.bit.CHSEL = 0x2;
	//SOC0 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcdRegs.ADCSOC0CTL.bit.ACQPS = 0x22;
	//Associate SOC0 with Post Processing Block (PPB) 1
	AdcdRegs.ADCPPB1CONFIG.bit.CONFIG = 0x0;
	//Set SOC0 PPB offset correction to about .1V (ADCOut - 124)
	AdcdRegs.ADCPPB1OFFREF = 0x7C;
	//End of conversion on SOC0 will set INT1 flag.
	AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = 0x0;
	//Enable interrupt flag
	AdcdRegs.ADCINTSEL1N2.bit.INT1E = 0x1;
	//Clear INT1 interrupt flag
	AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 0x1;

	///////////ADCIND4/SOC1 Values//////////////////////////////////////////

	//SOC1 will convert pin ADCIND4.
	AdcdRegs.ADCSOC1CTL.bit.CHSEL = 0x4;
	//SOC1 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcdRegs.ADCSOC1CTL.bit.ACQPS = 0x22;
	//Associate SOC1 with Post Processing Block (PPB) 2
	AdcdRegs.ADCPPB2CONFIG.bit.CONFIG = 0x1;
	//Set SOC1 PPB offset correction to about .1V (ADCOut - 124)
	AdcdRegs.ADCPPB2OFFREF = 0x7C;
	//End of conversion on SOC1 will set INT2 flag.
	AdcdRegs.ADCINTSEL1N2.bit.INT2SEL = 0x1;
	//Enable interrupt flag
	AdcdRegs.ADCINTSEL1N2.bit.INT2E = 0x1;
	//Clear INT2 interrupt flag
	AdcdRegs.ADCINTFLGCLR.bit.ADCINT2 = 0x1;

	EDIS;

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize ADC modules to be used with 5V output line (Block E on System-Block Diagram).
	//ADCINA2 will be used to measure the current on this line.
	EALLOW;

	///////////ADCINA2/SOC2 Values//////////////////////////////////////////

	//SOC2 will convert pin ADCINA2.
	AdcaRegs.ADCSOC2CTL.bit.CHSEL = 0x2;
	//SOC2 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcaRegs.ADCSOC2CTL.bit.ACQPS = 0x22;
	//Associate SOC2 with Post Processing Block (PPB) 3
	AdcaRegs.ADCPPB3CONFIG.bit.CONFIG = 0x2;
	//Set SOC2 PPB offset correction to about .1V (ADCOut - 124)
	AdcaRegs.ADCPPB3OFFREF = 0x7C;
	//End of conversion on SOC2 will set INT3 flag.
	AdcaRegs.ADCINTSEL3N4.bit.INT3SEL = 0x2;
	//Enable interrupt flag
	AdcaRegs.ADCINTSEL3N4.bit.INT3E = 0x1;
	//Clear INT3 interrupt flag
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 0x1;

	EDIS;

	/////////////////////////////////////////////////////////////////////////////////////////
	//Initialize ADC modules to be used with 12V output line (Block F on System-Block Diagram).
	//ADCINA4 will be used to measure the current on this line.
	EALLOW;

	///////////ADCINA4/SOC3 Values//////////////////////////////////////////

	//SOC3 will convert pin ADCINA4.
	AdcaRegs.ADCSOC3CTL.bit.CHSEL = 0x4;
	//SOC3 sample window is 26 SYSCLOCK cycles (~82ns).
	AdcaRegs.ADCSOC3CTL.bit.ACQPS = 0x22;
	//Associate SOC3 with Post Processing Block (PPB) 4
	AdcaRegs.ADCPPB4CONFIG.bit.CONFIG = 0x3;
	//Set SOC3 PPB offset correction to about .1V (ADCOut - 124)
	AdcaRegs.ADCPPB4OFFREF = 0x7C;
	//End of conversion on SOC3 will set INT4 flag.
	AdcaRegs.ADCINTSEL3N4.bit.INT4SEL = 0x3;
	//Enable interrupt flag
	AdcaRegs.ADCINTSEL3N4.bit.INT4E = 0x1;
	//Clear INT2 interrupt flag
	AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 0x1;

	EDIS;


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
					//Force start of conversion on SOC1
					AdccRegs.ADCSOCFRC1.bit.SOC0 = 0x01;

					//Wait for end of conversion.
					while(AdccRegs.ADCINTFLG.bit.ADCINT1 == 0){}  //Wait for ADCINT1
					AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;        //Clear ADCINT1

					//Get ADC sample result from SOC1
					sample = AdcaResultRegs.ADCPPB1RESULT.bit.PPBRESULT + sample;

					break;
				case HYDRO_CUR://ADCINA1/SOC0/PPB1/INT2
					//Force start of conversion on SOC0
					AdcaRegs.ADCSOCFRC1.bit.SOC0 = 0x01;

					//Wait for end of conversion.
					while(AdcaRegs.ADCINTFLG.bit.ADCINT2 == 0){}  //Wait for ADCINT2
					AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;        //Clear ADCINT2

					//Get ADC sample result from SOC0
					sample = AdcaResultRegs.ADCPPB1RESULT.bit.PPBRESULT + sample;

					break;
				case SOL_VOLT://ADCINC2/SOC1/PPB2/INT2
					//Force start of conversion on SOC1
					AdccRegs.ADCSOCFRC1.bit.SOC1 = 0x01;

					//Wait for end of conversion.
					while(AdccRegs.ADCINTFLG.bit.ADCINT2 == 0){}  //Wait for ADCINT2
					AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;        //Clear ADCINT2

					//Get ADC sample result from SOC1
					sample = AdccResultRegs.ADCPPB2RESULT.bit.PPBRESULT + sample;

					break;
				case SOL_CUR://ADCINA0/SOC1/PPB2/INT2
					//Force start of conversion on SOC0
					AdcaRegs.ADCSOCFRC1.bit.SOC1 = 0x01;

					//Wait for end of conversion.
					while(AdcaRegs.ADCINTFLG.bit.ADCINT2 == 0){}  //Wait for ADCINT2
					AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;        //Clear ADCINT2

					//Get ADC sample result from SOC0
					sample = AdcaResultRegs.ADCPPB2RESULT.bit.PPBRESULT + sample;
					break;
				case AC_VOLT://ADCINB2/SOC0/PPB1/INT1
					//Force start of conversion on SOC0
					AdcbRegs.ADCSOCFRC1.bit.SOC0 = 0x01;

					//Wait for end of conversion.
					while(AdcbRegs.ADCINTFLG.bit.ADCINT1 == 0){}  //Wait for ADCINT1
					AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;        //Clear ADCINT1

					//Get ADC sample result from SOC0
					sample = AdcbResultRegs.ADCPPB1RESULT.bit.PPBRESULT + sample;
					break;
				case AC_CUR://ADCINB1/SOC1/PPB2/INT2
					//Force start of conversion on SOC1
					AdcbRegs.ADCSOCFRC1.bit.SOC1 = 0x01;

					//Wait for end of conversion.
					while(AdcbRegs.ADCINTFLG.bit.ADCINT2 == 0){}  //Wait for ADCINT2
					AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;        //Clear ADCINT2

					//Get ADC sample result from SOC1
					sample = AdcbResultRegs.ADCPPB2RESULT.bit.PPBRESULT + sample;
					break;
				case CAP_VOLT://ADCIND2/SOC0/PPB1/INT1
					//Force start of conversion on SOC0
					AdcdRegs.ADCSOCFRC1.bit.SOC0 = 0x01;

					//Wait for end of conversion.
					while(AdcdRegs.ADCINTFLG.bit.ADCINT1 == 0){}  //Wait for ADCINT1
					AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;        //Clear ADCINT1

					//Get ADC sample result from SOC0
					sample = AdcdResultRegs.ADCPPB1RESULT.bit.PPBRESULT + sample;

					break;
				case CAP_CUR://ADCIND4/SOC1/PPB2/INT2
					//Force start of conversion on SOC1
					AdcdRegs.ADCSOCFRC1.bit.SOC1 = 0x01;

					//Wait for end of conversion.
					while(AdcdRegs.ADCINTFLG.bit.ADCINT2 == 0){}  //Wait for ADCINT2
					AdcdRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;        //Clear ADCINT2

					//Get ADC sample result from SOC1
					sample = AdcdResultRegs.ADCPPB2RESULT.bit.PPBRESULT + sample;

					break;
				case FIVEV_CUR://ADCINA2/SOC2/PPB3/INT3
					//Force start of conversion on SOC2
					AdcaRegs.ADCSOCFRC1.bit.SOC2 = 0x01;

					//Wait for end of conversion.
					while(AdcaRegs.ADCINTFLG.bit.ADCINT3 == 0){}  //Wait for ADCINT3
					AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1;        //Clear ADCINT3

					//Get ADC sample result from SOC2
					sample = AdcaResultRegs.ADCPPB3RESULT.bit.PPBRESULT + sample;

					break;
				case TWELVEV_CUR://ADCINA4/SOC3/PPB/INT4
					//Force start of conversion on SOC3
					AdcaRegs.ADCSOCFRC1.bit.SOC3 = 0x01;

					//Wait for end of conversion.
					while(AdcaRegs.ADCINTFLG.bit.ADCINT4 == 0){}  //Wait for ADCINT4
					AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 1;        //Clear ADCINT4

					//Get ADC sample result from SOC3
					sample = AdcaResultRegs.ADCPPB4RESULT.bit.PPBRESULT + sample;

					break;
			}//END SWITCH
	}//END FOR

	//Set the final value by calculating a sample average
	finalVal = (int16_t)(sample/SAMPLEAVG);

    EDIS;

    //Return the raw temperature because the devices don't have slope/offset values
    return(finalVal);

}//END FUNCTION


//*************************************************************************************************************************
//NAME: convertADC
//
//DESC:
//
//DATE: 3 April 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************

char* convertADC(int16_t adcVal)
{
	float convVal = 0, finalVal = 0, decHalfFloat;
	int intAdcVal = (int)adcVal, intHalf, decHalfInt;
	char* finalValString;
	finalValString = (char*)malloc(16);

	//Divide the intput value by the correct resolution (2^12 or 2^16)
	if(AdcbRegs.ADCCTL2.bit.RESOLUTION == 0)
		convVal = (float)intAdcVal / 4096;
	else
		convVal = adcVal/ 65536;

	//Multiply it by reference voltage ~3.3V (VDDA)
	finalVal = convVal * VREFHIB;

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
		sprintf(finalValString, "%d.%d",intHalf,decHalfInt);
	else if(decHalfInt < 1000 & decHalfInt > 100)
		sprintf(finalValString, "%d.0%d",intHalf,decHalfInt);
	else if(decHalfInt < 100 & decHalfInt > 10)
		sprintf(finalValString, "%d.00%d",intHalf,decHalfInt);
	else
		sprintf(finalValString, "%d.000%d",intHalf,decHalfInt);

	//Output final formatted string as IntegerPortion.(LeadingZeros)DecimalPortion
    return((char *)finalValString);
}//END FUNCTION


//########################################################################################################################
