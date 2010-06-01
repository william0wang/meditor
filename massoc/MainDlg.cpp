// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include <sstream>
#include <iomanip>
#include <atltime.h>
#include <shobjidl.h>

#include "AtlStdioFile.h"
#include "MainDlg.h"
#include "shared.h"
#include "Reg.h"

CMainDlg::CMainDlg(HINSTANCE dll, int appLang)
{
	m_wndListCtrl.RegisterClass();

	m_appLang = appLang;
	m_dll_getted = false;
	m_have_icons = false;
	m_is_vista = false;

	m_rightOpen = TRUE;
	m_rightPlay = FALSE;
	m_mpc = FALSE;

	HINSTANCE old_res = NULL;
	if(dll) {
		old_res = _Module.GetResourceInstance();
		_Module.SetResourceInstance(dll);
	}

	m_type_video = ResStr(IDS_ASSOC_V);
	m_type_video2 = ResStr(IDS_ASSOC_V2);
	m_type_audio = ResStr(IDS_ASSOC_A);
	m_type_list = ResStr(IDS_ASSOC_LIST);
	m_type_flash = ResStr(IDS_ASSOC_FLASH);
	m_type_dshow = ResStr(IDS_ASSOC_DS);
	m_type_inner = ResStr(IDS_ASSOC_SEC);

	m_str_player_use = ResStr(IDS_PLAYER_USE);
	m_str_player_samedir = ResStr(IDS_PLAYER_SAMEDIR);
	m_str_assos_input = ResStr(IDS_ASSOC_INPUT);
	m_str_assos_del = ResStr(IDS_ASSOC_DEL);
	m_str_assos_op = ResStr(IDS_ASSOC_OP);
	m_str_assos_pl = ResStr(IDS_ASSOC_PL);
	m_str_assos_as = ResStr(IDS_ASSOC_AS);
	m_str_assos_ct = ResStr(IDS_ASSOC_CT);
	m_str_assos_mv = ResStr(IDS_ASSOC_MV);
	m_str_assos_mov = ResStr(IDS_ASSOC_MOV);
	m_str_assos_avi = ResStr(IDS_ASSOC_AVI);
	m_str_assos_avs = ResStr(IDS_ASSOC_AVS);
	m_str_assos_list = ResStr(IDS_ASSOC_LISTFILE);
	m_str_assos_ex = ResStr(IDS_ASSOC_EX);
	m_str_assos_inf = ResStr(IDS_ASSOC_INF);
	m_str_assos_typ = ResStr(IDS_ASSOC_TYP);
	m_str_assos_ico = ResStr(IDS_ASSOC_ICO);

	m_str_ui_ok = ResStr(IDS_ASSOC_BOK);
	m_str_ui_add = ResStr(IDS_ASSOC_BADD);
	m_str_ui_del = ResStr(IDS_ASSOC_BDEL);
	m_str_ui_all = ResStr(IDS_ASSOC_BALL);
	m_str_ui_none = ResStr(IDS_ASSOC_BNONE);
	m_str_ui_rem = ResStr(IDS_ASSOC_BRE);
	m_str_ui_icons = ResStr(IDS_ASSOC_ICONS);
	m_str_ui_right = ResStr(IDS_ASSOC_RENU);
	m_str_ui_open = ResStr(IDS_ASSOC_OPEN);
	m_str_ui_play = ResStr(IDS_ASSOC_PLAY);

	if(old_res)
		_Module.SetResourceInstance(old_res);

	OSVERSIONINFO version;

	version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&version)) {
		if(version.dwMajorVersion >= 6)
			m_is_vista = true;
	}

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);

	if(appLang == 2)
		inifile = m_program_dir + _T("massoc_en.ini");
	else if(appLang == 3 || appLang == 4)
		inifile = m_program_dir + _T("massoc_tc.ini");
	else
		inifile = m_program_dir + _T("massoc.ini");

	CString right = m_program_dir.Right(8);
	if(right == _T("\\codecs\\"))
		m_program_dir = m_program_dir.Left(m_program_dir.GetLength() - 7);
	else {
		right = m_program_dir.Right(7);
		if(right == _T("\\tools\\"))
			m_program_dir = m_program_dir.Left(m_program_dir.GetLength() - 6);
	}

	m_mpc_exe = m_program_dir + _T("tools\\mplayerc.exe");
	if(IsFileExist(m_mpc_exe))
		m_mpc = TRUE;

	m_icons_dll = _T("micons.dll");
	m_icons_org = m_program_dir + _T("micons.dll");
	m_player_exe = m_program_dir + _T("mplayer.exe");
	m_editor_exe = m_program_dir + _T("meditor2.exe");
	if(!IsFileExist(m_editor_exe))
		m_editor_exe = m_program_dir + _T("meditor.exe");

	LoadAssocINI();

}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}


LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	::SetWindowText(GetDlgItem(IDOK), m_str_ui_ok);
	::SetWindowText(GetDlgItem(IDC_ADD), m_str_ui_add);
	::SetWindowText(GetDlgItem(IDC_DEL), m_str_ui_del);
	::SetWindowText(GetDlgItem(IDC_ALL), m_str_ui_all);
	::SetWindowText(GetDlgItem(IDC_NONE), m_str_ui_none);
	::SetWindowText(GetDlgItem(IDC_RECOMMAND), m_str_ui_rem);
	::SetWindowText(GetDlgItem(IDC_CHECK_RMENU), m_str_ui_open);
	::SetWindowText(GetDlgItem(IDC_CHECK_RMENU2), m_str_ui_play);
	::SetWindowText(GetDlgItem(IDC_STATIC_ICONS), m_str_ui_icons);
	::SetWindowText(GetDlgItem(IDC_STATIC_RIGHT), m_str_ui_right);

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	m_wndListCtrl.SubclassWindow( GetDlgItem( IDC_LISTCTRL ) );

	InitBasicList();

	m_icon.Attach(GetDlgItem(IDC_COMBO_ICON));

	m_icon.SetRedraw(FALSE);

	m_icon.SetAutoResize(TRUE);

	m_icon.AddString(_T("micons.dll"));
	m_icon.SetCurSel(0);

	CString m_skin_dir = m_program_dir + _T("skin");
	if(IsFileExist(m_skin_dir)) {
		TCHAR szCurPath[MAX_PATH + 1];
		::GetCurrentDirectory(MAX_PATH,szCurPath);
		::SetCurrentDirectory(m_skin_dir);
		CFindFile finder;
		if(finder.FindFile(_T("micons*.dll"))) {
			m_icon.AddString(finder.GetFileName());
			while(finder.FindNextFile())
				m_icon.AddString(finder.GetFileName());
		}
		finder.Close();
		::SetCurrentDirectory(szCurPath);
	}

	int index = m_icon.FindStringExact(0, m_icons_dll);
	if(index > 0)
		m_icon.SetCurSel(index);

	m_icon.SetRedraw(TRUE);

	DoDataExchange();

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(TRUE);

	SaveAssocIni();

	ApplyChange();

	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBnClickedAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CString ico;
	CListArray < CString > tComboList;
	CListArray < CString > aComboList;

	tComboList.Add(m_type_video);
	tComboList.Add(m_type_video2);
	tComboList.Add(m_type_audio);
	tComboList.Add(m_type_flash);
	tComboList.Add(m_type_list);
	tComboList.Add(m_type_dshow);
	tComboList.Add(m_type_inner);

	for(int i = 0; i < 40; i++)
	{
		ico.Format(_T("%d"), i);
		aComboList.Add(ico);
	}

	int nNewItem = m_wndListCtrl.AddItem( L"" );
	m_wndListCtrl.SetItemFormat( nNewItem, 3, ITEM_FORMAT_COMBO, ITEM_FLAGS_NONE, tComboList );
	m_wndListCtrl.SetItemComboIndex( nNewItem, 3, 0 );
	m_wndListCtrl.SetItemFormat( nNewItem, 4, ITEM_FORMAT_COMBO, ITEM_FLAGS_NONE, aComboList );
	m_wndListCtrl.SetItemComboIndex( nNewItem, 4, 0 );	
	return 0;
}

LRESULT CMainDlg::OnBnClickedDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int item, subitem, i=0;
	if(m_wndListCtrl.GetFocusItem(item, subitem))
	{
		m_wndListCtrl.DeleteItem(item);
		if((UINT)item < m_AssocList.size()) {
			vector<AssocItem>::iterator it;
			for(it=m_AssocList.begin();it!= m_AssocList.end();it++)
			{
				if(i == item) {
					m_AssocList.erase(it);
					break;
				}
				++i;
			}
		}
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_wndListCtrl.LockWindowUpdate(TRUE);
	for(int i = 0; i < m_wndListCtrl.GetItemCount(); i++)
	{
		m_wndListCtrl.SetItemCheck( i, 0,  TRUE);
	}
	m_wndListCtrl.LockWindowUpdate(FALSE);
	m_wndListCtrl.InvalidateRect(NULL);
	return 0;
}

LRESULT CMainDlg::OnBnClickedNone(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_wndListCtrl.LockWindowUpdate(TRUE);
	for(int i = 0; i < m_wndListCtrl.GetItemCount(); i++)
	{
		m_wndListCtrl.SetItemCheck( i, 0,  FALSE);
	}
	m_wndListCtrl.LockWindowUpdate(FALSE);
	m_wndListCtrl.InvalidateRect(NULL);
	return 0;
}

LRESULT CMainDlg::OnBnClickedRecommand(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_wndListCtrl.LockWindowUpdate(TRUE);
	for(UINT i = 0; i < m_AssocList.size(); i++)
	{
		if(m_AssocList[i].m_recommand)
			m_wndListCtrl.SetItemCheck( i, 0,  TRUE);
		else
			m_wndListCtrl.SetItemCheck( i, 0,  FALSE);
	}
	m_wndListCtrl.LockWindowUpdate(FALSE);
	m_wndListCtrl.InvalidateRect(NULL);
	return 0;
}

