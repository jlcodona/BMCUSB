//////////////////////////////////////////////////////////////////////////////////////
//	Support for Critical Sections
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _CMYAUTOLOCK_F_
#define _CMYAUTOLOCK_F_

#include <mmsystem.h>

class CMyAutoLock
{
public:
	CMyAutoLock(CComAutoCriticalSection& cs) :
		m_cs(cs)
	{
		m_cs.Lock();
	}
	~CMyAutoLock()
	{
		m_cs.Unlock();
	}
protected:
	CComAutoCriticalSection& m_cs;
};


#define MAX_NUM_OF_ACCESS_THREADS 128
class CMyAutoLockData
{

public:
	CMyAutoLockData(DWORD dwWaitTime) :
		m_dwFullLockThreadID(0xDEADFACE),
		m_dwWaitTime(dwWaitTime)
	{
		m_hSemHandle = CreateSemaphore(NULL,MAX_NUM_OF_ACCESS_THREADS,MAX_NUM_OF_ACCESS_THREADS,NULL);
		m_csSemaphore= CreateSemaphore(NULL,1,1,NULL);
	};

	~CMyAutoLockData()
	{
		CloseHandle(m_hSemHandle);
	};

public:
	HANDLE  m_hSemHandle;
	DWORD	m_dwFullLockThreadID;
	DWORD	m_dwWaitTime;
	HANDLE  m_csSemaphore;
};


class CMyAutoSemaphore
{
public:
	// Normal version to be used by thread function
	CMyAutoSemaphore(CMyAutoLockData& autoData) :
		m_bTimeOut(false),
		m_dwLockCount(0),
		m_AutoData(autoData)
	{
		WaitForSingleObject(m_AutoData.m_csSemaphore,INFINITE);
		if(m_AutoData.m_dwFullLockThreadID != GetCurrentThreadId())
		{
			m_bTimeOut = WaitForSingleObject(m_AutoData.m_hSemHandle,m_AutoData.m_dwWaitTime) == WAIT_TIMEOUT;
			if(m_bTimeOut)
				OutputDebugString(_T("Time out in auto semaphore object\n"));
			else 
				m_dwLockCount = 1;
		}
		ReleaseSemaphore(m_AutoData.m_csSemaphore,1,NULL);
	}

	CMyAutoSemaphore(CMyAutoLockData& autoData, int iMaxTimeToWait) :
		m_bTimeOut(false),
		m_dwLockCount(0),
		m_AutoData(autoData)
	{
		bool bCSTimeOut = false;
		if(WaitForSingleObject(m_AutoData.m_csSemaphore,iMaxTimeToWait) == WAIT_TIMEOUT)
		{
			m_bTimeOut = true;
			bCSTimeOut = true;
		}
		else if(m_AutoData.m_dwFullLockThreadID != GetCurrentThreadId())
		{
			m_bTimeOut = WaitForSingleObject(m_AutoData.m_hSemHandle,iMaxTimeToWait) == WAIT_TIMEOUT;
			if(!m_bTimeOut)
				m_dwLockCount = 1;
		}
		if(!bCSTimeOut)
			ReleaseSemaphore(m_AutoData.m_csSemaphore,1,NULL);
	}

	// Full Lock Version to be instatiated to block all threads
	CMyAutoSemaphore(CMyAutoLockData& autoData, DWORD *pdwTimeWaited) :
		m_bTimeOut(false),
		m_dwLockCount(0),
		m_AutoData(autoData)
	{

		if(m_AutoData.m_dwFullLockThreadID == GetCurrentThreadId()) // already have full lock
			return;
		
		WaitForSingleObject(m_AutoData.m_csSemaphore,INFINITE);

		DWORD dwStartTime = timeGetTime();
		while((m_dwLockCount < MAX_NUM_OF_ACCESS_THREADS) && 
			  ((timeGetTime()-dwStartTime) < (m_AutoData.m_dwWaitTime*MAX_NUM_OF_ACCESS_THREADS*2)))
		{
			if(WaitForSingleObjectEx(m_AutoData.m_hSemHandle,m_AutoData.m_dwWaitTime,false) != WAIT_TIMEOUT)
				m_dwLockCount++;
		}
		if(m_dwLockCount != MAX_NUM_OF_ACCESS_THREADS)
		{
			ATLTRACE(_T("Could not get full lock in %d MS (%d/%d)!!\n"),timeGetTime()-dwStartTime,m_dwLockCount,MAX_NUM_OF_ACCESS_THREADS);
			ReleaseSemaphore(m_AutoData.m_csSemaphore,1,NULL);
		}
		else
			m_AutoData.m_dwFullLockThreadID = GetCurrentThreadId(); 

						
		if(pdwTimeWaited)
			*pdwTimeWaited = timeGetTime()-dwStartTime;
//		ATLTRACE(_T("Full lock took %d\n"),timeGetTime()-dwStartTime);
	};
	
	~CMyAutoSemaphore()
	{
		long lCurrentVal = 0;
		if(m_dwLockCount > 1) // I've got a full lock on
		{
			m_AutoData.m_dwFullLockThreadID = 0xDEADFACE;
			ReleaseSemaphore(m_AutoData.m_csSemaphore,1,NULL);
//			ATLTRACE(_T("Full UNLOCK %d\n"),m_dwLockCount);
		}

		if(m_dwLockCount)
		{
			ReleaseSemaphore(m_AutoData.m_hSemHandle,m_dwLockCount,&lCurrentVal);
		}
	}


public:
	bool	m_bTimeOut;
protected:
	CMyAutoLockData& m_AutoData;
	DWORD	m_dwLockCount;
};


#endif