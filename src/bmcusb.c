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
#include "BMC_Mappings.h"
#include "CIUsbLib.h"
#include "CIUsbShared.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//#define DEBUG_BLURB   if(BMCUSB_DEBUG) fprintf(stderr,"DEBUG: file %s line %d: inside %s(...)\n",__FILE__,__LINE__,__FUNCTION__);
#define DEBUG_BLURB   if(BMCUSB_DEBUG) fprintf(stderr,"DEBUG: %s\n",__FUNCTION__);

static BMCUSB_DEVICE BMCDEV[MAX_BMCUSB_DEVICES];
static struct usb_device *bmc_devs[MAX_BMCUSB_DEVICES];
static int Nbmc = 0;

static int BMCUSB_DEBUG = 0;

void bmcusb_setDebug(int state) {
  BMCUSB_DEBUG = state;
  if(state)
    usb_set_debug(5);
  else
    usb_set_debug(0);   
}

int bmcusb_probe() {
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
      BMCDEV[Nbmc].mapping = mapMultiDM01;  // this is probably right. 
      
      int dnum = atoi(bus->dirname);
      int fnum = atoi(dev->filename);
      
      sprintf(BMCDEV[Nbmc].devpath,"/sys/bus/usb/devices/%d-%d",dnum,fnum);
      printf("%s\n",BMCDEV[Nbmc].devpath);
      
      Nbmc++;
    }
  }
  
  return(Nbmc);
}

int bmcusb_claim(int nDevId) {  
  DEBUG_BLURB
  int ret;

  if(nDevId>=Nbmc) {
    fprintf(stderr,"ERROR: There does not appear to be a BMC controller number %d in the system since there are only %d driver boxes.\n",nDevId,Nbmc);
    return(-1);
  }

  if(BMCDEV[nDevId].udev==NULL)
    BMCDEV[nDevId].udev = usb_open(BMCDEV[nDevId].dev);

  if(BMCDEV[nDevId].udev==NULL) {
    fprintf(stderr,"ERROR: failed to get a udev for interface %d\n",nDevId);
    return(1);
  }

  char string[256];

  if(BMCDEV[nDevId].dev->descriptor.iManufacturer) {
    ret = usb_get_string_simple(BMCDEV[nDevId].udev, BMCDEV[nDevId].dev->descriptor.iManufacturer, string, sizeof(string));
  
    if (ret > 0)
      printf("- Manufacturer : %s\n", string);
    else
      printf("- Unable to fetch manufacturer string\n");
  }

  if (BMCDEV[nDevId].dev->descriptor.iProduct) {
    ret = usb_get_string_simple(BMCDEV[nDevId].udev, BMCDEV[nDevId].dev->descriptor.iProduct, string, sizeof(string));
    if (ret > 0)
      printf("- Product      : %s\n", string);
    else
      printf("- Unable to fetch product string\n");
  }

  return(usb_claim_interface(BMCDEV[nDevId].udev,0));
}

char *bmcusb_getFirmwareVersion(int nDevId) {
  int ret;
  //err = vcmd(udev, eCIUsbCmndGetFirmwareVer, 0, 0, 0x16, string); // Returns: "DM Driver USB Interface"
  //err = vcmd(udev, 0xC0, 0, 0, 0x16, string); // Returns: "DM Driver USB Interface"
  ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndGetFirmwareVer, 0, 0, 0, BMCDEV[nDevId].firmware,0x16);
  
  if(ret!=0x16){
    printf("eCIUsbCmndGetFirmwareVer FAILED: ret=%d\n",ret);
    return(NULL);    
  } else
    return(BMCDEV[nDevId].firmware);
}

int bmcusb_release(int nDevId) {  // call before quitting.

  DEBUG_BLURB
  int ret = 0;
  
  if(BMCDEV[nDevId].udev!=NULL) {
    ret = usb_release_interface(BMCDEV[nDevId].udev,0);
    
    BMCDEV[nDevId].udev = NULL;
  }
  return ret;
}

