#pragma once
#include "stdafx.h"
/* url & credential length options */
#define MAX_CRED_LENGTH			2048
#define MAX_URL_LENGTH			2048

typedef struct _SGBrowserCredentials
{
	int Browser; // 0 = chrome, 1 = ie, 2 = firefox, 
	TCHAR Site[256];
	TCHAR UserName[80];
	TCHAR Password[256];
	CTime DateCreated;
	_SGBrowserCredentials()
	{
		Site[0] = 0;
		UserName[0] = 0;
		Password[0] = 0;
		DateCreated = NULL;
	}
} SGBrowserCredentials;

typedef CSimpleArray<SGBrowserCredentials> SGBrowserCredentialsArray;
