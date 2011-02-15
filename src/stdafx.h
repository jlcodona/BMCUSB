// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500     // The DPIO2 driver was written for Windows 2000
#endif				// Keep this at 0x500

#include <stdio.h>

#ifndef __linux__
#include <tchar.h>
#endif

#ifndef __linux__
#include <atlbase.h>
#include <atlcom.h>
#endif

//#include "Windows.h"

#pragma warning (disable : 4192)
// be sure to get the latest release of the following
#include "CIUsbLib.h"

#ifndef __linux__
#import "_CIUsbLib.tlb" no_namespace
using namespace std;
#endif

#define NUM_ACTUATORS	USB_NUM_ACTUATORS_MULTI
#define USB_DEVNAME	"Multi"

