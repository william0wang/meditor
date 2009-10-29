// MEditor2.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MEditor2.h"
#include "MEditor2Dlg.h"
#include "MDSPlayer.h"
#include "PreviewDlg.h"
#include "MFlashPlayer.h"
#include "MediaInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef UNICODE
#define _ttof _wtof
typedef std::wstring _tstring;
#else
#define _ttof atof
typedef std::string _tstring;
#endif

#define START_FLASHPLAYER 1001
#define START_MEDIAPLAYER 2002
#define START_MEDIAINFO 2005
#define START_PREVIEW 2008

// CMEditor2App

BEGIN_MESSAGE_MAP(CMEditor2App, CWinApp)
	ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()


// CMEditor2App 构造

CMEditor2App::CMEditor2App()
{
	hResourceHandleOld = NULL;
	hResourceHandleMod = NULL;
	AppLanguage = 0;
	DialogIDD = IDD_PREVIEW_DIALOG;
}


// 唯一的一个 CMEditor2App 对象

CMEditor2App theApp;


// CMEditor2App 初始化

BOOL CMEditor2App::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("MEditor2 - MPlayer 首选项"));

	CString   sCmdLine(this->m_lpCmdLine);
	int OpenType = 0;
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
	else if(ProgramName == _T("flashplayer.exe"))
	{
		OpenType = START_FLASHPLAYER;
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
		else if(sCmdLine.Find(_T("--Open FlashPlayer")) >= 0)
		{
			OpenType = START_FLASHPLAYER;
			sCmdLine.Replace(_T("--Open FlashPlayer"),_T(""));
		}
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
			TestURL(sCmdLine,128);
			sCmdLine = _T("mplayer.exe -nocache ") + sCmdLine;
			int outlen = 0;
			char *out = UnicodeToLocal(sCmdLine,outlen);
			WinExec(out, SW_SHOW);
			delete out;
			return FALSE;
		}
	}

	if(OpenType == START_MEDIAINFO)
	{
		CMediaInfoDlg info;
		m_pMainWnd = &info;
		INT_PTR nResponse = info.DoModal();
	}
	else if(OpenType == START_PREVIEW)
	{
		HANDLE gUniqueEvent = CreateEvent(NULL, TRUE, TRUE, _T("meditor2 - Preview"));
		if(GetLastError() == ERROR_ALREADY_EXISTS)
			return FALSE;

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

		CString aspect;
		offset = sCmdLine.Find(_T("--aspect"));
		if(offset > 0) {
			offset = sCmdLine.Find(_T("\""), offset);
			if(offset > 0) {
				aspect = sCmdLine.Right(sCmdLine.GetLength() - offset - 1);
				aspect.Trim();
				offset = aspect.Find(_T("\""));
				if(offset > 0)
					aspect = aspect.Left(offset);
				else
					aspect = _T("0");
			}
		}

		offset = sCmdLine.Find(_T("--duration"));
		if(offset < 0)
			return FALSE;
		CString len = sCmdLine.Right(sCmdLine.GetLength() - offset - 10);
		len.Trim();
		long time = _ttol(len);
		if(time < 10)
			return FALSE;

		AppLanguage = GetPrivateProfileInt(_T("Option"),_T("Language"),0,program_dir + _T("kk.ini"));
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

		CPreviewDlg preview;
		preview.m_filename = name;
		preview.ltime = time;
		preview.m_aspect = _ttof(aspect);
		m_pMainWnd = &preview;
		INT_PTR nResponse = preview.DoModal();

		if(gUniqueEvent)
			CloseHandle(gUniqueEvent);
	}
	else if(OpenType == START_FLASHPLAYER)
	{
		CMFlashPlayer flashplayer;
		m_pMainWnd = &flashplayer;
		flashplayer.IninFileName(sCmdLine);
		INT_PTR nResponse = flashplayer.DoModal();
	}
	else if(OpenType == START_MEDIAPLAYER)
	{
		CMDSPlayer dsplayer;
		m_pMainWnd = &dsplayer;
		dsplayer.IninFileName(sCmdLine);
		INT_PTR nResponse = dsplayer.DoModal();
	}
	else
	{
		int langfile_tc = 0;
		int langfile_en = 0;
		if(IsFileExist(program_dir + _T("codecs\\meditor2.tc.dll")))
			langfile_tc = 2;
		if(IsFileExist(program_dir + _T("meditor2.tc.dll")))
			langfile_tc = 1;
		if(IsFileExist(program_dir + _T("codecs\\meditor2.en.dll")))
			langfile_en = 2;
		if(IsFileExist(program_dir + _T("meditor2.en.dll")))
			langfile_en = 1;

		if(langfile_tc || langfile_en)
		{
			AppLanguage = GetPrivateProfileInt(_T("Option"),_T("Language"),0,program_dir + _T("kk.ini"));
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

			CString strSatellite = _T("");
			if(AppLanguage == 2 && langfile_en) {
				if(langfile_en == 1)
					strSatellite = program_dir + _T("meditor2.en.dll");
				else
					strSatellite = program_dir + _T("codecs\\meditor2.en.dll");
			} else if(langfile_tc && (AppLanguage == 3 || AppLanguage == 4)) {
				if(langfile_tc == 1)
					strSatellite = program_dir + _T("meditor2.tc.dll");
				else
					strSatellite = program_dir + _T("codecs\\meditor2.tc.dll");
			}
			if (strSatellite.GetLength() > 2)
			{
				hResourceHandleOld = AfxGetResourceHandle();
				hResourceHandleMod = LoadLibrary (strSatellite);
				if (hResourceHandleMod)
					AfxSetResourceHandle(hResourceHandleMod);
			}
		}

		CMEditor2Dlg dlg;
		m_pMainWnd = &dlg;
		dlg.m_OpenType = OpenType;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			//  “确定”来关闭对话框的代码
		}
		else if (nResponse == IDCANCEL)
		{
			//  “取消”来关闭对话框的代码
		}
	}
	return FALSE;
}
