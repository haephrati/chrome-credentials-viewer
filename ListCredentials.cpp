// Chrome Credentials Viewer
// By Michael Haephrati
// ©2014-2017 Secured Globe, Inc.

#include "stdafx.h"
#include "CredentialsViewerTool.h"
#include "ListCredentials.h"
#include "global.h"
#include "afxdialogex.h"
#include "CredentialsViewerToolDlg.h"

// CListCredentials dialog

IMPLEMENT_DYNAMIC(CListCredentials, CTabPageSSL)



CListCredentials::CListCredentials(CWnd* pParent /*=NULL*/)
: CTabPageSSL(CListCredentials::IDD, pParent)
{
	m_pParent = (CDialogEx *) pParent;
	DECLARE_EASYSIZE
}

CListCredentials::~CListCredentials()
{
}

void CListCredentials::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CREDENTIALS_LIST, m_CredentialsListCtrl);
}

void CListCredentials::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	static int nSortColumn = -1;
	static BOOL bSortAscending = TRUE;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int iColumn = pNMListView->iSubItem;
	if (iColumn == nSortColumn)
	{
		bSortAscending = !bSortAscending;
	}
	else
	{
		bSortAscending = TRUE;
	}
	nSortColumn = iColumn;
	int nItems = m_CredentialsListCtrl.GetItemCount();
	for (int item = 0; item < nItems; item++)
	{
		CString text = m_CredentialsListCtrl.GetItemText(item, iColumn);
		int value = _wtol(text);  // numeric sort
		if (!bSortAscending)
		{
			value = -value;
		}
		m_CredentialsListCtrl.SetItemData(item, value);
	}
	LPARAM lParamSort = 0;
}


int CALLBACK CListCredentials::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if (lParam1 < lParam2) { return -1; }
	if (lParam1 > lParam2) { return  1; }
	return  0;
}

void CListCredentials::SetTilesViewLinesCount(int nCount)
{
	LVTILEVIEWINFO lvtvwi = { 0 };
	lvtvwi.cbSize = sizeof(LVTILEVIEWINFO);
	lvtvwi.dwMask = LVTVIM_COLUMNS;
	lvtvwi.cLines = nCount;

	m_CredentialsListCtrl.SetTileViewInfo(&lvtvwi);
}

void CListCredentials::SetTilesViewTileFixedSize()
{
	LVTILEVIEWINFO lvtvwi = { 0 };
	lvtvwi.cbSize = sizeof(LVTILEVIEWINFO);
	lvtvwi.dwMask = LVTVIM_TILESIZE;

	lvtvwi.dwFlags = LVTVIF_FIXEDSIZE;
	lvtvwi.sizeTile.cx = 300;
	lvtvwi.sizeTile.cy = 150;

	m_CredentialsListCtrl.SetTileViewInfo(&lvtvwi);
}

void CListCredentials::SetItemTileLines(int nItem, UINT* parrColumns, UINT nCount)
{

	LVTILEINFO lvti = { 0 };
	lvti.cbSize = sizeof(LVTILEINFO);
	lvti.cColumns = nCount;
	lvti.iItem = nItem;
	lvti.puColumns = parrColumns;

	m_CredentialsListCtrl.SetTileInfo(&lvti);
}



void CListCredentials::InitCredentialsControl()
{
	CWinApp* pApp = AfxGetApp();
	COLORREF m_crText = 0;


	// Set up your transparent color as appropriate
	COLORREF crMask = RGB(0, 255, 0);

	CBitmap hBmp1;
	BITMAP temp;
	hBmp1.LoadBitmap(IDB_CHROME);

	m_imageList.Create(24, 24, ILC_MASK | ILC_COLOR32, 4, 4);
	m_imageList.Add(&hBmp1, crMask);

	hBmp1.Detach();

	m_CredentialsListCtrl.SetImageList(&m_imageList, LVSIL_SMALL);


	//set list style     
	m_CredentialsListCtrl.SetExtendedStyle(m_CredentialsListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CRect rect;
	// get list size   
	m_CredentialsListCtrl.GetClientRect(&rect);
	//
	 
	m_CredentialsListCtrl.InsertColumn(0, LABEL_BROWSER_NAME, LVCFMT_LEFT, 100);
	m_CredentialsListCtrl.InsertColumn(1, LABEL_ENTITY_NAME, LVCFMT_LEFT, 150);
	m_CredentialsListCtrl.InsertColumn(2, LABEL_USER, LVCFMT_LEFT, 100);
	m_CredentialsListCtrl.InsertColumn(3, LABEL_PASS, LVCFMT_LEFT, 100);
	m_CredentialsListCtrl.InsertColumn(4, LABEL_DATE, LVCFMT_LEFT, rect.Width() - 350);

	m_imageList.Detach();  
	m_CredentialsListCtrl.SetRedraw(true);



}
BEGIN_MESSAGE_MAP(CListCredentials, CDialogEx)
	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_CREDENTIALS_LIST, &CListCredentials::OnNMCustomdrawGrid)
	ON_WM_SIZING()
