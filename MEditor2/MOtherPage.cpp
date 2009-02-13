// MOtherPage.cpp : implementation file
//

#include "stdafx.h"
#include "meditor2.h"
#include "MOtherPage.h"
#include "MConfig.h"
#include "reg.h"
#include "MLinkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMOtherPage dialog

UINT CheckThread(LPVOID pParam)
{
	CMOtherPage* This = (CMOtherPage *) pParam;
	int checktime = 0;
CheckReal:
	checktime++;
	Sleep(300);
	if(This->CheckRealOnline())
		MessageBox(This->m_hWnd , ResStr(IDS_OTHER_REALOK),ResStr(IDS_OTHER_REALONLINE), 0);
	else
	{
		if(checktime < 5)
			goto CheckReal;
		MessageBox(This->m_hWnd , ResStr(IDS_OTHER_REALFAIL),ResStr(IDS_OTHER_REALONLINE), 0);
	}

	This->CheckRealThread = NULL;
	return 0;
}

CMOtherPage::CMOtherPage(CWnd* pParent /*=NULL*/)
	: CDialog(CMOtherPage::IDD, pParent)
	, m_mpc(FALSE)
	, m_disable_xptoolbars(FALSE)
	, m_disable_xpwindow(FALSE)
	, m_screensaver(FALSE)
	, m_theme(FALSE)
{
	//{{AFX_DATA_INIT(CMOtherPage)
	m_other = _T("");
	m_last_page = FALSE;
	m_video = _T("");
	m_audio = _T("");
	m_one = FALSE;
	CheckRealThread = NULL;
	//}}AFX_DATA_INIT
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	m_program.Format(_T("%s"),szFilePath);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
}


void CMOtherPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_MPLAYER, m_mplayer);
	DDX_Control(pDX, IDC_RADIO_MEDITOR, m_meditor);
	DDX_Control(pDX, IDC_EDIT_OTHER, m_other_c);
	DDX_Text(pDX, IDC_EDIT_OTHER, m_other);
	DDX_Check(pDX, IDC_CHECK_PAGE, m_last_page);
	DDX_Text(pDX, IDC_EDIT_VIDEO, m_video);
	DDX_Text(pDX, IDC_EDIT_AUDIO, m_audio);
	DDX_Check(pDX, IDC_CHECK_ONE, m_one);
	DDX_Control(pDX, IDC_BUTTON_ONLINE, m_realreg);
	DDX_Control(pDX, IDC_BUTTON_DONLINE, m_unrealreg);
	DDX_Control(pDX, IDC_CHECK_MPC, m_mpc_c);
	DDX_Check(pDX, IDC_CHECK_MPC, m_mpc);
	DDX_Check(pDX, IDC_CHECK_XPTOOLBARS, m_disable_xptoolbars);
	DDX_Check(pDX, IDC_CHECK_XPWINDOW, m_disable_xpwindow);
	DDX_Check(pDX, IDC_CHECK_SCREENSAVER, m_screensaver);
	DDX_Check(pDX, IDC_CHECK_THEME, m_theme);
	DDX_Check(pDX, IDC_CHECK_INFO, m_info_html);
}


BEGIN_MESSAGE_MAP(CMOtherPage, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_AUDIO, OnButtonAudio)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, OnButtonClean)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_VIDEO, OnButtonVideo)
	ON_BN_CLICKED(IDC_BUTTON_FLASH, OnButtonFlash)
	ON_BN_CLICKED(IDC_BUTTON_MEDIA, OnButtonMedia)
	ON_BN_CLICKED(IDC_BUTTON_ONLINE, OnButtonOnline)
	ON_BN_CLICKED(IDC_BUTTON_DONLINE, OnButtonDonline)
	ON_BN_CLICKED(IDC_RADIO_MPLAYER, OnRadioMplayer)
	ON_BN_CLICKED(IDC_RADIO_MEDITOR, OnRadioMeditor)
	ON_BN_CLICKED(IDC_BUTTON_LINK, OnBnClickedButtonLink)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON_AVS, &CMOtherPage::OnBnClickedButtonAvs)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMOtherPage message handlers

