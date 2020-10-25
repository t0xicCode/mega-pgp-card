/*
 * USB-GPG.c
 *
 * Created: 2020-10-25 9:48:03 AM
 * Author : t0xicCode
 */ 

#include "USB_GPG.h"

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	LEDs_Init();
	USB_Init();
}


int main(void)
{
	SetupHardware();
	GlobalInterruptEnable();
	
    /* Replace with your application code */
    while (1) 
    {
	    USB_USBTask();
		CCID_Device_USBTask(&CCID_Interface);
		//LEDs_ToggleLEDs(LEDS_LED3);
    }
}


