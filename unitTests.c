//###########################################################################
//	PROJECT: APS-Software
//
//	FILE:	unitTests.c
//
//	TITLE: Unit/Sub-System Test Functions
//
//	DESCRIPTION: Basic unit tests used to determine specific system functionality
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

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <file.h>
#include "F2837xS_device.h"
#include "APS_GlobalPrototypes.h"
#include "F2837xS_Examples.h"
#include "sci_io.h"



//*************************************************************************************************************************
//NAME: basicFuctionalityTest
//
//DESC: Initialize GPIO pins 12/13 for LEDs on Launchpad and toggle them to demonstrate system initlization and program
// 		linking has completed correctly.
//
//DATE: 26 March 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void basicFuctionalityTest(void)
{
	int loopCount = 0;

	//Initialize GPIOs for the LEDs and turn them off
	    EALLOW;
	    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
	    GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;
	    GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;
	    GpioDataRegs.GPACLEAR.bit.GPIO13 = 1;
	    EDIS;

	    //Toggle LEDS
	    EALLOW;
	    for(loopCount = 0;loopCount < 100; loopCount++)
	    {
	        GpioDataRegs.GPATOGGLE.bit.GPIO12 = 1;
	        DELAY_US(50000);
	        GpioDataRegs.GPATOGGLE.bit.GPIO13 = 1;
	        DELAY_US(50000);
	    }//END FOR
	    EDIS;
	    return;
}//END FUNCTION

//*************************************************************************************************************************
//NAME: initsci
//
//DESC: Initialize Serial Communication Interface A
//
//DATE: 30 March 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void initsci(void)
{
	//Clocks were turned on in initPeripheralClocks() function found in SysCtrl.c
	EALLOW;

	//One stop bit.
	SciaRegs.SCICCR.bit.STOPBITS = 0x1;

	//Parity Disabled
	SciaRegs.SCICCR.bit.PARITYENA = 0x0;

	//No loopback.
	SciaRegs.SCICCR.bit.LOOPBKENA = 0x0;

	//Idle-line mode protocol.
	SciaRegs.SCICCR.bit.ADDRIDLE_MODE = 0x0;

	//Set character length to 8 bits
	SciaRegs.SCICCR.bit.SCICHAR = 0x7;

	//Disable receive error interrupt, SCI sleep, and TXWake. Enable TX and RX
	SciaRegs.SCICTL1.all = 0x0003;

	//Enable SCITXBUF-register interrupt and Receiver-buffer/break interrupt enable
	SciaRegs.SCICTL2.bit.RXBKINTENA = 0x1;
	SciaRegs.SCICTL2.bit.TXINTENA = 0x1;

	//Set Baudrate to 115200 with LSPCLK at ~ 48.5 MHz
	SciaRegs.SCIHBAUD.all = 0x0000;
	SciaRegs.SCILBAUD.all = 51;

	//Relinquish SCI from Reset
	SciaRegs.SCICTL1.all = 0x0023;


    // For this example, only init the pins for the SCI-A port.
    EALLOW;
    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
    EDIS;


    while(!SciaRegs.SCICTL2.bit.TXRDY);
    SciaRegs.SCITXBUF.all = 0x1B;


	EDIS;

}//END FUNCTION


//*************************************************************************************************************************
//NAME: redirOut
//
//DESC: Redirect STDOUT to SCI to allow use of standard functions such as putchar
//
//DATE: 30 March 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void redirOut(void)
{
    volatile FILE *fid;
    volatile int status = 0;
    EALLOW;

    //Redirect STDOUT to SCI
    status = add_device("scia", _SSA, SCI_open, SCI_close, SCI_read, SCI_write, SCI_lseek, SCI_unlink, SCI_rename);
    fid = fopen("scia","w");
    freopen("scia:", "w", stdout);
    setvbuf(stdout, NULL, _IONBF, 0);
    EDIS;
}


