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

#ifndef BMCUSBH
#define BMCUSBH

#define LIBBMCUSB_VERSION 0.1

#include <stdint.h>

#include "CIUsbLib.h"
#include "CIUsbShared.h"

#include "BMC_Mappings.h"

#include <usb.h>

/* LIBRARY DEFINES */

typedef long 		bmc_controlID_t;
typedef uint16_t 	bmc_actuatorData_t;

typedef int16_t WORD;
typedef char BYTE;
// typedef int bool; 
// typedef enum { FALSE = 0, TRUE = 1 } bool; 
// enum bool { FALSE = 0, TRUE = 1 };
// #define 0 false
// #define 1 true

////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////

void bmcusb_setDebug(int state);

int bmcusb_probe(); 		// returns number of devices found.
int bmcusb_claim(int nDevId); 	// call before using.
int bmcusb_release(int nDevId); // call before quitting.

int bmcusb_printStatus(int nDevId);
int bmcusb_getStatus(int nDevId);
int bmcusb_setControl(int nDevId, int nCntlId);

struct usb_device *bmcusb_getDev(int nDevId);
usb_dev_handle *bmcusb_getUdev(int nDevId);

int bmcusb_assertReset (int nDevId);
int bmcusb_deassertReset(int nDevId);
int bmcusb_reset(int nDevId); // does both of the above.
char *bmcusb_getFirmwareVersion(int nDevId);

int bmcusb_setHV	(int nDevId, int ON);

int bmcusb_setFrameSync	(int nDevId, int ASSERT);
int bmcusb_setLVShdn	(int nDevId, int ASSERT);
int bmcusb_setExtI2C	(int nDevId, int ASSERT);

int bmcusb_zeroDM(int nDevId);
int bmcusb_constantDM(int nDevId, bmc_actuatorData_t value);

int bmcusb_setDM(int nDevId, bmc_actuatorData_t* acts_cmds); // buffer size is tacit.
int bmcusb_setMappedDM(int nDevId, bmc_actuatorData_t* acts_cmds); // buffer size is tacit.
int bmcusb_sendDM(int nDevId); // sends the buffer previously set.

int bmcusb_VendorRequest(int nDevId, int reqCode, int bToTarget, int wValue, int wIndex, char *buffer, int len);

// Utility functions:
void clearBuffer(bmc_actuatorData_t buf[], int N) ;
bmc_actuatorData_t *mapActs(int nDevId, bmc_actuatorData_t *FROM, bmc_actuatorData_t *TO);
bmc_actuatorData_t *_mapActs(bmc_actuatorData_t *FROM, bmc_actuatorData_t *TO, int *map);

#endif
