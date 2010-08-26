// MEditor2.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MEditor2.h"
#include "MEditor2Dlg.h"
#include "MDSPlayer.h"
#include "MFlashPlayer.h"

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

	if(OpenType == START_FLASHPLAYER)
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

		int AppLanguage = 0;
		CString langfile_tc;
		CString langfile_en;

		if(IsFileExist(program_dir + _T("tools\\meditor2.tc.dll")))
			langfile_tc = program_dir + _T("tools\\meditor2.tc.dll");
		if(IsFileExist(program_dir + _T("tools\\meditor2.en.dll")))
			langfile_en = program_dir + _T("tools\\");
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

		if (strSatellite.GetLength() > 2) {
			hResourceHandleOld = AfxGetResourceHandle();
			hResourceHandleMod = LoadLibrary(strSatellite);
			if (hResourceHandleMod)
				AfxSetResourceHandle(hResourceHandleMod);
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
