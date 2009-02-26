// MEditor2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MEditor2.h"
#include "MEditor2Dlg.h"
#include "MShowInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMEditor2Dlg 对话框




CMEditor2Dlg::CMEditor2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMEditor2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
	gUniqueEvent = NULL;
}

void CMEditor2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabSheet);
	DDX_Control(pDX, IDC_PROGRESS_APPLEY, m_progress_apply);
}

BEGIN_MESSAGE_MAP(CMEditor2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_SHOWHELP, OnBnClickedHelp)
	ON_WM_TIMER()	
	ON_COMMAND(IDM_NORMAL, &CMEditor2Dlg::OnNormal)
	ON_COMMAND(IDM_HIGH, &CMEditor2Dlg::OnHigh)
	ON_COMMAND(IDM_LOWER, &CMEditor2Dlg::OnLower)
	ON_COMMAND(ID_MPLAYER_INI, &CMEditor2Dlg::OnMplayerIni)
	ON_COMMAND(ID_INPUT_INI, &CMEditor2Dlg::OnInputIni)
	ON_COMMAND(ID_KK_INI, &CMEditor2Dlg::OnKkIni)
	ON_COMMAND(ID_LOG_TXT, &CMEditor2Dlg::OnLogTxt)
	ON_COMMAND(IDM_ABOUT, &CMEditor2Dlg::OnAbout)
	ON_COMMAND(IDC_GOTOHOME, &CMEditor2Dlg::OnGotohome)
	ON_COMMAND(IDC_SHOWLOG, &CMEditor2Dlg::OnShowlog)
	ON_COMMAND(IDC_SHOW_HELP, &CMEditor2Dlg::OnShowHelp)
	ON_COMMAND(IDC_SHOWFAQ, &CMEditor2Dlg::OnShowfaq)
	ON_COMMAND(IDC_RESETINFO, &CMEditor2Dlg::OnResetinfo)
END_MESSAGE_MAP()


// CMEditor2Dlg 消息处理程序