//*************************************************************************************************************************
//NAME: sciTest
//
//DESC: Write characters to SCI
//
//DATE: 30 March 2016
//
//AUTHOR: Dylan Schindler
//*************************************************************************************************************************
void sciTest(void)
{
	unsigned char ucChar;
	int count = 0;

    // For this example, only init the pins for the SCI-A port.
    EALLOW;
    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
    EDIS;

	redirOut();

    while(!SciaRegs.SCICTL2.bit.TXRDY);
    SciaRegs.SCITXBUF.all = 0x1B;
	EALLOW;
    for(count = 0; count < 100; count++)
    {
        //while(!SciaRegs.SCICTL2.bit.TXRDY);
       // SciaRegs.SCITXBUF.all = count;
    printf("Count Value = [%d]", count);
    ucChar = 10;
    putchar(ucChar);
    ucChar = 13;
    putchar(ucChar);
    }
    EDIS;

}


void sciTestwADC(ADC_Selection Select)
{
	unsigned char ucChar;
	int16_t adcSample;
	char adcValString[16];


    // For this example, only init the pins for the SCI-A port.
    EALLOW;
    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
    EDIS;


	redirOut();

    while(!SciaRegs.SCICTL2.bit.TXRDY);
    SciaRegs.SCITXBUF.all = 0x1B;

	EALLOW;

   while(1)
    {
    	adcSample = sampleADC(Select);
    	convertADC(adcSample,AC_VOLT, adcValString);
    	printf("ADC Value = %sV", adcValString);
    	ucChar = 10;
    	putchar(ucChar);
    	ucChar = 13;
    	putchar(ucChar);
    	DELAY_US(500000);
    }//END FOR
}//END FUNCTION

void sciTestwComp(ADC_Selection Select)
{
	unsigned char ucChar;
	int16_t adcSample;
	char adcValString [16];
	//char*date = __DATE__ + __TIME__


    // For this example, only init the pins for the SCI-A port.
    EALLOW;
    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
    EDIS;


	redirOut();

    while(!SciaRegs.SCICTL2.bit.TXRDY);
    SciaRegs.SCITXBUF.all = 0x1B;

	EALLOW;

	printf("Time is %s",__DATE__  );
    for(;;)
    {
    	//Trip flag has been set
    	if(EPwm2Regs.TZFLG.bit.OST)
    	{
    		//Wait for comparator Trip to de-assert
    		while(Cmpss3Regs.COMPSTS.bit.COMPLSTS);
        	adcSample = sampleADC(AC_VOLT);
        	convertADC(adcSample,AC_VOLT,adcValString);
        	printf("ADC Value = %sV", adcValString);
        	ucChar = 10;
        	putchar(ucChar);
        	ucChar = 13;
        	putchar(ucChar);
        	DELAY_US(500000);
        	//Clear flags
        	EALLOW;
        	EPwm1Regs.TZCLR.bit.INT = 0x1;
        	EPwm1Regs.TZCLR.bit.OST = 0x1;
        	EPwm1Regs.TZOSTCLR.bit.DCBEVT1 = 0x1;
        	EDIS;
    	}
    }//END FOR
}//END FUNCTION

void sciTestwComp_GPIO(ADC_Selection Select)
{
	unsigned char ucChar;
	int16_t adcSample;
	char adcValString[16];


    // For this example, only init the pins for the SCI-A port.
    EALLOW;
    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
    EDIS;


	redirOut();

    while(!SciaRegs.SCICTL2.bit.TXRDY);
    SciaRegs.SCITXBUF.all = 0x1B;

	EALLOW;

    for(;;)
    {
    	//Trip flag has been set
    	if(EPwm2Regs.TZFLG.bit.OST)
    	{
    		//Wait for comparator Trip to de-assert
    		while(Cmpss3Regs.COMPSTS.bit.COMPLSTS);
    		LoadSwitch(Select, LOAD_CLOSED);
        	adcSample = sampleADC(AC_VOLT);
        	convertADC(adcSample,AC_VOLT,adcValString);
        	printf("ADC Value = %sV", adcValString);
        	ucChar = 10;
        	putchar(ucChar);
        	ucChar = 13;
        	putchar(ucChar);
        	DELAY_US(500000);
        	//Clear flags
        	EALLOW;
        	EPwm2Regs.TZCLR.bit.INT = 0x1;
        	EPwm2Regs.TZCLR.bit.OST = 0x1;
        	EPwm2Regs.TZOSTCLR.bit.DCAEVT1 = 0x1;
        	EDIS;
    	}
    	else
        	LoadSwitch(Select, LOAD_OPEN);
    }//END FOR
}//END FUNCTION






//###########################################################################

