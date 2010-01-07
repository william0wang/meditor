// MPlayerPage.cpp : implementation file
//

#include "stdafx.h"
#include "meditor2.h"
#include "MPlayerPage.h"
#include "MConfig.h"
#include "MShowInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPlayerPage dialog


CMPlayerPage::CMPlayerPage(CWnd* pParent /*=NULL*/)
	: CDialog(CMPlayerPage::IDD, pParent)
	, m_htimer(FALSE)
	, m_rightmenu(FALSE)
	, m_gui_high(FALSE)
	, m_fixedvo(TRUE)
	, m_reload(FALSE)
	, m_no_dvdnav(FALSE)
	, m_def(_T(""))
	, m_filename(TRUE)
	, m_console(FALSE)
	, m_seekrt(TRUE)
	, m_bskin(TRUE)
	, m_ctlskin(TRUE)
	, m_alpha_control(TRUE)
{
	m_cfg = NULL;
	m_fullscreen = FALSE;
	m_guithread = FALSE;
	m_menu = FALSE;
	m_oneplayer = FALSE;
	m_quit = TRUE;
	m_show = FALSE;
	m_url = FALSE;
	m_dvd = _T("G:");
	m_end = _T("0:0:0");
	m_png = _T("");
	m_start = _T("0:0:0");
	m_monitor_s = _T("");
	m_quiet = TRUE;
	m_double = TRUE;
	m_colorkey_s = _T("");
	m_conf = FALSE;
	m_auto_fuzziness = _T("1");

	m_theme_s = ResStr(IDS_PLAYER_THEME);
	m_auto_s = ResStr(IDS_PLAYER_AUTO);
	m_no_s = ResStr(IDS_PLAYER_NO);

	m_str_language.Add(ResStr(IDS_PLAYER_AUTOSEL));
	m_str_language.Add(_T("简体中文"));
	m_str_language.Add(_T("English"));
	m_str_language.Add(_T("繁體中文"));
	m_str_language.Add(_T("繁體中文 BIG5"));

	m_str_ontop.Add(ResStr(IDS_PLAYER_NONE));
	m_str_ontop.Add(ResStr(IDS_PLAYER_ALLWAYS));
	m_str_ontop.Add(ResStr(IDS_PLAYER_PLAYING));

	m_str_autosync.Add(ResStr(IDS_PLAYER_AS_NO));
	m_str_autosync.Add(ResStr(IDS_PLAYER_AS_NORMAL));
	m_str_autosync.Add(ResStr(IDS_PLAYER_AS_FAST));
	m_str_autosync.Add(ResStr(IDS_PLAYER_AS_FLV));
	m_str_autosync.Add(ResStr(IDS_PLAYER_AS_FLVEX));
	m_str_autosync.Add(ResStr(IDS_PLAYER_AS_AVI));

	m_str_priority.Add(m_auto_s);
	m_str_priority.Add(ResStr(IDS_PLAYER_PREALTIME));
	m_str_priority.Add(ResStr(IDS_PLAYER_PHIGH));
	m_str_priority.Add(ResStr(IDS_PLAYER_PHNORMAL));
	m_str_priority.Add(ResStr(IDS_PLAYER_PNORMAL));
	m_str_priority.Add(ResStr(IDS_PLAYER_PLNORMAL));
	m_str_priority.Add(ResStr(IDS_PLAYER_PIDLE));

	m_str_loop.Add(ResStr(IDS_PLAYER_NONE));
	m_str_loop.Add(ResStr(IDS_PLAYER_FILE));
	m_str_loop.Add(ResStr(IDS_PLAYER_LIST));

	m_str_log.Add(ResStr(IDS_PLAYER_LOG_NO));
	m_str_log.Add(ResStr(IDS_PLAYER_LOG));
	m_str_log.Add(ResStr(IDS_PLAYER_LOG_V1));
	m_str_log.Add(ResStr(IDS_PLAYER_LOG_V2));
	m_str_log.Add(ResStr(IDS_PLAYER_LOG_V3));
	m_str_log.Add(ResStr(IDS_PLAYER_LOG_V4));
	m_str_log.Add(ResStr(IDS_PLAYER_LOG_V5));

	m_str_switchview.Add(ResStr(IDS_PLAYER_HIDE_NO));
	m_str_switchview.Add(ResStr(IDS_PLAYER_HIDE_M));
	m_str_switchview.Add(ResStr(IDS_PLAYER_HIDE_A));

	m_str_systray.Add(ResStr(IDS_PLAYER_TASK));
	m_str_systray.Add(ResStr(IDS_PLAYER_TRAY));
	m_str_systray.Add(ResStr(IDS_PLAYER_TRAYSTOP));

	m_str_autoplay.Add(ResStr(IDS_PLAYER_NOUSE));
	m_str_autoplay.Add(m_auto_s);
	m_str_autoplay.Add(ResStr(IDS_PLAYER_EX));

}

void CMPlayerPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMPlayerPage)
	DDX_Control(pDX, IDC_COMBO_AUTOPLAY, m_autoplay);
	DDX_Control(pDX, IDC_COMBO_SYSTRAY, m_systray);
	DDX_Control(pDX, IDC_COMBO_VIEW, m_switchview);
	DDX_Control(pDX, IDC_COMBO_CACHE, m_cache);
	DDX_Control(pDX, IDC_COMBO_COLORKEY, m_colorkey);
	DDX_Control(pDX, IDC_COMBO_AUTOSYNC, m_autosync);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_language);
	DDX_Control(pDX, IDC_COMBO_LOG, m_log);
	DDX_Control(pDX, IDC_COMBO_LOOP, m_loop);
	DDX_Control(pDX, IDC_COMBO_MONITOR, m_monitor);
	DDX_Control(pDX, IDC_COMBO_ONTOP, m_ontop);
	DDX_Control(pDX, IDC_COMBO_PRIORITY, m_priority);
	DDX_Control(pDX, IDC_COMBO_CTRLBAR, m_ctrlbar);
	DDX_Control(pDX, IDC_COMBO_ALPHA, m_alpha);
	DDX_AMCBString(pDX, IDC_COMBO_MONITOR, m_monitor_s);
	DDX_AMCBString(pDX, IDC_COMBO_COLORKEY, m_colorkey_s);
	DDX_AMCBString(pDX, IDC_COMBO_CTRLBAR, m_ctrlbar_s);
	DDX_Control(pDX, IDC_EDIT_FUZZINESS, m_fuzziness);
	DDX_Check(pDX, IDC_CHECK_FULLSCREEN, m_fullscreen);
	DDX_Check(pDX, IDC_CHECK_GUITHREAD, m_guithread);
	DDX_Check(pDX, IDC_CHECK_MENU, m_menu);
	DDX_Check(pDX, IDC_CHECK_ONLYONE, m_oneplayer);
	DDX_Check(pDX, IDC_CHECK_QUIT, m_quit);
	DDX_Check(pDX, IDC_CHECK_SHOW, m_show);
	DDX_Check(pDX, IDC_CHECK_URL, m_url);
	DDX_Text(pDX, IDC_EDIT_DVD, m_dvd);
	DDX_Text(pDX, IDC_EDIT_END, m_end);
	DDX_Text(pDX, IDC_EDIT_PNG, m_png);
	DDX_Text(pDX, IDC_EDIT_START, m_start);
	DDX_Check(pDX, IDC_CHECK_QUIET, m_quiet);
	DDX_Check(pDX, IDC_CHECK_DOUBLE, m_double);
	DDX_Check(pDX, IDC_CHECK_CONF, m_conf);
	DDX_Text(pDX, IDC_EDIT_FUZZINESS, m_auto_fuzziness);
	DDV_MaxChars(pDX, m_auto_fuzziness, 3);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_HTIMER, m_htimer);
	DDX_Check(pDX, IDC_CHECK_RIGHTMENU, m_rightmenu);
	DDX_Check(pDX, IDC_CHECK_GUI, m_gui_high);
	DDX_Check(pDX, IDC_CHECK_BOOST, m_fixedvo);
	DDX_Check(pDX, IDC_CHECK_RELOAD, m_reload);
	DDX_Check(pDX, IDC_CHECK_DVDNAV, m_no_dvdnav);
	DDX_Text(pDX, IDC_EDIT_DEF, m_def);
	DDX_Check(pDX, IDC_CHECK_TITLE, m_filename);
	DDX_Check(pDX, IDC_CHECK_CONSOLE, m_console);
	DDX_Check(pDX, IDC_CHECK_RTSEEK, m_seekrt);
	DDX_Check(pDX, IDC_CHECK_BSKIN, m_bskin);
	DDX_Check(pDX, IDC_CHECK_CTLSKIN, m_ctlskin);
	DDX_Check(pDX, IDC_CHECK_CTLALPHA, m_alpha_control);
}


BEGIN_MESSAGE_MAP(CMPlayerPage, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PNG, OnButtonPng)
	ON_BN_CLICKED(IDC_BUTTON_DVD, OnButtonDvd)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTOPLAY, OnSelchangeAutoplay)
	ON_BN_CLICKED(IDC_BUTTON_DEF, &CMPlayerPage::OnBnClickedButtonDef)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPlayerPage message handlers

void CMPlayerPage::OnButtonPng() 
{
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);
	CWnd* pMainCWnd = NULL; 
	pMainCWnd = GetActiveWindow(); 
	CString csFolder = _T( "" );
	HWND pMainHWnd = pMainCWnd->GetSafeHwnd();
	if(SelectFolder( pMainHWnd, csFolder ))
		m_png = csFolder;
	::SetCurrentDirectory(szFilePath);
	UpdateData(FALSE);
}

void CMPlayerPage::OnButtonDvd() 
{
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);
	
	CWnd* pMainCWnd = NULL; 
	pMainCWnd = GetActiveWindow(); 
	CString csFolder = _T( "" );
	HWND pMainHWnd = pMainCWnd->GetSafeHwnd();
	if(SelectFolder( pMainHWnd, csFolder ))
		m_dvd = csFolder;
	::SetCurrentDirectory(szFilePath);
	UpdateData(FALSE);
	
}

