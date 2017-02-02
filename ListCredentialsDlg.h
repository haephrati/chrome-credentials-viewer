#pragma once
#include "resource.h"

// CListDriveDlg 对话框

class CListDriveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CListDriveDlg)

public:
	CListDriveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CListDriveDlg();

	enum { IDD = IDD_LIST_CREDENTIALS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