void CMainDlg::SaveAssocIni()
{
	CAtlStdioFile file;
	CString len;
	if(file.Create(inifile,  GENERIC_WRITE, 0, CREATE_ALWAYS) == S_OK) {
		int index = m_icon.GetCurSel();
		if(index > 0)
			m_icon.GetLBText(index, m_icons_str);
		else
			m_icons_str = _T("micons.dll");
		len.Format(_T("icon=%s\r\n"), m_icons_str);
		file.WriteLine(len);

		for(int i = 0; i < m_wndListCtrl.GetItemCount(); i++) {
			bool is_checked = false;
			int type = ASSOC_TYPE_VIDEO;
			CString ext = m_wndListCtrl.GetItemText(i, 1);
			CString info = m_wndListCtrl.GetItemText(i, 2);
			CString stype = m_wndListCtrl.GetItemText(i, 3);
			CString icon = m_wndListCtrl.GetItemText(i, 4);

			if(ext.GetLength() < 1)
				continue;

			if(stype == m_type_list)
				type = ASSOC_TYPE_PLAYLIST;
			else if(stype == m_type_flash)
				type = ASSOC_TYPE_FLASH;
			else if(stype == m_type_dshow)
				type = ASSOC_TYPE_DSHOW;
			else if(stype == m_type_inner)
				type = ASSOC_TYPE_DSHOWINNER;

			len.Format(_T("%s,%s,%d,%s,%d"), ext, info,	type, icon, m_AssocList[i].m_recommand);
			file.WriteLine(len);
		}
		file.Close();
	}

}

