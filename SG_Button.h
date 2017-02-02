// Chrome Credentials Viewer
// By Michael Haephrati
// ©2014-2017 Secured Globe, Inc.

#ifndef _ST_BTN_H
#define _ST_BTN_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Return values
#ifndef	ST_BTN_OK
#define	ST_BTN_OK						0
#endif
#ifndef	ST_BTN_INVALIDRESOURCE
#define	ST_BTN_INVALIDRESOURCE			1
#endif
#ifndef	ST_BTN_FAILEDMASK
#define	ST_BTN_FAILEDMASK				2
#endif
#ifndef	ST_BTN_INVALIDINDEX
#define	ST_BTN_INVALIDINDEX				3
#endif
#ifndef	ST_BTN_INVALIDALIGN
#define	ST_BTN_INVALIDALIGN				4
#endif
#ifndef	ST_BTN_BADPARAM
#define	ST_BTN_BADPARAM					5
#endif

// Dummy identifier for grayscale icon
#ifndef	ST_BTN_AUTO_GRAY
#define	ST_BTN_AUTO_GRAY					(HICON)(0xffffffff - 1L)
#endif

class SG_Button : public CButton
{
public:
    SG_Button();
	~SG_Button();

    enum	{	ST_ALIGN_HORIZ	= 0,			// Icon/bitmap on the left, text on the right
				ST_ALIGN_VERT,					// Icon/bitmap on the top, text on the bottom
				ST_ALIGN_HORIZ_RIGHT			// Icon/bitmap on the right, text on the left
			};

	enum	{	ST_BTN_COLOR_BK_IN	= 0,		// Background color when mouse is INside
				ST_BTN_COLOR_FG_IN,				// Text color when mouse is INside
				ST_BTN_COLOR_BK_OUT,				// Background color when mouse is OUTside
				ST_BTN_COLOR_FG_OUT,				// Text color when mouse is OUTside
				ST_BTN_COLOR_BK_FOCUS,			// Background color when the button is focused
				ST_BTN_COLOR_FG_FOCUS,			// Text color when the button is focused

				ST_BTN_MAX_COLORS
			};

	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(SG_Button)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	DWORD SetDefaultColors(BOOL bRepaint = TRUE);
	DWORD SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint = TRUE);
	DWORD GetColor(BYTE byColorIndex, COLORREF* crpColor);
	DWORD OffsetColor(BYTE byColorIndex, short shOffset, BOOL bRepaint = TRUE);

	DWORD SetCheck(int nCheck, BOOL bRepaint = TRUE);
	int GetCheck();

	DWORD SetURL(LPCTSTR lpszURL = NULL);
	void DrawTransparent(BOOL bRepaint = FALSE);

	BOOL GetDefault();
	DWORD SetAlwaysTrack(BOOL bAlwaysTrack = TRUE);

	void SetTooltipText(int nText, BOOL bActivate = TRUE);
	void SetTooltipText(LPCTSTR lpszText, BOOL bActivate = TRUE);
	void ActivateTooltip(BOOL bEnable = TRUE);

	DWORD SetBtnCursor(int nCursorId = NULL, BOOL bRepaint = TRUE);

	DWORD SetFlat(BOOL bFlat = TRUE, BOOL bRepaint = TRUE);
	DWORD SetAlign(BYTE byAlign, BOOL bRepaint = TRUE);

	DWORD DrawBorder(BOOL bDrawBorder = TRUE, BOOL bRepaint = TRUE);
	DWORD DrawFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint = TRUE);

	DWORD SetIcon(int nIconIn, int nIconOut = NULL);
	DWORD SetIcon(HICON hIconIn, HICON hIconOut = NULL);

	DWORD SetBitmaps(int nBitmapIn, COLORREF crTransColorIn, int nBitmapOut = NULL, COLORREF crTransColorOut = 0);
	DWORD SetBitmaps(HBITMAP hBitmapIn, COLORREF crTransColorIn, HBITMAP hBitmapOut = NULL, COLORREF crTransColorOut = 0);

