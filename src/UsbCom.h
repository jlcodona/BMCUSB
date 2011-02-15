#pragma once

#pragma once
#include "CyAPI.h"
#include "CIUsbLib.h"
#include "CIUsbShared.h"

class CUsbCom
{
public:
	CUsbCom(void);
	~CUsbCom(void);

	// USB configuration
	CCyUSBDevice		*m_pUSBMaster;			// used for device count query and event messages
	CCyUSBDevice		*m_pUSBDev				[MAX_USB_DEVICES];
	CCyUSBEndPoint		*m_pDataOutEndpoint		[MAX_USB_DEVICES];
	CCyControlEndPoint	*m_pControlEndpoint		[MAX_USB_DEVICES];
	DWORD				 m_dwXferSize;

	bool ResetAll(LONG nDevId);
	void ProbeDevices();

	// USB I/O
	bool VendorRequest(LONG nDevId, BYTE byReqCode, bool bToTarget, WORD wValue, WORD wIndex, BYTE *pbyBuffer, long lLen);
	void CloseUsbDevice(int iDevice);
	bool OpenUsbDevice(int iDevice);
	int m_iNumDevices;
	int InitializeUsb( HWND hWnd, DWORD dwXferSize );
	void UpdateXferSize( DWORD dwXferSize );
	CString m_strFxVersion;
	CString UsbGetFxVersion(void);
	CComAutoCriticalSection	m_csUsbComm;
};

