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

#ifndef BMCUSBH
#define BMCUSBH

#define LIBBMCUSB_VERSION 0.1

#include <stdint.h>

/* LIBRARY DEFINES */

typedef long 		bmc_status_t;
typedef long 		bmc_controlID_t;
typedef uint16_t 	bmc_actuatorData_t;
uint8_t 

////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////

int bmcusb_probe(); 	// returns number of devices found.
int bmcusb_claim      	(int nDevId); 		// call before using.
int bmcusb_release    	(int nDevId); 		// call before quitting.
int bmcusb_getStatus	(int nDevId, bmc_status_t* p_nStatus); // get device status. 

int bmcusb_setControl	(int nDevId, bmc_controlID_t nCntlId, bmc_status_t* p_nStatus);

int bmcusb_zero		(int nDevId);
int bmcusb_setConstant	(int nDevId, bmc_actuatorData_t value);
int bmcusb_sendFrame	(int nDevId, bmc_actuatorData_t* acts_cmds,  int nBuffSize, bmc_status_t* status_out);
int bmcusb_streamFrame	(int nDevId, bmc_actuatorData_t* acts_cmds,  int nBuffSize, bmc_status_t* status_out);
int bmcusb_stepFrame	(int nDevId, bmc_actuatorData_t* acts_cmds,  int nBuffSize, bmc_status_t* status_out);
int bmcusb_flush	(int nDevId, bmc_status_t* p_nStatus);

// bmcusb_StreamFrameDataVar(LONG nDevId, VARIANT FAR* pFrameDataVara, LONG nBuffSize, LONG* p_nStatus);
// bmcusb_StepFrameDataVar(LONG nDevId, VARIANT FAR* pFrameDataVar, LONG nBuffSize, LONG* p_nStatus);

#endif
