#pragma once
typedef struct
{
	bool Chrome_On;
	bool IE_On;
	bool FF_On;
	wchar_t FF_Master[80];
} CVSettings;
class SG_ProgramSettings
{
public:
	SG_ProgramSettings();
	~SG_ProgramSettings();
	void SetDefaultValues();
	bool LoadSettings();
	void SaveSettings();
	void SetChromeState(bool state);
	void SetIEState(bool state);
	void SetFFState(bool state);
	bool GetChromeState();
	bool GetIEState();
	bool GetFFState();
private:
	CVSettings m_Settings;
};

