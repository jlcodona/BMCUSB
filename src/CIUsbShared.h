#ifndef CIUSBSHARED_H
#define CIUSBSHARED_H

// VID, PID
#define CI_DM_VID		0x1781
#define CI_MULTIDM_PID		0x0ED8
#define CI_MINIDM_PID		0x0ED9
#define FX2_BOOT_VID 		0x04b4
#define FX2_BOOT_PID 		0x8613

#define MULTI_FIRMWARE_VER	"1.00"
#define MINI_FIRMWARE_VER	"2.00"

// Vendor Requests to USB device.
enum
{
	eCIUsbCmndGetFirmwareVer	= 0xF0,
	eCIUsbCmndSetGetRam		= 0xF1,
	eCIUsbCmndSetGetCodeEeprom	= 0xF2,
	eCIUsbCmndResetAll		= 0xF3,
	eCIUsbCmndGetStatusBits		= 0xF4, // bit[0]=FrameErr,  bit[1]=RdHvaE, bit[2]=CableOk, bit[9]=ExEepromPresent
	eCIUsbCmndSetControlBits	= 0xF5, // bit[7]=Set/Reset, bit[1]=Freset, bit[2]=FrameSync, bit[3]=HvEnab
	eCIUsbCmndSetGetCodeExEeprom	= 0xF6,
	eCIUsbCmndSetDac		= 0xF7
};

#endif