void CMainDlg::LoadAssocINI()
{
	CAtlStdioFile file;
	AssocItem item;
	int itype, iico, ireco;
	bool find_icon = false;
	CString line, name, val, ext, info, type, ico;
	
	if(file.Create(inifile,  GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING) == S_OK) {
		while(file.ReadLine(line)) {
			int sp = -1;
			if(!find_icon) {
				sp = line.Find(_T("="));
				if(sp > 0) {
					name = line.Left(sp);
					if(name == _T("icon"))	{
						line.Delete(0,sp + 1);
						val = line;
						val.Trim();
						if(val.GetLength() > 1) {
							m_icons_dll = m_icons_str = val;
							find_icon = true;
						}
					}
				}
			} else {
				sp = line.Find(_T(","));
				if(sp < 0)
					continue;
				ext = line.Left(sp);
				line.Delete(0,sp + 1);

				sp = line.Find(_T(","));
				if(sp < 0)
					continue;
				info = line.Left(sp);
				line.Delete(0,sp + 1);

				sp = line.Find(_T(","));
				if(sp < 0)
					continue;
				type = line.Left(sp);
				line.Delete(0,sp + 1);

				ireco = 0;
				sp = line.Find(_T(","));
				if(sp > 0) {
					ico = line.Left(sp);
					line.Delete(0,sp + 1);
					ireco = _ttoi(line);
					if(ireco != 1)
						ireco = 0;
				} else
					ico = line;

				itype = _ttoi(type);
				iico = _ttoi(ico);

				if(ext.GetLength() > 0 && info.GetLength() > 0
					&& itype >=0 && itype < 6 && iico >= 0) {
					item.Set(ext, info, itype, iico, ireco);
					m_AssocList.push_back(item);
				}
			}
		}
		file.Close();
	} else {
		item.Set(L"3gp", L"3GP " + m_str_assos_mv, ASSOC_TYPE_VIDEO, 13, 1);
		m_AssocList.push_back(item);
		item.Set(L"asf", L"Advanced Streaming " + m_type_video, ASSOC_TYPE_VIDEO, 16, 1);
		m_AssocList.push_back(item);
		item.Set(L"avi", L"AVI " + m_str_assos_avi, ASSOC_TYPE_VIDEO, 1, 1);
		m_AssocList.push_back(item);
		item.Set(L"flv", L"Flash " + m_type_video, ASSOC_TYPE_VIDEO, 2, 1);
		m_AssocList.push_back(item);
		item.Set(L"mp4", L"MPEG-4 " + m_str_assos_mov, ASSOC_TYPE_VIDEO, 6, 1);
		m_AssocList.push_back(item);
		item.Set(L"mkv", L"Matroska" + m_type_video, ASSOC_TYPE_VIDEO, 3, 1);
		m_AssocList.push_back(item);
		item.Set(L"mov", L"QuickTime " + m_str_assos_mov, ASSOC_TYPE_VIDEO, 12, 1);
		m_AssocList.push_back(item);
		item.Set(L"mpeg", L"MPEG " + m_str_assos_mov, ASSOC_TYPE_VIDEO, 7, 1);
		m_AssocList.push_back(item);
		item.Set(L"mpg", L"MPEG " + m_str_assos_mov, ASSOC_TYPE_VIDEO, 8, 1);
		m_AssocList.push_back(item);
		item.Set(L"ogm", L"OGG " + m_type_video, ASSOC_TYPE_VIDEO, 4, 1);
		m_AssocList.push_back(item);
		item.Set(L"pmp", L"PMP " + m_str_assos_mov, ASSOC_TYPE_VIDEO, 38, 1);
		m_AssocList.push_back(item);
		item.Set(L"qt", L"QuickTime " + m_str_assos_mov, ASSOC_TYPE_VIDEO, 12, 1);
		m_AssocList.push_back(item);
		item.Set(L"rm", L"RealMedia " + m_type_video, ASSOC_TYPE_VIDEO, 14, 1);
		m_AssocList.push_back(item);
		item.Set(L"rmvb", L"RealMedia VBR " + m_type_video, ASSOC_TYPE_VIDEO, 15, 1);
		m_AssocList.push_back(item);
		item.Set(L"vob", L"DVD " + m_str_assos_mov, ASSOC_TYPE_VIDEO, 11, 1);
		m_AssocList.push_back(item);
		item.Set(L"webm", L"WebM " + m_type_video, ASSOC_TYPE_VIDEO, 35, 1);
		m_AssocList.push_back(item);
		item.Set(L"wmv", L"Windows Media " + m_type_video, ASSOC_TYPE_VIDEO, 17, 1);
		m_AssocList.push_back(item);

		item.Set(L"tp", L"HDTV " + m_str_assos_mov, ASSOC_TYPE_VIDEO, 10, 1);
		m_AssocList.push_back(item);
		item.Set(L"ts", L"HDTV " + m_str_assos_mov, ASSOC_TYPE_VIDEO, 9, 1);
		m_AssocList.push_back(item);
		item.Set(L"m2ts", L"HDTV " + m_str_assos_mov, ASSOC_TYPE_VIDEO, 9, 1);
		m_AssocList.push_back(item);
		item.Set(L"mts", L"HDTV " + m_str_assos_mov, ASSOC_TYPE_VIDEO, 9, 1);
		m_AssocList.push_back(item);

		item.Set(L"3g2", L"3GP " + m_str_assos_mv, ASSOC_TYPE_VIDEO2, 13, 1);
		m_AssocList.push_back(item);
		item.Set(L"3gp2", L"3GP " + m_str_assos_mv, ASSOC_TYPE_VIDEO2, 13, 1);
		m_AssocList.push_back(item);
		item.Set(L"3gpp", L"3GP " + m_str_assos_mv, ASSOC_TYPE_VIDEO2, 13, 1);
		m_AssocList.push_back(item);
		item.Set(L"avs", m_str_assos_avs, ASSOC_TYPE_VIDEO2, 35, 1);
		m_AssocList.push_back(item);
		item.Set(L"bik", L"Bink " + m_type_video, ASSOC_TYPE_VIDEO2, 35, 1);
		m_AssocList.push_back(item);
		item.Set(L"evo", L"HDTV " + m_type_video, ASSOC_TYPE_VIDEO2, 35, 1);
		m_AssocList.push_back(item);
		item.Set(L"dat", L"VCD " + m_str_assos_mov, ASSOC_TYPE_VIDEO2, 5, 1);
		m_AssocList.push_back(item);
		item.Set(L"divx", L"DIVX " + m_type_video, ASSOC_TYPE_VIDEO2, 35, 1);
		m_AssocList.push_back(item);
		item.Set(L"mpv", L"MPEG " + m_str_assos_mov, ASSOC_TYPE_VIDEO2, 7, 1);
		m_AssocList.push_back(item);
		item.Set(L"m1v", L"MPEG-1 " + m_str_assos_mov, ASSOC_TYPE_VIDEO2, 7, 1);
		m_AssocList.push_back(item);
		item.Set(L"m2v", L"MPEG-2 " + m_str_assos_mov, ASSOC_TYPE_VIDEO2, 7, 1);
		m_AssocList.push_back(item);
		item.Set(L"m4v", L"MPEG-4 " + m_str_assos_mov, ASSOC_TYPE_VIDEO2, 6, 1);
		m_AssocList.push_back(item);
		item.Set(L"pss", L"MPEG-1 " + m_str_assos_mov, ASSOC_TYPE_VIDEO2, 7, 1);
		m_AssocList.push_back(item);
		item.Set(L"pva", L"MPEG-1 " + m_str_assos_mov, ASSOC_TYPE_VIDEO2, 7, 1);
		m_AssocList.push_back(item);
		item.Set(L"tpr", L"MPEG-1 " + m_str_assos_mov, ASSOC_TYPE_VIDEO2, 7, 1);
		m_AssocList.push_back(item);
		item.Set(L"vp6", L"VP6/VP7 " + m_type_video, ASSOC_TYPE_VIDEO2, 35, 1);
		m_AssocList.push_back(item);
		//item.Set(L"vp7", L"VP7 " + m_type_video, ASSOC_TYPE_VIDEO2, 35);
		//m_AssocList.push_back(item);
		//item.Set(L"vp8", L"VP8 " + m_type_video, ASSOC_TYPE_VIDEO2, 35);
		//m_AssocList.push_back(item);

		item.Set(L"aac", L"Advanced Audio Coding " + m_type_audio, ASSOC_TYPE_AUDIO, 18);
		m_AssocList.push_back(item);
		item.Set(L"ac3", L"Dolby AC3 " + m_type_audio, ASSOC_TYPE_AUDIO, 19);
		m_AssocList.push_back(item);
		item.Set(L"aiff", L"AIFF Mac " + m_type_audio, ASSOC_TYPE_AUDIO, 36);
		m_AssocList.push_back(item);
		item.Set(L"amr", L"AMR " + m_type_audio, ASSOC_TYPE_AUDIO, 20, 1);
		m_AssocList.push_back(item);
		item.Set(L"dts", L"DTS " + m_type_audio, ASSOC_TYPE_AUDIO, 21, 1);
		m_AssocList.push_back(item);
		item.Set(L"flac", L"FLAC " + m_type_audio, ASSOC_TYPE_AUDIO, 22);
		m_AssocList.push_back(item);
		item.Set(L"m4a", L"MPEG-4 " + m_type_audio, ASSOC_TYPE_AUDIO, 23);
		m_AssocList.push_back(item);
		item.Set(L"mp2", L"MPEG " + m_type_audio, ASSOC_TYPE_AUDIO, 36);
		m_AssocList.push_back(item);
		item.Set(L"mp3", L"MPEG Laye-3 " + m_type_audio, ASSOC_TYPE_AUDIO, 24);
		m_AssocList.push_back(item);
		item.Set(L"mpa", L"MPEG " + m_type_audio, ASSOC_TYPE_AUDIO, 25);
		m_AssocList.push_back(item);
		item.Set(L"mpc", L"MusePack " + m_type_audio, ASSOC_TYPE_AUDIO, 26);
		m_AssocList.push_back(item);
		item.Set(L"mka", L"Matroska " + m_type_audio, ASSOC_TYPE_AUDIO, 27);
		m_AssocList.push_back(item);
		item.Set(L"ogg", L"Vorbis " + m_type_audio, ASSOC_TYPE_AUDIO, 28);
		m_AssocList.push_back(item);
		item.Set(L"ra", L"RealMedia " + m_type_audio, ASSOC_TYPE_AUDIO, 29, 1);
		m_AssocList.push_back(item);
		item.Set(L"wav", L"WAVE " + m_type_audio, ASSOC_TYPE_AUDIO, 30);
		m_AssocList.push_back(item);
		item.Set(L"wma", L"Windows Media " + m_type_audio, ASSOC_TYPE_AUDIO, 31);
		m_AssocList.push_back(item);

		item.Set(L"swf", L"Flash " + m_str_assos_ct, ASSOC_TYPE_FLASH, 0, 1);
		m_AssocList.push_back(item);

		item.Set(L"m3u", L"M3U " + m_str_assos_list, ASSOC_TYPE_PLAYLIST, 32);
		m_AssocList.push_back(item);
		item.Set(L"m3u8", L"M3U8 " + m_str_assos_list, ASSOC_TYPE_PLAYLIST, 33);
		m_AssocList.push_back(item);
		item.Set(L"pls", L"PLS " + m_str_assos_list, ASSOC_TYPE_PLAYLIST, 34, 1);
		m_AssocList.push_back(item);
	}
}

