/*
 * CCID.c
 *
 * Created: 2020-10-26 8:36:43 PM
 *  Author: t0xicCode
 */ 

#include "CCID.h"

USB_ClassInfo_CCID_Device_t CCID_Interface =
{
	.Config =
	{
		.InterfaceNumber              = INTERFACE_ID_CCID,
		.TotalSlots                   = 1,
		.DataINEndpoint               =
		{
			.Address              = CCID_IN_EPADDR,
			.Size                 = CCID_EPSIZE,
			.Banks                = 1,
		},
		.DataOUTEndpoint              =
		{
			.Address              = CCID_OUT_EPADDR,
			.Size                 = CCID_EPSIZE,
			.Banks                = 1,
		},
	},
};

#include <stdlib.h>
#include <string.h>

static void Iso7816_CreateSimpleAtr(uint8_t* const attr, uint8_t* const attrLength)
{
	attr[0] = 0x3B; //TS: direct convention

	uint8_t interfaceBytesPresence = 0;

	uint8_t historicalBytes[14]   = "LUFA CCID Demo"; // Must be equal or less than 15
	uint8_t historicalBytesLength = sizeof(historicalBytes);

	attr[1] = (interfaceBytesPresence << 4) + historicalBytesLength;
	memcpy(attr + 2, historicalBytes, historicalBytesLength);

	*attrLength = historicalBytesLength + 2;
}

/** Event handler for the library USB Configuration Changed evet. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;
	ConfigSuccess &= CCID_Device_ConfigureEndpoints(&CCID_Interface);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CCID_Device_ProcessControlRequest(&CCID_Interface);
}

/** Event handler for the CCID_PC_to_RDR_IccPowerOn message. This message is sent to the device
 *  whenever an application at the host wants to send a power off signal to a slot.
 *  THe slot must reply back with a recognizable ATR (answer to reset)
 */
uint8_t CALLBACK_CCID_IccPowerOn(USB_ClassInfo_CCID_Device_t* const CCIDInterfaceInfo,
                                 const uint8_t slot,
								 uint8_t* const atr,
								 uint8_t* const attrSize,
								 uint8_t* const error)
{
	if (slot < CCIDInterfaceInfo->Config.TotalSlots)
	{
		Iso7816_CreateSimpleAtr(atr, attrSize);
		*error = CCID_ERROR_NO_ERROR;
		return CCID_COMMANDSTATUS_PROCESSEDWITHOUTERROR | CCID_ICCSTATUS_PRESENTANDACTIVE;
	}
	else
	{
		*error = CCID_ERROR_SLOT_NOT_FOUND;
		return CCID_COMMANDSTATUS_FAILED | CCID_ICCSTATUS_NOICCPRESENT;
	}
}

/** Event handler for the CCID_PC_to_RDR_IccPowerOff message. This message is sent to the device
 *  whenever an application at the host wants to send a power off signal to a slot.
 */
uint8_t CALLBACK_CCID_IccPowerOff(USB_ClassInfo_CCID_Device_t* const CCIDInterfaceInfo,
                                  const uint8_t slot,
                                  uint8_t* const error)
{
	if (slot < CCIDInterfaceInfo->Config.TotalSlots)
	{
		*error = CCID_ERROR_NO_ERROR;
		return CCID_COMMANDSTATUS_PROCESSEDWITHOUTERROR | CCID_ICCSTATUS_NOICCPRESENT;
	}
	else
	{
		*error = CCID_ERROR_SLOT_NOT_FOUND;
		return CCID_COMMANDSTATUS_FAILED | CCID_ICCSTATUS_NOICCPRESENT;
	}
}

/** Event handler for the CCID_PC_to_RDR_GetSlotStatus. This message is sent to the device
 *  whenever an application at the host wants to the get the current slot status
 *
 */
uint8_t CALLBACK_CCID_GetSlotStatus(USB_ClassInfo_CCID_Device_t* const CCIDInterfaceInfo,
                                    const uint8_t slot,
                                    uint8_t* const error)
{
	if (slot < CCIDInterfaceInfo->Config.TotalSlots)
	{
		*error = CCID_ERROR_NO_ERROR;
		return CCID_COMMANDSTATUS_PROCESSEDWITHOUTERROR | CCID_ICCSTATUS_PRESENTANDACTIVE;
	}
	else
	{
		*error = CCID_ERROR_SLOT_NOT_FOUND;
		return CCID_COMMANDSTATUS_FAILED | CCID_ICCSTATUS_NOICCPRESENT;
	}
}

/** Event handler for the CCID_PC_to_RDR_SetParameters when T=0. This message is sent to
 *  the device whenever an application at the host wants to set the parameters for a
 *  given slot.
 */
