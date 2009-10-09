// RealDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "massoc.h"
#include "RealDlg.h"
#include "Reg.h"

#define WM_CHECKREAL        WM_USER + 101
#define WM_REGREAL          WM_USER + 102
#define WM_DREGREAL         WM_USER + 103

// CRealDlg 对话框

IMPLEMENT_DYNAMIC(CRealDlg, CDialog)

CRealDlg::CRealDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRealDlg::IDD, pParent)
{

}

CRealDlg::~CRealDlg()
{
}

void CRealDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRealDlg, CDialog)
	ON_MESSAGE(WM_REGREAL, OnNotifyReg)
	ON_MESSAGE(WM_DREGREAL, OnNotifyDReg)
	ON_MESSAGE(WM_CHECKREAL, OnNotifyCheck)
END_MESSAGE_MAP()


// CRealDlg 消息处理程序

BOOL CRealDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CRealDlg::OnNotifyReg(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

LRESULT CRealDlg::OnNotifyDReg(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

LRESULT CRealDlg::OnNotifyCheck(WPARAM wParam, LPARAM lParam)
{
	CReg reg;
	CString regstr = _T("CLSID\\{CFCDAA03-8BE4-11CF-B84B-0020AFBBCCFA}\\InprocServer32");
	if(!reg.ShowContent_STR(HKEY_CLASSES_ROOT,regstr,_T("")))
		return WM_DREGREAL;
	CString regstr1 = _T("Software\\RealNetworks\\Preferences\\DT_Codecs");
	if(!reg.ShowContent_STR(HKEY_CLASSES_ROOT,regstr1,_T("")))
		return WM_DREGREAL;
	return WM_REGREAL;
}
