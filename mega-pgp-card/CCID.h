/*
 * CCID.h
 *
 * Created: 2020-10-26 8:37:07 PM
 *  Author: t0xicCode
 */ 


#ifndef CCID_H_
#define CCID_H_

#include "Descriptor.h"

extern USB_ClassInfo_CCID_Device_t CCID_Interface;

void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

uint8_t CALLBACK_CCID_IccPowerOn(USB_ClassInfo_CCID_Device_t* const CCIDInterfaceInfo,
		const uint8_t Slot,
		uint8_t* const Atr,
		uint8_t* const AtrSize,
		uint8_t* const Error);
		
uint8_t CALLBACK_CCID_IccPowerOff(USB_ClassInfo_CCID_Device_t* const CCIDInterfaceInfo,
		const uint8_t Slot,
		uint8_t* const Error);
		
uint8_t CALLBACK_CCID_GetSlotStatus(USB_ClassInfo_CCID_Device_t* const CCIDInterfaceInfo,
		const uint8_t Slot,
		uint8_t* const Error);
		
uint8_t CALLBACK_CCID_XfrBlock(USB_ClassInfo_CCID_Device_t* const CCIDInterfaceInfo,
		const uint8_t Slot,
		const uint8_t* ReceivedBuffer,
		const uint8_t ReceivedBufferSize,
		uint8_t* const SendBuffer,
		uint8_t* const SentBufferSize,
		uint8_t* const Error);
		
uint8_t CALLBACK_CCID_Abort(USB_ClassInfo_CCID_Device_t* const CCIDInterfaceInfo,
		const uint8_t Slot,
		const uint8_t Seq,
		uint8_t* const error);



#endif /* CCID_H_ */