BOOL CMOtherPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_mplayer.SetCheck(1);
	if(m_cfg)
	{
		m_other = m_cfg->GetValue_Other();
		int value_i;
		bool value_b;
		m_cfg->GetValue_Integer(_T("use_windowblinds"),value_i,true);
		m_cfg->GetValue_Integer(_T("using_aero"),value_i,true);
		m_cfg->GetValue_Integer(_T("using_theme"),value_i,true);
		m_cfg->GetValue_Integer(_T("is_vista"),value_i,true);
		if(m_cfg->GetValue_Integer(_T("meditor_last_page"),value_i,true))
		{
			if(value_i > -1 && value_i < 7)
				m_last_page = TRUE;
			else
				m_last_page = FALSE;
		}
		if(m_cfg->GetValue_Boolean(_T("meditor_one_editor"),value_b,true))
		{
			if(value_b)
				m_one = TRUE;
			else
				m_one = FALSE;
		}
		if(m_cfg->GetValue_Boolean(_T("info_to_html"),value_b,true))
		{
			if(value_b)
				m_info_html = TRUE;
			else
				m_info_html = FALSE;
		}
		if(m_cfg->GetValue_Boolean(_T("disable_xptoolbars"),value_b,true))
		{
			if(value_b)
				m_disable_xptoolbars = TRUE;
			else
				m_disable_xptoolbars = FALSE;
		}
		if(m_cfg->GetValue_Boolean(_T("disable_xpwindow"),value_b,true))
		{
			if(value_b)
				m_disable_xpwindow = TRUE;
			else
				m_disable_xpwindow = FALSE;
		}
		if(m_cfg->GetValue_Boolean(_T("disable_screensaver"),value_b,true))
		{
			if(!value_b)
				m_screensaver = TRUE;
			else
				m_screensaver = FALSE;
		}
	}
	if(IsFileExist(m_program_dir + _T("codecs\\Real\\mloader.ini")))
	{
		m_meditor.SetCheck(1);
		m_mplayer.SetCheck(0);
	}
	m_mpc_exe = m_program_dir + _T("tools\\mplayerc.exe");
	if( IsFileExist(m_mpc_exe))
	{
		m_mpc_c.EnableWindow(TRUE);
		m_mpc = TRUE;
	}
	if(IsFileExist(m_program + _T(".manifest")))
		m_theme = TRUE;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMOtherPage::SetNormal()
{
	m_other = _T("");
	m_disable_xptoolbars = FALSE;
	m_disable_xpwindow = FALSE;
	m_screensaver = FALSE;
	m_info_html = FALSE;
	m_theme = FALSE;
}

void CMOtherPage::SetHigh()
{
	SetNormal();
	m_theme = TRUE;
}

void CMOtherPage::SetLower()
{
	SetNormal();
	m_disable_xptoolbars = TRUE;
	m_disable_xpwindow = TRUE;
}

void CMOtherPage::SaveConfig()
{
	UpdateData(TRUE);
	if(m_mplayer.GetCheck() == 1)
		DeleteFile(m_program_dir + _T("codecs\\Real\\mloader.ini"));
	else
	{
		TCHAR szProgramPath[MAX_PATH + 1];
		GetModuleFileName(NULL, szProgramPath, MAX_PATH);
		CString cmd;
		if(m_mpc)
			cmd = m_mpc_exe;
		else
			cmd.Format(_T("%s --Open MediaPlayer"), szProgramPath);
		WritePrivateProfileString(_T("Command"), _T("Program_Path"),  cmd , m_program_dir + _T("codecs\\Real\\mloader.ini"));
	}

	if(!m_cfg)
		return;

	if(m_last_page)
		m_cfg->SetValue(_T("meditor_last_page"),_T("1"),true,ex_meditor);
	else	
		m_cfg->SetValue(_T("meditor_last_page"),_T("0"),true,ex_meditor);

	if(m_one)
		m_cfg->SetValue(_T("meditor_one_editor"),_T("1"),true,ex_meditor);
	else	
		m_cfg->RemoveValue(_T("meditor_one_editor"),true);

	if(m_info_html)
		m_cfg->SetValue(_T("info_to_html"),_T("1"),true,ex_setting);
	else	
		m_cfg->RemoveValue(_T("info_to_html"),true);

	if(m_disable_xpwindow)
		m_cfg->SetValue(_T("disable_xpwindow"),_T("1"),true,ex_theme);
	else	
		m_cfg->RemoveValue(_T("disable_xpwindow"),true);

	if(m_disable_xptoolbars)
		m_cfg->SetValue(_T("disable_xptoolbars"),_T("1"),true,ex_theme);
	else	
		m_cfg->RemoveValue(_T("disable_xptoolbars"),true);

	if(m_screensaver)
		m_cfg->SetValue(_T("disable_screensaver"),_T("0"),true,ex_sysinfo);
	else	
		m_cfg->RemoveValue(_T("disable_screensaver"),true);

	m_other.TrimRight(_T("\r\n"));
	if(m_other.GetLength() > 1)
	{
		m_other += _T("\r\n");
		m_cfg->SetValue_Other(m_other);
	}
	else
		m_cfg->SetValue_Other(_T(""));

	if(m_theme)
		ExtractResource(TEXT("IDR_MANIFESTX"),MAKEINTRESOURCE(RT_MANIFEST),m_program + _T(".manifest"),false);
	else
		DeleteFile(m_program + _T(".manifest"));
}

