// RealDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "massoc.h"
#include "RealDlg.h"
#include "MShared.h"
#include "Reg.h"

#define WM_CHECKREAL        WM_USER + 101
#define WM_REGREAL          WM_USER + 102
#define WM_DREGREAL         WM_USER + 103

UINT CheckThread(LPVOID pParam)
{
	CRealDlg* This = (CRealDlg *) pParam;
	int checktime = 0;
	while (checktime < 5) {
		Sleep(300);
		if(This->CheckRealOnline()) {
			This->m_reg_ok = TRUE;
			break;
		}
		checktime++;
	}

	This->CheckRealThread = NULL;

	return 0;
}

// CRealDlg 对话框

IMPLEMENT_DYNAMIC(CRealDlg, CDialog)

CRealDlg::CRealDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRealDlg::IDD, pParent)
{
	if (theApp.hResourceHandleMod)
		AfxSetResourceHandle(theApp.hResourceHandleMod);
	CheckRealThread = NULL;
	m_reg_ok = FALSE;
}

CRealDlg::~CRealDlg()
{
}

void CRealDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRealDlg, CDialog)
END_MESSAGE_MAP()


// CRealDlg 消息处理程序

BOOL CRealDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	bool bSilent = false;

	if(m_cmdline.Find(_T("--real-online-silent")) >= 0)
		bSilent = true;

	if(m_cmdline.Find(_T("--real-online-reg")) >= 0) {
		if(RegRealOnline()) {
			while(CheckRealThread)
				Sleep(300);
			if(!bSilent) {
				if(m_reg_ok)
					MessageBox(ResStr(IDS_OTHER_REALOK),ResStr(IDS_OTHER_REALONLINE), MB_TOPMOST);
				else
					MessageBox(ResStr(IDS_OTHER_REALFAIL),ResStr(IDS_OTHER_REALONLINE), MB_TOPMOST);
			}
		}
	} else if(m_cmdline.Find(_T("--real-online-dreg")) >= 0) {
		DRegRealOnline();
		if(!bSilent) {
			if(!CheckRealOnline()) {
				MessageBox(ResStr(IDS_OTHER_UNREALOK),ResStr(IDS_OTHER_REALONLINE), MB_TOPMOST);
			} else {
				MessageBox(ResStr(IDS_OTHER_UNREALFAILS),ResStr(IDS_OTHER_REALONLINE), MB_TOPMOST);
			}
		}
	}

	OnOK();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CRealDlg::CheckRealOnline()
{
	CReg reg;
	CString regstr = _T("CLSID\\{CFCDAA03-8BE4-11CF-B84B-0020AFBBCCFA}\\InprocServer32");
	if(!reg.ShowContent_STR(HKEY_CLASSES_ROOT,regstr,_T(""))) {
		regstr = _T("SOFTWARE\\Classes\\CLSID\\{CFCDAA03-8BE4-11CF-B84B-0020AFBBCCFA}\\InprocServer32");
		if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE,regstr,_T("")))
			return false;
	}
	CString regstr1 = _T("Software\\RealNetworks\\Preferences\\DT_Codecs");
	if(!reg.ShowContent_STR(HKEY_CLASSES_ROOT,regstr1,_T(""))) {
		if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE,regstr1,_T("")))
			return false;
	}
	return true;
}

BOOL CRealDlg::RegRealOnline()
{
	TCHAR szPath[MAX_PATH + 1];
	TCHAR szCurPath[MAX_PATH + 1];
	CString m_dir, m_sysdir, m_prodir, m_datadir;

	if(CheckRealOnline() &&
		MessageBox(ResStr(IDS_OTHER_REALAGAIN),ResStr(IDS_OTHER_REALONLINE),MB_OKCANCEL|MB_TOPMOST) != IDOK)
		return FALSE;

	GetModuleFileName(NULL, szPath, MAX_PATH);
	(_tcsrchr(szPath, _T('\\')))[1] = 0;
	m_dir.Format(_T("%s"),szPath);

	::GetCurrentDirectory(MAX_PATH, szCurPath);
	::SetCurrentDirectory(szPath);

	::GetSystemDirectory(szPath, MAX_PATH);
	m_sysdir.Format(_T("%s\\"), szPath);

	SHGetSpecialFolderPath(NULL, szPath, CSIDL_PROGRAM_FILES, FALSE);
	m_prodir.Format(_T("%s\\"), szPath);

	SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	m_datadir.Format(_T("%s\\"), szPath);

	CopyFile(m_dir +_T("pncrt.dll") , m_sysdir + _T("pncrt.dll") , TRUE);
	CopyFile(m_dir +_T("msvcp71.dll") , m_sysdir + _T("msvcp71.dll") , TRUE);
	CopyFile(m_dir +_T("msvcr71.dll") , m_sysdir + _T("msvcr71.dll") , TRUE);
	CopyFile(m_dir +_T("Real\\pndx5016.dll") , m_sysdir + _T("pndx5016.dll") , FALSE);
	CopyFile(m_dir +_T("Real\\pndx5032.dll") , m_sysdir + _T("pndx5032.dll") , FALSE);
	CopyFile(m_dir +_T("Real\\rmoc3260.dll") , m_sysdir + _T("rmoc3260.dll") , FALSE);

	//Firefox plugins
	if(IsFileExist(m_prodir + _T("Mozilla Firefox"))) {
		CopyFile(m_dir +_T("Real\\Browser\\Components\\nppl3260.xpt") , m_prodir + _T("Mozilla Firefox\\components\\nppl3260.xpt"), FALSE);
		CopyFile(m_dir +_T("Real\\Browser\\Components\\nsJSRealPlayerPlugin.xpt") , m_prodir + _T("Mozilla Firefox\\components\\nsJSRealPlayerPlugin.xpt"), FALSE);
		CopyFile(m_dir +_T("Real\\Browser\\Plugins\\nppl3260.dll") , m_prodir + _T("Mozilla Firefox\\plugins\\nppl3260.dll"), FALSE);
		CopyFile(m_dir +_T("Real\\Browser\\Plugins\\nprpjplug.dll") , m_prodir + _T("Mozilla Firefox\\plugins\\nprpjplug.dll"), FALSE);
	}

	//Chrome plugins
	if(IsFileExist(m_datadir + _T("Google\\Chrome"))) {
		if(!IsFileExist(m_datadir + _T("Google\\Chrome\\plugins")))
			CreateDirectory(m_datadir + _T("Google\\Chrome\\plugins"), NULL);
		CopyFile(m_dir +_T("Real\\Browser\\Components\\nppl3260.xpt") , m_datadir + _T("Google\\Chrome\\plugins\\nppl3260.xpt"), FALSE);
		CopyFile(m_dir +_T("Real\\Browser\\Components\\nsJSRealPlayerPlugin.xpt") , m_datadir + _T("Google\\Chrome\\plugins\\nsJSRealPlayerPlugin.xpt"), FALSE);
		CopyFile(m_dir +_T("Real\\Browser\\Plugins\\nppl3260.dll") , m_datadir + _T("Google\\Chrome\\plugins\\nppl3260.dll"), FALSE);
		CopyFile(m_dir +_T("Real\\Browser\\Plugins\\nprpjplug.dll") , m_datadir + _T("Google\\Chrome\\plugins\\nprpjplug.dll"), FALSE);
	}

	CopyFile(m_dir +_T("Real\\realreg") , m_dir +_T("realreg.inf"), FALSE);
	WinExec("rundll32.exe setupapi,InstallHinfSection DefaultInstall 128 .\\realreg.inf",SW_HIDE);
	Sleep(1000);
	DeleteFile(m_dir +_T("realreg.inf"));
	ShellExecute(0, _T("open"), _T("regsvr32.exe") , _T(" /s \"")+ m_sysdir + _T("rmoc3260.dll\"") , NULL, SW_HIDE);

	::SetCurrentDirectory(szCurPath);

	if(CheckRealThread == NULL)
		CheckRealThread = AfxBeginThread(CheckThread,this);

	return TRUE;
}

