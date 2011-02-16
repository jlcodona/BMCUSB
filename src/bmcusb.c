/*** uDM140.c *************************************************************************
 * 
 * This is linux interface library for the BMC Multi-Driver Electronics.
 * This has not been tested with more than one Multi-Driver since that is all I have.  
 * 
 * I reserve all rights to this code, but allow it to be used by others for any purpose.
 * My code is not to be sold.  
 * Various definitions for the Multi-Driver were taken from the CI USB Library.
 * 
 * ************************************************************************************
 * Written by Johanan L. Codona
 * Center for Astronomical Adaptive Optics
 * Steward Observatory
 * University of Arizona
 * Tucson, AZ 85721
 * jlcodona@gmail.com
 * ************************************************************************************
 * CHANGES:
 * 20110215 JLCodona. Defined functions for first C version based in part on CIUsbLib.
 * ************************************************************************************/

#include "bmcusb.h"
#include "bmcusb_defs.h"

static BMCUSB_DEVICE BMCDEV[MAX_BMCUSB_DEVICES];
static struct usb_device *bmc_devs[MAX_BMCUSB_DEVICES];
static int Nbmc = 0;

// int bmcusb_probe()
int bmcusb_probe()
{
  struct usb_bus *bus;
  struct usb_device *dev;
  
  usb_init();
  
  usb_find_busses();
  usb_find_devices();

  Nbmc = 0;
  for (bus = usb_busses; bus; bus = bus->next) {
    for (dev = bus->devices; dev; dev = dev->next) {
      if(dev->descriptor.idVendor != BMC_USB_VENDOR) continue;
      if(dev->descriptor.idProduct != BMC_USB_MULTIDRIVER ) continue;
      
      bmc_devs[Nbmc] = dev; // Just add it to the list.
      BMCDEV[Nbmc].dev = dev; 
      BMCDEV[Nbmc].bus = bus; 
      BMCDEV[Nbmc].udev = (usb_dev_handle *) NULL; 
      Nbmc++;
    }
  }

  return(Nbmc);
}

// int bmcusb_claim(int nDevId); 		// call before using.
  
  // int bmcusb_release(int nDevId); 		// call before quitting.
  
  // int bmcusb_getStatus(int nDevId, bmc_status_t* p_nStatus); // get device status. 
  
  // int bmcusb_setControl(int nDevId, bmc_controlID_t nCntlId, bmc_status_t* p_nStatus);
  
  // int bmcusb_zero(int nDevId);
  
  // int bmcusb_setConstant(int nDevId, bmc_actuatorData_t value);
  
  // int bmcusb_sendFrame(int nDevId, bmc_actuatorData_t* acts_cmds,  int nBuffSize, bmc_status_t* status_out);
  
  // int bmcusb_streamFrame(int nDevId, bmc_actuatorData_t* acts_cmds,  int nBuffSize, bmc_status_t* status_out);
  
  // int bmcusb_stepFrame(int nDevId, bmc_actuatorData_t* acts_cmds,  int nBuffSize, bmc_status_t* status_out);
  
  // int bmcusb_flush(int nDevId, bmc_status_t* p_nStatus);
  
  
  #endif
  