void CMainDlg::InitBasicList()
{
	m_wndListCtrl.SetFocusSubItem( TRUE );
	m_wndListCtrl.ShowHeaderSort(FALSE);
	m_wndListCtrl.SetSingleSelect(TRUE);

	m_wndListCtrl.AddColumn( _T( "" ), 0, ITEM_IMAGE_CHECK_OFF, TRUE, ITEM_FORMAT_CHECKBOX );
	m_wndListCtrl.AddColumn( m_str_assos_ex, 100, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_EDIT, ITEM_FLAGS_CENTRE);
	m_wndListCtrl.AddColumn( m_str_assos_inf, 260, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_EDIT, ITEM_FLAGS_CENTRE);
	m_wndListCtrl.AddColumn( m_str_assos_typ, 130, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_COMBO, ITEM_FLAGS_CENTRE);
	m_wndListCtrl.AddColumn( m_str_assos_ico, 52, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_COMBO, ITEM_FLAGS_CENTRE);

	CString ico;
	CListArray < CString > tComboList;
	CListArray < CString > aComboList;

	tComboList.Add(m_type_video);
	tComboList.Add(m_type_video2);
	tComboList.Add(m_type_audio);
	tComboList.Add(m_type_flash);
	tComboList.Add(m_type_list);
	tComboList.Add(m_type_dshow);
	tComboList.Add(m_type_inner);

	for(int i = 0; i < 40; i++)
	{
		ico.Format(_T("%d"), i);
		aComboList.Add(ico);
	}

	for ( UINT nItem = 0; nItem < m_AssocList.size(); nItem++ )
	{
		wostringstream osItemText;

		CString type;
		COLORREF color;

		int nNewItem = m_wndListCtrl.AddItem( L"" );
		if(IsAssoced(m_AssocList[nItem].m_ext))
			m_wndListCtrl.SetItemCheck( nNewItem, 0,  TRUE);

		m_wndListCtrl.SetItemText( nNewItem, 1, m_AssocList[nItem].m_ext );

		m_wndListCtrl.SetItemText( nNewItem, 2, m_AssocList[nItem].m_info );

		switch(m_AssocList[nItem].m_type)
		{
		case ASSOC_TYPE_VIDEO:
			color = RGB(160,200,230);
			type = m_type_video;
			break;
		case ASSOC_TYPE_VIDEO2:
			color = RGB(160,230,230);
			type = m_type_video;
			break;
		case ASSOC_TYPE_AUDIO:
			color = RGB(230,250,180);
			type = m_type_audio;
			break;
		case ASSOC_TYPE_FLASH:
			color = RGB(230,150,150);
			type = m_type_flash;
			break;
		case ASSOC_TYPE_PLAYLIST:
			color = RGB(150,150,230);
			type = m_type_list;
			break;
		case ASSOC_TYPE_DSHOW:
			color = RGB(240,100,100);
			type = m_type_dshow;
			break;
		}
		m_wndListCtrl.SetItemColours( nNewItem, 1, color, RGB( 0, 0, 0 ) );
		m_wndListCtrl.SetItemColours( nNewItem, 2, color, RGB( 0, 0, 0 ) );
		m_wndListCtrl.SetItemColours( nNewItem, 3, color, RGB( 0, 0, 0 ) );
		m_wndListCtrl.SetItemColours( nNewItem, 4, color, RGB( 0, 0, 0 ) );
		m_wndListCtrl.SetItemFormat( nNewItem, 3, ITEM_FORMAT_COMBO, ITEM_FLAGS_NONE, tComboList );
		m_wndListCtrl.SetItemComboIndex( nNewItem, 3, m_AssocList[nItem].m_type );
		m_wndListCtrl.SetItemFormat( nNewItem, 4, ITEM_FORMAT_COMBO, ITEM_FLAGS_NONE, aComboList );
		m_wndListCtrl.SetItemComboIndex( nNewItem, 4, m_AssocList[nItem].m_icon );	
	}

}



bool CMainDlg::IsAssoced(CString ext)
{
	CReg reg;
	TCHAR dll[256], *sp;
	CString regstr;

	if(m_is_vista) {
		regstr = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + ext + _T("\\UserChoice");
		if(reg.ShowContent_STR(HKEY_CURRENT_USER, regstr, _T("Progid")))
		{
			regstr = _T("MPlayer.AssocFile.") + ext;
			if(!_tcsicmp(reg.content, regstr))
				return true;
			else
				return false;
		}
	}

	regstr = _T("mplayer.") + ext;
	if(reg.ShowContent_STR(HKEY_CLASSES_ROOT,regstr,_T("")))
	{
		if(!m_dll_getted)
		{
			m_dll_getted = true;
			regstr = _T("mplayer.") + ext + _T("\\DefaultIcon");
			if(reg.ShowContent_STR(HKEY_CLASSES_ROOT,regstr,_T("")))
			{
				_tcsncpy_s(dll, 255, _tcsrchr(reg.content, _T('\\')) + 1, _TRUNCATE);
				sp = _tcschr(dll, _T(','));
				if(sp) sp[0] = 0;
				m_icons_dll = dll;
			}
		}
		return true;
	}
	return false;
}

bool CMainDlg::AssocExtDefault(CString ext, CString info, CString icons, int type, bool bAssoc)
{
	CReg reg;
	CString SubKey, Name, Content;

	bool is_flash = false, is_playlist = false;
	switch(type)
	{
	case ASSOC_TYPE_FLASH:
		is_flash = true;
		break;
	case ASSOC_TYPE_PLAYLIST:
		is_playlist = true;
		break;
	}

	if(bAssoc) {
		SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites\\FileAssociations");
		Name =  _T(".") + ext;
		Content = _T("MPlayer.AssocFile.") + ext;
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

		Name =  _T("");
		SubKey =  _T("SOFTWARE\\Classes\\MPlayer.AssocFile.") + ext;
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,info);

		Name =  _T("FriendlyTypeName");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,info);

		Name =  _T("");
		SubKey =  _T("SOFTWARE\\Classes\\MPlayer.AssocFile.") + ext + _T("\\DefaultIcon");

		if(IsFileExist(_T("icons\\")  +ext +_T(".ico")))
			Content = m_program_dir +_T("icons\\") +ext +_T(".ico");
		else if(is_flash)
			Content = m_editor_exe + _T(",1");
		else if(IsFileExist(m_icons_dll) && icons.GetLength() > 0)
			Content = m_icons_dll +_T(",") + icons;
		else
			Content =  m_player_exe + _T(",0");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name , Content);

		SubKey =  _T("SOFTWARE\\Classes\\MPlayer.AssocFile.") + ext + _T("\\shell");
		if(!reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name ,_T("open")))
			return false;

		SubKey =  _T("SOFTWARE\\Classes\\MPlayer.AssocFile.") + ext + _T("\\shell\\open");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name ,_T(""));

		SubKey =  _T("SOFTWARE\\Classes\\MPlayer.AssocFile.") + ext + _T("\\shell\\open\\command");

		if(is_playlist)
			Content =  _T("\"") +m_player_exe +_T("\" -playlist \"%1\"");
		else if(is_flash)
			Content =  _T("\"") +m_editor_exe +_T("\" --Open FlashPlayer \"%1\"");
		else
			Content =  _T("\"") +m_player_exe +_T("\" \"%1\"");

		if(!reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name , Content))
			return false;

	} else if (IsAssoced(ext)){
		SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites\\FileAssociations");
		Name =  _T(".") + ext;
		reg.DeleteValue_STR(HKEY_LOCAL_MACHINE, SubKey, Name);

		SubKey =  _T("SOFTWARE\\Classes\\MPlayer.AssocFile.") + ext;
		Name =  _T("");
		reg.DeleteValue_STR(HKEY_LOCAL_MACHINE, SubKey, Name);

		HKEY	hKey = NULL;
		DWORD	numValue=0;
		DWORD   sizeValue = 512;
		TCHAR	ValueName[512];
		bool	need_delete = false;
		ValueName[0] = 0;

		SubKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + ext +_T("\\OpenWithProgids");

		reg.DeleteValue_STR(HKEY_CURRENT_USER, SubKey, _T("mplayer.") + ext);
		if(RegOpenKeyEx(HKEY_CURRENT_USER,	SubKey,	0,KEY_READ, &hKey) ==  ERROR_SUCCESS) {
			RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &numValue, NULL, NULL, NULL, NULL);
			if (numValue) {
				if(RegEnumValue(hKey, 0, ValueName, &sizeValue, 0, 0, 0, 0) == ERROR_SUCCESS)
					need_delete = true;
			}
			RegCloseKey(hKey);
		}
		if(need_delete) {
			SubKey = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + ext;
			reg.DeleteKey_STR(HKEY_CURRENT_USER, SubKey, _T("UserChoice"));
		}
	}
	return true;
}