BOOL CMPlayerPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for(int i = 0; i < m_str_language.GetCount(); i++)
		m_language.AddString(m_str_language[i]);
	m_language.SetCurSel(lang_auto);

	m_cache.AddString(m_auto_s);
	m_cache.AddString(_T("512 KB"));
	m_cache.AddString(_T("1 M"));
	m_cache.AddString(_T("2 M"));
	m_cache.AddString(_T("4 M"));
	m_cache.AddString(_T("8 M"));
	m_cache.AddString(_T("16 M"));
	m_cache.AddString(_T("32 M"));
	m_cache.SetCurSel(0);

	for(int i = 0; i < m_str_ontop.GetCount(); i++)
		m_ontop.AddString(m_str_ontop[i]);
	m_ontop.SetCurSel(top_none);

	m_colorkey.AddString(m_no_s);
	m_colorkey.AddString(_T("0x101010"));
	m_colorkey_s = _T("0x101010");

	for(int i = 0; i < m_str_autosync.GetCount(); i++)
		m_autosync.AddString(m_str_autosync[i]);
	m_autosync.SetCurSel(sync_none);

	for(int i = 0; i < m_str_priority.GetCount(); i++)
		m_priority.AddString(m_str_priority[i]);
	m_priority.SetCurSel(prio_auto);

	for(int i = 0; i < m_str_loop.GetCount(); i++)
		m_loop.AddString(m_str_loop[i]);
	m_loop.SetCurSel(loop_none);

	m_monitor.AddString(m_auto_s);
	m_monitor.AddString(_T("4:3"));
	m_monitor.AddString(_T("8:5"));
	m_monitor.AddString(_T("16:9"));
	m_monitor_s = m_auto_s;

	m_alpha.AddString(m_no_s);
	CString tmp;
	for (int i = 1; i < 256; i++)
	{
		tmp.Format(_T("%d"), i);
		m_alpha.AddString(tmp);
	}
	m_alpha.SetCurSel(0);

	CString m_skin_dir;
	TCHAR szCurPath[MAX_PATH + 1];
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szCurPath);
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_skin_dir.Format(_T("%sskin"),szFilePath);
	::SetCurrentDirectory(m_skin_dir);
	m_ctrlbar.AddString(m_no_s);
	m_ctrlbar.AddString(m_theme_s);

	CFileFind finder;
	if(finder.FindFile(_T("*.*"),0))
	{
		while(finder.FindNextFile())
		{
			if(finder.IsDirectory() && !finder.IsDots())
				m_ctrlbar.AddString(finder.GetFileName().MakeLower());
		}
		CString str = finder.GetFileName();
		if(finder.IsDirectory() && !finder.IsDots() && str.GetLength() > 1) {
			m_ctrlbar.AddString(str);
		}
	}
	::SetCurrentDirectory(szCurPath);
	int indexCtrl = m_ctrlbar.FindStringExact(0, _T("default"));
	if(indexCtrl > 1) {
		m_ctrlbar.SetCurSel(indexCtrl);
		m_ctrlbar_s = _T("default");
	} else {
		m_ctrlbar.SetCurSel(1);
		m_ctrlbar_s = m_theme_s;
	}
	m_ctrlbar.UpdateData(FALSE);

	for(int i = 0; i < m_str_log.GetCount(); i++)
		m_log.AddString(m_str_log[i]);
	m_log.SetCurSel(log_none);

	for(int i = 0; i < m_str_switchview.GetCount(); i++)
		m_switchview.AddString(m_str_switchview[i]);
	m_switchview.SetCurSel(switch_none);
	
	for(int i = 0; i < m_str_systray.GetCount(); i++)
		m_systray.AddString(m_str_systray[i]);
	m_systray.SetCurSel(sys_task);
	
	for(int i = 0; i < m_str_autoplay.GetCount(); i++)
		m_autoplay.AddString(m_str_autoplay[i]);
	m_autoplay.SetCurSel(auto_ex);

	InitFromConfig();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMPlayerPage::SetNormal()
{
	m_gui_high = FALSE;
	m_fixedvo = TRUE;
	m_reload = FALSE;
	m_no_dvdnav = FALSE;
	m_menu = FALSE;
	m_fullscreen = FALSE;
	m_guithread = TRUE;
	m_oneplayer = FALSE;
	m_quit = FALSE;
	m_show = TRUE;
	m_url = TRUE;
	m_quiet = TRUE;
	m_double = TRUE;
	m_conf = FALSE;
	m_htimer= FALSE;
	m_rightmenu = FALSE;
	m_end = _T("0:0:0");
	m_png = _T("");
	m_def = _T("");
	m_start = _T("0:0:0");
	m_auto_fuzziness = _T("6");
	m_colorkey_s = _T("0x000001");
	m_monitor_s = m_auto_s;
	m_language.SetCurSel(lang_auto);
	m_cache.SetCurSel(0);
	m_ontop.SetCurSel(top_playing);
	m_autosync.SetCurSel(sync_none);
	m_priority.SetCurSel(prio_auto);
	m_loop.SetCurSel(loop_none);
	m_log.SetCurSel(log_none);	
	m_switchview.SetCurSel(switch_none);
	m_systray.SetCurSel(sys_task);	
	m_autoplay.SetCurSel(auto_ex);
}

void CMPlayerPage::SetHigh()
{
	SetNormal();
}

void CMPlayerPage::SetLower()
{
	SetNormal();
	m_priority.SetCurSel(prio_high);
	m_autosync.SetCurSel(sync_normal);
	m_auto_fuzziness = _T("1");
}