int bmcusb_VendorRequest(int nDevId, int reqCode, int bToTarget, int wValue, int wIndex, char *buffer, int len){
    DEBUG_BLURB
    int ret;
    if(BMCUSB_DEBUG) printf("VendorRequest: %02X %d 0x%04x %d\n",reqCode,wValue,wIndex,len);
    if(!bToTarget)
      ret = usb_control_msg(BMCDEV[nDevId].udev,0xC0,reqCode,wValue,wIndex,buffer,len,TIMEOUT);
    else
      ret = usb_control_msg(BMCDEV[nDevId].udev,0x40,reqCode,wValue,wIndex,buffer,len,TIMEOUT);
      
    return(ret);
}

int bmcusb_assertReset(int nDevId){
  DEBUG_BLURB
  printf("\nDEBUG: ASSERT RESET COMMAND.\n");
  
  return(bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0082, NULL, 0));
}

int bmcusb_deassertReset(int nDevId){
  DEBUG_BLURB  
  printf("\nDEBUG: DE-ASSERT RESET COMMAND.\n");
  return(bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0002, NULL, 0));
}

int bmcusb_reset(int nDevId){
  DEBUG_BLURB
  if(BMCUSB_DEBUG) printf("\nDEBUG: RESET.\n");
  
  int ret;
  char goo[8]; // is this needed???
  
  ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0002, goo, 0);
  if(ret) return ret;
  
  ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0082, goo, 0);
  
  return ret; 
}

int bmcusb_setHV(int nDevId, int ON){
  int ret;
  DEBUG_BLURB
  
  if(ON) // Turn ON the High Voltage Power to the DM...
  {
    if(BMCUSB_DEBUG) printf("\nDEBUG: Turning HV ON.\n");
      // usleep(250000); // I think this is just for the MINI, but I am playing it safe.
    ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0088, NULL, 0);
  } 
  else  // Turn OFF the High Voltage Power to the DM...
  {
    if(BMCUSB_DEBUG) printf("\nDEBUG: Turning HV OFF.\n");
    ret = bmcusb_VendorRequest(nDevId, eCIUsbCmndSetControlBits, 1, 0, 0x0008, NULL, 0);
  }
  return ret;
}