END_MESSAGE_MAP()
BEGIN_EASYSIZE_MAP(CListCredentials)
END_EASYSIZE_MAP

// CListCredentials message handlers


void CListCredentials::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CTabPageSSL::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
}


BOOL CListCredentials::OnInitDialog()
{
	CTabPageSSL::OnInitDialog();
	INIT_EASYSIZE;

	// TODO:  Add extra initialization here
	

	InitCredentialsControl();
	m_font.CreateFont(16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH&FF_SWISS, _T("Arial"));
	//m_brush.CreateSolidBrush(RGB(255, 255, 255));
	m_brush.CreateSolidBrush(RGB(209, 224, 241));
	m_bkBrush.CreateSolidBrush(RGB(209, 224, 241));
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}




void CListCredentials::OnPaint()
{
	CPoint ptLocation(0, 0);
	CDC* pDC = NULL;

	CPaintDC dc(this); // device context for painting

	pDC = GetDC();

}


HBRUSH CListCredentials::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTabPageSSL::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd == this)
	{
		return m_bkBrush;
	}
	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_NOTE:
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(BG_COLOR_RGB);
			pDC->SelectObject(&m_font);
			return m_brush;

		}
		break;
		default:
			return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	pWnd->SetFont(&m_font); 
	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CListCredentials::OnSize(UINT nType, int cx, int cy)
{
	CTabPageSSL::OnSize(nType, cx, cy);
	UPDATE_EASYSIZE;
	if (m_CredentialsListCtrl.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);
		rect.top = rect.top + 30;
		rect.bottom = rect.bottom - 30;
		m_CredentialsListCtrl.MoveWindow(rect);
		m_CredentialsListCtrl.SetColumnWidth(3, rect.Width() - 505);
	}
}


void CListCredentials::OnNMCustomdrawGrid(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	COLORREF clrNewBkColor;

	*pResult = CDRF_DODEFAULT;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{

		COLORREF clrNewTextColor, clrNewBkColor;

		int    nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);

		//Set background color
		if (nItem % 2 == 0)
		{
			clrNewBkColor = RGB(255, 255, 255); //
		}
		else
		{
			clrNewBkColor = RGB(240, 240, 240); //
		}
		pLVCD->clrTextBk = clrNewBkColor;

	}
}
void CListCredentials::RemoveAll()
{
	m_CredentialsListCtrl.DeleteAllItems();
}
void CListCredentials::AddResult(int Browser, LPWSTR Site, LPWSTR User, LPWSTR Pass,LPWSTR Date)
{
	int nRow;

	nRow = m_CredentialsListCtrl.InsertItem(0, BROWSER_NAME_CHROME, Browser);
	m_CredentialsListCtrl.SetItemText(nRow, 1, Site);
	m_CredentialsListCtrl.SetItemText(nRow, 2, User);
	m_CredentialsListCtrl.SetItemText(nRow, 3, Pass);
	m_CredentialsListCtrl.SetItemText(nRow, 4, Date);

}


void CListCredentials::OnSizing(UINT fwSide, LPRECT pRect)
{
	CTabPageSSL::OnSizing(fwSide, pRect);

}


void CListCredentials::OnBnClickedScan()
{
	// TODO: Add your control notification handler code here
}
