// Chrome Credentials Viewer
// By Michael Haephrati
// ©2014-2017 Secured Globe, Inc.

#include "stdafx.h"
#include "CredentialsViewerTool.h"
#include "CredentialsViewerToolDlg.h"
#include "ListCredentialsDlg.h"
#include "SG_Browsers_Credentials.h"
#include "chrome_secrets.h"
#include "Utils.h"
#include "afxdialogex.h"
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "Crypt32.lib")
#include "SG_Helper_base64.h"



//  a working pre-multiply function. Note that this was taken from http://www.viksoe.dk/code/alphatut1.htm .
inline void PremultiplyBitmapAlpha(HDC hDC, HBITMAP hBmp)
{
	BITMAP bm = { 0 };
	GetObject(hBmp, sizeof(bm), &bm);
	BITMAPINFO* bmi = (BITMAPINFO*)_alloca(sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD)));
	::ZeroMemory(bmi, sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD)));
	bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BOOL bRes = ::GetDIBits(hDC, hBmp, 0, bm.bmHeight, NULL, bmi, DIB_RGB_COLORS);
	if (!bRes || bmi->bmiHeader.biBitCount != 32) return;
	LPBYTE pBitData = (LPBYTE) ::LocalAlloc(LPTR, bm.bmWidth * bm.bmHeight * sizeof(DWORD));
	if (pBitData == NULL) return;
	LPBYTE pData = pBitData;
	::GetDIBits(hDC, hBmp, 0, bm.bmHeight, pData, bmi, DIB_RGB_COLORS);
	for (int y = 0; y < bm.bmHeight; y++) 
	{
		for (int x = 0; x < bm.bmWidth; x++) 
		{
			pData[0] = (BYTE)((DWORD)pData[0] * pData[3] / 255);
			pData[1] = (BYTE)((DWORD)pData[1] * pData[3] / 255);
			pData[2] = (BYTE)((DWORD)pData[2] * pData[3] / 255);
			pData += 4;
		}
	}
	::SetDIBits(hDC, hBmp, 0, bm.bmHeight, pBitData, bmi, DIB_RGB_COLORS);
	::LocalFree(pBitData);
	_freea(bmi);
}
// CChromeCredentialsToolDlg dialog


CChromeCredentialsToolDlg::~CChromeCredentialsToolDlg()
{
	DestoryDisplayDlg();
	delete m_pListCredentialsDlg;
}
CChromeCredentialsToolDlg::CChromeCredentialsToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChromeCredentialsToolDlg::IDD, pParent)
{
	m_pListCredentialsDlg = NULL;

	EnableActiveAccessibility();
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_SG_LOGO);
	m_bMaxmizeFlag = FALSE;
	FILE *fp;
	_wfopen_s(&fp, utilsLogFilename, L"w");	// Create a new output file
	fclose(fp);

}

void CChromeCredentialsToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CREDENTIALS_TAB, m_tabCtrlExt);
	DDX_Control(pDX, IDC_BUTTON_MINIMIZE, m_btnMinimize);
	DDX_Control(pDX, IDC_BUTTON_MAXMIZE, m_btnMaxmize);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btnClose);

}

BEGIN_MESSAGE_MAP(CChromeCredentialsToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CChromeCredentialsToolDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_MAXMIZE, &CChromeCredentialsToolDlg::OnBnClickedButtonMaxmize)
	ON_BN_CLICKED(IDC_BUTTON_MINIMIZE, &CChromeCredentialsToolDlg::OnBnClickedButtonMinimize)

	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CChromeCredentialsToolDlg)
	EASYSIZE(IDC_BUTTON_MINIMIZE, ES_KEEPSIZE, ES_BORDER, ES_BORDER, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_BUTTON_MAXMIZE, ES_KEEPSIZE, ES_BORDER, ES_BORDER, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_BUTTON_CLOSE, ES_KEEPSIZE, ES_BORDER, ES_BORDER, ES_KEEPSIZE, 0)

	EASYSIZE(IDC_CREDENTIALS_TAB, ES_BORDER, ES_BORDER, ES_BORDER, ES_BORDER, 0)
	EASYSIZE(IDC_COPYRIGHT, ES_KEEPSIZE, ES_KEEPSIZE, ES_BORDER, ES_BORDER, 0)
	EASYSIZE(IDC_NOTE2, ES_BORDER, ES_KEEPSIZE, ES_BORDER, ES_BORDER, 0)
	EASYSIZE(IDC_WEBSITE, ES_KEEPSIZE, ES_KEEPSIZE, ES_BORDER, ES_BORDER, 0) 

