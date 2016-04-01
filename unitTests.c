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


static unsigned short indexX=0;
static unsigned short indexY=0;

const unsigned char escRed[] = {0x1B, 0x5B, '3','1', 'm'};
const unsigned char escWhite[] = {0x1B, 0x5B, '3','7', 'm'};
const unsigned char escLeft[] = {0x1B, 0x5B, '3','7', 'm'};

void drawTILogo2(void);

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

    // For this example, only init the pins for the SCI-A port.
    EALLOW;
    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
    EDIS;

	int count = 0;

	redirOut();

    while(!SciaRegs.SCICTL2.bit.TXRDY);
    SciaRegs.SCITXBUF.all = 0x1B;
	EALLOW;
    //putchar(0x1B); //Escape char
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

int16_t sampleADC(void)
{
	int16_t temp;

    //Force start of conversion on SOC0
	AdcbRegs.ADCSOCFRC1.all = 0x03;

    //Wait for end of conversion.
    while(AdcbRegs.ADCINTFLG.bit.ADCINT1 == 0){}  //Wait for ADCINT1
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;        //Clear ADCINT1

    //Get temp sensor sample result from SOC0
    temp = AdcbResultRegs.ADCRESULT1;

    //Return the raw temperature because the devices don't have slope/offset values
    return(temp);

}

void sciTestwADC(void)
{
	unsigned char ucChar;
	int16_t adcSample;

    // For this example, only init the pins for the SCI-A port.
    EALLOW;
    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
    EDIS;

	int count = 0;

	redirOut();

    while(!SciaRegs.SCICTL2.bit.TXRDY);
    SciaRegs.SCITXBUF.all = 0x1B;

	EALLOW;

    adcSample = sampleADC();

    for(count = 0; count < 100; count++)
    {
        //while(!SciaRegs.SCICTL2.bit.TXRDY);
       // SciaRegs.SCITXBUF.all = count;
    printf("ADC Value = [%d]", adcSample);
    ucChar = 10;
    putchar(ucChar);
    ucChar = 13;
    putchar(ucChar);
    }
    EDIS;

}







//###########################################################################

