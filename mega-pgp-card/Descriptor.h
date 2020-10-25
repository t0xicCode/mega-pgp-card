/*
 * Descriptor.h
 *
 * Created: 2020-10-26 4:41:24 PM
 *  Author: t0xicCode
 */

#ifndef DESCRIPTOR_H_
#define DESCRIPTOR_H_

#include <avr/pgmspace.h>

#include "LUFA/Drivers/USB/USB.h"

/* Macros: */
/** Endpoint address of the CCID data IN endpoint, for device-to-host data transfers. */
#define CCID_IN_EPADDR (ENDPOINT_DIR_IN | 2)

/** Endpoint address of the CCID data OUT endpoint, for host-to-device data transfers. */
#define CCID_OUT_EPADDR (ENDPOINT_DIR_OUT | 1)

/** Endpoint size in bytes of the CCID data being sent between IN and OUT endpoints. */
#define CCID_EPSIZE 64

/* Type Defines: */
/** Type define for the device configuration descriptor structure. This must be defined in the
 *  application code, as the configuration descriptor contains several sub-descriptors which
 *  vary between devices, and which describe the device's usage to the host.
 */
typedef struct
{
	USB_Descriptor_Configuration_Header_t Config;
	USB_Descriptor_Interface_t CCID_Interface;
	USB_CCID_Descriptor_t CCID_SmartCard;
	USB_Descriptor_Endpoint_t CCID_BulkInEndpoint;
	USB_Descriptor_Endpoint_t CCID_BulkOutEndpoint;
} USB_Descriptor_Configuration_t;

/** Enum for the device interface descriptor IDs within the device. Each interface descriptor
 *  should have a unique ID index associated with it, which can be used to refer to the
 *  interface from other descriptors.
 */
enum InterfaceDescriptors_t
{
	INTERFACE_ID_CCID = 0, /**< CCID interface descriptor ID */
};

/** Enum for the device string descriptor IDs within the device. Each string descriptor should
 *  have a unique ID index associated with it, which can be used to refer to the string from
 *  other descriptors.
 */
enum StringDescriptors_t
{
	STRING_ID_Language = 0,			/**< Supported Languages string descriptor ID (must be zero) */
	STRING_ID_Manufacturer = 1, /**< Manufacturer string ID */
	STRING_ID_Product = 2,			/**< Product string ID */
	STRING_ID_Serial = 3,				/**< Serial string ID */
};
#define NUM_STRING_DESCRIPTORS 4

/* Function Prototypes: */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint16_t wIndex, const void **const DescriptorAddress) ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif /* DESCRIPTOR_H_ */