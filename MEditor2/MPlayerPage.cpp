// MPlayerPage.cpp : implementation file
//

#include "stdafx.h"
#include "meditor2.h"
#include "MPlayerPage.h"
#include "MConfig.h"

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
{
	m_cfg = NULL;
	//{{AFX_DATA_INIT(CMPlayerPage)
	m_control = TRUE;
	m_fullscreen = FALSE;
	m_guithread = FALSE;
	m_menu = TRUE;
	m_oneplayer = FALSE;
	m_quit = FALSE;
	m_show = FALSE;
	m_url = FALSE;
	m_osd_font = _T("");
	m_osdsize_s = _T("");
	m_dvd = _T("G:");
	m_end = _T("0:0:0");
	m_png = _T("");
	m_start = _T("0:0:0");
	m_monitor_s = _T("");
	m_quiet = TRUE;
	m_double = TRUE;
	m_colorkey_s = _T("");
	m_conf = FALSE;
	m_bottom = TRUE;
	m_auto_fuzziness = _T("1");
	//}}AFX_DATA_INIT
}

void CMPlayerPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMPlayerPage)
	DDX_Control(pDX, IDC_EDIT_FUZZINESS, m_fuzziness);
	DDX_Control(pDX, IDC_COMBO_AUTOPLAY, m_autoplay);
	DDX_Control(pDX, IDC_COMBO_SYSTRAY, m_systray);
	DDX_Control(pDX, IDC_CHECK_BOTTOM, m_bottom_c);
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
	DDX_Control(pDX, IDC_COMBO_OSDF, m_osd_font_c);
	DDX_Control(pDX, IDC_COMBO_OSDTIME, m_osdtime);
	DDX_Control(pDX, IDC_COMBO_OSDSIZE, m_osdsize);
	DDX_Control(pDX, IDC_COMBO_OSDMODE, m_osdmode);
	DDX_Check(pDX, IDC_CHECK_CTRL, m_control);
	DDX_Check(pDX, IDC_CHECK_FULLSCREEN, m_fullscreen);
	DDX_Check(pDX, IDC_CHECK_GUITHREAD, m_guithread);
	DDX_Check(pDX, IDC_CHECK_MENU, m_menu);
	DDX_Check(pDX, IDC_CHECK_ONLYONE, m_oneplayer);
	DDX_Check(pDX, IDC_CHECK_QUIT, m_quit);
	DDX_Check(pDX, IDC_CHECK_SHOW, m_show);
	DDX_Check(pDX, IDC_CHECK_URL, m_url);
	DDX_CBString(pDX, IDC_COMBO_OSDF, m_osd_font);
	DDX_CBString(pDX, IDC_COMBO_OSDSIZE, m_osdsize_s);
	DDV_MaxChars(pDX, m_osdsize_s, 3);
	DDX_Text(pDX, IDC_EDIT_DVD, m_dvd);
	DDX_Text(pDX, IDC_EDIT_END, m_end);
	DDX_Text(pDX, IDC_EDIT_PNG, m_png);
	DDX_Text(pDX, IDC_EDIT_START, m_start);
	DDX_CBString(pDX, IDC_COMBO_MONITOR, m_monitor_s);
	DDX_Check(pDX, IDC_CHECK_QUIET, m_quiet);
	DDX_Check(pDX, IDC_CHECK_DOUBLE, m_double);
	DDX_CBString(pDX, IDC_COMBO_COLORKEY, m_colorkey_s);
	DDX_Check(pDX, IDC_CHECK_CONF, m_conf);
	DDX_Check(pDX, IDC_CHECK_BOTTOM, m_bottom);
	DDX_Text(pDX, IDC_EDIT_FUZZINESS, m_auto_fuzziness);
	DDV_MaxChars(pDX, m_auto_fuzziness, 3);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_HTIMER, m_htimer);
}


