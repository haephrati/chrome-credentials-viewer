#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include <stddef.h>
#include <Windows.h>
#include <VersionHelpers.h>

#define STR_NULL L""
#define CHR_MINUS L'-'
#define CHR_SPACE L' '
#define CHR_COLON L':'
#define SG_DATE_FORMAT L"%Y%m%d%H%M%S"
#define SG_FRIEDLY_DATEFORMAT L"%d-%m-%Y, %H:%M:%S"
#define DEFAULT_EARLIEST_DATE L"19000101000000"
#define CHAR_BW_SLASH L'\\'
#define STR_0 L"0"

extern 	CString utilsLogFilename;

namespace utils
{

	// convert chrome time to system time.
	SYSTEMTIME ChromeTimeToSysTime(const CString& ChromeTime);

	// convert system time to chrome time.
	CString SystemTimeToChromeTime(const SYSTEMTIME& stTime);

	CTime ChromeCredentialsTimeToCTime(const CString& ChromeTime);

	CString ChromeTimeToStringW(CString ChromeTime);

	CString ChromeCredentialsTimeToString(const CString& ChromeTime);

	long long GetFileSize(const TCHAR *filename);

	CString SG_Date_Format_2_UTC(CString original);

	CTime Time2UTC(CTime original);

	void TimeToLongLong(FILETIME *time, ULONGLONG *result);

	void TimeToStringW(FILETIME *time, WCHAR *buffer, size_t length);

	void WriteStatus(LPCTSTR lpText, ...);

	int decode_b64(const char *src, unsigned char *dest,
		size_t len, size_t *decoded);
	static int decode_4chars(const char *src, unsigned char *dest);


}