bool CMainDlg::AssocExtension(CString ext, CString info, CString icons, int type, bool Assoc)
{
	CReg reg;
	CString SubKey, Name, Content, Player;

	bool is_flash = false, is_playlist = false, is_dshow = false, is_inner = false;
	switch(type)
	{
	case ASSOC_TYPE_FLASH:
		is_flash = true;
		break;
	case ASSOC_TYPE_DSHOW:
		is_dshow = true;
		break;
	case ASSOC_TYPE_PLAYLIST:
		is_playlist = true;
		break;
	case ASSOC_TYPE_DSHOWINNER:
		is_inner = true;
		break;
	}

	if(Assoc) {
		if(m_is_vista) {
			SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites\\FileAssociations");
			Name =  _T(".") + ext;
			Content = _T("MPlayer.AssocFile.") + ext;
			if(reg.ShowContent_STR(HKEY_LOCAL_MACHINE, SubKey, Name))
			{
				if(!_tcsicmp(reg.content, Content)) {
					SubKey =  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + ext + _T("\\UserChoice");
					Name =  _T("Progid");
					if(reg.SetValue_S_STR(HKEY_CURRENT_USER, SubKey, Name, Content))
						return true;
				}
			}
		}

		SubKey =  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + ext + _T("\\OpenWithProgids");
		Name =  _T("mplayer.") + ext;
		reg.SetValue_N_STR(HKEY_CURRENT_USER,SubKey, Name);

		SubKey =  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + ext;
		Name =  _T("UserChoice");
		reg.DeleteKey_STR(HKEY_CURRENT_USER,SubKey, Name);

		Name =  _T("");
		SubKey =  _T("mplayer.") + ext;
		reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,info);

		SubKey =  _T("mplayer.") + ext + _T("\\DefaultIcon");
		if(IsFileExist(_T("icons\\")  +ext +_T(".ico")))
			Content = m_program_dir +_T("icons\\") +ext +_T(".ico");
		else if(is_flash)
			Content =  m_editor_exe + _T(",1");
		else if(IsFileExist(m_icons_dll) && icons.GetLength() > 0)
			Content = m_icons_dll +_T(",") + icons;
		else if(is_inner && m_mpc)
			Content =  m_mpc_exe + _T(",0");
		else if(is_inner)
			Content =  m_editor_exe + _T(",2");
		else
			Content =  m_player_exe + _T(",0");

		reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name , Content);

		SubKey =  _T("mplayer.") + ext + _T("\\shell");
		if(!reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,_T("open")))
			return false;

		SubKey =  _T("mplayer.") + ext + _T("\\shell\\open");

		Player = _T(" MPlayer ");

		if(is_playlist)
			Content =  _T("\"") +m_player_exe +_T("\" -playlist \"%1\"");
		else if(is_flash) {
			Player = _T(" MFlashPlayer ");
			Content =  _T("\"") + m_editor_exe +_T("\" --Open FlashPlayer \"%1\"");
		} else if(is_inner && m_mpc) {
			Player = _T(" MPC-HC ");
			Content =  _T("\"") + m_mpc_exe +_T("\" \"%1\"");
		} else if(is_inner) {
			Player = _T(" MSimplePlayer ");
			Content =  _T("\"") + m_editor_exe +_T("\" --Open MediaPlayer \"%1\"");
		} else
			Content =  _T("\"") +m_player_exe +_T("\" \"%1\"");

		if(m_rightOpen)
			reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,m_str_player_use + Player + m_str_assos_op);
		else
			reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,_T(""));

		if(m_rightPlay) {
			SubKey =  _T("mplayer.") + ext + _T("\\shell\\play");
			reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,m_str_player_use + Player + m_str_assos_pl);
			SubKey =  _T("mplayer.") + ext + _T("\\shell\\play\\command");

			reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name , Content);
		} else {
			SubKey =  _T("mplayer.") + ext + _T("\\shell\\play");
			reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, _T("command"));
			SubKey =  _T("mplayer.") + ext + _T("\\shell");
			reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, _T("play"));
		}

		SubKey =  _T("mplayer.") + ext + _T("\\shell\\open\\command");

		if(!reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name , Content))
			return false;

		SubKey = _T(".") + ext;
		Content = _T("mplayer.") + ext;
		if (reg.ShowContent_STR(HKEY_CLASSES_ROOT ,SubKey,_T(""))) {
			CString BackSrt(reg.content);
			if(BackSrt.Find(_T("mplayer.")) != 0)
				reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, _T("mplayer backup"), BackSrt);
		}
		if(!reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, _T(""),Content))
			return false;

	} else {
		if(m_is_vista) {
			if(IsAssoced(ext))	{
				SubKey =  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + ext;
				Name =  _T("UserChoice");
				reg.DeleteKey_STR(HKEY_CURRENT_USER,SubKey, Name);
			}
		}

		SubKey =  _T("mplayer.") + ext;
		if (reg.ShowContent_STR(HKEY_CLASSES_ROOT ,SubKey,_T("mplayer backup"))) {
			CString BackSrt(reg.content);
			if(BackSrt.Find(_T("mplayer."))  !=  0)
				reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, _T(""), BackSrt);
			reg.DeleteValue_STR(HKEY_CLASSES_ROOT,SubKey,_T("mplayer backup"));
		}

		SubKey =  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + ext + _T("\\OpenWithProgids");
		Name =  _T("mplayer.") + ext;
		reg.DeleteValue_STR(HKEY_CURRENT_USER,SubKey, Name);

		SubKey =  _T("mplayer.") + ext + _T("\\shell\\open");
		Name =  _T("command");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + ext + _T("\\shell");
		Name =  _T("open");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + ext + _T("\\shell\\play");
		Name =  _T("command");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + ext + _T("\\shell");
		Name =  _T("play");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + ext;
		Name =  _T("shell");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + ext;
		Name =  _T("DefaultIcon");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("");
		Name =  _T("mplayer.") + ext;
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);

	}
	return true;
}