void CMPlayerPage::InitFromConfig()
{
	if(!m_cfg)
		return;
	int value_i;
	bool value_b,auto_play_b = false;
	double value_d;
	CString value_s;

	if(m_cfg->GetValue_Boolean(_T("use-filedir-conf"),value_b))
	{
		if(value_b)
			m_conf = TRUE;
		else
			m_conf = FALSE;
	}
	if(m_cfg->GetValue_String(_T("colorkey"),value_s))
	{
		if(value_s != _T("0x101010"))
			m_colorkey.AddString(value_s);
		m_colorkey_s = value_s;
	}
	if(m_cfg->GetValue_Boolean(_T("nocolorkey"),value_b))
	{
		m_colorkey_s = m_no_s;
	}
	if(m_cfg->GetValue_Boolean(_T("double"),value_b))
	{
		if(value_b)
			m_double = TRUE;
		else
			m_double = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("quiet"),value_b))
	{
		if(value_b)
			m_quiet = TRUE;
		else
			m_quiet = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("autoplay"),value_b))
	{
		if(value_b)
		{
			auto_play_b = true;
			m_autoplay.SetCurSel(auto_normal);
		}
		else
			m_autoplay.SetCurSel(auto_none);
	}
	else
		m_autoplay.SetCurSel(auto_none);

	if(m_cfg->GetValue_Integer(_T("autoplay_fuzziness"),value_i,true))
	{
		if(value_i > 0)
		{
			if(auto_play_b)
			{
				m_autoplay.SetCurSel(auto_ex);
				m_fuzziness.EnableWindow(TRUE);
			}
			m_auto_fuzziness.Format(_T("%d"),value_i);
		}
		else
			m_auto_fuzziness = _T("0");
	}
	else if(auto_play_b)
	{
		m_autoplay.SetCurSel(auto_ex);
		m_fuzziness.EnableWindow(TRUE);
	}

	if(m_cfg->GetValue_Integer(_T("switchview"),value_i))
	{
		switch (value_i)
		{
		case 1:
			m_switchview.SetCurSel(switch_manual);
			break;
		case 2:
			m_switchview.SetCurSel(switch_auto);
			break;
		default:
			m_switchview.SetCurSel(switch_none);
		}
	}
	if(m_cfg->GetValue_Integer(_T("systray"),value_i))
	{
		switch (value_i)
		{
		case 1:
			m_systray.SetCurSel(sys_tray);
			break;
		case 2:
			m_systray.SetCurSel(sys_tray_stop);
			break;
		default:
			m_systray.SetCurSel(sys_task);
			break;
		}
	}
	if(m_cfg->GetValue_Boolean(_T("fs"),value_b))
	{
		if(value_b)
			m_fullscreen = TRUE;
		else
			m_fullscreen = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("idle"),value_b))
	{
		if(value_b)
			m_show = TRUE;
		else
			m_show = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("fixed-vo"),value_b))
	{
		if(value_b)
			m_fixedvo = TRUE;
		else
			m_fixedvo = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("use-filename-title"),value_b))
	{
		if(!value_b)
			m_filename = FALSE;
		else
			m_filename = TRUE;
	}
	if(m_cfg->GetValue_Boolean(_T("skinned_player"),value_b,true))
	{
		if(!value_b)
			m_bskin = FALSE;
		else
			m_bskin = TRUE;
	}
	if(m_cfg->GetValue_Boolean(_T("seek_realtime"),value_b,true))
	{
		if(value_b)
			m_seekrt = TRUE;
		else
			m_seekrt = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("gui_thread"),value_b,true))
	{
		if(value_b)
			m_guithread = TRUE;
		else
			m_guithread = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("gui_priority_lowest"),value_b,true))
	{
		if(!value_b)
			m_gui_high = TRUE;
		else
			m_gui_high = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("reload_when_open"),value_b,true))
	{
		if(value_b)
			m_reload = TRUE;
		else
			m_reload = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("no_dvdnav"),value_b,true))
	{
		if(value_b)
			m_no_dvdnav = TRUE;
		else
			m_no_dvdnav = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("use_rightmenu"),value_b,true))
	{
		if(value_b)
			m_rightmenu = TRUE;
		else
			m_rightmenu = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("high_accuracy_timer"),value_b,true))
	{
		if(value_b)
			m_htimer = TRUE;
		else
			m_htimer = FALSE;
	}

	if(m_cfg->GetValue_Boolean(_T("show_menubar"),value_b,true))
	{
		if(value_b)
			m_menu = TRUE;
		else
			m_menu = FALSE;
	}

	if(m_cfg->GetValue_Boolean(_T("auto_hide_control"),value_b,true))
	{
		if(!value_b)
			m_alpha_control = FALSE;
		else
			m_alpha_control = TRUE;
	}

	if(m_cfg->GetValue_Boolean(_T("skin_controlbar"),value_b,true))
	{
		if(!value_b)
			m_ctlskin = FALSE;
		else
			m_ctlskin = TRUE;
	}

	if(m_cfg->GetValue_Boolean(_T("show_controlbar"),value_b,true))
	{
		if(value_b) {
			if(m_cfg->GetValue_String(_T("skin"),value_s,true)) {
				int index = m_ctrlbar.FindStringExact(0, value_s);
				if(index < 0) {
					value_s = _T("default");
					index = m_ctrlbar.FindStringExact(0, value_s);
				}
				if(!m_ctlskin && value_s == _T("inner")) {
					m_ctrlbar.SetCurSel(1);
					m_ctrlbar_s = m_theme_s;
				} else if(index > 0) {
					m_ctrlbar.SetCurSel(index);
					m_ctrlbar_s = value_s;
				} else
					m_ctrlbar.SetCurSel(0);
			} else {
				int index = m_ctrlbar.FindStringExact(0,  _T("default"));
				if(index > 0) {
					m_ctrlbar.SetCurSel(index);
					m_ctrlbar_s = _T("default");
				} else {
					m_ctrlbar.SetCurSel(0);
					m_ctrlbar_s = m_no_s;
				}
			}
		} else {
			m_ctrlbar.SetCurSel(0);
			m_ctrlbar_s = m_no_s;
		}
		m_ctrlbar.UpdateData(FALSE);
	} else if(m_cfg->GetValue_String(_T("skin"),value_s,true)){
		int index = m_ctrlbar.FindStringExact(0, value_s);
		if(!m_ctlskin && value_s == _T("inner")) {
			m_ctrlbar.SetCurSel(1);
			m_ctrlbar_s = m_theme_s;
		} else if(index > 0) {
			m_ctrlbar.SetCurSel(index);
			m_ctrlbar_s = value_s;
		} else {
			index = m_ctrlbar.FindStringExact(0,  _T("default"));
			if(index > 0) {
				m_ctrlbar.SetCurSel(index);
				m_ctrlbar_s = _T("default");
			} else {
				m_ctrlbar.SetCurSel(0);
				m_ctrlbar_s = m_no_s;
			}
		}
		m_ctrlbar.UpdateData(FALSE);
	}
	if(m_cfg->GetValue_Boolean(_T("one_player"),value_b,true))
	{
		if(value_b)
			m_oneplayer = TRUE;
		else
			m_oneplayer = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("always_quit"),value_b,true))
	{
		if(value_b)
			m_quit = TRUE;
		else
			m_quit = FALSE;
	}
	if(m_cfg->GetValue_Integer(_T("log"),value_i,true))
	{
		if(value_i)
		{
			if(value_i == 2) m_console = TRUE;
			if(m_cfg->GetValue_Integer(_T("v"),value_i))
			{
				switch (value_i)
				{
				case 1:
					m_log.SetCurSel(log_v1);
					break;
				case 2:
					m_log.SetCurSel(log_v2);
					break;
				case 3:
					m_log.SetCurSel(log_v3);
					break;
				case 4:
					m_log.SetCurSel(log_v4);
					break;
				case 5:
					m_log.SetCurSel(log_v5);
					break;
				default:
					m_log.SetCurSel(log_normal);
				}
			}
			else
				m_log.SetCurSel(log_normal);
		}
	}

	if(m_cfg->GetValue_Integer(_T("ontop"),value_i))
	{
		switch (value_i)
		{
		case 0:
			m_ontop.SetCurSel(top_none);
			break;
		case 1:
			m_ontop.SetCurSel(top_always);
			break;
		default:
			m_ontop.SetCurSel(top_playing);
		}
	}
	if(m_cfg->GetValue_Integer(_T("cache"),value_i))
	{
		if(value_i <= 0)
			m_cache.SetCurSel(0);
		else if(value_i <= 512)
			m_cache.SetCurSel(1);
		else if(value_i <= 1024)
			m_cache.SetCurSel(2);
		else if(value_i <= 2048)
			m_cache.SetCurSel(3);
		else if(value_i <= 4096)
			m_cache.SetCurSel(4);
		else if(value_i <= 8192)
			m_cache.SetCurSel(5);
		else if(value_i <=16384)
			m_cache.SetCurSel(6);
		else
			m_cache.SetCurSel(7);
	}
	if(m_cfg->GetValue_Integer(_T("Language"),value_i,true))
	{
		switch (value_i)
		{
		case lang_auto:
			m_language.SetCurSel(lang_auto);
			break;
		case lang_sc:
			m_language.SetCurSel(lang_sc);
			break;
		case lang_en:
			m_language.SetCurSel(lang_en);
			break;
		case lang_tc:
			m_language.SetCurSel(lang_tc);
			break;
		case lang_big5:
			m_language.SetCurSel(lang_big5);
			break;
		default:
			m_language.SetCurSel(lang_auto);
		}
	}
	if(m_cfg->GetValue_Integer(_T("controlbar_alpha"),value_i,true))
	{
		if(value_i > 0 && value_i < 256)
			m_alpha.SetCurSel(value_i);
	}
	if(m_cfg->GetValue_Integer(_T("end_pos"),value_i,true))
	{
		m_end.Format(_T("%d:%d:%d"),value_i/3600,(value_i/60)%60,value_i%60 );
	}
	if(m_cfg->GetValue_Integer(_T("loop"),value_i))
	{
		if(m_cfg->GetValue_Boolean(_T("loop_all"),value_b,true))
		{
			if(value_b)
				m_loop.SetCurSel(loop_list);
			else
				m_loop.SetCurSel(loop_single);
		}
		else
			m_loop.SetCurSel(loop_single);
	}
	if(m_cfg->GetValue_Integer(_T("autosync"),value_i))
	{
		int sync = value_i;
		if(sync == 0)
		{
			if(m_cfg->GetValue_Double(_T("mc"),value_d))
			{
				if(value_d <= 0.01)
					m_autosync.SetCurSel(sync_flvex);
				else
					m_autosync.SetCurSel(sync_flv);
			}
			else if(m_cfg->GetValue_Boolean(_T("nobps"),value_b))
			{
				if(value_b)
					m_autosync.SetCurSel(sync_avi);
			}
		}
		else if(sync > 0 && sync <= 30 )
			m_autosync.SetCurSel(sync_normal);
		else if(sync > 30 )
			m_autosync.SetCurSel(sync_fast);
		else
			m_autosync.SetCurSel(sync_none);
	}

	if(m_cfg->GetValue_String(_T("urlcp"),value_s))
	{
		if(value_s == _T("GBK"))
			m_url = TRUE;
		else
			m_url = FALSE;
	}
	if(m_cfg->GetValue_String(_T("priority"),value_s))
	{
		if(value_s == _T("realtime"))
			m_priority.SetCurSel(prio_realtime);
		else if(value_s == _T("high"))
			m_priority.SetCurSel(prio_high);
		else if(value_s == _T("abovenormal"))
			m_priority.SetCurSel(prio_abovenormal);
		else if(value_s == _T("normal"))
			m_priority.SetCurSel(prio_normal);
		else if(value_s == _T("belownormal"))
			m_priority.SetCurSel(prio_belownormal);
		else if(value_s == _T("idle"))
			m_priority.SetCurSel(prio_idle);
		else
			m_priority.SetCurSel(prio_auto);
	}
	if(m_cfg->GetValue_String(_T("monitoraspect"),value_s))
	{
		m_monitor_s = value_s;
	}
	if(m_cfg->GetValue_String(_T("ss"),value_s))
	{
		m_start = value_s;
	}
	if(m_cfg->GetValue_String(_T("dvd-device"),value_s))
	{
		m_dvd = value_s;
	}
	if(m_cfg->GetValue_String(_T("default_dir"),value_s,true))
	{
		m_def = value_s;
	}
	if(m_cfg->GetValue_String(_T("screenshot_dir"),value_s,true))
	{
		m_png = value_s;
	}
	else if(m_cfg->GetValue_String(_T("ScreenShot_DIR"),value_s,true))
	{
		m_png = value_s;
		m_cfg->RemoveValue(_T("ScreenShot_DIR"), true);
	}

	if(m_cfg->GetValue_String(_T("skin"),value_s,true))
	{

	}

	UpdateData(FALSE);
}

