#ifndef BMCUSB_DEFS_H

#include <usb.h>
#include "bmcusb.h"

#include "CIUsbShared.h"
#include "CIUsbLib.h"

typedef struct {
  char devpath[256]; // The /dev/bus/usb path
  char firmware[32]; // way too big.
  struct usb_bus *bus;
  struct usb_device *dev;
  usb_dev_handle *udev;
  bmc_actuatorData_t actuators[USB_NUM_ACTUATORS_MULTI]; // assumed already mapped and ready to go.
  int *mapping;
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

#endif
