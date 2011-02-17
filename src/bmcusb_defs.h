#ifndef BMCUSB_DEFS_H

#include <usb.h>

#ifdef CI_DM_VID
#include "CIUsbShared.h"
#endif

#ifdef CIUsb_MESSAGE_USBDEVICE
#include "CIUsbLib.h"
"CIUsbShared.h"
#endif

typedef struct {
  char devpath[256]; // The /dev/bus/usb path
  struct usb_bus *bus;
  struct usb_device *dev;
  usb_dev_handle *udev;
} BMCUSB_DEVICE;

#define MAX_BMCUSB_DEVICES 2
#define TIMEOUT 1000

#define BMCUSB_VENDOR ((u_int16_t) 0x1781)
#define BMCUSB_MULTIDRIVER ((u_int16_t) 0x0ED8)

// testing only!!! (optical mouse)
// #define BMCUSB_VENDOR ((u_int16_t) 0x046d)
// #define BMCUSB_MULTIDRIVER ((u_int16_t) 0xc03e)

// 03f0:0317 printer
// #define BMCUSB_VENDOR ((u_int16_t) 0x03f0)
// #define BMCUSB_MULTIDRIVER ((u_int16_t) 0x0317)

typedef int16_t WORD;
typedef char BYTE;
typedef int bool; 

#endif
