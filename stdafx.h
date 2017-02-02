
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#include <Wlanapi.h>
#define CRT_SECURE_NO_WARNINGS
#ifndef WLAN_PROFILE_GET_PLAINTEXT_KEY
#define WLAN_PROFILE_GET_PLAINTEXT_KEY 4 // Dont have the latest platform SDK on this box
#endif
#include <wincrypt.h>
#include <wincred.h>
#include <wlanapi.h>


#include <Windows.h>
#include <WinInet.h>
#include <Shlwapi.h>
#include <stdio.h>
#include <strsafe.h>
#include <tchar.h>

#include <Wincrypt.h>
#include <atlbase.h>
#include <string>
#include <sstream>
#include <string>
#include "CppSQLite3U.h"
#include "pstore.h"
#include "cppsqlite3u.h"
#include <Windows.h>
#include <wincred.h>
#include <WinCrypt.h>
#include <WinInet.h>
#include <shlwapi.h>
#include <userenv.h>
#include <wchar.h>
#include <string.h>
#include <tchar.h>
#include <LMCons.h>
#include <stdlib.h>
#include <stdio.h>
#include <Shlobj.h>
#include <locale>
#include <codecvt>
#include <VersionHelpers.h>
#include <iostream>
#include <stdint.h>
using namespace std;


/* password types */
enum pass_type_t
{
	ie6_autoc,
	ie6_hbasic,
	ie7_autoc,
	ie7_hbasic,
	ie10_totden,
	outlook_15,
	firefox3,
	firefox4,
	chrome,
	/* types from here onwards don't have urls */
	no_url,
	gen_net,
	domain_vis,
	msn_msngr,
	winlive,
	gtalk,
	header
};

#define SF_MEMCPY memcpy


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


