#pragma once

#include "stdafx.h"
#include "TabPageSSL.h"
#include "afxcmn.h"
#include "easysize.h"
#include "resource.h"
// CListCredentials dialog

class CListCredentials : public CTabPageSSL
{
	DECLARE_EASYSIZE
	DECLARE_DYNAMIC(CListCredentials)

public:
	CListCredentials(CWnd* pParent = NULL);   // standard constructor
	virtual ~CListCredentials();

// Dialog Data
	enum { IDD = IDD_LIST_CREDENTIALS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_CredentialsListCtrl;
	CImageList m_imageList;			// Image list for the browser type (chrome, ie, firefox)
	CDialogEx *m_pParent;
	CBrush m_brush;
	CFont m_font;
	CBrush  m_bkBrush;

private:
	void InitCredentialsControl();
	void SetTilesViewLinesCount(int nCount);
	void SetTilesViewTileFixedSize();
	void SetItemTileLines(int nItem, UINT* parrColumns, UINT nCount);
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual BOOL OnInitDialog();
	void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);


	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMCustomdrawGrid(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnBnClickedScan();
	void AddResult(int Browser, LPWSTR Site, LPWSTR User, LPWSTR Pass, LPWSTR Date);
	void RemoveAll();

};
