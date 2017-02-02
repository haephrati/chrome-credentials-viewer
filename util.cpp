// Chrome Credentials Viewer
// By Michael Haephrati
// ©2014-2017 Secured Globe, Inc.

#include "stdafx.h"
#include <shlwapi.h>
#include <ShlObj.h>
#include <sddl.h>
#include "Utils.h"
#include "Windows.h"
#include "Shellapi.h"
static const char *b64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
#include <atlbase.h>
#include "resource.h"
#include "SG_helper_base64.h"
#include <VersionHelpers.h>

#define TICKS_PER_SECOND 10000000
#define EPOCH_DIFFERENCE 11644473600LL
#define APPLE_DIFFERENCE 12622780800LL



CString utilsLogFilename = L"Credentials Report.txt";

namespace utils
{

	// convert chrome time to system time.
	SYSTEMTIME ChromeTimeToSysTime(const CString& ChromeTime)
	{
		ULONGLONG lltm = _wcstoui64(ChromeTime.GetString(), NULL, 10);
		ULARGE_INTEGER uLarge;
		uLarge.QuadPart = lltm * 10;
		FILETIME ftTime;
		ftTime.dwHighDateTime = uLarge.HighPart;
		ftTime.dwLowDateTime = uLarge.LowPart;
		SYSTEMTIME stTime;
		FileTimeToSystemTime(&ftTime, &stTime);
		return stTime;

	}

	// convert system time to chrome time.
	CString SystemTimeToChromeTime(const SYSTEMTIME& stTime)
	{
		CString str;
		TCHAR *buffer = str.GetBufferSetLength(100);
		swprintf(buffer, 100, L"%04d%02d%02d%02d%02d%02d",
			stTime.wYear, stTime.wMonth, stTime.wDay,
			stTime.wHour, stTime.wMinute, stTime.wSecond);
		return str;
	}

	CTime ChromeCredentialsTimeToCTime(const CString& ChromeTime)
	{
		SYSTEMTIME st = ChromeTimeToSysTime(ChromeTime);
		CTime result(st);
		return result;
	}
	CString ChromeTimeToStringW(CString ChromeTime)
	{
		CString result = L"";
		result = ChromeTime.Mid(0, 4) + ChromeTime.Mid(5, 2) + ChromeTime.Mid(8, 2) + ChromeTime.Mid(11, 2) + ChromeTime.Mid(14, 2) + ChromeTime.Mid(17, 2);
		return result;
	}

	CString ChromeCredentialsTimeToString(const CString& ChromeTime)
	{
		CString result =  SystemTimeToChromeTime(ChromeTimeToSysTime(ChromeTime));
		return result;
	}
	long long GetFileSize(const TCHAR *filename)
	{
		WIN32_FILE_ATTRIBUTE_DATA fad;
		if (GetFileAttributesEx(filename, GetFileExInfoStandard, &fad))
		{
			LARGE_INTEGER size;
			size.LowPart = fad.nFileSizeLow;
			size.HighPart = fad.nFileSizeHigh;
			return size.QuadPart;
		}
		return 0;
	}
	CString SG_Date_Format_2_UTC(CString original)
	{
		// Should also address dates such as: L"2016-03-01 12:29:37"
		CString converted = STR_NULL;
		int Year, Month, Day, Hour, Minute;
		original.Remove(CHR_MINUS);
		original.Remove(CHR_SPACE);
		original.Remove(CHR_COLON);


		if (original != STR_NULL && original.GetLength() >= 12)
		{
			Year = _wtol(original.Left(4));
			if (Year <= 1900)
			{
				utils::WriteStatus( L"Internal error: Year of %s is %d", original, Year);
				return DEFAULT_EARLIEST_DATE;
			}
			Month = _wtol(original.Mid(4, 2));
			Day = _wtol(original.Mid(6, 2));
			Hour = _wtol(original.Mid(8, 2));
			Minute = _wtol(original.Mid(10, 2));
			CTime result(Year, Month, Day, Hour, Minute, 0);
			converted = Time2UTC(result).Format(SG_DATE_FORMAT);
		}
		return converted;
	}
	CTime Time2UTC(CTime original)
	{
		CString Formatted = original.FormatGmt(SG_DATE_FORMAT);
		int Year, Month, Day, Hour, Minute;

		if (Formatted != STR_NULL && Formatted.GetLength() >= 12)
		{
			Year = _wtol(Formatted.Left(4));
			Month = _wtol(Formatted.Mid(4, 2));
			Day = _wtol(Formatted.Mid(6, 2));
			Hour = _wtol(Formatted.Mid(8, 2));
			Minute = _wtol(Formatted.Mid(10, 2));
			CTime result(Year, Month, Day, Hour, Minute, 0);
			return result;
		}
		else
		{
			return (CTime)NULL;
		}
	}