void CMainDlg::AssocDefaults()
{
	CReg reg;
	CString SubKey, Name, Content;

	Name =  _T("MPlayer");
	Content = _T("SOFTWARE\\MPlayer\\Capabilites");
	SubKey =  _T("SOFTWARE\\RegisteredApplications");
	reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

	Name =  _T("");
	Content = _T("");
	SubKey =  _T("SOFTWARE\\MPlayer");
	reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

	SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
	Name =  _T("ApplicationDescription");
	Content = _T("MPlayer WW - The Movie Player.");
	reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

	SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
	Name =  _T("ApplicationName");
	Content = _T("MPlayer WW");
	reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

	SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
	Name =  _T("ApplicationIcon");
	Content = m_player_exe + _T(",0");
	reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

	CString icon;
	for(UINT i = 0; i < m_AssocList.size(); i++) {
		icon.Format(_T("%d"), m_AssocList[i].m_icon);
		AssocExtDefault(m_AssocList[i].m_ext, m_AssocList[i].m_info, icon, m_AssocList[i].m_type, true);
	}
}

void CMainDlg::ApplyChange()
{
	if(!IsFileExist(m_player_exe))
	{
		MessageBox(m_str_player_samedir);
		return;
	}
	int index = m_icon.GetCurSel();
	if(index > 0) {
		m_icon.GetLBText(index, m_icons_str);
		m_icons_dll = m_program_dir + _T("skin\\") + m_icons_str;
		if(!IsFileExist(m_icons_dll))
			m_icons_dll = m_icons_org;
	} else
		m_icons_dll = m_icons_org;

	if(m_is_vista)
		AssocDefaults();

	for(int i = 0; i < m_wndListCtrl.GetItemCount(); i++) {
		bool is_checked = false;
		int type = ASSOC_TYPE_VIDEO;
		int check = _ttoi(m_wndListCtrl.GetItemText(i, 0));
		CString ext = m_wndListCtrl.GetItemText(i, 1);
		CString info = m_wndListCtrl.GetItemText(i, 2);
		CString stype = m_wndListCtrl.GetItemText(i, 3);
		CString icon = m_wndListCtrl.GetItemText(i, 4);

		if(ext.GetLength() < 1)
			continue;

		if(stype == m_type_list)
			type = ASSOC_TYPE_PLAYLIST;
		else if(stype == m_type_flash)
			type = ASSOC_TYPE_FLASH;
		else if(stype == m_type_dshow)
			type = ASSOC_TYPE_DSHOW;
		else if(stype == m_type_inner)
			type = ASSOC_TYPE_DSHOWINNER;

		if(check == 1)
			is_checked = true;

		AssocExtension(ext, info, icon, type, is_checked);
	}

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST & SHCNF_FLUSH, 0, 0);
}


void CMainDlg::ApplyDefault()
{
	m_icons_dll = m_program_dir + _T("skin\\") + m_icons_str;
	if(!IsFileExist(m_icons_dll))
		m_icons_dll = m_icons_org;
	AssocDefaults();

	IApplicationAssociationRegistrationUI* pAARUI = NULL;

	HRESULT hr = CoCreateInstance( CLSID_ApplicationAssociationRegistrationUI
		, NULL, CLSCTX_INPROC, __uuidof( IApplicationAssociationRegistrationUI ),reinterpret_cast< void** > (&pAARUI) );

	if ( SUCCEEDED( hr )) {
		hr = pAARUI->LaunchAdvancedAssociationUI(_T( "MPlayer"));
		pAARUI->Release();
	}
}