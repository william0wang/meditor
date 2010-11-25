// MEditor2.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MEditor2.h"
#include "MEditor2Dlg.h"

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

	if(sCmdLine.Find(_T("Open Editor")) >= 0)
		OpenType = 3;

	CString langfile_tc;
	CString langfile_en;

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

	hResourceHandleOld = NULL;
	hResourceHandleMod = NULL;
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

	if (hResourceHandleMod)
		FreeLibrary(hResourceHandleMod);

	if (nResponse == IDOK)
	{
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		//  “取消”来关闭对话框的代码
	}
	return FALSE;
}