#ifdef	ST_BTN_USE_BCMENU
	DWORD SetMenu(UINT nMenu, HWND hParentWnd, BOOL bWinXPStyle = TRUE, UINT nToolbarID = NULL, CSize sizeToolbarIcon = CSize(16, 16), COLORREF crToolbarBk = RGB(255, 0, 255), BOOL bRepaint = TRUE);
#else
	DWORD SetMenu(UINT nMenu, HWND hParentWnd, BOOL bRepaint = TRUE);
#endif

	static short GetVersionI()		{return 35;}
	static LPCTSTR GetVersionC()	{return (LPCTSTR)_T("3.5");}

	BOOL	m_bShowDisabledBitmap;

protected:
    //{{AFX_MSG(SG_Button)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnClicked();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnCancelMode();
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

#ifdef	ST_BTN_USE_BCMENU
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
#endif

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	HICON CreateGrayscaleIcon(HICON hIcon);
	virtual DWORD OnDrawBackground(CDC* pDC, LPCRECT pRect);
	virtual DWORD OnDrawBorder(CDC* pDC, LPCRECT pRect);

	BOOL		m_bIsFlat;			// Is a flat button?
	BOOL		m_bMouseOnButton;	// Is mouse over the button?
	BOOL		m_bDrawTransparent;	// Draw transparent?
	BOOL		m_bIsPressed;		// Is button pressed?
	BOOL		m_bIsFocused;		// Is button focused?
	BOOL		m_bIsDisabled;		// Is button disabled?
	BOOL		m_bIsDefault;		// Is default button?
	BOOL		m_bIsCheckBox;		// Is the button a checkbox?
	BYTE		m_byAlign;			// Align mode
	BOOL		m_bDrawBorder;		// Draw border?
	BOOL		m_bDrawFlatFocus;	// Draw focus rectangle for flat button?
	COLORREF	m_crColors[ST_BTN_MAX_COLORS];	// Colors to be used
	HWND		m_hParentWndMenu;	// Handle to window for menu selection
	BOOL		m_bMenuDisplayed;	// Is menu displayed ?

#ifdef	ST_BTN_USE_BCMENU
	BCMenu		m_menuPopup;		// BCMenu class instance
#else
	HMENU		m_hMenu;			// Handle to associated menu
#endif

private:
	LRESULT OnSetCheck(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetCheck(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	void CancelHover();
	void FreeResources(BOOL bCheckForNULL = TRUE);
	void PrepareImageRect(BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, BOOL bIsPressed, DWORD dwWidth, DWORD dwHeight, CRect* rpImage);
	HBITMAP CreateBitmapMask(HBITMAP hSourceBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTransColor);
	void DrawTheIcon(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, BOOL bIsPressed, BOOL bIsDisabled);
	void DrawTheBitmap(CDC* pDC, BOOL bHasTitle, RECT *rItem, CRect *rCaption, BOOL bIsPressed, BOOL bIsDisabled);
	void PaintBk(CDC* pDC);

	void InitToolTip();

	HCURSOR		m_hCursor;			// Handle to cursor
	CToolTipCtrl m_ToolTip;			// Tooltip

	CDC			m_dcBk;
	CBitmap		m_bmpBk;
	CBitmap*	m_pbmpOldBk;

	BOOL		m_bAlwaysTrack;		// Always hilight button?
	int			m_nCheck;			// Current value for checkbox
	UINT		m_nTypeStyle;		// Button style

	TCHAR		m_szURL[_MAX_PATH];	// URL to open when clicked

#pragma pack(1)
	typedef struct _STRUCT_ICONS
	{
		HICON		hIcon;			// Handle to icon
		DWORD		dwWidth;		// Width of icon
		DWORD		dwHeight;		// Height of icon
	} STRUCT_ICONS;
#pragma pack()

#pragma pack(1)
	typedef struct _STRUCT_BITMAPS
	{
		HBITMAP		hBitmap;		// Handle to bitmap
		DWORD		dwWidth;		// Width of bitmap
		DWORD		dwHeight;		// Height of bitmap
		HBITMAP		hMask;			// Handle to mask bitmap
		COLORREF	crTransparent;	// Transparent color
	} STRUCT_BITMAPS;
#pragma pack()

	STRUCT_ICONS	m_csIcons[2];
	STRUCT_BITMAPS	m_csBitmaps[2];

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif
