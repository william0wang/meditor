// RealDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RealOnline.h"
#include "shared.h"
#include "resource.h"
#include "Reg.h"

#define WM_CHECKREAL        WM_USER + 101
#define WM_REGREAL          WM_USER + 102
#define WM_DREGREAL         WM_USER + 103

UINT CheckThread(LPVOID pParam)
{
	CRealOnline* This = (CRealOnline *) pParam;
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

// CRealOnline 对话框


CRealOnline::CRealOnline(CString LangDll)
{
	CheckRealThread = NULL;
	m_reg_ok = FALSE;

	HINSTANCE dll = NULL, old_res = NULL;
	if (LangDll.GetLength() > 2)
		dll = LoadLibrary(LangDll);

	if(dll) {
		old_res = _Module.GetResourceInstance();
		_Module.SetResourceInstance(dll);
	}

	str_title = ResStr(IDS_OTHER_REALONLINE);
	str_ok = ResStr(IDS_OTHER_REALOK);
	str_fail = ResStr(IDS_OTHER_REALFAIL);
	str_uok = ResStr(IDS_OTHER_UNREALOK);
	str_ufail = ResStr(IDS_OTHER_UNREALFAILS);
	str_again = ResStr(IDS_OTHER_REALAGAIN);
	str_on = ResStr(IDS_OTHER_REALON);

	if(old_res) {
		_Module.SetResourceInstance(old_res);
		FreeLibrary(dll);
	}

}

BOOL CRealOnline::InstallReal()
{
	bool bSilent = false;

	if(m_cmdline.Find(_T("--real-online-silent")) >= 0)
		bSilent = true;

	if(m_cmdline.Find(_T("--real-online-reg")) >= 0) {
		if(RegRealOnline()) {
			while(CheckRealThread)
				Sleep(300);
			if(!bSilent) {
				if(m_reg_ok)
					MessageBox(NULL, str_ok,str_title, MB_TOPMOST);
				else
					MessageBox(NULL, str_fail,str_title, MB_TOPMOST);
			}
		}
	} else if(m_cmdline.Find(_T("--real-online-dreg")) >= 0) {
		if(DRegRealOnline()) {
			if(!bSilent) {
				if(!CheckRealOnline()) {
					MessageBox(NULL, str_uok,str_title, MB_TOPMOST);
				} else {
					MessageBox(NULL, str_ufail,str_title, MB_TOPMOST);
				}
			}
		}
	}
	return TRUE;
}

BOOL CRealOnline::CheckRealOnline()
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

BOOL CRealOnline::RegRealOnline()
{
	TCHAR szPath[MAX_PATH + 1];
	TCHAR szCurPath[MAX_PATH + 1];
	CString m_sysdir, m_prodir, m_datadir;

	if(CheckRealOnline() &&
		MessageBox(NULL, str_again, str_title,MB_OKCANCEL|MB_TOPMOST) != IDOK)
		return FALSE;
	
	::GetCurrentDirectory(MAX_PATH, szCurPath);
	::SetCurrentDirectory(m_dir);

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
	if(FileExist(m_prodir + _T("Mozilla Firefox"))) {
		CopyFile(m_dir +_T("Real\\Browser\\Components\\nppl3260.xpt") , m_prodir + _T("Mozilla Firefox\\components\\nppl3260.xpt"), FALSE);
		CopyFile(m_dir +_T("Real\\Browser\\Components\\nsJSRealPlayerPlugin.xpt") , m_prodir + _T("Mozilla Firefox\\components\\nsJSRealPlayerPlugin.xpt"), FALSE);
		CopyFile(m_dir +_T("Real\\Browser\\Plugins\\nppl3260.dll") , m_prodir + _T("Mozilla Firefox\\plugins\\nppl3260.dll"), FALSE);
		CopyFile(m_dir +_T("Real\\Browser\\Plugins\\nprpjplug.dll") , m_prodir + _T("Mozilla Firefox\\plugins\\nprpjplug.dll"), FALSE);
	}

	//Chrome plugins
	if(FileExist(m_datadir + _T("Google\\Chrome"))) {
		if(!FileExist(m_datadir + _T("Google\\Chrome\\plugins")))
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
		CheckRealThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CheckThread,this,0,0);

	return TRUE;
}

BOOL CRealOnline::DRegRealOnline()
{
	TCHAR szPath[MAX_PATH + 1];
	TCHAR szCurPath[MAX_PATH + 1];
	CString m_sysdir, m_prodir, m_datadir;

	if(!CheckRealOnline()) {
		MessageBox(NULL, str_on, str_title, MB_TOPMOST);
		return FALSE;
	}
	
	::GetCurrentDirectory(MAX_PATH, szCurPath);
	::SetCurrentDirectory(m_dir);

	::GetSystemDirectory(szPath, MAX_PATH);
	m_sysdir.Format(_T("%s\\"), szPath);

	SHGetSpecialFolderPath(NULL, szPath, CSIDL_PROGRAM_FILES, FALSE);
	m_prodir.Format(_T("%s\\"), szPath);

	SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	m_datadir.Format(_T("%s\\"), szPath);


	if(FileExist(m_sysdir + _T("rmoc3260.dll")))
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
	if(FileExist(m_prodir + _T("Mozilla Firefox"))) {
		DeleteFile(m_prodir + _T("Mozilla Firefox\\components\\nppl3260.xpt"));
		DeleteFile(m_prodir + _T("Mozilla Firefox\\components\\nsJSRealPlayerPlugin.xpt"));
		DeleteFile(m_prodir + _T("Mozilla Firefox\\plugins\\nppl3260.dll"));
		DeleteFile(m_prodir + _T("Mozilla Firefox\\plugins\\nprpjplug.dll"));
	}

	//Chrome plugins
	if(FileExist(m_datadir + _T("Google\\Chrome\\plugins"))) {
		DeleteFile(m_datadir + _T("Google\\Chrome\\plugins\\nppl3260.xpt"));
		DeleteFile(m_datadir + _T("Google\\Chrome\\plugins\\nsJSRealPlayerPlugin.xpt"));
		DeleteFile(m_datadir + _T("Google\\Chrome\\plugins\\nppl3260.dll"));
		DeleteFile(m_datadir + _T("Google\\Chrome\\plugins\\nprpjplug.dll"));
	}

	::SetCurrentDirectory(szCurPath);

	return TRUE;
}
