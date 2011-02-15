// UsbExMulti.cpp : Defines the entry point for the console application.

#include "stdafx.h"

// define a macro for short-hand error processing
#define CheckHr(s) if (FAILED(hr)){printf(s);return 0;}

// main entry point of console program
int _tmain(int argc, _TCHAR* argv[])
{
    // Initializes the COM library on the current thread and
    // identifies the concurrency model as single-thread apartment.
    // Applications must initialize the COM library before they
    // can call COM library functions
    CoInitialize(NULL);

    long lCurDev = -1;			// current USB device index: -1 means no devices
    long lStatus = 0;			// CIUsbLib return status
    CComPtr<IHostDrv> pIHostDrv;	// CIUsbLib COM Pointer

    // Creates a single uninitialized object of the class associated with a specified CLSID=__uuidof(CHostDrv)
    // This object will be found on the system if CIUsbLib.dll is registered via regsvr32.exe
    HRESULT hr = CoCreateInstance(__uuidof(CHostDrv), NULL, CLSCTX_INPROC, __uuidof(IHostDrv), (LPVOID *) &pIHostDrv);
    if (hr == REGDB_E_CLASSNOTREG)
    {
        printf("The CHostDrv class is not registered.\nUse regsvr32.exe to register CIUsbLib.dll\n");
        return 0;
    }
    else if (FAILED(hr))
    {
        printf("Error (0x%08x) creating CHostDrv object (CIUsbLib.dll)\n", hr);
        return 0;
    }

    // Check for USB devices supported by the CIUsbLib
    // The array lDevices is set by CIUsb_GetAvailableDevices to indicate which devices are present in the system
    // In order to recognize MULTI DM devices, {CiGenUSB.sys, CiGenUSB.inf} need to be installed properly
    long lDevices[MAX_USB_DEVICES] = {-1};
    hr = pIHostDrv->CIUsb_GetAvailableDevices(lDevices, sizeof(lDevices)/sizeof(long), &lStatus);
    CheckHr("Failure to get available USB devices.\n");

    // loop through devices found
    for (int i=0; i<MAX_USB_DEVICES; i++)
    {
        // check for device indices not equal to -1
        if (lDevices[i] != -1)
        {
            // if we have any present, check specifically for MULTI via CIUsb_STATUS_DEVICENAME
            char cDevName[4096] = {0};
            hr = pIHostDrv->CIUsb_GetStatus(lCurDev, CIUsb_STATUS_DEVICENAME, (long *) cDevName);
            CheckHr("Failure to get available USB device name.\n");

            // check the device for MULTI signature
            bool fFoundMulti = (strstr(cDevName, USB_DEVNAME)!=NULL);
            if (fFoundMulti)
            {
                // record device index
                lCurDev = i;
                // report devices present
                printf("Found: %s\n", cDevName);
                // bail after finding first device (simplest method)
                break;
            }
        }
    }

    // if lCurDev is still -1, we found none
    if (lCurDev == -1)
    {
        printf("No Multi DM devices were found.\n");
        return 0;
    }

    // reset the hardware: control signal FRESET is active low
    hr = pIHostDrv->CIUsb_SetControl(lCurDev, CIUsb_CONTROL_DEASSERT_FRESET, &lStatus);
    CheckHr("Failure to deassert MULTI hardware reset control.\n");
    hr = pIHostDrv->CIUsb_SetControl(lCurDev, CIUsb_CONTROL_ASSERT_FRESET,   &lStatus);
    CheckHr("Failure to assert MULTI hardware reset control.\n");

    // assert high voltage enable
    hr = pIHostDrv->CIUsb_SetControl(lCurDev, CIUsb_CONTROL_ASSERT_HV_ENAB,  &lStatus);
    CheckHr("Failure to enable MULTI hardware high voltage enable.\n");

    ///////////////////////////////////////////////////////////////////////////////////
    // The following is the start of an example application sequence.
    // Five actuators will be poked using the index and value arrays.

    USHORT	sActData	[NUM_ACTUATORS]  = {0x0000};	// unmapped actuator data for sending to the DM
    USHORT	sMapData	[NUM_ACTUATORS]  = {0x0000};	// mapped actuator data for sending to the DM

    int i160TestMap[NUM_ACTUATORS] = // example actuator map
        { 2,32,72,96,1,24,64,88,134,10,40,14,104,80,12,56,33,122,97,13,3,48,73,112,22,25,
          65,89,135,23,41,15,105,81,119,57,34,123,98,120,4,49,74,113,130,26,66,90,136,131,
          42,16,106,82,133,58,35,124,99,142,5,50,75,114,0,27,67,91,137,11,43,17,107,83,132,
          59,36,125,100,143,6,51,76,115,144,28,68,92,138,145,44,18,108,84,146,60,37,126,101,
          147,7,52,77,116,148,29,69,93,139,149,45,19,109,85,150,61,38,127,102,151,8,53,78,
          117,152,30,70,94,140,153,46,20,110,86,154,62,39,128,103,155,9,54,79,118,156,31,
          71,95,141,157,47,21,111,87,158,63,55,129,121,159
        };

    // iActIndex:  actuator index (raster=unmapped)
    // sActVAlues: values to set each actuator
#define NUM_TEST_POKES	5
    int		iActIndex	[NUM_TEST_POKES] = {     2,     15,     39,    125,    150};
    USHORT	sActVAlues	[NUM_TEST_POKES] = {0x1000, 0x2000, 0x3000, 0x4000, 0x5000};

    for (int i=0; i<NUM_TEST_POKES; i++)
    {
        if (iActIndex[i] < 0 || iActIndex[i] >= NUM_ACTUATORS)
        {
            printf("Actuator index %d is out of range... skipping to next poke.\n", iActIndex[i]);
            continue;
        }

        // modify the actuator data at index iActIndex[i] with value sActVAlues[i]
        sActData[iActIndex[i]] = sActVAlues[i];

        // copy our actuator data into the mapped buffer;
        // use the actuator map to re-order the data into the proper sequence
        for ( int j=0; j<NUM_ACTUATORS; j++ )
            sMapData[j] = sActData[i160TestMap[j]];

        // send the actuator data to the DM synchronously
        hr = pIHostDrv->CIUsb_StepFrameData(lCurDev, (UCHAR *) sMapData, NUM_ACTUATORS*sizeof(short), &lStatus);
        CheckHr("Failure to send MULTI frame data.\n");

        // check for framing errors
        if (lStatus == H_DEVICE_NOT_FOUND)
        {
            printf("Framing error: device not found");
            return 0;
        }
        else
            if (lStatus == H_DEVICE_TIMEOUT)
            {
                printf("Framing error: device timeout");
                return 0;
            }

        // print the loop status
        printf("Ouput frame %d: Actuator Index [%04d] = 0x%04x\n", i, iActIndex[i], sActVAlues[i]);
    }

    return 0;
}

