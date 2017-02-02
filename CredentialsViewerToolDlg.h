
// CredentialsViewerToolDlg.h : header file
//

#pragma once

#include "TabCtrlSSL.h" // for tabcontrol
#include "afxcmn.h"
#include "afxwin.h"
#include "SG_Button.h"
#include "easysize.h"
#include "ListCredentialsDlg.h"
#include "listCredentials.h"
#include "SG_Browsers_Credentials.h"
#define BG_COLOR_RGB RGB(3, 73, 117) 
#define FG_COLOR_RGB RGB(255,255,255)
#define LABEL_BROWSER_NAME _T("Browser")
#define LABEL_ENTITY_NAME _T("Site")
#define LABEL_USER _T("User")
#define LABEL_PASS _T("Password")
#define LABEL_DATE _T("Date")
#define MY_BORDER_SIZE 3
#define BROWSER_NAME_CHROME L"Chrome"

// CChromeCredentialsToolDlg dialog8
class CChromeCredentialsToolDlg : public CDialogEx
{
	DECLARE_EASYSIZE
// Construction
public:
	CChromeCredentialsToolDlg(CWnd* pParent = NULL);	// standard constructor
	~CChromeCredentialsToolDlg();

// Dialog Data
	enum { IDD = IDD_CREDENTIALSTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	void ActivateCredentialsTab();

public:
	CListCredentials *m_pListCredentialsDlg;

	CTabCtrlSSL m_tabCtrlExt;

	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonMaxmize();
	afx_msg void OnBnClickedButtonMinimize();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

	CBrush		m_bkBrush;
	CFont		m_font;

	SG_Button m_btnMinimize;
	SG_Button m_btnMaxmize;
	SG_Button m_btnClose;
	SG_Button m_Settings;

	BOOL m_bMaxmizeFlag;
	CRect m_rectDlg;


	BOOL CreateMainInterfaceObject();
	BOOL CreatDisplayDlg();
	BOOL InitMainView();
	void SelShowWindow(int n);
	void DestoryDisplayDlg();
	void MoveControler(int left, int top, int right, int bottom, UINT nID);

	afx_msg void OnTcnSelchangeCredentialsTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSettings();
	void DisplayCredentials();

};
int ProcessChromeCredentials(SGBrowserCredentialsArray *credentials);
