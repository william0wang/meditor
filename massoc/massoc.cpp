// massoc2.cpp : main source file for massoc2.exe
//

#include "stdafx.h"

#include "resource.h"

#include "AVS.h"
#include "Real.h"
#include "shared.h"
#include "MainDlg.h"

CAppModule _Module;

int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CString sCmdLine(lpstrCmdLine);

	if(sCmdLine.Find(_T("--install-avs")) >= 0) {
		int offset = sCmdLine.Find(_T("\""));
		if(offset < 0)
			return FALSE;
		CString path = sCmdLine.Right(sCmdLine.GetLength() - offset - 1);
		path.Trim();
		offset = path.Find(_T("\""));
		if(offset <= 0)
			return FALSE;
		path = path.Left(offset);

		CAVS avs;
		avs.Install(path);
		return FALSE;
	}

	int AppLanguage = 0;
	CString langfile_tc;
	CString langfile_en;
	HINSTANCE instance_dll = NULL;
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	CString program_dir;
	program_dir.Format(_T("%s"),szFilePath);

	CString right = program_dir.Right(7);
	if(right == _T("\\tools\\"))
		program_dir = program_dir.Left(program_dir.GetLength() - 6);
	else {
		right = program_dir.Right(8);
		if(right == _T("\\codecs\\"))
			program_dir = program_dir.Left(program_dir.GetLength() - 7);
	}

	if(IsFileExist(program_dir + _T("tools\\meditor2.tc.dll")))
		langfile_tc = program_dir + _T("tools\\meditor2.tc.dll");
	if(IsFileExist(program_dir + _T("tools\\meditor2.en.dll")))
		langfile_en = program_dir + _T("tools\\meditor2.en.dll");
	if(IsFileExist(program_dir + _T("codecs\\meditor2.tc.dll")))
		langfile_tc = program_dir + _T("codecs\\meditor2.tc.dll");
	if(IsFileExist(program_dir + _T("codecs\\meditor2.en.dll")))
		langfile_en = program_dir + _T("codecs\\meditor2.en.dll");
	if(IsFileExist(program_dir + _T("meditor2.tc.dll")))
		langfile_tc = program_dir + _T("meditor2.tc.dll");
	if(IsFileExist(program_dir + _T("meditor2.en.dll")))
		langfile_en = program_dir + _T("meditor2.en.dll");

	if(langfile_tc.GetLength()>1 || langfile_en.GetLength()> 1) {
		AppLanguage = GetPrivateProfileInt(_T("Option"),_T("Language"), 0, program_dir + _T("kk.ini"));
		if(AppLanguage == 0) {
			LANGID   _SysLangId = GetSystemDefaultLangID();
			if(PRIMARYLANGID(_SysLangId)   ==   LANG_CHINESE) {
				if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_SIMPLIFIED)
					AppLanguage = 1;		//Simplified Chinese GBK
				else if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_TRADITIONAL)
					AppLanguage = 4;		//Traditional Chinese Big5
				else
					AppLanguage = 3;		//ANSI
			} else
				AppLanguage = 2;			//ANSI
		}
	}

	CString strSatellite = _T("");
	if(AppLanguage == 2 && langfile_en.GetLength() > 1)
		strSatellite = langfile_en;
	else if((AppLanguage == 3 || AppLanguage == 4) && langfile_tc.GetLength() > 1)
		strSatellite = langfile_tc;

	if (strSatellite.GetLength() > 2)
		instance_dll = LoadLibrary(strSatellite);

	if(sCmdLine.Find(_T("--real-online")) >= 0)	{
		HANDLE gUniqueEvent = CreateEvent(NULL, TRUE, TRUE, _T("massoc2-realOnline"));
		if(GetLastError() == ERROR_ALREADY_EXISTS) {
			if(instance_dll)
				FreeLibrary(instance_dll);
			return FALSE;
		}

		if(instance_dll)
			_Module.SetResourceInstance(instance_dll);

		CRealDlg dlg;
		dlg.m_cmdline = sCmdLine;
		dlg.m_dir = program_dir + _T("codecs\\");
		dlg.InstallReal();

		if(gUniqueEvent)
			CloseHandle(gUniqueEvent);

		if(instance_dll)
			FreeLibrary(instance_dll);

		return FALSE;
	}


	CMainDlg dlgMain(instance_dll, AppLanguage);

	HANDLE gUniqueEvent = CreateEvent(NULL, TRUE, TRUE, _T("massoc2-associations"));
	if(GetLastError() == ERROR_ALREADY_EXISTS) {
		if(instance_dll)
			FreeLibrary(instance_dll);
		return FALSE;
	}

	if(sCmdLine.Find(_T("--shell-associations-updater")) >= 0) {
		dlgMain.ApplyDefault();
		if(gUniqueEvent)
			CloseHandle(gUniqueEvent);
		if(instance_dll)
			FreeLibrary(instance_dll);
		return 0;
	}

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		if(gUniqueEvent)
			CloseHandle(gUniqueEvent);
		if(instance_dll)
			FreeLibrary(instance_dll);
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();

	if(gUniqueEvent)
		CloseHandle(gUniqueEvent);
	if(instance_dll)
		FreeLibrary(instance_dll);

	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