END_EASYSIZE_MAP



// CChromeCredentialsToolDlg message handlers
int compare(const void *arg1, const void *arg2)
{
	SGBrowserCredentials *element1 = (SGBrowserCredentials *)(arg1);
	SGBrowserCredentials *element2 = (SGBrowserCredentials *)(arg2);
	return (element1->DateCreated < element2->DateCreated);

}

void CChromeCredentialsToolDlg::DisplayCredentials()
{
	m_pListCredentialsDlg->RemoveAll();
	SGBrowserCredentialsArray credentials;
	ProcessChromeCredentials(&credentials);
	// Sort the credentials by their date/time stamp
	qsort(credentials.GetData(), credentials.GetSize(), sizeof(SGBrowserCredentials), compare);
	for (int i = 0; i < credentials.GetSize(); i++)
		m_pListCredentialsDlg->AddResult(credentials[i].Browser, credentials[i].Site, credentials[i].UserName, credentials[i].Password, credentials[i].DateCreated.FormatGmt(SG_FRIEDLY_DATEFORMAT).GetBuffer());

}
BOOL CChromeCredentialsToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	INIT_EASYSIZE;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Display the title + icon
	CImage m_Image;
	m_Image.LoadFromResource(::AfxGetResourceHandle(), IDB_TITLE);
	
	// TODO: Add extra initialization here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	HBITMAP   hBitmap;
	CStatic   *pStatic = (CStatic*)GetDlgItem(IDC_STATIC_PIC);
	hBitmap = (HBITMAP)LoadImage(
		AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDB_TITLE),
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADMAP3DCOLORS);
	pStatic->ModifyStyle(0xF, SS_BITMAP);  
	pStatic->SetBitmap(hBitmap);


	m_bkBrush.CreateSolidBrush(BG_COLOR_RGB);

	m_btnMinimize.SetBitmaps(IDB_MINIMIZE2, BG_COLOR_RGB, IDB_MINIMIZE1, BG_COLOR_RGB);
	m_btnMinimize.DrawTransparent(true);
	m_btnMinimize.OffsetColor(SG_Button::ST_BTN_COLOR_BK_IN, 30);

	m_btnClose.SetBitmaps(IDB_CLOSE2, BG_COLOR_RGB, IDB_CLOSE1, BG_COLOR_RGB);
	m_btnClose.DrawTransparent(true);
	m_btnClose.OffsetColor(SG_Button::ST_BTN_COLOR_BK_IN, 30);

	m_btnMaxmize.SetBitmaps(IDB_MAXMIZE2, BG_COLOR_RGB, IDB_MAXMIZE1, BG_COLOR_RGB);
	m_btnMaxmize.DrawTransparent(true);
	m_btnMaxmize.OffsetColor(SG_Button::ST_BTN_COLOR_BK_IN, 30);



	if (!InitMainView())
	{
		MessageBox(_T("Initialize the main view failure!"));
	}
	DisplayCredentials();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChromeCredentialsToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChromeCredentialsToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChromeCredentialsToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}











HBRUSH CChromeCredentialsToolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd == this)
	{
		return m_bkBrush;
	}
	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_STATIC_PIC:
		{

			CImage m_Image;
			m_Image.LoadFromResource(::AfxGetResourceHandle(), IDB_TITLE);

			CPaintDC dc(this);

			CRect rect(0, 0, 16, 16);

			static bool pmdone = false;
			if (!pmdone)
			{
				PremultiplyBitmapAlpha(dc, m_Image);
				pmdone = true;
			}

			BLENDFUNCTION bf;
			bf.BlendOp = AC_SRC_OVER;
			bf.BlendFlags = 0;
			bf.SourceConstantAlpha = 255;
			bf.AlphaFormat = AC_SRC_ALPHA;

			HDC src_dc = m_Image.GetDC();
			::AlphaBlend(dc, rect.left, rect.top, 16, 16, src_dc, 0, 0, 16, 16, bf);
			m_Image.ReleaseDC();
			break;
		}
		case IDC_WEBSITE:
		case IDC_NOTE:
		case IDC_NOTE2:
		case IDC_COPYRIGHT:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(BG_COLOR_RGB);
			pDC->SetTextColor(FG_COLOR_RGB);
			pDC->SelectObject(&m_font);
			return m_bkBrush;
		}
		break;
	}

	return hbr;

}