void CMPlayerPage::SaveConfig()
{
	if(!m_cfg) return;

	UpdateData(TRUE);

	if(m_fixedvo)
		m_cfg->SetValue(_T("fixed-vo"),_T("1"));
	else
		m_cfg->SetValue(_T("fixed-vo"),_T("0"));

	if(m_conf)
		m_cfg->SetValue(_T("use-filedir-conf"),_T("1"));
	else
		m_cfg->RemoveValue(_T("use-filedir-conf"));
	
	if(m_colorkey_s == m_no_s)
	{
		m_cfg->SetValue(_T("nocolorkey"),_T("1"));
		m_cfg->RemoveValue(_T("colorkey"));
	}
	else
	{
		m_cfg->RemoveValue(_T("nocolorkey"));
		m_cfg->SetValue(_T("colorkey"), m_colorkey_s);
	}
		
	if(m_double)
		m_cfg->SetValue(_T("double"),_T("1"));
	else
		m_cfg->RemoveValue(_T("double"));
	
	if(m_quiet)
		m_cfg->SetValue(_T("quiet"),_T("1"));
	else
		m_cfg->RemoveValue(_T("quiet"));
	
	if(m_fullscreen)
		m_cfg->SetValue(_T("fs"),_T("1"));
	else
		m_cfg->RemoveValue(_T("fs"));
		
	if(m_show)
		m_cfg->SetValue(_T("idle"),_T("1"));
	else
		m_cfg->RemoveValue(_T("idle"));
	
	if(m_url)
		m_cfg->SetValue(_T("urlcp"),_T("GBK"));
	else
		m_cfg->RemoveValue(_T("urlcp"));

	if(m_filename)
		m_cfg->RemoveValue(_T("use-filename-title"));
	else
		m_cfg->SetValue(_T("use-filename-title") ,_T("0"));

	if(m_bskin)
		m_cfg->RemoveValue(_T("skinned_player"),true);
	else
		m_cfg->SetValue(_T("skinned_player") ,_T("0") , true , ex_gui);

	if(m_ctlskin)
		m_cfg->RemoveValue(_T("skin_controlbar"),true);
	else
		m_cfg->SetValue(_T("skin_controlbar") ,_T("0") , true , ex_gui);

	if(m_alpha_control)
		m_cfg->RemoveValue(_T("auto_hide_control"),true);
	else
		m_cfg->SetValue(_T("auto_hide_control") ,_T("0") , true , ex_gui);

	if(m_guithread)
		m_cfg->SetValue(_T("gui_thread") ,_T("1") , true , ex_option);
	else
		m_cfg->RemoveValue(_T("gui_thread"),true);

	if(m_gui_high)
		m_cfg->SetValue(_T("gui_priority_lowest") ,_T("0") , true,ex_gui);
	else
		m_cfg->RemoveValue(_T("gui_priority_lowest"), true );

	if(m_reload)
		m_cfg->SetValue(_T("reload_when_open") ,_T("1") , true,ex_setting);
	else
		m_cfg->RemoveValue(_T("reload_when_open"), true );

	if(m_seekrt)
		m_cfg->RemoveValue(_T("seek_realtime"), true );
	else
		m_cfg->SetValue(_T("seek_realtime") ,_T("0") , true, ex_setting);

	if(m_no_dvdnav)
		m_cfg->SetValue(_T("no_dvdnav") ,_T("1") , true,ex_setting);
	else
		m_cfg->RemoveValue(_T("no_dvdnav"), true );

	if(m_rightmenu)
		m_cfg->SetValue(_T("use_rightmenu") ,_T("1") , true , ex_gui);
	else
		m_cfg->RemoveValue(_T("use_rightmenu") , true);

	if(m_htimer)
		m_cfg->SetValue(_T("high_accuracy_timer") ,_T("1") , true , ex_option);
	else
		m_cfg->RemoveValue(_T("high_accuracy_timer") , true);

	if(m_menu)
		m_cfg->SetValue(_T("show_menubar") ,_T("1") , true , ex_gui);
	else
		m_cfg->RemoveValue(_T("show_menubar"), true);

	int indexCtrl = m_ctrlbar.GetCurSel();
	if(indexCtrl > 1) {
		m_cfg->RemoveValue(_T("show_controlbar") , true);
		m_cfg->SetValue(_T("skin"), m_ctrlbar_s, true, ex_gui);
	} else if(indexCtrl == 1) {
		m_cfg->RemoveValue(_T("show_controlbar") , true);
		m_cfg->SetValue(_T("skin_controlbar"),_T("0") ,true ,ex_gui);
		m_cfg->SetValue(_T("skinned_player") ,_T("0") , true , ex_gui);
		m_cfg->SetValue(_T("skin"), _T("inner"), true, ex_gui);
	} else {
		m_cfg->SetValue(_T("show_controlbar"),_T("0") ,true ,ex_gui);
	}

	if(m_oneplayer)
		m_cfg->SetValue(_T("one_player") ,_T("1") , true , ex_setting);
	else
		m_cfg->RemoveValue(_T("one_player"),true);

	if(m_quit)
		m_cfg->RemoveValue(_T("always_quit"), true);
	else
		m_cfg->SetValue(_T("always_quit"),_T("0") ,true ,ex_option);
	
	if(m_auto_fuzziness != _T("1"))
		m_cfg->SetValue(_T("autoplay_fuzziness") , m_auto_fuzziness , true , ex_option);
	else
		m_cfg->RemoveValue(_T("autoplay_fuzziness"),true);
	
	int vautoplay= m_autoplay.GetCurSel();
	switch (vautoplay)
	{
	case auto_normal:
		m_cfg->SetValue(_T("autoplay"),_T("1"));
		m_cfg->SetValue(_T("autoplay_fuzziness") , _T("0") , true , ex_option);
		break;
	case auto_none:
		m_cfg->RemoveValue(_T("autoplay"));
		break;
	default:
		m_cfg->SetValue(_T("autoplay"),_T("1"));
		if(m_auto_fuzziness == _T("1") || m_auto_fuzziness == _T("0"))
			m_cfg->RemoveValue(_T("autoplay_fuzziness"),true);
		break;
	}
	
	if(m_monitor_s != m_auto_s)
		m_cfg->SetValue(_T("monitoraspect"),m_monitor_s );
	else
		m_cfg->RemoveValue(_T("monitoraspect"));
	
	
	if(m_dvd != _T(""))
		m_cfg->SetValue(_T("dvd-device"),m_dvd );
	else
		m_cfg->RemoveValue(_T("dvd-device"));
	
	
	int vsystray= m_systray.GetCurSel();
	switch (vsystray)
	{
	case sys_tray:
		m_cfg->SetValue(_T("systray") ,_T("1") );
		break;
	case sys_tray_stop:
		m_cfg->SetValue(_T("systray") ,_T("2") );
		break;
	default:
		m_cfg->RemoveValue(_T("systray"));
	}
	
	int vswitchview= m_switchview.GetCurSel();
	switch (vswitchview)
	{
	case switch_manual:
		m_cfg->SetValue(_T("switchview") ,_T("1") );
		break;
	case switch_auto:
		m_cfg->SetValue(_T("switchview") ,_T("2") );
		break;
	default:
		m_cfg->RemoveValue(_T("switchview"));
	}

	if(m_def.GetLength() > 2)
		m_cfg->SetValue(_T("default_dir"),m_def, true , ex_setting);
	else
		m_cfg->RemoveValue(_T("default_dir"), true);

	if(m_png.GetLength() > 2)
		m_cfg->SetValue(_T("screenshot_dir"),m_png, true , ex_setting);
	else
		m_cfg->RemoveValue(_T("screenshot_dir"), true);
		
	if(m_start != _T("0:0:0") && m_start != _T("")  && m_start != _T("0") )
	{
		int h=-1,m=-1,ss=-1;
		_stscanf_s(m_start,_T("%d:%d:%d"),&h,&m,&ss);
		if( m == -1)
			ss = h;
		else if ( ss == -1)
			ss = h * 60 + m;
		else
			ss += h * 3600 + m *60;
		if(ss > 0)
			m_cfg->SetValue(_T("ss"), m_start);
		else
		m_cfg->RemoveValue(_T("ss"));
	}
	else
		m_cfg->RemoveValue(_T("ss"));
	
	if(m_end != _T("0:0:0") && m_end != _T("")  && m_end != _T("0") )
	{
		int h=-1,m=-1,ss=-1;
		_stscanf_s(m_end,_T("%d:%d:%d"),&h,&m,&ss);
		if( m == -1)
			ss = h;
		else if ( ss == -1)
			ss = h * 60 + m;
		else
			ss += h * 3600 + m *60;
		if(ss > 0)
		{
			CString endtime;
			endtime.Format(_T("%d"),ss);
			m_cfg->SetValue(_T("end_pos"), endtime ,true ,ex_status);
		}
		else
		m_cfg->RemoveValue(_T("end_pos"), true);
	}
	else
		m_cfg->RemoveValue(_T("end_pos"), true);
	
	int vlog = m_log.GetCurSel();
	if(vlog != log_none || m_console)
	{
		if(m_console)
			m_cfg->SetValue(_T("log") ,_T("2") , true , ex_option);
		else
			m_cfg->SetValue(_T("log") ,_T("1") , true , ex_option);
		switch (vlog)
		{
		case log_v1:
			m_cfg->SetValue(_T("v") ,_T("1") );
			break;
		case log_v2:
			m_cfg->SetValue(_T("v") ,_T("2") );
			break;
		case log_v3:
			m_cfg->SetValue(_T("v") ,_T("3") );
			break;
		case log_v4:
			m_cfg->SetValue(_T("v") ,_T("4") );
			break;
		case log_v5:
			m_cfg->SetValue(_T("v") ,_T("5") );
			break;
		default:
			m_cfg->RemoveValue(_T("v"));
		}
	}
	else
	{
		m_cfg->RemoveValue(_T("v"));
		m_cfg->SetValue(_T("log"),_T("0") ,true ,ex_option);
	}
	
	int vtop = m_ontop.GetCurSel();
	switch (vtop)
	{
	case top_playing:
		m_cfg->SetValue(_T("ontop") ,_T("2") );
		break;
	case top_always:
		m_cfg->SetValue(_T("ontop") ,_T("1") );
		break;
	default:
		m_cfg->RemoveValue(_T("ontop"));
	}
	
	int vcache = m_cache.GetCurSel();
	switch (vcache)
	{
	case 1:
		m_cfg->SetValue(_T("cache") ,_T("512") );
		break;
	case 2:
		m_cfg->SetValue(_T("cache") ,_T("1024") );
		break;
	case 3:
		m_cfg->SetValue(_T("cache") ,_T("2048") );
		break;
	case 4:
		m_cfg->SetValue(_T("cache") ,_T("4096") );
		break;
	case 5:
		m_cfg->SetValue(_T("cache") ,_T("8192") );
		break;
	case 6:
		m_cfg->SetValue(_T("cache") ,_T("16384") );
		break;
	case 7:
		m_cfg->SetValue(_T("cache") ,_T("32768") );
		break;
	default:
		m_cfg->RemoveValue(_T("cache"));
	}
	
	int vlang = m_language.GetCurSel();
	switch (vlang)
	{
	case lang_sc:
		m_cfg->SetValue(_T("Language"),_T("1") ,true ,ex_option);
		break;
	case lang_en:
		m_cfg->SetValue(_T("Language"),_T("2") ,true ,ex_option);
		break;
	case lang_tc:
		m_cfg->SetValue(_T("Language"),_T("3") ,true ,ex_option);
		break;
	case lang_big5:
		m_cfg->SetValue(_T("Language"),_T("4") ,true ,ex_option);
		break;
	default:
		m_cfg->SetValue(_T("Language"),_T("0") ,true ,ex_option);
	}

	int valpha = m_alpha.GetCurSel();
	if(valpha > 0)
	{
		CString tmp;
		tmp.Format(_T("%d"), valpha);
		m_cfg->SetValue(_T("controlbar_alpha"), tmp, true, ex_gui);
	}
	else
		m_cfg->RemoveValue(_T("controlbar_alpha"), true);
	
	int vloop = m_loop.GetCurSel();
	switch (vloop)
	{
	case loop_list:
		m_cfg->SetValue(_T("loop"), _T("0"));
		m_cfg->SetValue(_T("loop_all"), _T("1") ,true ,ex_setting);
		break;
	case loop_single:
		m_cfg->SetValue(_T("loop"), _T("0"));
		m_cfg->RemoveValue(_T("loop_all") ,true);
		break;
	default:
		m_cfg->RemoveValue(_T("loop"));
	}
	
	int vautosync = m_autosync.GetCurSel();
	switch (vautosync)
	{
	case sync_normal:
		m_cfg->SetValue(_T("autosync"), _T("30"));
		m_cfg->RemoveValue(_T("mc"));
		m_cfg->RemoveValue(_T("nobps"));
		break;
	case sync_fast:
		m_cfg->SetValue(_T("autosync"), _T("100"));
		m_cfg->RemoveValue(_T("mc"));
		m_cfg->RemoveValue(_T("nobps"));
		break;
	case sync_flv:
		m_cfg->SetValue(_T("autosync"), _T("0"));
		m_cfg->SetValue(_T("mc"), _T("1"));
		m_cfg->RemoveValue(_T("nobps"));
		break;
	case sync_flvex:
		m_cfg->SetValue(_T("autosync"), _T("0"));
		m_cfg->SetValue(_T("mc"), _T("0.001"));
		m_cfg->RemoveValue(_T("nobps"));
		break;
	case sync_avi:
		m_cfg->SetValue(_T("autosync"), _T("0"));
		m_cfg->SetValue(_T("nobps"), _T("1"));
		m_cfg->RemoveValue(_T("mc"));
		break;
	default:
		m_cfg->SetValue(_T("autosync"), _T("0"));
		m_cfg->RemoveValue(_T("mc"));
		m_cfg->RemoveValue(_T("nobps"));
	}
	
	int vpriority = m_priority.GetCurSel();
	switch (vpriority)
	{
	case prio_realtime:
		m_cfg->SetValue(_T("priority"), _T("realtime"));
		ShowInfo(type_realtime);
		break;
	case prio_high:
		m_cfg->SetValue(_T("priority"), _T("high"));
		break;
	case prio_abovenormal:
		m_cfg->SetValue(_T("priority"), _T("abovenormal"));
		break;
	case prio_normal:
		m_cfg->SetValue(_T("priority"), _T("normal"));
		break;
	case prio_belownormal:
		m_cfg->SetValue(_T("priority"), _T("belownormal"));
		break;
	case prio_idle:
		m_cfg->SetValue(_T("priority"), _T("idle"));
		break;
	default:
		m_cfg->RemoveValue(_T("priority"));
	}
}

void CMPlayerPage::SetInfoDlg(CMShowInfoDlg *infoDlg)
{
	info = infoDlg;
}

void CMPlayerPage::ShowInfo(int type)
{
	if(info)
	{
		if(info->IsShow(type))
			info->DoModal();
	}
}

BOOL CMPlayerPage::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case   WM_KEYDOWN:
		switch(pMsg->wParam)
		{
		case   VK_RETURN:
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMPlayerPage::OnSelchangeAutoplay() 
{
	if(m_autoplay.GetCurSel() == auto_ex)
		m_fuzziness.EnableWindow(TRUE);
	else
		m_fuzziness.EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CMPlayerPage::OnBnClickedButtonDef()
{
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);
	CWnd* pMainCWnd = NULL; 
	pMainCWnd = GetActiveWindow(); 
	CString csFolder = _T( "" );
	HWND pMainHWnd = pMainCWnd->GetSafeHwnd();
	if(SelectFolder( pMainHWnd, csFolder ))
		m_def = csFolder;
	::SetCurrentDirectory(szFilePath);
	UpdateData(FALSE);
}
