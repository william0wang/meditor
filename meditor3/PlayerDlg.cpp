// PlayerDlg.cpp : implementation of the CPlayerDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "shared.h"
#include "Reg.h"
#include "ShowInfoDlg.h"
#include "PlayerDlg.h"

enum
{
	sync_none = 0,
	sync_normal,
	sync_fast,
	sync_flv,
	sync_flvex,
	sync_avi
};

enum
{
	prio_auto = 0,
	prio_realtime,
	prio_high,
	prio_abovenormal,
	prio_normal,
	prio_belownormal,
	prio_idle,
};

enum
{
	play_none = 0,
	play_auto,
	play_ex_6,
	play_ex_10,
	play_ex_20,
	play_ex_50,
	play_ex_100,
	play_ex_200,
};

enum
{
	log_none = 0,
	log_normal,
	log_v1,
	log_v2,
	log_v3,
	log_v4,
	log_v5,
	log_console,
	log_console_v1,
	log_console_v2,
	log_console_v3,
	log_console_v4,
	log_console_v5,
};

enum 
{
	cache_none = 0,
	cache_512K,
	cache_1,
	cache_2,
	cache_4,
	cache_8,
	cache_16,
	cache_32,
	cache_64,
	cache_96,
	cache_128,
	cache_256,
	cache_auto,
};

enum
{
	loop_none = 0,
	loop_single,
	loop_list
};

CPlayerDlg::CPlayerDlg(UINT IDD_DLG)
{
	IDD = IDD_DLG;


}

BOOL CPlayerDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CPlayerDlg::OnIdle()
{
	return FALSE;
}


void CPlayerDlg::InitCheckBox()
{
	CString value_s;

	if(mconfig.GetBoolean(_T("use-filedir-conf")))
		m_conf = TRUE;
	else
		m_conf = FALSE;

	if(mconfig.GetBoolean(_T("double"), true))
		m_double = TRUE;
	else
		m_double = FALSE;

	if(mconfig.GetBoolean(_T("quiet"), true))
		m_quiet = TRUE;
	else
		m_quiet = FALSE;

	if(mconfig.GetBoolean(_T("fs")))
		m_fullscreen = TRUE;
	else
		m_fullscreen = FALSE;

	if(mconfig.GetBoolean(_T("idx")))
		m_idx = TRUE;
	else
		m_idx = FALSE;

	if(mconfig.GetBoolean(_T("framedrop")))
		m_framedrop = TRUE;
	else
		m_framedrop = FALSE;

	mconfig.GetString(_T("urlcp"), value_s);
	if(!value_s.Compare(_T("GBK")))
		m_url = TRUE;
	else
		m_url = FALSE;

	if(mconfig.GetBoolean(_T("gui_thread"), false, true))
		m_guithread = TRUE;
	else
		m_guithread = FALSE;

	if(mconfig.GetBoolean(_T("seek_realtime"), true, true))
		m_seekrt = TRUE;
	else
		m_seekrt = FALSE;

	if(mconfig.GetBoolean(_T("gui_priority_lowest"), true, true))
		m_gui_high = FALSE;
	else
		m_gui_high = TRUE;

	if(mconfig.GetBoolean(_T("reload_when_open"), false, true))
		m_reload = TRUE;
	else
		m_reload = FALSE;

	if(mconfig.GetBoolean(_T("no_dvdnav"), false, true))
		m_no_dvdnav = TRUE;
	else
		m_no_dvdnav = FALSE;

	if(mconfig.GetBoolean(_T("high_accuracy_timer"), false, true))
		m_htimer = TRUE;
	else
		m_htimer = FALSE;

	if(mconfig.GetBoolean(_T("one_player"), false, true))
		m_oneplayer = TRUE;
	else
		m_oneplayer = FALSE;

	if(mconfig.GetBoolean(_T("always_quit"), true, true))
		m_quit = TRUE;
	else
		m_quit = FALSE;
}

