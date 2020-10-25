/*
 * Descriptor.c
 *
 * Created: 2020-10-26 4:41:12 PM
 *  Author: t0xicCode
 */

#include "Descriptor.h"

/**
 * USB Device Descriptor. Top-level information read-out by the host.
 */
const USB_Descriptor_Device_t PROGMEM DeviceDescriptor =
		{
				.Header = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

				.USBSpecification = VERSION_BCD(1, 1, 0),
				.Class = USB_CSCP_NoDeviceClass,
				.SubClass = USB_CSCP_NoDeviceSubclass,
				.Protocol = USB_CSCP_NoDeviceProtocol,

				.Endpoint0Size = FIXED_CONTROL_ENDPOINT_SIZE,

				.VendorID = 0x03EB,	 // Donated by Atmel // TODO: CHANGEME
				.ProductID = 0x206E, // TODO: CHANGEME
				.ReleaseNumber = VERSION_BCD(0, 0, 1),

				.ManufacturerStrIndex = STRING_ID_Manufacturer,
				.ProductStrIndex = STRING_ID_Product,
				.SerialNumStrIndex = STRING_ID_Serial,

				.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS};

/**
 * USB Configuration Descriptor.
 */
const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor =
		{
				.Config =
						{
								.Header = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

								.TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
								.TotalInterfaces = 1,

								.ConfigurationNumber = 1,
								.ConfigurationStrIndex = NO_DESCRIPTOR,

								.ConfigAttributes = (USB_CONFIG_ATTR_RESERVED | USB_CONFIG_ATTR_SELFPOWERED),

								.MaxPowerConsumption = USB_CONFIG_POWER_MA(100)},
				.CCID_Interface =
						{
								.Header = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

								.InterfaceNumber = INTERFACE_ID_CCID,
								.AlternateSetting = 0x00,

								.TotalEndpoints = 2,

								.Class = CCID_CSCP_CCIDClass,
								.SubClass = CCID_CSCP_NoSpecificSubclass,
								.Protocol = CCID_CSCP_NoSpecificProtocol,

								.InterfaceStrIndex = NO_DESCRIPTOR},
				.CCID_SmartCard =
						{
								.Header = {.Size = sizeof(USB_CCID_Descriptor_t), .Type = CCID_DTYPE_Functional},
								.CCID = CCID_CURRENT_SPEC_RELEASE_NUMBER,
								.MaxSlotIndex = 0x00,
								.VoltageSupport = CCID_VOLTAGESUPPORT_5V,
								.Protocols = CCID_PROTOCOLS_T1,
								.DefaultClock = CCID_DESCRIPTOR_CLOCK_MHZ(16),
								.MaximumClock = CCID_DESCRIPTOR_CLOCK_MHZ(16),
								.NumClockSupported = 0,
								.DataRate = 307200,
								.MaxDataRate = 307200,
								.NumDataRatesSupported = 0,
								.MaxIFSD = 2038,
								.SynchProtocols = 0,
								.Mechanical = 0,
								.Features = CCID_Features_ExchangeLevel_ShortAPDU | CCID_Features_Auto_ParameterConfiguration | CCID_Features_Auto_ICCActivation | CCID_Features_Auto_VoltageSelection,
								.MaxCCIDMessageLength = CCID_EPSIZE,
								.ClassGetResponse = 0xff,
								.ClassEnvelope = 0xff,
								.LcdLayout = 0,
								.PINSupport = 0,
								.MaxCCIDBusySlots = 1

						},
				.CCID_BulkInEndpoint =
						{
								.Header = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

								.EndpointAddress = CCID_IN_EPADDR,
								.Attributes = EP_TYPE_BULK,
								.EndpointSize = CCID_EPSIZE,
								.PollingIntervalMS = 0x05},

				.CCID_BulkOutEndpoint =
						{
								.Header = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

								.EndpointAddress = CCID_OUT_EPADDR,
								.Attributes = EP_TYPE_BULK,
								.EndpointSize = CCID_EPSIZE,
								.PollingIntervalMS = 0x05}};

/* Descriptor Strings */
const USB_Descriptor_String_t PROGMEM LanguageString = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);
const USB_Descriptor_String_t PROGMEM ManufacturerString = USB_STRING_DESCRIPTOR(L"MagnaX Software");
const USB_Descriptor_String_t PROGMEM ProductString = USB_STRING_DESCRIPTOR(L"USB PGP Key");
const USB_Descriptor_String_t PROGMEM SerialString = USB_STRING_DESCRIPTOR(L"0100000001");

/**
 * Callback that returns the address to the USB descriptors.
 */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint16_t wIndex, const void **const DescriptorAddress)
{
	const uint8_t DescriptorType = (wValue >> 8);
	const uint8_t DescriptorNumber = (wValue & 0xFF);

	const void *Address = NULL;
	uint16_t Size = NO_DESCRIPTOR;

	switch (DescriptorType)
	{
	case DTYPE_Device:
		Address = &DeviceDescriptor;
		Size = sizeof(USB_Descriptor_Device_t);
		break;
	case DTYPE_Configuration:
		Address = &ConfigurationDescriptor;
		Size = sizeof(USB_Descriptor_Configuration_t);
		break;
	case DTYPE_String:
		switch (DescriptorNumber)
		{
		case STRING_ID_Language:
			Address = &LanguageString;
			Size = pgm_read_byte(&LanguageString.Header.Size);
			break;
		case STRING_ID_Manufacturer:
			Address = &ManufacturerString;
			Size = pgm_read_byte(&ManufacturerString.Header.Size);
			break;
		case STRING_ID_Product:
			Address = &ProductString;
			Size = pgm_read_byte(&ProductString.Header.Size);
			break;
		case STRING_ID_Serial:
			Address = &SerialString;
			Size = pgm_read_byte(&SerialString.Header.Size);
			break;
		}
		break;
	}

	*DescriptorAddress = Address;
	return Size;
}
