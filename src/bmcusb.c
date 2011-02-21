/*** bmcusb.h *************************************************************************
 * 
 * This is linux interface library for the BMC Multi-Driver Electronics.
 * WARNING: This has not been tested with more than one Multi-Driver since that is all I have.  
 * 
 * I assume that the actuator buffer is 160 elements in the linear case. This is BMC's choice.
 * In the 2D functions, I assume 12x12.  Bounds checking is not possible, so beware.
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
 * 20110215 JLCodona. Defined functions for first C version in part on CIUsbLib.
 * ************************************************************************************/


#include "bmcusb.h"
#include "bmcusb_defs.h"
#include "CIUsbLib.h"
#include "CIUsbShared.h"
#include <stdio.h>
#include <string.h>

//#define DEBUG_BLURB   if(BMCUSB_DEBUG) fprintf(stderr,"DEBUG: file %s line %d: inside %s(...)\n",__FILE__,__LINE__,__FUNCTION__);
#define DEBUG_BLURB   if(BMCUSB_DEBUG) fprintf(stderr,"DEBUG: %s\n",__FUNCTION__);


static BMCUSB_DEVICE BMCDEV[MAX_BMCUSB_DEVICES];
static struct usb_device *bmc_devs[MAX_BMCUSB_DEVICES];
static int Nbmc = 0;

static bool BMCUSB_DEBUG = FALSE;

void bmcusb_setDebug(bool state)
{
  BMCUSB_DEBUG = state;
  if(state)
    usb_set_debug(5);
  else
    usb_set_debug(0);   
}

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

int bmcusb_claim(int nDevId)   // call before using.
{
  DEBUG_BLURB
  if(BMCDEV[nDevId].udev==NULL)
    BMCDEV[nDevId].udev = usb_open(BMCDEV[nDevId].dev);

  if(BMCDEV[nDevId].udev==NULL) {
    fprintf(stderr,"ERROR: failed to get a udev for interface %d\n",nDevId);
    return(1);
}
  
  return(usb_claim_interface(BMCDEV[nDevId].udev,0));
}

int bmcusb_release(int nDevId) // call before quitting.
{
  DEBUG_BLURB
  int ret = 0;
  
  if(BMCDEV[nDevId].udev!=NULL) {
    ret = usb_release_interface(BMCDEV[nDevId].udev,0);
    
    BMCDEV[nDevId].udev = NULL;
  }
  return ret;
}

int bmcusb_VendorRequest(int nDevId, int reqCode, int bToTarget, int wValue, int wIndex, char *buffer, int len){
  // NOTE: Find out what to do with direction bToTarget....
  DEBUG_BLURB
  //printf("VendorRequest: req=%X val=%d index=0x%04x buflen=%d\n",reqCode,wValue,wIndex,len);
  printf("VendorRequest: %02X %d 0x%04x %d\n",reqCode,wValue,wIndex,len);
  return(usb_control_msg(BMCDEV[nDevId].udev,USB_TYPE_VENDOR,reqCode,wValue,wIndex,buffer,len,TIMEOUT));
}

int bmcusb_assertReset(int nDevId)
{
  DEBUG_BLURB
  printf("\nDEBUG: ASSERT RESET COMMAND.\n");

  return(bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0082, NULL, 0));
}

int bmcusb_deassertReset(int nDevId)
{
  DEBUG_BLURB  
  printf("\nDEBUG: DE-ASSERT RESET COMMAND.\n");
  return(bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0002, NULL, 0));
}

int bmcusb_reset(int nDevId)
{
  DEBUG_BLURB
  printf("\nDEBUG: RESET.\n");

  int ret;
  
  ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0002, NULL, 0);
  if(ret) return ret;
  
  ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0082, NULL, 0);
  
  return ret; 
}

int bmcusb_setHV(int nDevId, bool ON)
{
  int ret;
  DEBUG_BLURB
  
  if(ON) // Turn ON the High Voltage Power to the DM...
  {
    printf("\nDEBUG: Turning HV ON.\n");
    ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0088, NULL, 0);
  } 
  else  // Turn OFF the High Voltage Power to the DM...
  {
    printf("\nDEBUG: Turning HV OFF.\n");
    ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0008, NULL, 0);
  }
    return ret;
}

int bmcusb_setFrameSync(int nDevId, bool ASSERT)
{
  int ret;
  DEBUG_BLURB
  
  if(ASSERT)
  {
    ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0084, NULL, 0);
  } 
  else
  {
    ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0004, NULL, 0);
  }
    return ret;
}

int bmcusb_setLVShdn(int nDevId, bool ASSERT)  // I have **NO IDEA** what this means!
{
  int ret;
  DEBUG_BLURB
  
  if(ASSERT)
  {
    ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0090, NULL, 0);
  } 
  else
  {
    ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0010, NULL, 0);
  }
    return ret;
}

int bmcusb_setExtI2C(int nDevId, bool ASSERT)
{
  int ret;
  DEBUG_BLURB

  if(ASSERT)
  {
    ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x00A0, NULL, 0);
  } 
  else
  {
    ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0020, NULL, 0);
  }
    return ret;
}

int bmcusb_zeroDM(int nDevId)
{
  DEBUG_BLURB
  return bmcusb_constantDM(nDevId, 0);
}

int bmcusb_constantDM(int nDevId, bmc_actuatorData_t value)
{
  bmc_actuatorData_t ACTS[USB_NUM_ACTUATORS_MULTI];
  int n;
  
  DEBUG_BLURB
  for(n=0;n<USB_NUM_ACTUATORS_MULTI;n++) 
    ACTS[n] = value;

  return(bmcusb_sendDM(nDevId,ACTS));
}

#define ENDPOINT 2
int bmcusb_sendDM(int nDevId, bmc_actuatorData_t* act_cmds)
{
  DEBUG_BLURB
  // int retval = usb_bulk_write(BMCDEV[nDevId].udev, ENDPOINT, (const char *) act_cmds, USB_NUM_ACTUATORS_MULTI*sizeof(bmc_actuatorData_t), TIMEOUT);
  int retval = usb_bulk_write(BMCDEV[nDevId].udev, ENDPOINT, (const char *) act_cmds, 320, TIMEOUT);
  
/*  int n;
  printf("sendDM: ");
  for(n=0;n<USB_NUM_ACTUATORS_MULTI;n++)
    printf("%04x ", act_cmds[n]);
  printf("\n");*/
  
  if(retval!=USB_BYTES_PER_FRAME_MULTI) {
    if(retval==-110) {
      fprintf(stderr,"\nFAILED: Actuator bulk step write failed for dev=%d. TIMEOUT\n\n",nDevId);
    } else
      fprintf(stderr,"\nFAILED: Actuator bulk step write failed for dev=%d.  bytes written=%d <%s>\n\n",
	    nDevId,retval,usb_strerror());
    return(1);
  } 
  
  return(0);
}

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