void CPlayerDlg::SaveConfig()
{
	DoDataExchange(TRUE);

	if(m_conf)
		mconfig.SetValue(_T("use-filedir-conf"), _T("1"));
	else
		mconfig.RemoveValue(_T("use-filedir-conf"));

	if(m_double)
		mconfig.SetValue(_T("double"), _T("1"));
	else
		mconfig.RemoveValue(_T("double"));

	if(m_quiet)
		mconfig.SetValue(_T("quiet"), _T("1"));
	else
		mconfig.RemoveValue(_T("quiet"));

	if(m_fullscreen)
		mconfig.SetValue(_T("fs"), _T("1"));
	else
		mconfig.RemoveValue(_T("fs"));

	if(m_url)
		mconfig.SetValue(_T("urlcp"), _T("GBK"));
	else
		mconfig.RemoveValue(_T("urlcp"));

	if(m_framedrop)
		mconfig.SetValue(_T("framedrop"), _T("1"));
	else
		mconfig.RemoveValue(_T("framedrop"));

	if(m_idx)
		mconfig.SetValue(_T("idx"), _T("1"));
	else
		mconfig.RemoveValue(_T("idx"));

	if(m_guithread)
		mconfig.SetValue(_T("gui_thread"), _T("1"), true, ex_option);
	else
		mconfig.RemoveValue(_T("gui_thread"), true);

	if(m_gui_high)
		mconfig.SetValue(_T("gui_priority_lowest"), _T("0"), true, ex_gui);
	else
		mconfig.RemoveValue(_T("gui_priority_lowest"), true);

	if(m_reload)
		mconfig.SetValue(_T("reload_when_open"), _T("1"), true, ex_setting);
	else
		mconfig.RemoveValue(_T("reload_when_open"), true);

	if(m_no_dvdnav)
		mconfig.SetValue(_T("no_dvdnav"), _T("1"), true, ex_setting);
	else
		mconfig.RemoveValue(_T("no_dvdnav"), true);

	if(m_htimer)
		mconfig.SetValue(_T("high_accuracy_timer"), _T("1"), true, ex_option);
	else
		mconfig.RemoveValue(_T("high_accuracy_timer"), true);

	if(m_seekrt)
		mconfig.RemoveValue(_T("seek_realtime"), true);
	else
		mconfig.SetValue(_T("seek_realtime"), _T("0"), true, ex_setting);
	
	if(m_oneplayer)
		mconfig.SetValue(_T("one_player"), _T("1") ,true ,ex_setting);
	else
		mconfig.RemoveValue(_T("one_player"), true);

	if(m_quit)
		mconfig.RemoveValue(_T("always_quit"), true);
	else
		mconfig.SetValue(_T("always_quit"), _T("0"), true, ex_option);
	
	//Cache
	mconfig.RemoveValue(_T("auto-cache"));
	mconfig.RemoveValue(_T("cache-min"));
	switch (aComboCache.GetSelectedIndex())
	{
	case cache_512K:
		mconfig.SetValue(_T("cache"), _T("512") );
		break;
	case cache_1:
		mconfig.SetValue(_T("cache"), _T("1024") );
		break;
	case cache_2:
		mconfig.SetValue(_T("cache"), _T("2048") );
		break;
	case cache_4:
		mconfig.SetValue(_T("cache"), _T("4096") );
		break;
	case cache_8:
		mconfig.SetValue(_T("cache"), _T("8192") );
		break;
	case cache_16:
		mconfig.SetValue(_T("cache"), _T("16384") );
		break;
	case cache_32:
		mconfig.SetValue(_T("cache"), _T("32768") );
		break;
	case cache_64:
		mconfig.SetValue(_T("cache"), _T("65536") );
		break;
	case cache_96:
		mconfig.SetValue(_T("cache"), _T("98304") );
		break;
	case cache_128:
		mconfig.SetValue(_T("cache"), _T("131072") );
		mconfig.SetValue(_T("cache-min"), _T("10") );
		break;
	case cache_256:
		mconfig.SetValue(_T("cache"), _T("262144") );
		mconfig.SetValue(_T("cache-min"), _T("8") );
		break;
	case cache_auto:
		mconfig.SetValue(_T("auto-cache"), _T("1") );
		mconfig.RemoveValue(_T("cache"));
		break;
	default:
		mconfig.RemoveValue(_T("cache"));
		break;
	}
	
	//Loop
	switch (aComboLoop.GetSelectedIndex())
	{
	case loop_list:
		mconfig.SetValue(_T("loop"), _T("0"));
		mconfig.SetValue(_T("loop_all"), _T("1"), true, ex_setting);
		break;
	case loop_single:
		mconfig.SetValue(_T("loop"), _T("0"));
		mconfig.RemoveValue(_T("loop_all"), true);
		break;
	default:
		mconfig.RemoveValue(_T("loop"));
	}

	//AutoSync
	switch (aComboASync.GetSelectedIndex())
	{
	case sync_normal:
		mconfig.SetValue(_T("autosync"), _T("30"));
		mconfig.RemoveValue(_T("mc"));
		mconfig.RemoveValue(_T("nobps"));
		break;
	case sync_fast:
		mconfig.SetValue(_T("autosync"), _T("100"));
		mconfig.RemoveValue(_T("mc"));
		mconfig.RemoveValue(_T("nobps"));
		break;
	case sync_flv:
		mconfig.SetValue(_T("autosync"), _T("0"));
		mconfig.SetValue(_T("mc"), _T("1"));
		mconfig.RemoveValue(_T("nobps"));
		break;
	case sync_flvex:
		mconfig.SetValue(_T("autosync"), _T("0"));
		mconfig.SetValue(_T("mc"), _T("0.001"));
		mconfig.RemoveValue(_T("nobps"));
		break;
	case sync_avi:
		mconfig.SetValue(_T("autosync"), _T("0"));
		mconfig.SetValue(_T("nobps"), _T("1"));
		mconfig.RemoveValue(_T("mc"));
		break;
	default:
		mconfig.SetValue(_T("autosync"), _T("0"));
		mconfig.RemoveValue(_T("mc"));
		mconfig.RemoveValue(_T("nobps"));
		break;
	}

	//log
	switch (aComboLog.GetSelectedIndex())
	{
	case log_v1:
		mconfig.SetValue(_T("v") ,_T("1") );
		goto LogFile;
	case log_v2:
		mconfig.SetValue(_T("v") ,_T("2") );
		goto LogFile;
	case log_v3:
		mconfig.SetValue(_T("v") ,_T("3") );
		goto LogFile;
	case log_v4:
		mconfig.SetValue(_T("v") ,_T("4") );
		goto LogFile;
	case log_v5:
		mconfig.SetValue(_T("v") ,_T("5") );
	case log_normal:
LogFile:
		mconfig.SetValue(_T("log"), _T("1"), true, ex_option);
		break;
	case log_console_v1:
		mconfig.SetValue(_T("v") ,_T("1") );
		goto LogConsole;
	case log_console_v2:
		mconfig.SetValue(_T("v") ,_T("2") );
		goto LogConsole;
	case log_console_v3:
		mconfig.SetValue(_T("v") ,_T("3") );
		goto LogConsole;
	case log_console_v4:
		mconfig.SetValue(_T("v") ,_T("4") );
		goto LogConsole;
	case log_console_v5:
		mconfig.SetValue(_T("v") ,_T("5") );
	case log_console:
LogConsole:
		mconfig.SetValue(_T("log") ,_T("2") , true , ex_option);
		break;
	default:
		mconfig.RemoveValue(_T("v"));
		mconfig.SetValue(_T("log"), _T("0"), true, ex_option);
		break;
	}

	//AutoPlay
	switch (aComboAPlay.GetSelectedIndex())
	{
	case play_auto:
		mconfig.SetValue(_T("autoplay"), _T("1"));
		mconfig.SetValue(_T("autoplay_fuzziness"), _T("0"), true, ex_option);
		break;
	case play_none:
		mconfig.RemoveValue(_T("autoplay"));
		break;
	case play_ex_10:
		mconfig.SetValue(_T("autoplay_fuzziness"), _T("10"), true, ex_option);
		goto AutoPlay;
	case play_ex_20:
		mconfig.SetValue(_T("autoplay_fuzziness"), _T("20"), true, ex_option);
		goto AutoPlay;
	case play_ex_50:
		mconfig.SetValue(_T("autoplay_fuzziness"), _T("50"), true, ex_option);
		goto AutoPlay;
	case play_ex_100:
		mconfig.SetValue(_T("autoplay_fuzziness"), _T("100"), true, ex_option);
		goto AutoPlay;
	case play_ex_200:
		mconfig.SetValue(_T("autoplay_fuzziness"), _T("200"), true, ex_option);
		goto AutoPlay;
	case play_ex_6:
		mconfig.SetValue(_T("autoplay_fuzziness"), _T("6"), true, ex_option);
	default:
AutoPlay:
		mconfig.SetValue(_T("autoplay"), _T("1"));
		break;
	}

	//Monitor
	CString m_monitor_s = aComboMonitor.GetSelectedString();
	if(m_monitor_s != rStr.autos)
		mconfig.SetValue(_T("monitoraspect"), m_monitor_s );
	else
		mconfig.RemoveValue(_T("monitoraspect"));

	//Priority
	switch (aComboPriority.GetSelectedIndex())
	{
	case prio_realtime:
		mconfig.SetValue(_T("priority"), _T("realtime"));
		ShowInfoDlg(type_realtime);
		break;
	case prio_high:
		mconfig.SetValue(_T("priority"), _T("high"));
		break;
	case prio_abovenormal:
		mconfig.SetValue(_T("priority"), _T("abovenormal"));
		break;
	case prio_normal:
		mconfig.SetValue(_T("priority"), _T("normal"));
		break;
	case prio_belownormal:
		mconfig.SetValue(_T("priority"), _T("belownormal"));
		break;
	case prio_idle:
		mconfig.SetValue(_T("priority"), _T("idle"));
		break;
	default:
		mconfig.RemoveValue(_T("priority"));
	}

	//Skip Time
	CString value_s;
	int value_i = ListStringtoTime(ListTime.GetItemText(0, 1));
	if(value_i > 0) {
		value_s.Format(_T("%d"), value_i);
		mconfig.SetValue(_T("ss"), value_s);
	} else
		mconfig.RemoveValue(_T("ss"));

	value_i = ListStringtoTime(ListTime.GetItemText(0, 3));
	if(value_i > 0) {
		value_s.Format(_T("%d"), value_i);
		mconfig.SetValue(_T("end_pos"), value_s, true, ex_status);
	} else
		mconfig.RemoveValue(_T("end_pos"), true);
}