BOOL CMOtherPage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CMOtherPage::OnButtonAudio() 
{
	// TODO: Add your control notification handler code here
	
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT
		,ResStr(IDS_OTHER_TYPEA) + _T("|*.mp3;*.mp2;*.m4a;*.aac;*.mpc;*.wma;*.ogg;\
*.arm;*.mka;*.flac;*.ac3;*.dts;*.wav;*.ra;*.aif|") + ResStr(IDS_OTHER_ALL) + _T("(*.*)|*.*||"));

	dlg.m_ofn.lpstrTitle=ResStr(IDS_OTHER_OPENA);
    if(dlg.DoModal()==IDOK)
	{
		m_audio = dlg.GetPathName();
	}
	dlg.DestroyWindow();
	::SetCurrentDirectory(szFilePath);
	UpdateData(FALSE);
}

void CMOtherPage::OnButtonClean() 
{
	// TODO: Add your control notification handler code here
	m_video = _T("");
	m_audio = _T("");
	UpdateData(FALSE);
}

void CMOtherPage::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	if(m_video == _T("") || m_audio == _T(""))
	{
		MessageBox(ResStr(IDS_OTHER_EMPTY));
		return;
	}
	if(!IsFileExist(m_program_dir + _T("mplayer.exe")))
	{
		MessageBox(ResStr(IDS_PLAYER_SAMEDIR));
		return;
	}
	CString mpcmd;
	mpcmd.Format(_T("-audiofile \"%s\" \"%s\""), m_audio, m_video);
	ShellExecute(0, _T("open"), _T("mplayer.exe"), mpcmd, NULL, SW_SHOW);
}

void CMOtherPage::OnButtonVideo() 
{
	// TODO: Add your control notification handler code here
	
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT
		,ResStr(IDS_OTHER_TYPEV) + _T("|*.avi;*.mkv;*.ogm;*.mp4;*.m4v;*.m4p;*.m4b;*.flv;*.vp6;\
*.divx;*.vg2;*.dat;*.mpg;*.mpeg;*.tp;*.ts;*.tpr;*.pva;*.pps;*.mpe;*.vob;*.rm;*.rmvb;\
*.wmv;*.asf;*.wmp;*.wm;*.mov;*.qt;*.3gp;*.3gpp;*.3g2;*.3gp2|") + ResStr(IDS_OTHER_ALL) + _T("(*.*)|*.*||"));

//	dlg.m_ofn.lpstrFile = new wchar_t[102400];
//	memset(dlg.m_ofn.lpstrFile,0,sizeof(wchar_t)   *   102400);
//	dlg.m_ofn.nMaxFile = 102400;
	dlg.m_ofn.lpstrTitle=ResStr(IDS_OTHER_OPENV);
    if(dlg.DoModal()==IDOK)
	{
		m_video = dlg.GetPathName();
	}
	dlg.DestroyWindow();
	::SetCurrentDirectory(szFilePath);
	UpdateData(FALSE);
}

void CMOtherPage::OnButtonFlash() 
{
	// TODO: Add your control notification handler code here
	CString m_program;
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	m_program.Format(_T("%s"),szFilePath);
	ShellExecute(0, _T("open"), m_program , _T(" --Open FlashPlayer"), NULL, SW_SHOW);
}

void CMOtherPage::OnButtonMedia() 
{
	// TODO: Add your control notification handler code here
	CString m_program;
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	m_program.Format(_T("%s"),szFilePath);
	ShellExecute(0, _T("open"), m_program , _T(" --Open MediaPlayer"), NULL, SW_SHOW);
}