	// This time formating is copied from MSDN.
	// Localized time string could be better. `wever, this output seems clear enough.
	void TimeToLongLong(FILETIME *time, ULONGLONG *result)
	{
		SYSTEMTIME sysTime, localTime;
		FileTimeToSystemTime(time, &sysTime);
		SystemTimeToTzSpecificLocalTime(NULL, &sysTime, &localTime);

		*result = localTime.wYear * 31536000ULL +
			localTime.wMonth * 1036800ULL +
			localTime.wDay * 86400ULL +
			localTime.wHour * 3600ULL +
			localTime.wMinute * 60ULL +
			localTime.wSecond;
	}

	// This time formating is copied from MSDN.
	// Localized time string could be better. However, this output seems clear enough.
	void TimeToStringW(FILETIME *time, WCHAR *buffer, size_t length)
	{
		SYSTEMTIME sysTime, localTime;
		FileTimeToSystemTime(time, &sysTime);
		if (SystemTimeToTzSpecificLocalTime(NULL, &sysTime, &localTime))
		{
			swprintf(buffer, length, L"%04d%02d%02d%02d%02d%02d",
				localTime.wYear, localTime.wMonth, localTime.wDay,
				localTime.wHour, localTime.wMinute, localTime.wSecond);
		}
		else
		{
			swprintf(buffer, length, STR_0); // Sergio - in case of SystemTimeToTzSpecificLocalTime fails
		}
	}

	void WriteStatus(LPCTSTR lpText, ...)
	{
		FILE *fp;
		CString sMsg;
		CString sLine;
		va_list ptr;
		va_start(ptr, lpText);
		sMsg.FormatV(lpText, ptr);

		sLine.Format(L"%s", (LPCTSTR)sMsg);
		_wfopen_s(&fp, utilsLogFilename, L"a");
		if (fp)
		{
			fwprintf(fp, L"%s", sLine);
			fclose(fp);
		}
	}

	/* returns 0 on failure (invalid b64 input characters)
	* 1 on success
	*/
	int decode_b64(const char *src, unsigned char *dest,
		size_t len, size_t *decoded)
	{
		unsigned char buf[3];

		if (len % 4)
			return 0;

		while (len) 
		{
			int decode_count = decode_4chars(src, buf);
			if (decode_count < 1)
				return 0;
			SF_MEMCPY(dest, buf, decode_count);
			src += 4;
			dest += decode_count;
			len -= 4;
			*decoded += decode_count;
		}

		return 1;
	}
	static int decode_4chars(const char *src, unsigned char *dest)
	{
		unsigned char index_array[4];
		int i, count = 0;

		for (i = 0; i < 4; ++i) 
		{
			unsigned char conv;

			if (!conv_letter(src[i], &conv))
				return 0;
			//if (conv == 0)
			//	--count;
			index_array[i] = conv;
		}

		dest[0] = (index_array[0] << 2) | (index_array[1] >> 4);
		dest[1] = (index_array[1] << 4) | (index_array[2] >> 2);
		dest[2] = (index_array[2] << 6) | index_array[3];

		count += 3;
		return count;
	}


}
