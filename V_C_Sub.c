//###########################################################################
//
// FILE:	V_C_Sub.c
//
// TITLE:	Voltage/Current Measurement Subsystem
//
// DESCRIPTION:
//
//
//
//
//
//###########################################################################
// Author: Dylan Schindler
// Release Date:
//###########################################################################
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <F2837xS_adc.h>
#include <F2837xS_Adc_defines.h>

void adc_init()
{
	EALLOW;

	//Write Configurations
	AdcdRegs.ADCCTL2.bit.PRESCALE = 0x06;	//Set ADCCLK divider to 4
	AdcSetMode(ADC_ADCD,ADC_RESOLUTION_12BIT,ADC_SIGNALMODE_SINGLE);	//Configure ADCD for single-ended signal with 12b resolution

	//Set pulse positions to late
	AdcdRegs.



}


int16_t sampleADC(void)
{
	int16_t value;

    //Force start of conversion on SOC0
    AdcaRegs.ADCSOCFRC1.all = 0x03;

    //Wait for end of conversion.
    while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0){}  //Wait for ADCINT1
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;        //Clear ADCINT1

    //Get temp sensor sample result from SOC0
    temp = AdcaResultRegs.ADCRESULT1;

    //Return the raw temperature because the devices don't have slope/offset values
    return(temp);

}