bool CMOtherPage::CheckRealOnline()
{
	CReg reg;
	CString regstr = _T("CLSID\\{CFCDAA03-8BE4-11CF-B84B-0020AFBBCCFA}\\InprocServer32");
	if(!reg.ShowContent_STR(HKEY_CLASSES_ROOT,regstr,_T("")))
		return false;
	CString regstr1 = _T("Software\\RealNetworks\\Preferences\\DT_Codecs");
	if(!reg.ShowContent_STR(HKEY_CLASSES_ROOT,regstr1,_T("")))
		return false;
	return true;
}

void CMOtherPage::OnButtonOnline() 
{
	// TODO: Add your control notification handler code here
	if(CheckRealOnline())
	{
		if(MessageBox(ResStr(IDS_OTHER_REALAGAIN),ResStr(IDS_OTHER_REALONLINE),MB_OKCANCEL) != IDOK)
			return;
	}
	
	if(CheckRealThread != NULL)
		return;
	UpdateData(TRUE);
	TCHAR szFilePath[MAX_PATH + 1];
	CString m_dir, m_sysdir;
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_dir.Format(_T("%s"),szFilePath);

	if(m_mplayer.GetCheck() == 1)
		DeleteFile(m_dir + _T("codecs\\Real\\mloader.ini"));
	else
	{
		TCHAR szProgramPath[MAX_PATH + 1];
		GetModuleFileName(NULL, szProgramPath, MAX_PATH);
		CString cmd;
		if(m_mpc)
			cmd = m_mpc_exe;
		else
			cmd.Format(_T("%s --Open MediaPlayer"), szProgramPath);
		WritePrivateProfileString(_T("Command"), _T("Program_Path"),  cmd , m_dir + _T("codecs\\Real\\mloader.ini"));
	}

	if(!IsFileExist(m_dir + _T("codecs\\Real\\rmoc3260.dll")))
	{
		if(IsFileExist(m_dir + _T("codecs\\realonline.7z")))
			Decode7zFile(m_dir + _T("codecs\\realonline.7z"),m_dir + _T("codecs\\"));
		else if(IsFileExist(m_dir + _T("codecs\\realonline.rar")))
			UnRarFile(m_dir + _T("codecs\\realonline.rar"),m_dir + _T("codecs\\"));
	}

	if((!IsFileExist(m_dir + _T("codecs\\Real\\realreg"))) ||
		(!IsFileExist(m_dir + _T("codecs\\Real\\rmoc3260.dll"))) ||
		(!IsFileExist(m_dir + _T("codecs\\Real\\mloader.exe"))) ||
		(!IsFileExist(m_dir + _T("codecs\\pncrt.dll"))))
	{
		MessageBox(ResStr(IDS_OTHER_REALFAILS),ResStr(IDS_OTHER_REALONLINE));
		return;
	}

	TCHAR szCurPath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szCurPath);
	::SetCurrentDirectory(szFilePath);
	TCHAR szSystemPath[MAX_PATH + 1];
	::GetSystemDirectory(szSystemPath,MAX_PATH);
	m_sysdir.Format(_T("%s\\"),szSystemPath);
	CopyFile(m_dir +_T("codecs\\pncrt.dll") , m_sysdir + _T("pncrt.dll") , TRUE);
	CopyFile(m_dir +_T("codecs\\msvcp71.dll") , m_sysdir + _T("msvcp71.dll") , FALSE);
	CopyFile(m_dir +_T("codecs\\msvcr71.dll") , m_sysdir + _T("msvcr71.dll") , FALSE);
	CopyFile(m_dir +_T("codecs\\Real\\rmoc3260.dll") , m_sysdir + _T("rmoc3260.dll") , TRUE);
	CopyFile(m_dir +_T("codecs\\Real\\realreg") , m_dir +_T("realreg.inf") , TRUE);
	WinExec("rundll32.exe setupapi,InstallHinfSection DefaultInstall 128 .\\realreg.inf",SW_HIDE);
	DeleteFile(m_dir +_T("realreg.inf"));
	ShellExecute(0, _T("open"), _T("regsvr32.exe") , _T(" /s \"")+ m_sysdir + _T("rmoc3260.dll\"") , NULL, SW_HIDE);
	ShellExecute(0, _T("open"), _T("regedit") , _T(" /s \"")+ m_dir + _T("codecs\\Real\\aero\"") , NULL, SW_HIDE);
	::SetCurrentDirectory(szCurPath);

	if(CheckRealThread == NULL)
	{
		CheckRealThread = AfxBeginThread(CheckThread,this);
	}
}