BEGIN_MESSAGE_MAP(CMPlayerPage, CDialog)
	//{{AFX_MSG_MAP(CMPlayerPage)
	ON_BN_CLICKED(IDC_BUTTON_PNG, OnButtonPng)
	ON_BN_CLICKED(IDC_BUTTON_DVD, OnButtonDvd)
	ON_BN_CLICKED(IDC_CHECK_CTRL, OnCheckCtrl)
	ON_CBN_SELCHANGE(IDC_COMBO_AUTOPLAY, OnSelchangeAutoplay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPlayerPage message handlers

void CMPlayerPage::OnButtonPng() 
{
	// TODO: Add your control notification handler code here
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
	// TODO: Add your control notification handler code here
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
	
	// TODO: Add extra initialization here
	m_language.AddString(_T("自动选择"));
	m_language.AddString(_T("简体中文"));
	m_language.AddString(_T("English"));
	m_language.AddString(_T("繁體中文"));
	m_language.AddString(_T("繁體中文 BIG5"));
	m_language.SetCurSel(lang_auto);

	m_cache.AddString(_T("自动"));
	m_cache.AddString(_T("512 KB"));
	m_cache.AddString(_T("1 M"));
	m_cache.AddString(_T("2 M"));
	m_cache.AddString(_T("4 M"));
	m_cache.AddString(_T("8 M"));
	m_cache.AddString(_T("16 M"));
	m_cache.AddString(_T("32 M"));
	m_cache.SetCurSel(0);
	
	m_ontop.AddString(_T("从不"));
	m_ontop.AddString(_T("始终"));
	m_ontop.AddString(_T("播放时"));
	m_ontop.SetCurSel(top_none);

	m_colorkey.AddString(_T("无"));
	m_colorkey.AddString(_T("0x000001"));
	m_colorkey_s = _T("0x000001");

	m_autosync.AddString(_T("无同步模式(推荐)"));
	m_autosync.AddString(_T("普通同步模式"));
	m_autosync.AddString(_T("快速同步模式"));
	m_autosync.AddString(_T("FLV/RM 模式"));
	m_autosync.AddString(_T("FLV 强力同步模式"));
	m_autosync.AddString(_T("AVI 修正模式"));
	m_autosync.SetCurSel(sync_none);

	m_priority.AddString(_T("自动"));
	m_priority.AddString(_T("实时(不推荐)"));
	m_priority.AddString(_T("高"));
	m_priority.AddString(_T("高于标准"));
	m_priority.AddString(_T("标准"));
	m_priority.AddString(_T("低于标准(不推荐)"));
	m_priority.AddString(_T("空闲(不推荐)"));
	m_priority.SetCurSel(prio_auto);

	m_loop.AddString(_T("从不"));
	m_loop.AddString(_T("单文件"));
	m_loop.AddString(_T("播放列表"));
	m_loop.SetCurSel(loop_none);

	m_monitor.AddString(_T("自动"));
	m_monitor.AddString(_T("4:3"));
	m_monitor.AddString(_T("16:9"));
	m_monitor_s = _T("自动");

	m_log.AddString(_T("不记录日志"));
	m_log.AddString(_T("简单日志"));
	m_log.AddString(_T("详细日志(v=1)"));
	m_log.AddString(_T("详细日志(v=2)"));
	m_log.AddString(_T("详细日志(v=3)"));
	m_log.AddString(_T("详细日志(v=4)"));
	m_log.AddString(_T("详细日志(v=5)"));
	m_log.SetCurSel(log_none);
	
	m_switchview.AddString(_T("不隐藏"));
	m_switchview.AddString(_T("手动隐藏"));
	m_switchview.AddString(_T("自动隐藏"));
	m_switchview.SetCurSel(switch_none);
	
	m_osdsize.AddString(_T("2"));
	m_osdsize.AddString(_T("2.5"));
	m_osdsize.AddString(_T("3"));
	m_osdsize.AddString(_T("3.5"));
	m_osdsize.AddString(_T("4"));
	m_osdsize.AddString(_T("4.5"));
	m_osdsize.AddString(_T("5"));
	m_osdsize_s = _T("3");
	
	m_osdtime.AddString(_T("不显示日期时间"));
	m_osdtime.AddString(_T("时:分 (24小时制)"));
	m_osdtime.AddString(_T("时:分 (12小时制)"));
	m_osdtime.AddString(_T("时:分:秒 (24小时制)"));
	m_osdtime.AddString(_T("时:分:秒 (12小时制)"));
	m_osdtime.AddString(_T("年.月.日 时:分 (24)"));
	m_osdtime.AddString(_T("年.月.日 时:分 (12)"));
	m_osdtime.AddString(_T("年.月.日 时:分:秒 (24)"));
	m_osdtime.AddString(_T("年.月.日 时:分:秒 (12)"));
	m_osdtime.AddString(_T("无播放信息 时:分 (24)"));
	m_osdtime.SetCurSel(time_none);
	
	m_osdmode.AddString(_T("默认模式"));
	m_osdmode.AddString(_T("显示播放时间"));
	m_osdmode.AddString(_T("显示剩余时间"));
	m_osdmode.AddString(_T("不显示OSD"));
	m_osdmode.SetCurSel(osd_normal);
	
	m_systray.AddString(_T("任务栏"));
	m_systray.AddString(_T("系统托盘"));
	m_systray.AddString(_T("托盘并暂停"));
	m_systray.SetCurSel(sys_task);
	
	m_autoplay.AddString(_T("不使用"));
	m_autoplay.AddString(_T("自动"));
	m_autoplay.AddString(_T("增强"));
	m_autoplay.SetCurSel(auto_ex);

	TCHAR szCurPath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szCurPath);

	TCHAR szFontPath[MAX_PATH + 1];
	SHGetSpecialFolderPath(NULL, szFontPath , CSIDL_FONTS , FALSE);

	::SetCurrentDirectory(szFontPath);

	CFileFind finder;
	if(finder.FindFile(_T("*.ttf"),0))
	{
		while(finder.FindNextFile())
		{
			m_osd_font_c.AddString(finder.GetFileName());
		}
		CString str = finder.GetFileName();
		if(str.GetLength() > 1)
			m_osd_font_c.AddString(str);
	}
	if(finder.FindFile(_T("*.ttc"),0))
	{
		while(finder.FindNextFile())
		{
			m_osd_font_c.AddString(finder.GetFileName());
		}
		CString str = finder.GetFileName();
		if(str.GetLength() > 1)
			m_osd_font_c.AddString(str);
	}
	::SetCurrentDirectory(szCurPath);
	m_osd_font = _T("simhei.ttf");

	InitFromConfig();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMPlayerPage::SetNormal()
{
	m_control = TRUE;
	m_menu = TRUE;
	m_bottom = TRUE;
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
	m_dvd = _T("G:");
	m_end = _T("0:0:0");
	m_png = _T("");
	m_start = _T("0:0:0");
	m_auto_fuzziness = _T("1");
	m_colorkey_s = _T("0x000001");
	m_monitor_s = _T("自动");
	m_osdsize_s = _T("3");
	m_language.SetCurSel(lang_auto);
	m_cache.SetCurSel(0);
	m_ontop.SetCurSel(top_playing);
	m_autosync.SetCurSel(sync_none);
	m_priority.SetCurSel(prio_auto);
	m_loop.SetCurSel(loop_none);
	m_log.SetCurSel(log_none);	
	m_switchview.SetCurSel(switch_none);	
	m_osdtime.SetCurSel(time_none);	
	m_osdmode.SetCurSel(osd_normal);
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
	m_bottom = FALSE;
	m_priority.SetCurSel(prio_high);
	m_autosync.SetCurSel(sync_normal);
}

void CMPlayerPage::InitFromConfig()
{
	if(!m_cfg)
		return;
	int value_i;
	bool value_b,auto_play_b = false;
	double value_d;
	CString value_s;

	m_cfg->GetValue_Boolean(_T("fixed-vo"),value_b);

	if(m_cfg->GetValue_Boolean(_T("use-filedir-conf"),value_b))
	{
		if(value_b)
			m_conf = TRUE;
		else
			m_conf = FALSE;
	}
	if(m_cfg->GetValue_String(_T("colorkey"),value_s))
	{
		if(value_s != _T("0x000001"))
			m_colorkey.AddString(value_s);
		m_colorkey_s = value_s;
	}
	if(m_cfg->GetValue_Boolean(_T("nocolorkey"),value_b))
	{
		m_colorkey_s = _T("无");
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
	if(m_cfg->GetValue_Boolean(_T("gui_thread"),value_b,true))
	{
		if(value_b)
			m_guithread = TRUE;
		else
			m_guithread = FALSE;
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
	if(m_cfg->GetValue_Boolean(_T("show_playbar"),value_b,true))
	{
		if(value_b)
			m_bottom = TRUE;
		else
			m_bottom = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("show_controlbar"),value_b,true))
	{
		if(value_b)
			m_control = TRUE;
		else
		{
			m_control = FALSE;
			m_bottom = FALSE;
			m_bottom_c.EnableWindow(FALSE);
		}
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
	if(m_cfg->GetValue_Boolean(_T("log"),value_b,true))
	{
		if(value_b)
		{
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
	if(m_cfg->GetValue_Integer(_T("osdlevel"),value_i))
	{
		switch (value_i)
		{
		case 0:
			m_osdmode.SetCurSel(osd_none);
			break;
		case 3:
			m_osdmode.SetCurSel(osd_play_time);
			break;
		case 4:
			m_osdmode.SetCurSel(osd_left_time);
			break;
		default:
			m_osdmode.SetCurSel(osd_normal);
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
	if(m_cfg->GetValue_Integer(_T("osd_systime"),value_i,true))
	{
		if(value_i >= 1 && value_i <= 9)
			m_osdtime.SetCurSel(value_i);
		else
			m_osdtime.SetCurSel(time_none);
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

	if(m_cfg->GetValue_String(_T("font"),value_s))
	{
		value_s.TrimLeft(_T('"'));
		value_s.TrimRight(_T('"'));
		m_osd_font = value_s;
	}
	if(m_cfg->GetValue_String(_T("subfont-osd-scale"),value_s))
	{
		m_osdsize_s = value_s;
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
	if(m_cfg->GetValue_String(_T("ScreenShot_DIR"),value_s,true))
	{
		m_png = value_s;
	}

	UpdateData(FALSE);
}

void CMPlayerPage::SaveConfig()
{
	if(!m_cfg)
		return;
	UpdateData(TRUE);

	m_cfg->SetValue(_T("fixed-vo"),_T("yes"));

	if(m_conf)
		m_cfg->SetValue(_T("use-filedir-conf"),_T("yes"));
	else
		m_cfg->RemoveValue(_T("use-filedir-conf"));
	
	if(m_colorkey_s == _T("无"))
	{
		m_cfg->SetValue(_T("nocolorkey"),_T("yes"));
		m_cfg->RemoveValue(_T("colorkey"));
	}
	else
	{
		m_cfg->RemoveValue(_T("nocolorkey"));
		m_cfg->SetValue(_T("colorkey"), m_colorkey_s);
	}
		
	if(m_double)
		m_cfg->SetValue(_T("double"),_T("yes"));
	else
		m_cfg->RemoveValue(_T("double"));
	
	if(m_quiet)
		m_cfg->SetValue(_T("quiet"),_T("yes"));
	else
		m_cfg->RemoveValue(_T("quiet"));
	
	if(m_fullscreen)
		m_cfg->SetValue(_T("fs"),_T("yes"));
	else
		m_cfg->RemoveValue(_T("fs"));
		
	if(m_show)
		m_cfg->SetValue(_T("idle"),_T("yes"));
	else
		m_cfg->RemoveValue(_T("idle"));
	
	if(m_url)
		m_cfg->SetValue(_T("urlcp"),_T("GBK"));
	else
		m_cfg->RemoveValue(_T("urlcp"));
	
	if(m_guithread)
		m_cfg->SetValue(_T("gui_thread") ,_T("1") , true , ex_option);
	else
		m_cfg->SetValue(_T("gui_thread"),_T("0") ,true ,ex_option);

	if(m_htimer)
		m_cfg->SetValue(_T("high_accuracy_timer") ,_T("1") , true , ex_option);
	else
		m_cfg->RemoveValue(_T("high_accuracy_timer") , true);
	
	if(m_menu)
		m_cfg->SetValue(_T("show_menubar") ,_T("1") , true , ex_option);
	else
		m_cfg->SetValue(_T("show_menubar"),_T("0") ,true ,ex_option);

	if(m_control)
		m_cfg->SetValue(_T("show_controlbar") ,_T("1") , true , ex_option);
	else
		m_cfg->SetValue(_T("show_controlbar"),_T("0") ,true ,ex_option);
	
	if(m_bottom)
		m_cfg->SetValue(_T("show_playbar") ,_T("1") , true , ex_option);
	else
		m_cfg->SetValue(_T("show_playbar"),_T("0") ,true ,ex_option);
	
	if(m_oneplayer)
		m_cfg->SetValue(_T("one_player") ,_T("1") , true , ex_option);
	else
		m_cfg->SetValue(_T("one_player"),_T("0") ,true ,ex_option);
	
	if(m_quit)
		m_cfg->SetValue(_T("always_quit") ,_T("1") , true , ex_option);
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
	
	m_cfg->SetValue(_T("font"), _T("\"") + m_osd_font + _T("\""));
	m_cfg->SetValue(_T("subfont-osd-scale"),m_osdsize_s );
	if(m_monitor_s != _T("自动"))
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
	
	if(m_png != _T(""))
		m_cfg->SetValue(_T("ScreenShot_DIR"),m_png, true , ex_option);
	else
		m_cfg->RemoveValue(_T("ScreenShot_DIR"), true);
		
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
	if(vlog != log_none)
	{
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
	
	int vosdmode = m_osdmode.GetCurSel();
	switch (vosdmode)
	{
	case osd_play_time:
		m_cfg->SetValue(_T("osdlevel") ,_T("3") );
		break;
	case osd_left_time:
		m_cfg->SetValue(_T("osdlevel") ,_T("4") );
		break;
	case osd_none:
		m_cfg->SetValue(_T("osdlevel") ,_T("0") );
		break;
	default:
		m_cfg->RemoveValue(_T("osdlevel"));
	}
	
	int vosdtime = m_osdtime.GetCurSel();
	switch (vosdtime)
	{
	case time_t1:
		m_cfg->SetValue(_T("osd_systime") ,_T("1") ,true, ex_option);
		break;
	case time_t2:
		m_cfg->SetValue(_T("osd_systime") ,_T("2") ,true, ex_option);
		break;
	case time_t3:
		m_cfg->SetValue(_T("osd_systime") ,_T("3") ,true, ex_option);
		break;
	case time_t4:
		m_cfg->SetValue(_T("osd_systime") ,_T("4") ,true, ex_option);
		break;
	case time_dt1:
		m_cfg->SetValue(_T("osd_systime") ,_T("5") ,true, ex_option);
		break;
	case time_dt2:
		m_cfg->SetValue(_T("osd_systime") ,_T("6") ,true, ex_option);
		break;
	case time_dt3:
		m_cfg->SetValue(_T("osd_systime") ,_T("7") ,true, ex_option);
		break;
	case time_dt4:
		m_cfg->SetValue(_T("osd_systime") ,_T("8") ,true, ex_option);
		break;
	case time_ot:
		m_cfg->SetValue(_T("osd_systime") ,_T("9") ,true, ex_option);
		break;
	default:
		m_cfg->RemoveValue(_T("osd_systime"),true);
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
	
	int vloop = m_loop.GetCurSel();
	switch (vloop)
	{
	case loop_list:
		m_cfg->SetValue(_T("loop"), _T("0"));
		m_cfg->SetValue(_T("loop_all"), _T("1") ,true ,ex_option);
		break;
	case loop_single:
		m_cfg->SetValue(_T("loop"), _T("0"));
		m_cfg->SetValue(_T("loop_all"), _T("0") ,true ,ex_option);
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
		m_cfg->SetValue(_T("nobps"), _T("yes"));
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

BOOL CMPlayerPage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
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

void CMPlayerPage::OnCheckCtrl() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_control)
		m_bottom_c.EnableWindow(TRUE);
	else
		m_bottom_c.EnableWindow(FALSE);
}

void CMPlayerPage::OnSelchangeAutoplay() 
{
	// TODO: Add your control notification handler code here
	if(m_autoplay.GetCurSel() == auto_ex)
		m_fuzziness.EnableWindow(TRUE);
	else
		m_fuzziness.EnableWindow(FALSE);
	UpdateData(FALSE);
}