void CChromeCredentialsToolDlg::OnBnClickedButtonClose()
{
	OnOK();
}


void CChromeCredentialsToolDlg::OnBnClickedButtonMaxmize()
{
	if (m_bMaxmizeFlag)
	{
		MoveWindow(m_rectDlg);
		CRect rt;
		m_tabCtrlExt.GetClientRect(&rt);
		m_pListCredentialsDlg->MoveWindow(&rt);
		ShowWindow(SW_SHOW);
		m_bMaxmizeFlag = false;
		m_btnMaxmize.SetBitmaps(IDB_MAXMIZE2, BG_COLOR_RGB, IDB_MAXMIZE1, BG_COLOR_RGB);

	}
	else
	{

		::GetWindowRect(m_hWnd, m_rectDlg);
		int nWidth = GetSystemMetrics(SM_CXSCREEN);
		int nHeight = GetSystemMetrics(SM_CYSCREEN);

		MoveWindow(0, 0, nWidth, nHeight);
		CRect rt;
		m_tabCtrlExt.GetClientRect(&rt);
		m_pListCredentialsDlg->MoveWindow(&rt);
		m_bMaxmizeFlag = true;

		m_btnMaxmize.SetBitmaps(IDB_ORIGINAL2, BG_COLOR_RGB, IDB_ORIGINAL1, BG_COLOR_RGB);

	}

}


void CChromeCredentialsToolDlg::OnBnClickedButtonMinimize()
{
	theApp.GetMainWnd()->ShowWindow(SW_MINIMIZE);
}


LRESULT CChromeCredentialsToolDlg::OnNcHitTest(CPoint point)
{

	LRESULT ret = CDialogEx::OnNcHitTest(point);
	return (ret == HTCLIENT) ? HTCAPTION : ret;

}


void CChromeCredentialsToolDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	CDialogEx::OnNcLButtonDown(nHitTest, point);
}


BOOL CChromeCredentialsToolDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CChromeCredentialsToolDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	UPDATE_EASYSIZE;

}


void CChromeCredentialsToolDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(0, 0, fwSide, pRect);
	CRect rt;
	m_tabCtrlExt.GetClientRect(&rt);
	m_pListCredentialsDlg->MoveWindow(&rt);
}
BOOL CChromeCredentialsToolDlg::CreateMainInterfaceObject()
{
	BOOL bRet = TRUE;
	m_pListCredentialsDlg = new CListCredentials();

	return bRet;
}
BOOL CChromeCredentialsToolDlg::CreatDisplayDlg()
{
	bool bRet = true;
	HWND hWnd = GetSafeHwnd();

	//create the monitor dialog
	if (m_pListCredentialsDlg != NULL && m_pListCredentialsDlg->Create(IDD_LIST_CREDENTIALS_DLG, GetDlgItem(IDC_CREDENTIALS_TAB)))
	{
		bRet = true;
	}
	else
	{
		return false;
	}

	return bRet;
}
BOOL CChromeCredentialsToolDlg::InitMainView()
{
	BOOL bRet = TRUE;
	CreatDisplayDlg();
	CRect rt;
	GetClientRect(&rt);
	m_tabCtrlExt.GetClientRect(&rt);
	m_pListCredentialsDlg->MoveWindow(&rt);

	m_tabCtrlExt.ShowWindow(SW_SHOW);
	SelShowWindow(0);

	return bRet;
}
void CChromeCredentialsToolDlg::SelShowWindow(int n)
{
	switch (n)
	{
	case 0:  // show the main start interface
		m_pListCredentialsDlg->ShowWindow(TRUE);
		break;

	default:
		break;
	}
}
void CChromeCredentialsToolDlg::DestoryDisplayDlg()
{
	if (m_pListCredentialsDlg != NULL)
	{
		m_pListCredentialsDlg->DestroyWindow();
		delete m_pListCredentialsDlg;
		m_pListCredentialsDlg = NULL;
	}
	return;
}
void CChromeCredentialsToolDlg::MoveControler(int left, int top, int right, int bottom, UINT nID)
{
	CRect rt;
	GetClientRect(&rt);
	rt.top = top;
	rt.left = left;
	rt.right = right;
	rt.bottom = bottom;
	GetDlgItem(nID)->MoveWindow(&rt);
}





int ProcessChromeCredentials(SGBrowserCredentialsArray *credentials)
{
	SG_GetChromeCredentials(credentials);
	return credentials->GetSize();
}




