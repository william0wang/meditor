// meditor3.cpp : main source file for meditor3.exe
//

#include "stdafx.h"

#include "resource.h"

#include "PreviewDlg.h"
#include "UpdateDlg.h"
#include "MainDlg.h"
#include "shared.h"
#include "InputList.h"
#include "AtlStdioFile.h"
#include "FlashPlayerDlg.h"
#include "DShowPlayerDlg.h"
#include "AssocDlgAdmin.h"
#include "RealOnline.h"

enum START_TYPE
{
	START_NORMAL = 0,
	START_HOTKEY,
	START_ASSOC,
	START_ONTOP,
	START_ASSOC_ADAIM,
	START_FLASHPLAYER = 1001,
	START_DSHOWPLAYER,
	START_PREVIEW,
	START_CHECK_UPDATE,
	START_DOWNLOAD_UPDATE,
	START_CLEAN_UP,
	START_PLAY_URL,
	START_REALONLINE,
};

CAppModule _Module;

CString strSatellite = _T("");
int AppLanguage = 0;


void GetLangDLL(CString program_dir)
{
	AppLanguage = GetPrivateProfileInt(_T("Option"),_T("Language"), 0, program_dir + _T("kk.ini"));
	if(AppLanguage == 0) {
		LANGID   _SysLangId   =   GetSystemDefaultLangID();
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

	CString langfile_tc, langfile_en;
	if(FileExist(program_dir + _T("meditor2.en.dll")))
		langfile_en = program_dir + _T("meditor2.en.dll");
	else if(FileExist(program_dir + _T("tools\\meditor2.en.dll")))
		langfile_en = program_dir + _T("tools\\meditor2.en.dll");

	if(FileExist(program_dir + _T("meditor2.tc.dll")))
		langfile_tc = program_dir + _T("meditor2.tc.dll");
	else if(FileExist(program_dir + _T("tools\\meditor2.tc.dll")))
		langfile_tc = program_dir + _T("tools\\meditor2.tc.dll");

	if(AppLanguage == 2 && langfile_en.GetLength() > 1)
		strSatellite = langfile_en;
	else if((AppLanguage == 3 || AppLanguage == 4) && langfile_tc.GetLength() > 1)
		strSatellite = langfile_tc;
}

int GetOpenType(CString ProgramName, CString &sCmdLine)
{
	if( ProgramName == _T("dshowplayer.exe")) {
		return START_DSHOWPLAYER;
	} else if(ProgramName == _T("flashplayer.exe")) {
		return START_FLASHPLAYER;
	} else if(sCmdLine.GetLength() > 0) {
		if(sCmdLine.Find(_T("--open-hotkey")) >= 0) {
			return START_HOTKEY;
		} else if(sCmdLine.Find(_T("--open-assoc")) >= 0) {
			return START_ASSOC;
		} else if(sCmdLine.Find(_T("--open-ontop")) >= 0) {
			return START_ONTOP;
		} else if(sCmdLine.Find(_T("--assoc-admin")) >= 0) {
			return START_ASSOC_ADAIM;
		} else if(sCmdLine.Find(_T("--shell-associations-updater")) >= 0) {
			return START_ASSOC_ADAIM;
		} else if(sCmdLine.Find(_T("--generate-preview")) >= 0) {
			return START_PREVIEW;
		} else if(sCmdLine.Find(_T("--check-update")) >= 0) {
			return START_CHECK_UPDATE;
		} else if(sCmdLine.Find(_T("--download-update")) >= 0) {
			return START_DOWNLOAD_UPDATE;
		} else if(sCmdLine.Find(_T("--clean-up")) >= 0) {
			return START_CLEAN_UP;
		} else if(sCmdLine.Find(_T("--real-online")) >= 0) {
			return START_REALONLINE;
		} else if(sCmdLine.Find(_T("--flash-player")) >= 0) {
			sCmdLine.Replace(_T("--flash-player"), _T(""));
			return START_FLASHPLAYER;
		} else if(sCmdLine.Find(_T("--dshow-player")) >= 0) {
			sCmdLine.Replace(_T("--dshow-player"), _T(""));
			return START_DSHOWPLAYER;
		} else if(sCmdLine.Find(_T("prer://")) >= 0) {
			return START_DSHOWPLAYER;
		} else if(sCmdLine.Find(_T("prek://")) >= 0 || sCmdLine.Find(_T("prea://")) >= 0 || sCmdLine.Find(_T("prem://")) >= 0) {
			sCmdLine.Replace(_T("prek://") ,_T("http://"));
			sCmdLine.Replace(_T("prea://") ,_T("http://"));
			sCmdLine.Replace(_T("prem://") ,_T("http://"));
			TestURL(sCmdLine, 128);
			sCmdLine = _T("mplayer.exe -nocache ") + sCmdLine;
			WinExec(unicode2local(sCmdLine.GetBuffer()).c_str(), SW_SHOW);
			sCmdLine.ReleaseBuffer();
			return START_PLAY_URL;
		}
	}
	return START_NORMAL;
}

void CleanUp(CString program_dir)
{
	CString line;
	CString listfile = program_dir + _T("help\\mcleanup.lst");
	Sleep(1000);

	CAtlStdioFile file;
	if(SUCCEEDED(file.OpenFile(listfile,  GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING)))	{
		while(file.ReadLine(line)) {
			if(line.GetLength() < 3)
				continue;
			if(line.Find(_T(".exe")) > 0 && line.Find(_T("\\")) <= 0)
				MyTerminateProcess(line);
			line = program_dir + line;
			if(FileExist(line)) {
				if(FileIsDirectory(line))
					DeleteFolder(line);
				else
					DeleteFile(line);
			}
		}
		file.Close();
	}
}

BOOL StartPreview(CString sCmdLine, int nCmdShow)
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

	if(AppLanguage == 2) {
		DialogIDD = IDD_PREVIEW_DIALOG_EN;
	} else if(AppLanguage == 3 || AppLanguage == 4) {
		DialogIDD = IDD_PREVIEW_DIALOG_TC;
	}

	CPreviewDlg dlgPreview(DialogIDD);
	dlgPreview.m_filename = name;
	dlgPreview.ltime = time;

	if(dlgPreview.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Preview dialog creation failed!\n"));
		return FALSE;
	}

	dlgPreview.ShowWindow(nCmdShow);

	return TRUE;
}