LRESULT CPlayerDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	UIAddChildWindowContainer(m_hWnd);
	int value_i;
	double value_d;
	CString value_s;

	SetDlgItemText(IDC_CHECK_FULLSCREEN, rStr.cfulls);
	SetDlgItemText(IDC_CHECK_RTSEEK, rStr.crseek);
	SetDlgItemText(IDC_CHECK_ONLYONE, rStr.conep);
	SetDlgItemText(IDC_CHECK_GUITHREAD, rStr.cguit);
	SetDlgItemText(IDC_CHECK_GUI, rStr.cguip);
	SetDlgItemText(IDC_CHECK_URL, rStr.curl);
	SetDlgItemText(IDC_CHECK_QUIT, rStr.cquit);
	SetDlgItemText(IDC_CHECK_QUIET, rStr.cquiet);
	SetDlgItemText(IDC_CHECK_DOUBLE, rStr.cdouble);
	SetDlgItemText(IDC_CHECK_CONF, rStr.cconf);
	SetDlgItemText(IDC_CHECK_HTIMER, rStr.chtime);
	SetDlgItemText(IDC_CHECK_DVDNAV, rStr.cdvd);
	SetDlgItemText(IDC_CHECK_RELOAD, rStr.creload);
	SetDlgItemText(IDC_CHECK_FRAMEDROP, rStr.cfdrop);
	SetDlgItemText(IDC_CHECK_REINDEX, rStr.cidx);

	SetDlgItemText(IDC_STATIC_DEF, rStr.copen);
	SetDlgItemText(IDC_STATIC_PNG, rStr.cpng);
	SetDlgItemText(IDC_BUTTON_PNG, rStr.browser);
	SetDlgItemText(IDC_BUTTON_DEF, rStr.browser);

	InitCheckBox();
	
	aComboPriority.InitCtrl(GetDlgItem(IDC_LISTCTRL_PRIORITY));
	aComboASync.InitCtrl(GetDlgItem(IDC_LISTCTRL_AUTOSYNC));
	aComboAPlay.InitCtrl(GetDlgItem(IDC_LISTCTRL_AUTOPLAY));
	aComboCache.InitCtrl(GetDlgItem(IDC_LISTCTRL_CACHE));
	aComboMonitor.InitCtrl(GetDlgItem(IDC_LISTCTRL_MONITOR), TRUE);
	aComboLoop.InitCtrl(GetDlgItem(IDC_LISTCTRL_LOOP));
	aComboLog.InitCtrl(GetDlgItem(IDC_LISTCTRL_LOG));

	//Skip Time
	RECT rc;
	int width = 68, widtht;
	ListTime.RegisterClass();
	ListTime.ShowBorder(FALSE);
	ListTime.Show3DColor();
	ListTime.SubclassWindow( GetDlgItem(IDC_LISTCTRL_TIME) );
	ListTime.ShowHeader(FALSE);
	ListTime.ShowSelected(FALSE);
	ListTime.GetClientRect(&rc);
	widtht = (rc.right - rc.left)/2 - width-1;
	ListTime.AddColumn(_T(""), width, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_NONE);
	ListTime.AddColumn(_T(""), widtht, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_DATETIME, ITEM_FLAGS_TIME_ONLY);
	ListTime.AddColumn(_T(""), width, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_NONE);
	ListTime.AddColumn(_T(""), widtht, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_DATETIME, ITEM_FLAGS_TIME_ONLY);
	ListTime.AddItem(rStr.cstart);
	ListTime.SetItemText(0, 2, rStr.cend);
	ListTime.SelectItem(0, 0);
	value_i = 0;
	if(mconfig.GetString(_T("ss"), value_s)) {
		if(value_s.Find(_T(":")) > 0) {
			int a1, a2, a3;
			int len = _stscanf_s(value_s.GetBuffer(), _T("%d:%d:%d"), &a1, &a2, &a3);
			if(len == 1)
				value_i = a1;
			else if(len == 2)
				value_i = a1 *60 + a2;
			else if(len == 3)
				value_i = a1 * 3600 + a2 *60 + a3;
		} else
			value_i = _ttoi(value_s);
	}
	ListTime.SetItemText(0, 1, ListTimetoString(value_i));
	ListTime.SetItemText(0, 3, ListTimetoString(mconfig.GetInteger(_T("end_pos"), 0, true)));

	//Priority
	aComboPriority.SetTitle(rStr.piority);
	aComboPriority.AddString(rStr.autos);
	aComboPriority.AddString(rStr.piority1);
	aComboPriority.AddString(rStr.piority2);
	aComboPriority.AddString(rStr.piority3);
	aComboPriority.AddString(rStr.piority4);
	aComboPriority.AddString(rStr.piority5);
	aComboPriority.AddString(rStr.piority6);

	if(mconfig.GetString(_T("priority"), value_s)) {
		if(value_s == _T("realtime"))
			aComboPriority.SelecteIndex(prio_realtime);
		else if(value_s == _T("high"))
			aComboPriority.SelecteIndex(prio_high);
		else if(value_s == _T("abovenormal"))
			aComboPriority.SelecteIndex(prio_abovenormal);
		else if(value_s == _T("normal"))
			aComboPriority.SelecteIndex(prio_normal);
		else if(value_s == _T("belownormal"))
			aComboPriority.SelecteIndex(prio_belownormal);
		else if(value_s == _T("idle"))
			aComboPriority.SelecteIndex(prio_idle);
		else
			aComboPriority.SelecteIndex(prio_auto);
	} else
		aComboPriority.SelecteIndex(prio_auto);

	//AutoSync
	aComboASync.SetTitle(rStr.async);
	aComboASync.AddString(rStr.async0);
	aComboASync.AddString(rStr.async1);
	aComboASync.AddString(rStr.async2);
	aComboASync.AddString(rStr.async3);
	aComboASync.AddString(rStr.async4);
	aComboASync.AddString(rStr.async5);

	int sync = mconfig.GetInteger(_T("autosync"), -1);
	if(sync == 0) {
		value_d = mconfig.GetDouble(_T("mc"), -1);
		if(value_d > 0) {
			if(value_d <= 0.01)
				aComboASync.SelecteIndex(sync_flvex);
			else
				aComboASync.SelecteIndex(sync_flv);
		} else if(mconfig.GetBoolean(_T("nobps")))
			aComboASync.SelecteIndex(sync_avi);
		else
			aComboASync.SelecteIndex(sync_none);
	} else if(sync > 0 && sync <= 30 )
		aComboASync.SelecteIndex(sync_normal);
	else if(sync > 30 )
		aComboASync.SelecteIndex(sync_fast);
	else
		aComboASync.SelecteIndex(sync_none);

	//AutoPlay
	aComboAPlay.SetTitle(rStr.aplay);
	aComboAPlay.AddString(rStr.playna);
	aComboAPlay.AddString(rStr.autos);
	aComboAPlay.AddString(rStr.playex + _T(" (n = 6)"));
	aComboAPlay.AddString(rStr.playex + _T(" (n = 10)"));
	aComboAPlay.AddString(rStr.playex + _T(" (n = 20)"));
	aComboAPlay.AddString(rStr.playex + _T(" (n = 50)"));
	aComboAPlay.AddString(rStr.playex + _T(" (n = 100)"));
	aComboAPlay.AddString(rStr.playex + _T(" (n = 200)"));

	if(mconfig.GetBoolean(_T("autoplay"))) {
		value_i = mconfig.GetInteger(_T("autoplay_fuzziness"), 0, true);
		if(value_i > 0) {
			if(value_i >= 200)
				aComboAPlay.SelecteIndex(play_ex_200);
			else if(value_i >= 100)
				aComboAPlay.SelecteIndex(play_ex_100);
			else if(value_i >= 50)
				aComboAPlay.SelecteIndex(play_ex_50);
			else if(value_i >= 20)
				aComboAPlay.SelecteIndex(play_ex_20);
			else if(value_i >= 10)
				aComboAPlay.SelecteIndex(play_ex_10);
			else
				aComboAPlay.SelecteIndex(play_ex_6);
		} else
			aComboAPlay.SelecteIndex(play_auto);
	} else
		aComboAPlay.SelecteIndex(play_none);

	//Cache
	aComboCache.SetTitle(rStr.cache);
	aComboCache.AddString(rStr.nos);
	aComboCache.AddString(_T("512 KB"));
	aComboCache.AddString(_T("1 M"));
	aComboCache.AddString(_T("2 M"));
	aComboCache.AddString(_T("4 M"));
	aComboCache.AddString(_T("8 M"));
	aComboCache.AddString(_T("16 M"));
	aComboCache.AddString(_T("32 M"));
	aComboCache.AddString(_T("64 M"));
	aComboCache.AddString(_T("96 M"));
	aComboCache.AddString(_T("128 M"));
	aComboCache.AddString(_T("256 M"));
	aComboCache.AddString(rStr.autos);
	
	mconfig.GetInteger(_T("cache-min"));
	if(mconfig.GetBoolean(_T("auto-cache")))
		aComboCache.SelecteIndex(cache_auto);
	else {
		value_i = mconfig.GetInteger(_T("cache"), -1);
		if(value_i <= 0)
			aComboCache.SelecteIndex(cache_none);
		else if(value_i <= 512)
			aComboCache.SelecteIndex(cache_512K);
		else if(value_i <= 1024)
			aComboCache.SelecteIndex(cache_1);
		else if(value_i <= 2048)
			aComboCache.SelecteIndex(cache_2);
		else if(value_i <= 4096)
			aComboCache.SelecteIndex(cache_4);
		else if(value_i <= 8192)
			aComboCache.SelecteIndex(cache_8);
		else if(value_i <=16384)
			aComboCache.SelecteIndex(cache_16);
		else if(value_i <=32768)
			aComboCache.SelecteIndex(cache_32);
		else if(value_i <=65536)
			aComboCache.SelecteIndex(cache_64);
		else if(value_i <=98304)
			aComboCache.SelecteIndex(cache_96);
		else if(value_i <=131072)
			aComboCache.SelecteIndex(cache_128);
		else
			aComboCache.SelecteIndex(cache_256);
	}


	//Monitor
	aComboMonitor.SetTitle(rStr.monitor);
	aComboMonitor.AddString(rStr.autos);
	aComboMonitor.AddString(_T("4:3"));
	aComboMonitor.AddString(_T("8:5"));
	aComboMonitor.AddString(_T("16:9"));

	if(mconfig.GetString(_T("monitoraspect"),value_s)){
		if(aComboMonitor.FindStringExactNoCase(0, value_s) < 0)
			aComboMonitor.AddString(value_s);
		aComboMonitor.SelecteText(value_s);
	} else 
		aComboMonitor.SelecteIndex(0);

	//Loop
	aComboLoop.SetTitle(rStr.loops);
	aComboLoop.AddString(rStr.playna);
	aComboLoop.AddString(rStr.loopf);
	aComboLoop.AddString(rStr.loopl);

	if(mconfig.GetInteger(_T("loop"), -1) >= 0) {
		if(mconfig.GetBoolean(_T("loop_all"), false, true))
			aComboLoop.SelecteIndex(loop_list);
		else
			aComboLoop.SelecteIndex(loop_single);
	} else
		aComboLoop.SelecteIndex(loop_none);

	//Log
	aComboLog.SetTitle(rStr.log);
	aComboLog.AddString(rStr.logn);
	aComboLog.AddString(rStr.log0);
	aComboLog.AddString(rStr.log1);
	aComboLog.AddString(rStr.log2);
	aComboLog.AddString(rStr.log3);
	aComboLog.AddString(rStr.log4);
	aComboLog.AddString(rStr.log5);
	aComboLog.AddString(rStr.logc);
	aComboLog.AddString(rStr.logc +_T("(v=1)"));
	aComboLog.AddString(rStr.logc +_T("(v=2)"));
	aComboLog.AddString(rStr.logc +_T("(v=3)"));
	aComboLog.AddString(rStr.logc +_T("(v=4)"));
	aComboLog.AddString(rStr.logc +_T("(v=5)"));

	value_i = mconfig.GetInteger(_T("log"), 0, true);
	if(value_i) {
		if(value_i == 2) {
			value_i = mconfig.GetInteger(_T("v"), 0);
			value_i += log_console;
			if(value_i < log_console || value_i > log_console_v5)
				value_i = log_console;
			aComboLog.SelecteIndex(value_i);
		} else {
			value_i = mconfig.GetInteger(_T("v"), 0);
			value_i += log_normal;
			if(value_i < log_normal || value_i > log_v5)
				value_i = log_normal;
			aComboLog.SelecteIndex(value_i);
		}
	} else
		aComboLog.SelecteIndex(log_none);
	

	DoDataExchange();
	
	return TRUE;
}

LRESULT CPlayerDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CPlayerDlg::OnBnClickedButtonPng(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DoDataExchange(TRUE);
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);
	CString csFolder;
	if(SelectFolder(m_hWnd, csFolder))
		m_png = csFolder;
	::SetCurrentDirectory(szFilePath);
	DoDataExchange();
	return 0;
}

LRESULT CPlayerDlg::OnBnClickedButtonDef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DoDataExchange(TRUE);
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);
	CString csFolder;
	if(SelectFolder(m_hWnd, csFolder))
		m_def = csFolder;
	::SetCurrentDirectory(szFilePath);
	DoDataExchange();
	return 0;
}

long CPlayerDlg::ListStringtoTime(CString time)
{
	int ttime = 0, itime = _ttoi(time);

	ttime = itime % 100;
	ttime += ((itime/100) % 100) * 60;
	ttime += ((itime/10000)) * 3600;

	return ttime;

}

CString CPlayerDlg::ListTimetoString(long time)
{
	CString stime = _T("0");

	if(time <= 0)
		return stime;

	stime.Format(_T("00000000%02d%02d%02d"), time/3600, (time/60)%60, time%60);

	return stime;
}