BOOL CMEditor2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_config.LoadConfig(m_program_dir + _T("kk.ini"),true);
	bool value_b;
	if(m_config.GetValue_Boolean(_T("meditor_one_editor"),value_b,true))
	{
		if(value_b)
		{
//			gUniqueEvent = CreateEvent(NULL, TRUE, TRUE, _T("MEditorII - MPlayer 首选项"));
//			if(GetLastError() == ERROR_ALREADY_EXISTS)
			CWnd *m_wnd = FindWindow(NULL, ResStr(IDS_PLAYER_NAME) + _T("    "));
			if(m_wnd != NULL)
			{
				CWnd *pWndChild = m_wnd->GetLastActivePopup();
				if (m_wnd->IsIconic())
					m_wnd->ShowWindow(SW_RESTORE);
				pWndChild->SetForegroundWindow();
				OnCancel();
				return FALSE;
			}
		}
	}

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码

	SetWindowText(ResStr(IDS_PLAYER_NAME) + _T("    "));
	m_config.LoadConfig(m_program_dir + _T("mplayer.ini"));

	m_player.m_cfg = &m_config;
	m_video.m_cfg = &m_config;
	m_audio.m_cfg = &m_config;
	m_subtitle.m_cfg = &m_config;
	m_decode.m_cfg = &m_config;
	m_resume.m_cfg = &m_config;
	m_other.m_cfg = &m_config;
	m_profile.m_cfg = &m_config;
	m_assos.m_cfg = &m_config;
	infoDlg.SetConfig(&m_config);

	m_player.SetInfoDlg(&infoDlg);
	m_video.SetInfoDlg(&infoDlg);
	m_subtitle.SetInfoDlg(&infoDlg);

	CString p_str = ResStr(IDS_TAB_PLAYER);
	CString v_str = ResStr(IDS_TAB_VIDEO);
	CString a_str = ResStr(IDS_TAB_AUDIO);
	CString sub_str = ResStr(IDS_TAB_SUBTITLE);
	CString d_str = ResStr(IDS_TAB_DECODE);
	CString r_str = ResStr(IDS_TAB_RESUME);
	CString o_str = ResStr(IDS_TAB_OTHER);
	CString f_str = ResStr(IDS_TAB_PROFILE);
	CString i_str = ResStr(IDS_TAB_INPUT);
	CString s_str = ResStr(IDS_TAB_ASSOS);

	int PlayerPage = IDD_PLAYER_DIALOG;
	int VideoPage = IDD_VIDEO_DIALOG;
	int AudioPage = IDD_AUDIO_DIALOG;
	int SubtitlePage = IDD_SUBTITLE_DIALOG;
	int DecodePage = IDD_DECODE_DIALOG;
	int ResumePage = IDD_RESUME_DIALOG;
	int OtherPage = IDD_OTHER_DIALOG;
	int ProfilePage = IDD_PROFILE_DIALOG;
	int InputPage = IDD_INPUT_DIALOG;
	int AssosPage = IDD_ASSOS_DIALOG;
	if(theApp.AppLanguage == 2){
		PlayerPage = IDD_PLAYER_DIALOG_EN;
		VideoPage = IDD_VIDEO_DIALOG_EN;
		AudioPage = IDD_AUDIO_DIALOG_EN;
		SubtitlePage = IDD_SUBTITLE_DIALOG_EN;
		DecodePage = IDD_DECODE_DIALOG_EN;
		ResumePage = IDD_RESUME_DIALOG_EN;
		OtherPage = IDD_OTHER_DIALOG_EN;
		ProfilePage = IDD_PROFILE_DIALOG_EN;
		InputPage = IDD_INPUT_DIALOG_EN;
		AssosPage = IDD_ASSOS_DIALOG_EN;
	} else if (theApp.AppLanguage == 3 && theApp.AppLanguage == 4) {
		PlayerPage = IDD_PLAYER_DIALOG_TC;
		VideoPage = IDD_VIDEO_DIALOG_TC;
		AudioPage = IDD_AUDIO_DIALOG_TC;
		SubtitlePage = IDD_SUBTITLE_DIALOG_TC;
		DecodePage = IDD_DECODE_DIALOG_TC;
		ResumePage = IDD_RESUME_DIALOG_TC;
		OtherPage = IDD_OTHER_DIALOG_TC;
		ProfilePage = IDD_PROFILE_DIALOG_TC;
		InputPage = IDD_INPUT_DIALOG_TC;
		AssosPage = IDD_ASSOS_DIALOG_TC;
	}
	if(theApp.hResourceHandleOld)
		AfxSetResourceHandle(theApp.hResourceHandleOld);
	m_TabSheet.AddPage(  p_str, &m_player, PlayerPage);
	m_TabSheet.AddPage(  v_str , &m_video, VideoPage);
	m_TabSheet.AddPage(  a_str , &m_audio, AudioPage);
	m_TabSheet.AddPage(  sub_str , &m_subtitle, SubtitlePage);
	m_TabSheet.AddPage( d_str , &m_decode, DecodePage);
	m_TabSheet.AddPage( r_str , &m_resume, ResumePage);
	m_TabSheet.AddPage( o_str , &m_other, OtherPage);
	m_TabSheet.AddPage( f_str , &m_profile, ProfilePage);
	m_TabSheet.AddPage( i_str , &m_Input, InputPage);
	m_TabSheet.AddPage( s_str , &m_assos, AssosPage);
	m_TabSheet.Show();
	if(theApp.hResourceHandleMod)
		AfxSetResourceHandle(theApp.hResourceHandleMod);
	
	int value_i;
	if(m_config.GetValue_Integer(_T("meditor_last_page"),value_i,true))
	{
		if(value_i >= Player && value_i <= Assos)
			m_TabSheet.SetCurSel(value_i);
	}

	if(m_OpenType == 1)
	{
		//快捷键设置
		::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		m_TabSheet.SetCurSel(Input);
	}
	else if(m_OpenType == 2)
	{
		//文件关联设置
		m_TabSheet.SetCurSel(Assos);
	}
	else if(m_OpenType == 3)
	{
		//由MPlayer调用
		::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	}

	GetDlgItem(IDC_APPLY)->SetFocus();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMEditor2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMEditor2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMEditor2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CMEditor2Dlg::SaveAll()
{
	if(!m_Input.SaveInputConfig())
		return false;
	m_player.SaveConfig();
	m_decode.SaveConfig();
	m_subtitle.SaveConfig();
	m_video.SaveConfig();
	m_audio.SaveConfig();
	m_other.SaveConfig();
	m_resume.SaveConfig();
	m_assos.SaveConfig();
	m_profile.SaveConfig();
	bool value_b;
	if(m_config.GetValue_Boolean(_T("meditor_last_page"),value_b,true))
	{
		if(value_b)
		{
			CString str;
			str.Format(_T("%d"),m_TabSheet.GetCurSel());
			m_config.SetValue(_T("meditor_last_page"),str,true,ex_meditor);
		}
		else
			m_config.RemoveValue(_T("meditor_last_page"),true);
	}
	m_config.SaveConfig(m_program_dir + _T("mplayer.ini"));
	ShowInfo(type_reload);
	m_config.SaveConfig(m_program_dir + _T("kk.ini"),true);
	return true;
}

void CMEditor2Dlg::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pos = 0;
	m_progress_apply.SetRange(0,20);
	m_progress_apply.SetPos(m_pos);
	m_progress_apply.ShowWindow(SW_SHOW);
	SetTimer(0,10,NULL);
	SaveAll();
}

void CMEditor2Dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(! SaveAll())
		return;
	m_assos.ApplyChange(true);
	OnOK();
}

void CMEditor2Dlg::OnBnClickedHelp()
{
	CString m_help = m_program_dir + _T("help");
	
	if(IsFileExist(m_help))
	{
		//switch(m_TabSheet.GetCurSel())
		//{
		//default:
			m_help += _T("/man_page.html");
		//	break;
		//}
		if(IsFileExist(m_help))
		{
			ShellExecute(0, _T("open"), m_help, _T(""), NULL, SW_SHOW);
			return;
		}
	}
	m_program_dir + _T("man_page_ww.html");
	if(IsFileExist(m_help))
	{
		ShellExecute(0, _T("open"), m_help, _T(""), NULL, SW_SHOW);
	}
	else
		ShellExecute(0, _T("open"),_T("http://mplayer-ww.sourceforge.net/") , _T(""), NULL, SW_SHOW);
}

void CMEditor2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pos++;
	if(m_pos <= 20)
		m_progress_apply.SetPos(m_pos);
	else
	{
		KillTimer(0);
		m_progress_apply.ShowWindow(SW_HIDE);
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CMEditor2Dlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(gUniqueEvent)
		CloseHandle(gUniqueEvent);

	return CDialog::DestroyWindow();
}

void CMEditor2Dlg::OnNormal()
{
	m_player.SetNormal();
	m_decode.SetNormal();
	m_video.SetNormal();
	m_audio.SetNormal();
	m_other.SetNormal();
	m_player.UpdateData(FALSE);
	m_decode.UpdateData(FALSE);
	m_video.UpdateData(FALSE);
	m_audio.UpdateData(FALSE);
	m_other.UpdateData(FALSE);
}

void CMEditor2Dlg::OnHigh()
{
	m_player.SetHigh();
	m_decode.SetHigh();
	m_video.SetHigh();
	m_audio.SetHigh();
	m_other.SetHigh();
	m_player.UpdateData(FALSE);
	m_decode.UpdateData(FALSE);
	m_video.UpdateData(FALSE);
	m_audio.UpdateData(FALSE);
	m_other.UpdateData(FALSE);
}