int bmcusb_setFrameSync(int nDevId, int ASSERT){
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

int bmcusb_setLVShdn(int nDevId, int ASSERT) { // I have **NO IDEA** what this means!

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

int bmcusb_setExtI2C(int nDevId, int ASSERT){
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

int bmcusb_zeroDM(int nDevId){
  DEBUG_BLURB
  return bmcusb_constantDM(nDevId, 0);
}

int bmcusb_constantDM(int nDevId, bmc_actuatorData_t value) {
  DEBUG_BLURB
  int n;
  
  for(n=0;n<USB_NUM_ACTUATORS_MULTI;n++) 
    BMCDEV[nDevId].actuators[n] = value;
  
  return(bmcusb_sendDM(nDevId));
}

int bmcusb_setDM(int nDevId, bmc_actuatorData_t *value) {
  DEBUG_BLURB
  int n;
  
  for(n=0;n<USB_NUM_ACTUATORS_MULTI;n++) 
    BMCDEV[nDevId].actuators[n] = value[n];
  
  return(bmcusb_sendDM(nDevId));
}

int bmcusb_setMappedDM(int nDevId, bmc_actuatorData_t *values) {
  DEBUG_BLURB
  int n, *map;
  
  map = BMCDEV[nDevId].mapping;
  
  bmc_actuatorData_t *acts = BMCDEV[nDevId].actuators;
  
  for(n=0;n<USB_NUM_ACTUATORS_MULTI;n++) 
    acts[n] = values[map[n]];
  
  return(bmcusb_sendDM(nDevId));
}

#define ENDPOINT 2
int bmcusb_sendDM(int nDevId) { // This sends the buffer in BMCDEV.  Set it separately.

  DEBUG_BLURB
  if(BMCUSB_DEBUG) {
    int n;
    printf("sendDM: ");
    for(n=0;n<USB_NUM_ACTUATORS_MULTI;n++) {
      if(!(n%12)) 
	printf("\n");
      printf("%04x ", BMCDEV[nDevId].actuators[n]);
    }
    printf("\n");
  }

  int retval = usb_bulk_write(BMCDEV[nDevId].udev, 2, (const char *) (BMCDEV[nDevId].actuators), 320, TIMEOUT);
    //retval = usb_bulk_write(udev, 2, (const char *) sActData, NUM_ACTUATORS*sizeof(u_int16_t), TIMEOUT);
  if(retval!=USB_BYTES_PER_FRAME_MULTI) {
    if(retval==-110) {
      fprintf(stderr,"\nFAILED: Actuator bulk step write failed for dev=%d. TIMEOUT\n\n",nDevId);
    } else {
      fprintf(stderr,"\nFAILED: Actuator bulk step write failed for dev=%d.  bytes written=%d <%s>\n\n",
	      nDevId,retval,usb_strerror());
    }
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

void bmcusb_setMap(int nDevId, int *mapping) {
  BMCDEV[nDevId].mapping = mapping;
}

struct usb_device *bmcusb_getDev(int nDevId) {
    return(BMCDEV[nDevId].dev);
}

usb_dev_handle *bmcusb_getUdev(int nDevId) {
    return(BMCDEV[nDevId].udev);
}

int bmcusb_printStatus(int nDevId){
    int status = bmcusb_getStatus(nDevId);

    printf("STATUS[%d]: ",nDevId);

    // bit[0]=FrameErr,  bit[1]=RdHvaE, bit[2]=CableOk, bit[9]=ExEepromPresent
    // bit[7]=Set/Reset, bit[1]=Freset, bit[2]=FrameSync, bit[3]=HvEnab
    if(status & 0x01) printf("[FRAME ERROR]");
    if(status & 0x02) printf("[RdHvaE]");
    if(!(status & 0x04)) printf("[Check Cables]");
    if(status & 0x08) printf("[bit3]");
    if(status & 0x10) printf("[bit4]");
    if(status & 0x20) printf("[bit5]");
    if(status & 0x40) printf("[bit6]");
    if(status & 0x80) printf("[bit7]");
    if(status & 0x100) printf("[bit8]");
    if(status & 0x200) printf("[ExEepromPresent]");
    if(status & 0x400) printf("[bit10]");
    printf("\n");
    return(status);
}

int bmcusb_getStatus(int nDevId){
    u_int16_t statusBits = 0;

    bmcusb_VendorRequest(nDevId,eCIUsbCmndGetStatusBits,0,0,0,(char *) &statusBits,sizeof(u_int16_t));

    return(statusBits);
}

int bmcusb_setControl(int nDevId, int nCntlId){
    //if (!m_UsbComm.VendorRequest(nDevId, eCIUsbCmndSetControlBits, true /*ToTarget*/, 0 /*value*/, 0x0010 /*index*/, NULL, 0))
    return(bmcusb_VendorRequest(nDevId,eCIUsbCmndSetControlBits,1,0,0x0010,NULL,0));
}

bmc_actuatorData_t *mapActs(int nDevId, bmc_actuatorData_t *FROM, bmc_actuatorData_t *TO) {
  int n, *map = BMCDEV[nDevId].mapping;
  
  for(n=0;n<USB_NUM_ACTUATORS_MULTI;n++)
    TO[n] = FROM[map[n]];
  
  return(TO);
}

bmc_actuatorData_t *mapNActs(int nDevId, bmc_actuatorData_t *FROM, bmc_actuatorData_t *TO, int nActs) {
  int n, *map = BMCDEV[nDevId].mapping;

  for(n=0;n<nActs;n++)
    TO[n] = FROM[map[n]];

  return(TO);
}

bmc_actuatorData_t *_mapActs(bmc_actuatorData_t *FROM, bmc_actuatorData_t *TO, int *map) {
  int n;
  
  for(n=0;n<USB_NUM_ACTUATORS_MULTI;n++)
    TO[n] = FROM[map[n]];
  
  return(TO);
}