BOOL CRealDlg::DRegRealOnline()
{
	TCHAR szPath[MAX_PATH + 1];
	TCHAR szCurPath[MAX_PATH + 1];
	CString m_dir, m_sysdir, m_prodir, m_datadir;

	if(!CheckRealOnline()) {
		MessageBox(ResStr(IDS_OTHER_REALON),ResStr(IDS_OTHER_REALONLINE), MB_TOPMOST);
		return FALSE;
	}

	GetModuleFileName(NULL, szPath, MAX_PATH);
	(_tcsrchr(szPath, _T('\\')))[1] = 0;
	m_dir.Format(_T("%s"), szPath);

	::GetCurrentDirectory(MAX_PATH, szCurPath);
	::SetCurrentDirectory(szPath);

	::GetSystemDirectory(szPath, MAX_PATH);
	m_sysdir.Format(_T("%s\\"), szPath);

	SHGetSpecialFolderPath(NULL, szPath, CSIDL_PROGRAM_FILES, FALSE);
	m_prodir.Format(_T("%s\\"), szPath);

	SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	m_datadir.Format(_T("%s\\"), szPath);


	if(IsFileExist(m_sysdir + _T("rmoc3260.dll")))
		ShellExecute(0, _T("open"), _T("regsvr32.exe") , _T(" /u /s \"")+ m_sysdir + _T("rmoc3260.dll\"") , NULL, SW_HIDE);
	else
		ShellExecute(0, _T("open"), _T("regsvr32.exe") , _T(" /u /s \"")+ m_dir + _T("Real\\rmoc3260.dll\"") , NULL, SW_HIDE);
	DeleteFile(m_sysdir +_T("rmoc3260.dll"));
	DeleteFile(m_sysdir + _T("pndx5016.dll"));
	DeleteFile(m_sysdir + _T("pndx5032.dll"));
	CopyFile(m_dir +_T("Real\\unrealreg") , m_dir +_T("unrealreg.inf") , TRUE);
	WinExec("rundll32.exe setupapi,InstallHinfSection DefaultInstall 128 .\\unrealreg.inf",SW_HIDE);
	Sleep(500);
	DeleteFile(m_dir +_T("unrealreg.inf"));

	//Firefox plugins
	if(IsFileExist(m_prodir + _T("Mozilla Firefox"))) {
		DeleteFile(m_prodir + _T("Mozilla Firefox\\components\\nppl3260.xpt"));
		DeleteFile(m_prodir + _T("Mozilla Firefox\\components\\nsJSRealPlayerPlugin.xpt"));
		DeleteFile(m_prodir + _T("Mozilla Firefox\\plugins\\nppl3260.dll"));
		DeleteFile(m_prodir + _T("Mozilla Firefox\\plugins\\nprpjplug.dll"));
	}

	//Chrome plugins
	if(IsFileExist(m_datadir + _T("Google\\Chrome\\plugins"))) {
		DeleteFile(m_datadir + _T("Google\\Chrome\\plugins\\nppl3260.xpt"));
		DeleteFile(m_datadir + _T("Google\\Chrome\\plugins\\nsJSRealPlayerPlugin.xpt"));
		DeleteFile(m_datadir + _T("Google\\Chrome\\plugins\\nppl3260.dll"));
		DeleteFile(m_datadir + _T("Google\\Chrome\\plugins\\nprpjplug.dll"));
	}

	::SetCurrentDirectory(szCurPath);

	return TRUE;
}