BOOL StartDownloadUpdate(CString sCmdLine, int nCmdShow)
{
	UINT DialogIDD = IDD_DIALOG_UPDATE;

	if(AppLanguage == 2) {
		DialogIDD = IDD_DIALOG_UPDATE_EN;
	} else if(AppLanguage == 3 || AppLanguage == 4) {
		DialogIDD = IDD_DIALOG_UPDATE_TC;
	}

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

	offset = sCmdLine.Find(_T("--url"));
	if(offset < 0)
		return FALSE;
	offset = sCmdLine.Find(_T("\""), offset);
	if(offset < 0)
		return FALSE;
	CString url = sCmdLine.Right(sCmdLine.GetLength() - offset - 1);
	url.Trim();
	offset = url.Find(_T("\""));
	if(offset <= 0)
		return FALSE;
	url = url.Left(offset);

	CString urlbase = _T("");
	offset = sCmdLine.Find(_T("--urlbase"));
	if(offset >= 0) {
		offset = sCmdLine.Find(_T("\""), offset);
		if(offset > 0) {
			urlbase = sCmdLine.Right(sCmdLine.GetLength() - offset - 1);
			urlbase.Trim();
			offset = urlbase.Find(_T("\""));
			if(offset > 0)
				urlbase = urlbase.Left(offset);
		}
	}

	CUpdateDlg dlgUpdate(DialogIDD);

	dlgUpdate.m_bDownload = TRUE;
	dlgUpdate.m_filename = name.GetBuffer();
	dlgUpdate.m_url = url.GetBuffer();
	dlgUpdate.m_urlbase = urlbase.GetBuffer();

	url.ReleaseBuffer();
	name.ReleaseBuffer();
	urlbase.ReleaseBuffer();

	if(dlgUpdate.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Update dialog creation failed!\n"));
		return FALSE;
	}

	dlgUpdate.ShowWindow(nCmdShow);

	return TRUE;
}

