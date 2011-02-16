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
  char *devnode; // The /dev/bus/usb path
  struct usb_bus *bus;
  struct usb_device *dev;
  usb_dev_handle *udev;
} BMCUSB_DEVICE;

#define MAX_BMCUSB_DEVICES 2

#endif
