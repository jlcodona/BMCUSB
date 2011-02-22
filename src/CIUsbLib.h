#ifndef CIUSB_H
#define CIUSB_H

#pragma once

#define	MAX_USB_DEVICES				32	// This is the maximum number of devices supported
#define USB_NUM_ACTUATORS_MULTI		160	// Number of words sent in either 128 or 140 actuator modes (MULTI)
#define USB_BYTES_PER_FRAME_MULTI		(USB_NUM_ACTUATORS_MULTI*2)
#define USB_NUM_ACTUATORS_MINI		32	// Number of words sent in either 32 actuator mode (MINI)
#define USB_BYTES_PER_FRAME_MINI		(USB_NUM_ACTUATORS_MINI*2)
#define EXT_EEPROM_BYTES				128

// Error codes from CIUSBLib interface methods
#define H_DEVICE_STATUS_OK			 0	// No Errors
#define H_DEVICE_NOT_FOUND			-1	// USB Device was not found
#define H_DEVICE_NO_COMM				-2	// USB communication was lost
#define H_DEVICE_CMD_ERR				-3	// Unknown command
#define H_DEVICE_TIMEOUT				-4	// USB transfer time out

// USB Message callback definitions
#define CIUsb_MESSAGE_USBDEVICE		0	// USB PNP Message
#define CIUsb_MSG_USBDEV_REMOVAL		0	// USB PNP Message: removal
#define CIUsb_MSG_USBDEV_ARRIVAL		1	// USB PNP Message: arrival

// Used in CIUSBLib CIUsb_GetStatus() 
#define CIUsb_STATUS_DEVICENAME			0	// Get Device Name
#define CIUsb_STATUS_VID					1	// Get Device Vendor ID
#define CIUsb_STATUS_PID					2	// Get Device Product ID
#define CIUsb_STATUS_FRAME_ERROR			3	// Get value of FRAME_ERROR bit
#define CIUsb_STATUS_PD_HVA_E				4	// Get value of PD_HVA_E bit
#define CIUsb_STATUS_CABLE_OK				5	// Get value of CABLE_OK bit
#define CIUsb_STATUS_EXT_EEPROM			6	// Get value of EEPROM probe: N=size, 0=absent
#define CIUsb_STATUS_EXT_EEPROM_READ		7	// Get EEPROM data

// Used in CIUSBLib CIUsb_SetControl() 
#define CIUsb_CONTROL_ASSERT_FRAME_SYNC		0	// Set FRAME_SYC = 1
#define CIUsb_CONTROL_DEASSERT_FRAME_SYNC	1	// Set FRAME_SYC = 0
#define CIUsb_CONTROL_ASSERT_FRESET			2	// Set FRESET    = 1
#define CIUsb_CONTROL_DEASSERT_FRESET		3	// Set FRESET    = 0
#define CIUsb_CONTROL_ASSERT_HV_ENAB			4	// Set HV_ENAB   = 1
#define CIUsb_CONTROL_DEASSERT_HV_ENAB		5	// Set HV_ENAB   = 0
#define CIUsb_CONTROL_ASSERT_LV_SHDN			6	// Set LV_SHDN   = 1
#define CIUsb_CONTROL_DEASSERT_LV_SHDN		7	// Set LV_SHDN   = 0
#define CIUsb_CONTROL_ASSERT_EXT_I2C			8	// Set EXT_I2C   = 1
#define CIUsb_CONTROL_DEASSERT_EXT_I2C		9	// Set EXT_I2C   = 0
#define CIUsb_CONTROL_MINI_MODE				10	// Set MINI Mode (frame size = 64 bytes)

#endif