BOOL StartCheckUpdate(CString program_dir, CString sCmdLine, int nCmdShow)
{
	UINT DialogIDD = IDD_DIALOG_UPDATE;

	if(AppLanguage == 2) {
		DialogIDD = IDD_DIALOG_UPDATE_EN;
	} else if(AppLanguage == 3 || AppLanguage == 4) {
		DialogIDD = IDD_DIALOG_UPDATE_TC;
	}

	CUpdateDlg dlgUpdate(DialogIDD);

	GetMPlayerVersion(program_dir + _T("mplayer.exe"), dlgUpdate.nsvn, dlgUpdate.ndate);

	int offset = sCmdLine.Find(_T("--version"));
	if(offset >= 0) {
		CString len = sCmdLine.Right(sCmdLine.GetLength() - offset - 9);
		len.Trim();

		dlgUpdate.nsvn = _ttoi(len);
	}

	offset = sCmdLine.Find(_T("--date"));
	if(offset >= 0) {
		CString len = sCmdLine.Right(sCmdLine.GetLength() - offset - 6);
		len.Trim();
		dlgUpdate.ndate = _ttoi(len);
	}

	if(dlgUpdate.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Update dialog creation failed!\n"));
		return FALSE;
	}

	dlgUpdate.ShowWindow(nCmdShow);

	return TRUE;
}

BOOL StartAssocAdmin(CString sCmdLine, int nCmdShow)
{
	if(!IsRunAsAdmin()) {
		SHELLEXECUTEINFO sei = { sizeof(sei) };
		TCHAR szFilePath[MAX_PATH + 1];
		GetModuleFileName(NULL, szFilePath, MAX_PATH);
		sei.lpVerb = _T("runas");
		sei.lpFile = szFilePath;
		sei.lpParameters = sCmdLine;
		sei.hwnd = NULL;
		sei.nShow = SW_NORMAL;

		ShellExecuteEx(&sei);
		return FALSE;
	}

	CAssocDlgAdmin dlgAssoc(strSatellite, AppLanguage);

	if(sCmdLine.Find(_T("--shell-associations-updater")) >= 0) {
		dlgAssoc.ApplyDefault();
		return FALSE;
	}

	dlgAssoc.DoModal();

	return TRUE;
}

BOOL StartRealOnlineAdmin(CString sCmdLine, int nCmdShow)
{
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	if(!IsRunAsAdmin()) {
		SHELLEXECUTEINFO sei = { sizeof(sei) };
		sei.lpVerb = _T("runas");
		sei.lpFile = szFilePath;
		sei.lpParameters = sCmdLine;
		sei.hwnd = NULL;
		sei.nShow = SW_NORMAL;

		ShellExecuteEx(&sei);
		return FALSE;
	}
	CString program_dir;
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	program_dir.Format(_T("%s"), szFilePath);

	CRealOnline Real(strSatellite);
	Real.m_cmdline = sCmdLine;
	Real.m_dir = program_dir + _T("codecs\\");
	Real.InstallReal();

	return FALSE;
}