uint8_t CALLBACK_CCID_SetParameters_T0(USB_ClassInfo_CCID_Device_t* const CCIDInterfaceInfo,
									   const uint8_t Slot,
									   uint8_t* const Error,
									   USB_CCID_ProtocolData_T0_t* const T0)
{
	if (Slot == 0)
	{
		// Set parameters
		memcpy(&CCIDInterfaceInfo->ProtocolData, T0, sizeof(USB_CCID_ProtocolData_T0_t));

		*Error = CCID_ERROR_NO_ERROR;
		return CCID_COMMANDSTATUS_PROCESSEDWITHOUTERROR | CCID_ICCSTATUS_PRESENTANDACTIVE;
	}
	else
	{
		*Error = CCID_ERROR_SLOT_NOT_FOUND;
		return CCID_COMMANDSTATUS_FAILED | CCID_ICCSTATUS_NOICCPRESENT;
	}
}

/** Event handler for the CCID_PC_to_RDR_GetParameters when T=0. This message is sent to
 *  the device whenever an application at the host wants to get the current parameters for
 *  a given slot.
 */
uint8_t CALLBACK_CCID_GetParameters_T0(USB_ClassInfo_CCID_Device_t* const CCIDInterfaceInfo,
									   const uint8_t Slot,
									   uint8_t* const Error,
									   uint8_t* const ProtocolNum,
									   USB_CCID_ProtocolData_T0_t* const T0)
{
	if (Slot == 0)
	{
		*ProtocolNum = CCID_PROTOCOLNUM_T0;
		memcpy(T0, &CCIDInterfaceInfo->ProtocolData, sizeof(USB_CCID_ProtocolData_T0_t));

		*Error = CCID_ERROR_NO_ERROR;
		return CCID_COMMANDSTATUS_PROCESSEDWITHOUTERROR | CCID_ICCSTATUS_PRESENTANDACTIVE;
	}
	else
	{
		*Error = CCID_ERROR_SLOT_NOT_FOUND;
		return CCID_COMMANDSTATUS_FAILED | CCID_ICCSTATUS_NOICCPRESENT;
	}
}

/** Event handler for the CCID_PC_to_RDR_XfrBlock. This message is sent to the device
 *  whenever an application at the host wants to send a block of bytes to the device
 *  THe device reply back with an array of bytes
 */
uint8_t CALLBACK_CCID_XfrBlock(USB_ClassInfo_CCID_Device_t* const CCIDInterfaceInfo,
							   const uint8_t Slot,
							   const uint8_t* ReceivedBuffer,
							   const uint8_t ReceivedBufferSize,
							   uint8_t* const SendBuffer,
							   uint8_t* const SentBufferSize,
							   uint8_t* const Error)
{
	if (Slot < CCIDInterfaceInfo->Config.TotalSlots)
	{
		uint8_t OkResponse[2] = {0x90, 0x00};

		memcpy(SendBuffer, OkResponse, sizeof(OkResponse));
		*SentBufferSize = sizeof(OkResponse);

		*Error = CCID_ERROR_NO_ERROR;
		return CCID_COMMANDSTATUS_PROCESSEDWITHOUTERROR | CCID_ICCSTATUS_NOICCPRESENT;
	}
	else
	{
		 *Error = CCID_ERROR_SLOT_NOT_FOUND;
         return CCID_COMMANDSTATUS_FAILED | CCID_ICCSTATUS_NOICCPRESENT;
	}
}

uint8_t CALLBACK_CCID_Abort(USB_ClassInfo_CCID_Device_t* const CCIDInterfaceInfo,
                            const uint8_t Slot,
							const uint8_t Seq,
							uint8_t* const Error)
{
	if (CCIDInterfaceInfo->State.Aborted && Slot == 0 && CCIDInterfaceInfo->State.AbortedSeq == Seq)
	{
		CCIDInterfaceInfo->State.Aborted    = false;
		CCIDInterfaceInfo->State.AbortedSeq = -1;

		*Error = CCID_ERROR_NO_ERROR;
		return CCID_COMMANDSTATUS_PROCESSEDWITHOUTERROR | CCID_ICCSTATUS_PRESENTANDACTIVE;
	}
	else if (!CCIDInterfaceInfo->State.Aborted)
	{
		*Error = CCID_ERROR_CMD_NOT_ABORTED;
		return CCID_COMMANDSTATUS_PROCESSEDWITHOUTERROR | CCID_ICCSTATUS_PRESENTANDACTIVE;
	}
	else if (Slot != 0)
	{
		*Error = CCID_ERROR_SLOT_NOT_FOUND;
		return CCID_COMMANDSTATUS_FAILED | CCID_ICCSTATUS_NOICCPRESENT;
	}
	else
	{
		*Error = CCID_ERROR_NOT_SUPPORTED;
		return CCID_COMMANDSTATUS_FAILED | CCID_ICCSTATUS_NOICCPRESENT;
	}
}