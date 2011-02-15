// HostDrv.h : Declaration of the CHostDrv

#pragma once
#include "resource.h"       // main symbols
#include "CIUsbLib.h"
#include "UsbCom.h"

#define USB_FRAME_BUNDLE 8
#define MAX_USB_XFER_SIZE (USB_BYTES_PER_FRAME_MULTI*USB_FRAME_BUNDLE)
#define NUM_IOS 32

class UsbReadWriteInfo: public OVERLAPPED
{
public:
	UsbReadWriteInfo(DWORD dwSize = MAX_USB_XFER_SIZE) :
		pContext(NULL),
		lLen(0),
		pdwError(NULL),
		dwIOStartTime(0),
		dwIOFinishTime(0),
		hSema(NULL),
		pEndPoint(NULL)
	{
		hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		pBuffer = (BYTE *) VirtualAlloc(NULL, dwSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	}

	~UsbReadWriteInfo()
	{
		CloseHandle(hEvent);
		VirtualFree(pBuffer, 0, MEM_RELEASE);
	}

	HANDLE hSema;
	UCHAR *pContext;
	BYTE *pBuffer;
	long lLen;
	DWORD *pdwError;
	DWORD dwIOStartTime;
	DWORD dwIOFinishTime;
	CCyUSBEndPoint *pEndPoint;
};

// IHostDrv
[
	object,
	//uuid("71BFB71C-707F-424a-ADAE-85E29A2B1F0C"), // previous version
	//uuid("44630B03-D0C1-4f34-8EB6-8971F2E98120"), // previous version
	uuid("41487896-06E3-42ed-AA6A-25FC6E99C4FB"),
	dual,	helpstring("IHostDrv Interface"),
	pointer_default(unique)
]

__interface IHostDrv : IDispatch
{
	[id( 1), helpstring("method CIUsb_GetStatus")]			 HRESULT CIUsb_GetStatus			([in] LONG nDevId, [in] LONG nStatId, [out] LONG* p_nStatus);
	[id( 2), helpstring("method CIUsb_SetControl")]			 HRESULT CIUsb_SetControl			([in] LONG nDevId, [in] LONG nCntlId, [out] LONG* p_nStatus);
	[id( 3), helpstring("method CIUsb_SendFrameData")]		 HRESULT CIUsb_SendFrameData		([in] LONG nDevId, [in] BYTE* pFrameData,  [in] LONG nBuffSize, [out] LONG* p_nStatus);
	[id( 4), helpstring("method CIUsb_SetNotify")]			 HRESULT CIUsb_SetNotify			([in] HWND hWindow, [in] UINT uMessageId);
	[id( 5), helpstring("method CIUsb_GetAvailableDevices")] HRESULT CIUsb_GetAvailableDevices	([out] LONG *pDeviceIds, [in] LONG nSizeBuff, LONG * p_nStatus);
	[id( 6), helpstring("method CIUsb_StreamFrameData")]	 HRESULT CIUsb_StreamFrameData		([in] LONG nDevId, [in] BYTE* pFrameData,  [in] LONG nBuffSize, [out] LONG* p_nStatus);
	[id( 7), helpstring("method CIUsb_StepFrameData")]		 HRESULT CIUsb_StepFrameData		([in] LONG nDevId, [in] BYTE* pFrameData,  [in] LONG nBuffSize, [out] LONG* p_nStatus);
	[id( 8), helpstring("method CIUsb_FlushStream")]		 HRESULT CIUsb_FlushStream		    ([in] LONG nDevId, [out] LONG* p_nStatus);
	[id( 9), helpstring("method CIUsb_StreamFrameDataVar")]	 HRESULT CIUsb_StreamFrameDataVar	([in] LONG nDevId, [in] VARIANT FAR* pFrameDataVar, [in] LONG nBuffSize, [out] LONG* p_nStatus);
	[id(10), helpstring("method CIUsb_StepFrameDataVar")]	 HRESULT CIUsb_StepFrameDataVar		([in] LONG nDevId, [in] VARIANT FAR* pFrameDataVar, [in] LONG nBuffSize, [out] LONG* p_nStatus);

};

// CHostDrv
[
	coclass,
	threading(apartment),
	vi_progid("CIUsbLib.HostDrv"),
	progid("CIUsbLib.HostDrv.1"),
	version(1.0),
	//uuid("8D1AC81B-4A84-47cb-8D9E-635088105DBA"), // previous version
	//uuid("64003207-073C-4477-99DA-DABC2AE91775"), // previous version
	uuid("615FAAA3-B515-4d4c-9F04-013D13FEB154"),
	helpstring("HostDrv Class")
]

class ATL_NO_VTABLE CHostDrv : 
	public IHostDrv,
	public CWindowImpl<CHostDrv>
{
public:
	CHostDrv(){}

	// Optionally specify name of the new Windows class
	DECLARE_WND_CLASS(_T("CHostDrvWindowClass")) 

	BEGIN_MSG_MAP(CMyWindow)
		MESSAGE_HANDLER(WM_DEVICECHANGE, OnDeviceChange)
	END_MSG_MAP()

	LRESULT OnDeviceChange(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct();
	void FinalRelease();

public:
	STDMETHOD(CIUsb_GetStatus)			(LONG nDevId, LONG nStatId, LONG* p_nStatus);
	STDMETHOD(CIUsb_SetControl)			(LONG nDevId, LONG nCntlId, LONG* p_nStatus);
	STDMETHOD(CIUsb_SendFrameData)		(LONG nDevId, BYTE* pFrameData,  LONG nBuffSize, LONG* p_nStatus);
	STDMETHOD(CIUsb_StreamFrameData)	(LONG nDevId, BYTE* pFrameData,  LONG nBuffSize, LONG* p_nStatus);
	STDMETHOD(CIUsb_StepFrameData)		(LONG nDevId, BYTE* pFrameData,  LONG nBuffSize, LONG* p_nStatus);
	STDMETHOD(CIUsb_SetNotify)			(HWND hWindow, UINT uMessageId);
	STDMETHOD(CIUsb_GetAvailableDevices)(LONG *pDeviceIds, LONG nSizeBuff, LONG * p_nStatus);
	STDMETHOD(CIUsb_FlushStream)		(LONG nDevId, LONG* p_nStatus);
	STDMETHOD(CIUsb_StreamFrameDataVar)	(LONG nDevId, VARIANT FAR* pFrameDataVara, LONG nBuffSize, LONG* p_nStatus);
	STDMETHOD(CIUsb_StepFrameDataVar)	(LONG nDevId, VARIANT FAR* pFrameDataVar, LONG nBuffSize, LONG* p_nStatus);
	CUsbCom	m_UsbComm; 

private:
	DWORD	m_dwUsbBytesPerFrame;
	HWND	m_hNotifyWindow;
	UINT	m_hNotifyMessage;
	CComAutoCriticalSection	m_csAccess;
	HWND m_hWnd;
	UsbReadWriteInfo *m_pUsbRwInfo[NUM_IOS];
	HANDLE m_hXferSema;
	int m_iCurrent;
	int m_iFrame;
	bool m_fThreadRunning;
	HANDLE m_hThread;
	static DWORD WINAPI WorkerThread(LPVOID arg);
	void Flush();
};