int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CString   sCmdLine(lpstrCmdLine);
	int nRet;
	CString ProgramName;
	CString program_dir;
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);

	ProgramName.Format(_T("%s"), _tcsrchr(szFilePath, _T('\\')) + 1);
	ProgramName.MakeLower();

	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	program_dir.Format(_T("%s"), szFilePath);

	int OpenType = GetOpenType(ProgramName, sCmdLine);
	if(OpenType == START_PLAY_URL)
		return FALSE;

	GetLangDLL(program_dir);

	switch(OpenType)
	{
	case START_CLEAN_UP:
		CleanUp(program_dir);
		_Module.RemoveMessageLoop();
		return 0;
	case START_PREVIEW:
		if(!StartPreview(sCmdLine, nCmdShow))
			return 0;
		nRet = theLoop.Run();
		break;
	case START_CHECK_UPDATE:
		if(!StartCheckUpdate(program_dir, sCmdLine, nCmdShow))
			return 0;
		nRet = theLoop.Run();
		break;
	case START_DOWNLOAD_UPDATE:
		if(!StartDownloadUpdate(sCmdLine, nCmdShow))
			return 0;
		nRet = theLoop.Run();
		break;
	case START_FLASHPLAYER:
		{
			CFlashPlayerDlg flashplayer;
			flashplayer.IninFileName(sCmdLine);
			flashplayer.m_applang = AppLanguage;
			if(flashplayer.Create(NULL) == NULL) {
				ATLTRACE(_T("FlashPlayer dialog creation failed!\n"));
				return 0;
			}
			flashplayer.ShowWindow(nCmdShow);
			nRet = theLoop.Run();
			break;
		}
	case START_DSHOWPLAYER:
		{
			CDShowPlayerDlg dshowplayer;
			dshowplayer.IninFileName(sCmdLine);
			dshowplayer.m_applang = AppLanguage;
			if(dshowplayer.Create(NULL) == NULL) {
				ATLTRACE(_T("DshowPlayer dialog creation failed!\n"));
				return 0;
			}
			dshowplayer.ShowWindow(nCmdShow);
			nRet = theLoop.Run();
			break;
		}
	case START_ASSOC_ADAIM:
		{
			HANDLE gUniqueEvent = CreateEvent(NULL, TRUE, TRUE, _T("meditor3-associations"));
			if(GetLastError() == ERROR_ALREADY_EXISTS)
				return FALSE;
			if(!StartAssocAdmin(sCmdLine, nCmdShow)) {
				CloseHandle(gUniqueEvent);
				return 0;
			}

			CloseHandle(gUniqueEvent);
			break;
		}
	case START_REALONLINE:
		{
			HANDLE gUniqueEvent = CreateEvent(NULL, TRUE, TRUE, _T("meditor3-realonline"));
			if(GetLastError() == ERROR_ALREADY_EXISTS)
				return FALSE;
			if(!StartRealOnlineAdmin(sCmdLine, nCmdShow)) {
				CloseHandle(gUniqueEvent);
				return 0;
			}

			CloseHandle(gUniqueEvent);
		}
	default:
		{
			HANDLE gUniqueCheckEvent = CreateEvent(NULL, TRUE, TRUE, _T("meditor v3 - MPlayer Preferences version 3"));
			if(GetLastError() == ERROR_ALREADY_EXISTS) {

				HWND hWnd = FindWindow(_T("MEDITOR_V3_CLASS"), NULL);
				if(IsWindow(hWnd))	{
					if(OpenType > 0)
						SetWindowPos(hWnd, HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
					if (IsIconic(hWnd))
						ShowWindow(hWnd, SW_RESTORE);
					HWND hWndChild = GetLastActivePopup(hWnd);
					SetForegroundWindow(hWndChild);
				}
				return FALSE;
			}

			HINSTANCE instance_dll = NULL, instance_org = NULL;

			if (strSatellite.GetLength() > 2)
				instance_dll = LoadLibrary(strSatellite);

			if(instance_dll)
				instance_org = _Module.SetResourceInstance(instance_dll);

			rStr.LoadString();

			if(instance_org)
				_Module.SetResourceInstance(instance_org);

			WNDCLASS wcx;
			memset(&wcx, 0, sizeof(wcx));
			GetClassInfo(NULL, WC_DIALOG, &wcx);
			wcx.lpszClassName = _T("MEDITOR_V3_CLASS");
			RegisterClass(&wcx);

			CMainDlg dlgMain;
			dlgMain.m_appLang = AppLanguage;
			dlgMain.m_OpenType = OpenType;

			if(dlgMain.Create(NULL) == NULL) {
				ATLTRACE(_T("Main dialog creation failed!\n"));
				return 0;
			}

			dlgMain.ShowWindow(nCmdShow);
			nRet = theLoop.Run();
			CloseHandle(gUniqueCheckEvent);
		}
		break;
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

	AtlAxWinInit();

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
