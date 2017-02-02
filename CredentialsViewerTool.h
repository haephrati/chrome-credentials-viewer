
// CredentialsTool.h : main header file for the Credentials Viewer application
//

#pragma once
#include "CredentialsViewerToolDlg.h"
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "SG_ProgramSettings.h"
extern SG_ProgramSettings GlobalSettings;

// CCredentialsToolApp:
// See CredentialsToolApp.cpp for the implementation of this class
//

class CCredentialsToolApp : public CWinApp
{
public:
	CCredentialsToolApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCredentialsToolApp theApp;
