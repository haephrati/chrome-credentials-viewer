#include "stdafx.h"
#include "SG_ProgramSettings.h"


SG_ProgramSettings::SG_ProgramSettings()
{
}


SG_ProgramSettings::~SG_ProgramSettings()
{
}
void SG_ProgramSettings::SetDefaultValues()
{
	m_Settings.Chrome_On = true;
	m_Settings.FF_On = true;
	m_Settings.IE_On = true;
}
bool SG_ProgramSettings::LoadSettings()
{
	bool result = false;
	CWinApp* pApp = AfxGetApp();
	if (pApp)
	{
		CString DoChrome, DoIE, DoFF, Master;
		DoChrome = pApp->GetProfileString(L"WindowsCredentialsViewer", L"DoChrome");
		DoIE = pApp->GetProfileString(L"WindowsCredentialsViewer", L"DoIE");
		DoFF = pApp->GetProfileString(L"WindowsCredentialsViewer", L"DoFF");
		if (DoChrome != L"" && DoIE != L"" && DoFF != L"")
		{
			SetChromeState(DoChrome == L"1");
			SetIEState(DoIE == L"1");
			SetFFState(DoFF == L"1");
			result = true;
		}
	}
	if (result == false)	// No stored settings were found
	{
		SetDefaultValues();
	}
	return result;
}
void SG_ProgramSettings::SaveSettings()
{
	CWinApp* pApp = AfxGetApp();
	if (pApp)
	{
		pApp->WriteProfileString(L"WindowsCredentialsViewer", L"DoChrome", (GetChromeState()) ? L"1" : L"0");
		pApp->WriteProfileString(L"WindowsCredentialsViewer", L"DoIE", (GetIEState()) ? L"1" : L"0");
		pApp->WriteProfileString(L"WindowsCredentialsViewer", L"DoFF", (GetFFState()) ? L"1" : L"0");
	}
}
void SG_ProgramSettings::SetChromeState(bool state)
{
	m_Settings.Chrome_On = state;
}
void SG_ProgramSettings::SetIEState(bool state)
{
	m_Settings.IE_On = state;

}
void SG_ProgramSettings::SetFFState(bool state)
{
	m_Settings.FF_On = state;
}
bool SG_ProgramSettings::GetChromeState()
{
	return(m_Settings.Chrome_On);

}
bool SG_ProgramSettings::GetIEState()
{
	return(m_Settings.IE_On);

}
bool SG_ProgramSettings::GetFFState()
{
	return(m_Settings.FF_On);
}