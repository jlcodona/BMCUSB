/*** bmcusb.h *************************************************************************
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
#include "CIUsbLib.h"
#include <stdio.h>
#include <string.h>

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
      if(dev->descriptor.idVendor != BMCUSB_VENDOR) continue;
      if(dev->descriptor.idProduct != BMCUSB_MULTIDRIVER ) continue;
      
      bmc_devs[Nbmc] = dev; // Just add it to the list.
      BMCDEV[Nbmc].dev = dev; 
      BMCDEV[Nbmc].bus = bus; 
      BMCDEV[Nbmc].udev = (usb_dev_handle *) NULL; 
      
      int dnum = atoi(bus->dirname);
      int fnum = atoi(dev->filename);
      
      sprintf(BMCDEV[Nbmc].devpath,"/sys/bus/usb/devices/%d-%d",dnum,fnum);
      printf("%s\n",BMCDEV[Nbmc].devpath);
      
      Nbmc++;
    }
  }
  
  return(Nbmc);
}

// int bmcusb_claim(int nDevId); 		// call before using.
int bmcusb_claim(int nDevId) 
{
  return(usb_claim_interface(BMCDEV[nDevId].udev,0));
}

// int bmcusb_release(int nDevId); 		// call before quitting.
int bmcusb_release(int nDevId) 
{
  return(usb_release_interface(BMCDEV[nDevId].udev,0));
}

// int bmcusb_getStatus(int nDevId, bmc_status_t* p_nStatus); // get device status. 

// int bmcusb_setControl(int nDevId, bmc_controlID_t nCntlId, bmc_status_t* p_nStatus);

// int bmcusb_zeroDM(int nDevId);
int bmcusb_zeroDM(int nDevId)
{
  return bmcusb_setConstantDM(nDevId, 0);
}

// int bmcusb_setConstantDM(int nDevId, bmc_actuatorData_t value);

// int bmcusb_sendFrame(int nDevId, bmc_actuatorData_t* acts_cmds,  int nBuffSize, bmc_status_t* status_out);

// int bmcusb_streamFrame(int nDevId, bmc_actuatorData_t* acts_cmds,  int nBuffSize, bmc_status_t* status_out);

// int bmcusb_stepFrame(int nDevId, bmc_actuatorData_t* acts_cmds,  int nBuffSize, bmc_status_t* status_out);

// int bmcusb_flush(int nDevId, bmc_status_t* p_nStatus);

void clearBuffer(bmc_actuatorData_t buf[], int N) {
  int n;
  for(n=0;n<N;n++) {
    buf[n] = 0;
  }
}

bmc_actuatorData_t *mapActs(bmc_actuatorData_t *FROM, bmc_actuatorData_t *TO, const int actMap[]) {
  int n;
  for(n=0;n<USB_NUM_ACTUATORS_MULTI;n++)
    TO[n] = FROM[actMap[n]];
  
  return(TO);
}

