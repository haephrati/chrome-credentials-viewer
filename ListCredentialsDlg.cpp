// ListDriveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ListDriveDlg.h"
#include "afxdialogex.h"


// CListDriveDlg 对话框

IMPLEMENT_DYNAMIC(CListDriveDlg, CDialogEx)

CListDriveDlg::CListDriveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CListDriveDlg::IDD, pParent)
{

}

CListDriveDlg::~CListDriveDlg()
{
}

void CListDriveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CListDriveDlg, CDialogEx)
END_MESSAGE_MAP()


// CListDriveDlg 消息处理程序