void CMEditor2Dlg::OnLower()
{
	m_player.SetLower();
	m_decode.SetLower();
	m_video.SetLower();
	m_audio.SetLower();
	m_other.SetLower();
	m_player.UpdateData(FALSE);
	m_decode.UpdateData(FALSE);
	m_video.UpdateData(FALSE);
	m_audio.UpdateData(FALSE);
	m_other.UpdateData(FALSE);
}

void CMEditor2Dlg::OnMplayerIni()
{
	// TODO: 在此添加命令处理程序代码
	if(IsFileExist(m_program_dir + _T("mplayer.ini")))
		ShellExecute(0, _T("open"), m_program_dir +_T("mplayer.ini"), NULL, NULL, SW_SHOW);
}

void CMEditor2Dlg::OnInputIni()
{
	// TODO: 在此添加命令处理程序代码
	if(IsFileExist(m_program_dir + _T("input.ini")))
		ShellExecute(0, _T("open"), m_program_dir +_T("input.ini"), NULL, NULL, SW_SHOW);
}

void CMEditor2Dlg::OnKkIni()
{
	// TODO: 在此添加命令处理程序代码
	if(IsFileExist(m_program_dir + _T("kk.ini")))
		ShellExecute(0, _T("open"), m_program_dir +_T("kk.ini"), NULL, NULL, SW_SHOW);
}

void CMEditor2Dlg::OnLogTxt()
{
	// TODO: 在此添加命令处理程序代码
	if(IsFileExist(m_program_dir + _T("log.txt")))
		ShellExecute(0, _T("open"), m_program_dir +_T("log.txt"), NULL, NULL, SW_SHOW);
}

void CMEditor2Dlg::OnAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CMEditor2Dlg::OnGotohome()
{
	ShellExecute(0, _T("open"),_T("http://mplayer-ww.sourceforge.net/") , _T(""), NULL, SW_SHOW);
}

void CMEditor2Dlg::OnShowlog()
{
	CString m_help = m_program_dir + _T("help/changelog.html");
	if(IsFileExist(m_help))
	{
		ShellExecute(0, _T("open"), m_help, _T(""), NULL, SW_SHOW);
		return;
	}
	m_program_dir + _T("man_page_ww.html");
	if(IsFileExist(m_help))
	{
		ShellExecute(0, _T("open"), m_help, _T(""), NULL, SW_SHOW);
	}
	else
		ShellExecute(0, _T("open"),_T("http://mplayer-ww.sourceforge.net/") , _T(""), NULL, SW_SHOW);
}

void CMEditor2Dlg::OnShowHelp()
{

	CString m_help = m_program_dir + _T("help/default.html");
	if(IsFileExist(m_help))
	{
		ShellExecute(0, _T("open"), m_help, _T(""), NULL, SW_SHOW);
		return;
	}
	m_program_dir + _T("man_page_ww.html");
	if(IsFileExist(m_help))
	{
		ShellExecute(0, _T("open"), m_help, _T(""), NULL, SW_SHOW);
	}
	else
		ShellExecute(0, _T("open"),_T("http://mplayer-ww.sourceforge.net/") , _T(""), NULL, SW_SHOW);
}

void CMEditor2Dlg::OnShowfaq()
{

	CString m_help = m_program_dir + _T("help/faq.html");
	if(IsFileExist(m_help))
	{
		ShellExecute(0, _T("open"), m_help, _T(""), NULL, SW_SHOW);
		return;
	}
	m_program_dir + _T("man_page_ww.html");
	if(IsFileExist(m_help))
	{
		ShellExecute(0, _T("open"), m_help, _T(""), NULL, SW_SHOW);
	}
	else
		ShellExecute(0, _T("open"),_T("http://mplayer-ww.sourceforge.net/") , _T(""), NULL, SW_SHOW);
}

void CMEditor2Dlg::OnResetinfo()
{
	infoDlg.ResetInfo();
	m_config.RemoveValue(_T("meditor_hide_info"),true);
}

void CMEditor2Dlg::ShowInfo(int type)
{
	if(infoDlg.IsShow(type))
		infoDlg.DoModal();
}