void CMOtherPage::OnButtonDonline() 
{
	// TODO: Add your control notification handler code here
	if(!CheckRealOnline())
	{
		MessageBox(ResStr(IDS_OTHER_REALON),ResStr(IDS_OTHER_REALONLINE));
		return;
	}
	UpdateData(TRUE);
	TCHAR szFilePath[MAX_PATH + 1];
	CString m_dir;
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_dir.Format(_T("%s"),szFilePath);

	TCHAR szCurPath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szCurPath);
	::SetCurrentDirectory(szFilePath);
	CString m_sysdir;
	TCHAR szSystemPath[MAX_PATH + 1];
	::GetSystemDirectory(szSystemPath,MAX_PATH);
	m_sysdir.Format(_T("%s\\"),szSystemPath);
	
	if( !IsFileExist(m_dir + _T("codecs\\Real\\unrealreg")) ||
		( !IsFileExist(m_sysdir + _T("rmoc3260.dll")) &&
		!IsFileExist(m_dir + _T("codecs\\Real\\rmoc3260.dll")) ) )
	{
		MessageBox(ResStr(IDS_OTHER_UNREALFAIL),ResStr(IDS_OTHER_REALONLINE));
		return;
	}
	if(IsFileExist(m_sysdir + _T("rmoc3260.dll")))
		ShellExecute(0, _T("open"), _T("regsvr32.exe") , _T(" /u /s \"")+ m_sysdir + _T("rmoc3260.dll\"") , NULL, SW_HIDE);
	else
		ShellExecute(0, _T("open"), _T("regsvr32.exe") , _T(" /u /s \"")+ m_dir + _T("codecs\\Real\\rmoc3260.dll\"") , NULL, SW_HIDE);
	DeleteFile(m_sysdir +_T("rmoc3260.dll"));
	CopyFile(m_dir +_T("codecs\\Real\\unrealreg") , m_dir +_T("unrealreg.inf") , TRUE);
	WinExec("rundll32.exe setupapi,InstallHinfSection DefaultInstall 128 .\\unrealreg.inf",SW_HIDE);
	DeleteFile(m_dir +_T("unrealreg.inf"));
	::SetCurrentDirectory(szCurPath);
	if(!CheckRealOnline())
		MessageBox(ResStr(IDS_OTHER_UNREALOK),ResStr(IDS_OTHER_REALONLINE));
	else
		MessageBox(ResStr(IDS_OTHER_UNREALFAILS),ResStr(IDS_OTHER_REALONLINE));
}

void CMOtherPage::OnRadioMplayer() 
{
	// TODO: Add your control notification handler code here
	m_meditor.SetCheck(0);
}

