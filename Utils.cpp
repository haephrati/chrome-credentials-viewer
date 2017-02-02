////////////////////////////////////////////////////////////////////////
//----------------------------Datattoo -------------------------------//
// Datattoo ©2013-2016 Secured Globe, Inc. support@securedglobe.com   //
//--------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////#include "stdafx.h"
#include <shlwapi.h>
#include <ShlObj.h>
#include <sddl.h>
#include "Utils.h"
#include "Windows.h"
#include "Shellapi.h"

#define TICKS_PER_SECOND 10000000
#define EPOCH_DIFFERENCE 11644473600LL


namespace utils
{
	// Use this function for temporary files with random names
	void GenerateRandomFileName(LPWSTR FullPath, LPWSTR &FileName, LPWSTR Extension, LPWSTR Prefix)
	{

		TCHAR Personal[255] = { L"" };

		wchar_t thisDir[MAX_PATH];
		::GetModuleFileNameW(nullptr, thisDir, ARRAYSIZE(thisDir));
		::PathRemoveFileSpecW(thisDir);
		wcscpy(FullPath, thisDir);
		wcscat(FullPath, L"\\");
		int curlen = wcslen(FullPath);
		FileName = FullPath + curlen;
		if (wcscmp(Prefix, L""))
		{
			wcscat(FullPath, Prefix);
			curlen += wcslen(Prefix);
		}
		while (true)
		{
			int FileNameLen = curlen + 11 * rand() / (RAND_MAX + 1) + 4;
			int i;
			for (i = curlen; i < FileNameLen; i++)
			{
				FullPath[i] = RANDOM_TCHAR;
			}
			FullPath[i] = 0;
			if (wcscmp(Extension, L""))	// Need to add an extension to the file
			{
				wcscat(FullPath, Extension);
			}
		}

		//utils::WriteLogFile(LOG_COLOR_WHITE,LOG_VERBOSE,L"TempFile: '%s'\n", FullPath);

	}

	// We use this function for baloon type alerts
	void DisplayAlert(const wchar_t *message, const wchar_t *title, int timeout)
	{
		NOTIFYICONDATA nid;
		memset(&nid, 0, sizeof(nid));
		nid.cbSize = sizeof(nid);
		nid.uFlags = NIF_INFO;
		wcsncpy(nid.szInfoTitle, title, 64);
		wcsncpy(nid.szInfo, message, 64);
		nid.uTimeout = timeout;
		nid.dwInfoFlags = NIIF_INFO;
		Shell_NotifyIcon(NIM_DELETE, &nid);
		Shell_NotifyIcon(NIM_ADD, &nid);
		WriteLogFile(L"Displayed alert (%s)", message);
	}

	// We use this function for logging and debugging. Works like printf
	void WriteLogFile(LPCTSTR lpText, ...)
	{
		FILE *fp;
		CTime Today = CTime::GetCurrentTime();

		CString sMsg;
		CString sLine;
		va_list ptr;
		va_start(ptr, lpText);
		sMsg.FormatV(lpText, ptr);



		sLine.Format(L"%s: %s\n", (LPCTSTR)Today.Format(L"%d.%m.%Y %H:%M"), (LPCTSTR)sMsg);


		fp = _wfopen(L"log.txt", L"a");
		if (fp)
		{
			fwprintf(fp, L"%s\n", sLine);
			fclose(fp);
		}
	}

}

