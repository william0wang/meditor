// MEditor2.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MEditor2.h"
#include "MEditor2Dlg.h"
#include "MDSPlayer.h"
#include "MFlashPlayer.h"
#include "MediaInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define START_FLASHPLAYER 1001
#define START_MEDIAPLAYER 2002
#define START_MEDIAINFO 2005

// CMEditor2App

BEGIN_MESSAGE_MAP(CMEditor2App, CWinApp)
	ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()


// CMEditor2App 构造

CMEditor2App::CMEditor2App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
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
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	ProgramName.Format(_T("%s"),szFilePath);
	ProgramName = ProgramName.Right(15);
	ProgramName.MakeLower();
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
		TCHAR szFilePath[MAX_PATH + 1];
		GetModuleFileName(NULL, szFilePath, MAX_PATH);
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
		CString program_dir;
		program_dir.Format(_T("%s"),szFilePath);
		if(IsFileExist(program_dir + _T("meditor2.tc.dll")))
			langfile_tc = 1;
		if(IsFileExist(program_dir + _T("codecs\\meditor2.tc.dll")))
			langfile_tc = 2;
		if(IsFileExist(program_dir + _T("meditor2.en.dll")))
			langfile_en = 1;
		if(IsFileExist(program_dir + _T("codecs\\meditor2.en.dll")))
			langfile_en = 2;

		if(langfile_tc || langfile_en)
		{
			int lang = GetPrivateProfileInt(_T("Option"),_T("Language"),0,program_dir + _T("kk.ini"));
			if(lang == 0)
			{
				LANGID   _SysLangId   =   GetSystemDefaultLangID();
				if(PRIMARYLANGID(_SysLangId)   ==   LANG_CHINESE)
				{
					if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_SIMPLIFIED)
						lang = 1;		//Simplified Chinese GBK
					else if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_TRADITIONAL)
						lang = 4;		//Traditional Chinese Big5
					else
						lang = 3;		//ANSI
				}
				else
					lang = 2;			//ANSI
			}

			if(lang == 2 && langfile_en)
			{
				LPCTSTR  strSatellite;
				if(langfile_en == 1)
					strSatellite = program_dir + _T("meditor2.en.dll");
				else
					strSatellite = program_dir + _T("codecs\\meditor2.en.dll");
				if (strSatellite)
				{
					HMODULE		hMod = LoadLibrary (strSatellite);
					if (hMod)
						AfxSetResourceHandle(hMod);
				}
			}
			else if(langfile_tc && (lang == 3 || lang == 4))
			{
				LPCTSTR  strSatellite;
				if(langfile_tc == 1)
					strSatellite = program_dir + _T("meditor2.tc.dll");
				else
					strSatellite = program_dir + _T("codecs\\meditor2.tc.dll");
				if (strSatellite)
				{
					HMODULE		hMod = LoadLibrary (strSatellite);
					if (hMod)
						AfxSetResourceHandle(hMod);
				}
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

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