void CMOtherPage::OnRadioMeditor() 
{
	// TODO: Add your control notification handler code here
	m_mplayer.SetCheck(0);
}
void CMOtherPage::OnBnClickedButtonLink()
{
	CMLinkDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		HRESULT hr = CoInitialize(NULL);
		if (SUCCEEDED(hr))
		{
			HRESULT hres;
			IShellLink* psl;
			IPersistFile* ppf;
			TCHAR szPath[MAX_PATH + 1];
			GetModuleFileName(NULL, szPath, MAX_PATH);
			CString meditor_exe(szPath);
			CString mplayer_exe(m_program_dir + _T("mplayer.exe"));
			CString DeskTop_dir;
			CString StartMenu_dir;
			CString LinkPath;
			if(GetSpecialFolder(CSIDL_DESKTOPDIRECTORY , szPath))
				DeskTop_dir.Format(_T("%s\\") , szPath);

			if(GetSpecialFolder(CSIDL_PROGRAMS , szPath))
			{
				CString dir;
				dir.Format(_T("%s\\MPlayer") , szPath);
				if(dlg.m_meditor_start || dlg.m_mplayer_start || dlg.m_tools_start
					 || dlg.m_flash_start  || dlg.m_dshow_start)
				{
					CreateDirectory(dir,NULL);
					StartMenu_dir.Format(_T("%s\\") , dir);
					dir.Format(_T("%s\\MPlayer\\Tools") , szPath);
					if(dlg.m_tools_start && IsFileExist(m_program_dir + _T("tools")))
						CreateDirectory(dir,NULL);
					else
						DeleteFolder(dir);
				}
				else
					DeleteFolder(dir);
			}
			hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
			if(! FAILED(hres))
			{
				psl->SetPath(meditor_exe);
				hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
				if(! FAILED(hres) )
				{
					if(DeskTop_dir.GetLength() > 3)
					{
						LinkPath.Format(_T("%s%s.lnk") , DeskTop_dir, ResStr(IDS_OTHER_LINK_EDITOR));
						if(dlg.m_meditor_desk)
							ppf->Save(LinkPath, STGM_READWRITE);
						else
							DeleteFile(LinkPath);
					}
					if(StartMenu_dir.GetLength() > 3)
					{
						LinkPath.Format(_T("%s%s.lnk") , StartMenu_dir, ResStr(IDS_OTHER_LINK_EDITOR));
						if(dlg.m_meditor_start)
							ppf->Save(LinkPath, STGM_READWRITE);
						else
							DeleteFile(LinkPath);

						LinkPath.Format(_T("%sFlash %s.lnk") , StartMenu_dir, ResStr(IDS_OTHER_LINK_PLAYER));
						if(dlg.m_flash_start)
						{
							psl->SetArguments(_T("--Open FlashPlayer"));
							psl->SetIconLocation(meditor_exe, 1);
							ppf->Save(LinkPath, STGM_READWRITE);
						}
						else
							DeleteFile(LinkPath);

						LinkPath.Format(_T("%sDirectShow %s.lnk") , StartMenu_dir, ResStr(IDS_OTHER_LINK_PLAYER));
						if(dlg.m_dshow_start)
						{
							psl->SetArguments(_T("--Open MediaPlayer"));
							psl->SetIconLocation(meditor_exe, 2);
							ppf->Save(LinkPath, STGM_READWRITE);
						}
						else
							DeleteFile(LinkPath);
					}
				}
			}

			ppf->Release();
			psl->Release();
			hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
			if( ! FAILED(hres))
			{
				psl->SetPath(mplayer_exe);
				hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
				if(! FAILED(hres) && StartMenu_dir.GetLength() > 5)
				{
					if(DeskTop_dir.GetLength() > 3)
					{
						LinkPath.Format(_T("%sMPlayer.lnk") , DeskTop_dir);
						if(dlg.m_mplayer_desk && IsFileExist(mplayer_exe))
							ppf->Save(LinkPath, STGM_READWRITE);
						else
							DeleteFile(LinkPath);
					}
					if(StartMenu_dir.GetLength() > 3)
					{
						LinkPath.Format(_T("%sMPlayer.lnk") , StartMenu_dir);
						if(dlg.m_mplayer_start && IsFileExist(mplayer_exe))
							ppf->Save(LinkPath, STGM_READWRITE);
						else
							DeleteFile(LinkPath);
					}
				}
			}

			CString tools_name;
			if(dlg.m_mpc_start)
			{
				tools_name.Format(_T("%stools\\mplayerc.exe") , m_program_dir);
				LinkPath.Format(_T("%sMedia Player Classic.lnk") , StartMenu_dir);
				if( IsFileExist(tools_name))
				{
					ppf->Release();
					psl->Release();
					hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
					if(! FAILED(hres))
					{
						psl->SetPath(tools_name);
						hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
						if(! FAILED(hres))
							ppf->Save(LinkPath, STGM_READWRITE);
					}
				}
				else
					DeleteFile(LinkPath);
			}

			if(dlg.m_tools_start && IsFileExist(m_program_dir + _T("tools")) && StartMenu_dir.GetLength() > 3)
			{
				for(int tools_i = 1 ; tools_i <= 8 ; tools_i++)
				{
					switch(tools_i)
					{
					case 1:
						tools_name.Format(_T("%stools\\mmg.exe") , m_program_dir);
						LinkPath.Format(_T("%sTools\\%s.lnk") , StartMenu_dir , ResStr(IDS_OTHER_TOOL1));
						break;
					case 2:
						tools_name.Format(_T("%stools\\MKVextractGUI.exe") , m_program_dir);
						LinkPath.Format(_T("%sTools\\%s.lnk") , StartMenu_dir , ResStr(IDS_OTHER_TOOL2));
						break;
					case 3:
						tools_name.Format(_T("%stools\\flvmdigui.exe") , m_program_dir);
						LinkPath.Format(_T("%sTools\\%s.lnk") , StartMenu_dir , ResStr(IDS_OTHER_TOOL3));
						break;
					case 4:
						tools_name.Format(_T("%stools\\FLVExtract.exe") , m_program_dir);
						LinkPath.Format(_T("%sTools\\%s.lnk") , StartMenu_dir , ResStr(IDS_OTHER_TOOL4));
						break;
					case 5:
						tools_name.Format(_T("%stools\\GSpot.exe") , m_program_dir);
						LinkPath.Format(_T("%sTools\\%s.lnk") , StartMenu_dir , ResStr(IDS_OTHER_TOOL5));
						break;
					case 6:
						tools_name.Format(_T("%stools\\MediaInfo.exe") , m_program_dir);
						LinkPath.Format(_T("%sTools\\%s.lnk") , StartMenu_dir , ResStr(IDS_OTHER_TOOL6));
						break;
					case 7:
						tools_name.Format(_T("%stools\\pmp_demuxer_gui.exe") , m_program_dir);
						LinkPath.Format(_T("%sTools\\%s.lnk") , StartMenu_dir , ResStr(IDS_OTHER_TOOL7));
						break;
					case 8:
						tools_name.Format(_T("%stools\\TSSplitter.exe") , m_program_dir);
						LinkPath.Format(_T("%sTools\\%s.lnk") , StartMenu_dir , ResStr(IDS_OTHER_TOOL8));
						break;
					default:
						tools_name = _T("");
						break;
					}
					if( IsFileExist(tools_name))
					{
						ppf->Release();
						psl->Release();
						hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
						if(! FAILED(hres))
						{
							psl->SetPath(tools_name);
							hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
							if(! FAILED(hres))
								ppf->Save(LinkPath, STGM_READWRITE);
						}
					}
					else
						DeleteFile(LinkPath);
				}
			}
			ppf->Release();
			psl->Release();
			CoUninitialize();
		}
	}
}

