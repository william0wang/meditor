// meditor3.cpp : main source file for meditor3.exe
//

#include "stdafx.h"

#include "resource.h"

#include "aboutdlg.h"
#include "PreviewDlg.h"

enum START_TYPE
{
	START_FLASHPLAYER = 1001,
	START_MEDIAPLAYER,
	START_MEDIAINFO,
	START_PREVIEW,
};

CAppModule _Module;

int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CString   sCmdLine(lpstrCmdLine);
	int OpenType = 0;
	int nRet;
	CString ProgramName;
	CString program_dir;
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	ProgramName.Format(_T("%s"),szFilePath);
	ProgramName = ProgramName.Right(15);
	ProgramName.MakeLower();
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	program_dir.Format(_T("%s"),szFilePath);

	if( ProgramName == _T("mediaplayer.exe"))
	{
		OpenType = START_MEDIAPLAYER;
	}
	else if(sCmdLine != _T(""))
	{
		if(sCmdLine.Find(_T("Open Input Editor")) >= 0)
			OpenType = 1;
		else if(sCmdLine.Find(_T("Open Assoc")) >= 0)
			OpenType = 2;
		else if(sCmdLine.Find(_T("Open Editor")) >= 0)
			OpenType = 3;
		else if(sCmdLine.Find(_T("--generate-preview")) >= 0)
			OpenType = START_PREVIEW;
		else if(sCmdLine.Find(_T("--Show Media Info")) >= 0)
			OpenType = START_MEDIAINFO;
		else if(sCmdLine.Find(_T("--Open MediaPlayer")) >= 0)
		{
			OpenType = START_MEDIAPLAYER;
			sCmdLine.Replace(_T("--Open MediaPlayer"),_T(""));
		}
		else if(sCmdLine.Find(_T("prer://")) >= 0)
			OpenType = START_MEDIAPLAYER;
		else if(sCmdLine.Find(_T("prek://")) >= 0 || sCmdLine.Find(_T("prea://")) >= 0 || sCmdLine.Find(_T("prem://")) >= 0)
		{
			sCmdLine.Replace(_T("prek://") ,_T("http://"));
			sCmdLine.Replace(_T("prea://") ,_T("http://"));
			sCmdLine.Replace(_T("prem://") ,_T("http://"));
			//TestURL(sCmdLine,128);
			//sCmdLine = _T("mplayer.exe -nocache ") + sCmdLine;
			//int outlen = 0;
			//char *out = UnicodeToLocal(sCmdLine,outlen);
			//WinExec(out, SW_SHOW);
			//delete out;
			//return FALSE;
		}
	}

	if(OpenType == START_PREVIEW)
	{
		int offset = sCmdLine.Find(_T("--filename"));
		if(offset < 0)
			return FALSE;
		offset = sCmdLine.Find(_T("\""), offset);
		if(offset < 0)
			return FALSE;
		CString name = sCmdLine.Right(sCmdLine.GetLength() - offset - 1);
		name.Trim();
		offset = name.Find(_T("\""));
		if(offset <= 0)
			return FALSE;
		name = name.Left(offset);

		offset = sCmdLine.Find(_T("--duration"));
		if(offset < 0)
			return FALSE;
		CString len = sCmdLine.Right(sCmdLine.GetLength() - offset - 10);
		len.Trim();
		long time = _ttol(len);
		if(time < 10)
			return FALSE;

		UINT DialogIDD = IDD_PREVIEW_DIALOG;

		int AppLanguage = GetPrivateProfileInt(_T("Option"),_T("Language"),0,program_dir + _T("kk.ini"));
		if(AppLanguage == 0)
		{
			LANGID   _SysLangId   =   GetSystemDefaultLangID();
			if(PRIMARYLANGID(_SysLangId)   ==   LANG_CHINESE)
			{
				if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_SIMPLIFIED)
					AppLanguage = 1;		//Simplified Chinese GBK
				else if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_TRADITIONAL)
					AppLanguage = 4;		//Traditional Chinese Big5
				else
					AppLanguage = 3;		//ANSI
			}
			else
				AppLanguage = 2;			//ANSI
		}

		if(AppLanguage == 2) {
			DialogIDD = IDD_PREVIEW_DIALOG_EN;
		} else if(AppLanguage == 3 || AppLanguage == 4) {
			DialogIDD = IDD_PREVIEW_DIALOG_TC;
		}

		CPreviewDlg dlgPreview;
		dlgPreview.m_filename = name;
		dlgPreview.ltime = time;

		if(dlgPreview.Create(NULL) == NULL)
		{
			ATLTRACE(_T("Preview dialog creation failed!\n"));
			return 0;
		}

		dlgPreview.ShowWindow(nCmdShow);
		nRet = theLoop.Run();

	} 
	_Module.RemoveMessageLoop();
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
