// massoc2.cpp : main source file for massoc2.exe
//

#include "stdafx.h"

#include "resource.h"

#include "AVS.h"
#include "Real.h"
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

	if(sCmdLine.Find(_T("--real-online")) >= 0)	{

		HANDLE gUniqueEvent = CreateEvent(NULL, TRUE, TRUE, _T("meditor2 - RealOnline"));
		if(GetLastError() == ERROR_ALREADY_EXISTS)
			return FALSE;

		CRealDlg dlg;
		dlg.m_cmdline = sCmdLine;
		dlg.InstallReal();

		if(gUniqueEvent)
			CloseHandle(gUniqueEvent);

		return FALSE;
	}

	CMainDlg dlgMain;

	HANDLE gUniqueEvent = CreateEvent(NULL, TRUE, TRUE, _T("meditor2 - Assoc"));
	if(GetLastError() == ERROR_ALREADY_EXISTS)
		return FALSE;

	if(sCmdLine.Find(_T("--shell-associations-updater")) >= 0) {
		dlgMain.ApplyDefault();
		if(gUniqueEvent)
			CloseHandle(gUniqueEvent);
		return 0;
	}

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		if(gUniqueEvent)
			CloseHandle(gUniqueEvent);
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();

	if(gUniqueEvent)
		CloseHandle(gUniqueEvent);

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