void CMOtherPage::OnBnClickedButtonAvs()
{
	TCHAR szFilePath[MAX_PATH + 1];
	TCHAR szSystemPath[MAX_PATH + 1];
	CString m_dir, m_sysdir;
	::GetSystemDirectory(szSystemPath,MAX_PATH);
	m_sysdir.Format(_T("%s\\"),szSystemPath);
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_dir.Format(_T("%s"),szFilePath);

	if(!IsFileExist(m_dir + _T("codecs\\AviSynth\\AviSynth.7z"))
		|| !IsFileExist(m_dir + _T("codecs\\AviSynth\\plugins\\DirectShowSource.dll")))
		return;

	Decode7zFile(m_dir + _T("codecs\\AviSynth\\AviSynth.7z"), m_sysdir);

	CReg reg;
	CString SubKey, Name, Content;

	SubKey = _T("SOFTWARE\\AviSynth");
	Name =  _T("plugindir2_5");
	Content = m_dir + _T("codecs\\AviSynth\\plugins");
	if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE,SubKey,Name))
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name, Content);
	else{
		Content = reg.content;
		if(!IsFileExist(Content + _T("\\DirectShowSource.dll")))
			CopyFile(m_dir + _T("codecs\\AviSynth\\plugins\\DirectShowSource.dll")
				, Content + _T("\\DirectShowSource.dll"), TRUE);
	}

	SubKey = _T("SOFTWARE\\AviSynth");
	Name =  _T("");
	Content = m_dir + _T("codecs\\AviSynth");
	if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE,SubKey,Name))
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name, Content);

	SubKey =  _T("SOFTWARE\\Classes\\CLSID\\{E6D6B700-124D-11D4-86F3-DB80AFD98778}");
	Name =  _T("");
	Content = _T("AviSynth");
	reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name, Content);

	SubKey =  _T("SOFTWARE\\Classes\\CLSID\\{E6D6B700-124D-11D4-86F3-DB80AFD98778}\\InProcServer32");
	Name =  _T("");
	Content = _T("AviSynth.dll");
	reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name, Content);

	Name =  _T("ThreadingModel");
	Content = _T("Apartment");
	reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name, Content);

	MessageBox(ResStr(IDS_MESSAGE_AVS), _T("AviSynth(AVS)"));
}
