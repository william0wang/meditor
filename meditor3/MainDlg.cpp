// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "Win7ShellApi.h"
#include "InputDlg.h"
#include "AssocDlg.h"
#include "PlayerDlg.h"
#include "ExtraDlg.h"
#include "OtherDlg.h"
#include "GuiDlg.h"
#include "shared.h"
#include "AboutDlg.h"
#include "MConfig.h"
#include "MainDlg.h"

CMConfig mconfig;

enum START_TYPE
{
	START_NORMAL = 0,
	START_HOTKEY,
	START_ASSOC,
	START_ONTOP,
};

enum 
{
	TAB_PAGE_GUI = 0,
	TAB_PAGE_PLAYER,
	TAB_PAGE_EXTRA,
	TAB_PAGE_INPUT,
	TAB_PAGE_ASSOC,
	TAB_PAGE_OTHER,
	TAB_PAGE_LAST,
};

static ChangeWindowMessageFilterFunction ChangeWindowMessageFilterDLL = NULL;

CMainDlg::CMainDlg()
{
	m_tablist.ShowBorder(FALSE);
	m_tablist.RegisterClass();

	m_InputDlg = NULL;
	m_AssocDlg = NULL;
	m_PlayerDlg = NULL;
	m_ExtraDlg = NULL;
	m_OtherDlg = NULL;
	m_GuiDlg = NULL;
	m_pos = 0;

	g_pTaskbarList = NULL;
	s_uTBBC = WM_NULL;
	isVista = false;

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (isVista && pMsg->message == s_uTBBC) {
		// Once we get the TaskbarButtonCreated message, we can call methods
		// specific to our window on a TaskbarList instance. Note that it's
		// possible this message can be received multiple times over the lifetime
		// of this window (if explorer terminates and restarts, for example).
		if (!g_pTaskbarList) {
			HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pTaskbarList));
			if (SUCCEEDED(hr)) {
				hr = g_pTaskbarList->HrInit();
				if (FAILED(hr)) {
					g_pTaskbarList->Release();
					g_pTaskbarList = NULL;
				}

				if(g_pTaskbarList) g_pTaskbarList->SetProgressState(this->m_hWnd, TBPF_NORMAL);
			}
		}
	}
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

	m_tablist.Show3DColor();
	m_tablist.ShowAlwaysForce();
	m_tablist.SubclassWindow(GetDlgItem(IDC_LIST_TAB));

	m_tablist.SetFocusSubItem( FALSE );
	m_tablist.ShowHeaderSort(FALSE);
	m_tablist.ShowHeader(FALSE);
	m_tablist.SetSingleSelect(TRUE);

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

	SetWindowText(rStr.title);

	if(m_appLang == 2)
		SetMenu(LoadMenu(NULL, MAKEINTRESOURCE(IDR_MAINFRAME_EN)));
	else if(m_appLang == 3 || m_appLang == 4)
		SetMenu(LoadMenu(NULL, MAKEINTRESOURCE(IDR_MAINFRAME_TC)));

	::SetWindowText(GetDlgItem(ID_UPDATE), rStr.check_update);
	::SetWindowText(GetDlgItem(IDOK), rStr.btn_ok);
	::SetWindowText(GetDlgItem(IDCANCEL), rStr.btn_cancle);	
	::SetWindowText(GetDlgItem(IDC_APPLY), rStr.btn_apply);

	m_progress_apply.Attach(GetDlgItem(IDC_PROGRESS_APPLEY));

	m_Images.CreateFromImage(IDB_BITMAP_TAB, 16, 0, RGB( 255, 0, 255 ), IMAGE_BITMAP, LR_CREATEDIBSECTION );
	
	m_tablist.SetImageList(m_Images);
	m_tablist.AddColumn(_T(""), 141, ITEM_IMAGE_NONE, FALSE);
	m_tablist.AddItem(rStr.gui, 0);
	m_tablist.AddItem(rStr.player, 1);
	m_tablist.AddItem(rStr.extra, 2);
	m_tablist.AddItem(rStr.hotkey, 3);
	m_tablist.AddItem(rStr.assoc, 4);
	m_tablist.AddItem(rStr.other, 5);

	mconfig.LoadConfig(m_program_dir + _T("kk.ini"), true);
	mconfig.LoadConfig(m_program_dir + _T("mplayer.ini"));

	RECT rc;
	POINT pos;
	::GetWindowRect(GetDlgItem(IDC_STATIC_PAGE), &rc);
	pos.x = rc.left;
	pos.y = rc.top;
	rc.bottom -= rc.top;
	rc.right -= rc.left;
	ScreenToClient(&pos);

	m_PlayerDlg = new CPlayerDlg(IDD_DIALOG_PAGE_PLAYER);
	m_PlayerDlg->Create(m_hWnd);
	m_PlayerDlg->MoveWindow(pos.x, pos.y, rc.right, rc.bottom);

	m_GuiDlg = new CGuiDlg(IDD_DIALOG_PAGE_GUI);
	m_GuiDlg->Create(m_hWnd);
	m_GuiDlg->MoveWindow(pos.x, pos.y, rc.right, rc.bottom);

	m_ExtraDlg = new CExtraDlg(IDD_DIALOG_PAGE_EXTRA);
	m_ExtraDlg->Create(m_hWnd);
	m_ExtraDlg->MoveWindow(pos.x, pos.y, rc.right, rc.bottom);

	m_OtherDlg = new COtherDlg(IDD_DIALOG_PAGE_OTHER);
	m_OtherDlg->Create(m_hWnd);
	m_OtherDlg->MoveWindow(pos.x, pos.y, rc.right, rc.bottom);

	m_InputDlg = new CInputDlg(IDD_DIALOG_PAGE_INPUT);
	m_InputDlg->Create(m_hWnd);
	m_InputDlg->MoveWindow(pos.x, pos.y, rc.right, rc.bottom);

	m_AssocDlg = new CAssocDlg(IDD_DIALOG_PAGE_ASSOC);
	m_AssocDlg->Create(m_hWnd);
	m_AssocDlg->MoveWindow(pos.x, pos.y, rc.right, rc.bottom);

	if(m_OpenType == START_HOTKEY) {
		::SetWindowPos(m_hWnd, HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		m_tablist.SelectItem(TAB_PAGE_INPUT);
	} else if(m_OpenType == START_ASSOC) {
		::SetWindowPos(m_hWnd, HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		m_tablist.SelectItem(TAB_PAGE_ASSOC);
	} else {
		int page = mconfig.GetInteger(_T("meditor3_last_page"), 0, true);
		if(page < 0 || page >= TAB_PAGE_LAST)
			page = 0;
		m_tablist.SelectItem(page);
		if(m_OpenType == START_ONTOP)
			::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	}
	
	OSVERSIONINFO version;
	version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&version)) {
		if(version.dwMajorVersion >= 6) {
			isVista = true;
			s_uTBBC = RegisterWindowMessage(L"TaskbarButtonCreated");
			HINSTANCE user32 = GetModuleHandle(L"user32.dll");
			if(user32) ChangeWindowMessageFilterDLL = (ChangeWindowMessageFilterFunction)GetProcAddress(user32, "ChangeWindowMessageFilter");
			if(ChangeWindowMessageFilterDLL) ChangeWindowMessageFilterDLL(s_uTBBC, MSGFLT_ADD);
		}
	}

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	if(m_PlayerDlg)
		m_PlayerDlg->SendMessage(WM_CLOSE);

	if(m_GuiDlg)
		m_GuiDlg->SendMessage(WM_CLOSE);

	if(m_OtherDlg)
		m_OtherDlg->SendMessage(WM_CLOSE);

	if(m_ExtraDlg)
		m_ExtraDlg->SendMessage(WM_CLOSE);

	if(m_InputDlg)
		m_InputDlg->SendMessage(WM_CLOSE);

	if(m_AssocDlg)
		m_InputDlg->SendMessage(WM_CLOSE);

	if (g_pTaskbarList)
	{
		g_pTaskbarList->Release();
		g_pTaskbarList = NULL;
	}
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SaveConfig();

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

void CMainDlg::SaveConfig()
{
	if(m_InputDlg)
		m_InputDlg->SaveInputConfig();
	if(m_PlayerDlg)
		m_PlayerDlg->SaveConfig();
	if(m_GuiDlg)
		m_GuiDlg->SaveConfig();
	if(m_OtherDlg)
		m_OtherDlg->SaveConfig();
	if(m_ExtraDlg)
		m_ExtraDlg->SaveConfig();

	mconfig.SaveConfig(m_program_dir + _T("kk.ini"), true);
	mconfig.SaveConfig(m_program_dir + _T("mplayer.ini"));
}

LRESULT CMainDlg::OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pos = 0;
	m_progress_apply.SetPos(0);
	m_progress_apply.ShowWindow(SW_SHOW);
	SetTimer(0, 30, NULL);
	if(g_pTaskbarList)
		g_pTaskbarList->SetProgressState(this->m_hWnd, TBPF_INDETERMINATE);

	SaveConfig();
	return 0;
}

LRESULT CMainDlg::OnTabSelected( LPNMHDR lpNMHDR )
{
	CListNotify *pListNotify = reinterpret_cast<CListNotify *>( lpNMHDR );
	
	CString last;
	last.Format(_T("%d"), pListNotify->m_nItem);
	mconfig.SetValue(_T("meditor3_last_page"), last, true, ex_meditor);

	switch(pListNotify->m_nItem)
	{
	case TAB_PAGE_GUI:
		m_GuiDlg->ShowWindow(SW_SHOW);
		m_PlayerDlg->ShowWindow(SW_HIDE);
		m_ExtraDlg->ShowWindow(SW_HIDE);
		m_InputDlg->ShowWindow(SW_HIDE);
		m_AssocDlg->ShowWindow(SW_HIDE);
		m_OtherDlg->ShowWindow(SW_HIDE);
		break;
	case TAB_PAGE_PLAYER:
		m_GuiDlg->ShowWindow(SW_HIDE);
		m_PlayerDlg->ShowWindow(SW_SHOW);
		m_ExtraDlg->ShowWindow(SW_HIDE);
		m_InputDlg->ShowWindow(SW_HIDE);
		m_AssocDlg->ShowWindow(SW_HIDE);
		m_OtherDlg->ShowWindow(SW_HIDE);
		break;
	case TAB_PAGE_EXTRA:
		m_GuiDlg->ShowWindow(SW_HIDE);
		m_PlayerDlg->ShowWindow(SW_HIDE);
		m_ExtraDlg->ShowWindow(SW_SHOW);
		m_InputDlg->ShowWindow(SW_HIDE);
		m_AssocDlg->ShowWindow(SW_HIDE);
		m_OtherDlg->ShowWindow(SW_HIDE);
		break;
	case TAB_PAGE_INPUT:
		m_GuiDlg->ShowWindow(SW_HIDE);
		m_PlayerDlg->ShowWindow(SW_HIDE);
		m_ExtraDlg->ShowWindow(SW_HIDE);
		m_InputDlg->ShowWindow(SW_SHOW);
		m_AssocDlg->ShowWindow(SW_HIDE);
		m_OtherDlg->ShowWindow(SW_HIDE);
		break;
	case TAB_PAGE_ASSOC:
		m_PlayerDlg->ShowWindow(SW_HIDE);
		m_GuiDlg->ShowWindow(SW_HIDE);
		m_ExtraDlg->ShowWindow(SW_HIDE);
		m_InputDlg->ShowWindow(SW_HIDE);
		m_AssocDlg->ShowWindow(SW_SHOW);
		m_OtherDlg->ShowWindow(SW_HIDE);
		break;
	case TAB_PAGE_OTHER:
		m_PlayerDlg->ShowWindow(SW_HIDE);
		m_GuiDlg->ShowWindow(SW_HIDE);
		m_ExtraDlg->ShowWindow(SW_HIDE);
		m_InputDlg->ShowWindow(SW_HIDE);
		m_AssocDlg->ShowWindow(SW_HIDE);
		m_OtherDlg->ShowWindow(SW_SHOW);
		break;
	default:
		mconfig.RemoveValue(_T("meditor3_last_page"), true);
		ATLTRACE(_T("User Selected: %d\n"), pListNotify->m_nItem);
		break;
	}

	return 0;
}


LRESULT CMainDlg::OnMplayerIni(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(FileExist(m_program_dir + _T("mplayer.ini")))
		ShellExecute(0, _T("open"), m_program_dir +_T("mplayer.ini"), NULL, NULL, SW_SHOW);
	return 0;
}

LRESULT CMainDlg::OnInputIni(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(FileExist(m_program_dir + _T("input.ini")))
		ShellExecute(0, _T("open"), m_program_dir +_T("input.ini"), NULL, NULL, SW_SHOW);
	return 0;
}

LRESULT CMainDlg::OnKkIni(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(FileExist(m_program_dir + _T("kk.ini")))
		ShellExecute(0, _T("open"), m_program_dir +_T("kk.ini"), NULL, NULL, SW_SHOW);
	return 0;
}

LRESULT CMainDlg::OnLogTxt(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(FileExist(m_program_dir + _T("log.txt")))
		ShellExecute(0, _T("open"), m_program_dir +_T("log.txt"), NULL, NULL, SW_SHOW);
	return 0;
}

LRESULT CMainDlg::OnGotohome(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShellExecute(0, _T("open"),_T("http://mplayer-ww.com/") , _T(""), NULL, SW_SHOW);
	return 0;
}

LRESULT CMainDlg::OnShowHelp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CString m_help = m_program_dir + _T("help/default.html");
	if(FileExist(m_help))
		ShellExecute(0, _T("open"), m_help, _T(""), NULL, SW_SHOW);
	else
		ShellExecute(0, _T("open"),_T("http://mplayer-ww.com/") , _T(""), NULL, SW_SHOW);
	return 0;
}

LRESULT CMainDlg::OnShowlog(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CString m_help = m_program_dir + _T("help/changelog.html");
	if(FileExist(m_help))
		ShellExecute(0, _T("open"), m_help, _T(""), NULL, SW_SHOW);
	else
		ShellExecute(0, _T("open"),_T("http://mplayer-ww.com/") , _T(""), NULL, SW_SHOW);
	return 0;
}

LRESULT CMainDlg::OnShowfaq(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CString m_help = m_program_dir + _T("help/faq.html");
	if(FileExist(m_help))
		ShellExecute(0, _T("open"), m_help, _T(""), NULL, SW_SHOW);
	else
		ShellExecute(0, _T("open"),_T("http://mplayer-ww.com/") , _T(""), NULL, SW_SHOW);
	return 0;
}

LRESULT CMainDlg::OnAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();

	return 0;
}

LRESULT CMainDlg::OnBnClickedUpdate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(0, _T("open"), m_program_dir +_T("meditor.exe"), _T("--check-update"), NULL, SW_SHOW);
	return 0;
}


LRESULT CMainDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//UINT_PTR nIDEvent = (UINT_PTR)wParam;
	m_pos++;
	if(m_pos <= 30) {
		if(m_pos <= 10) {
			if(isVista)
				m_progress_apply.SetPos(100);
			else
				m_progress_apply.SetPos(10 * m_pos);

			if(m_pos == 10)
				m_progress_apply.ShowWindow(SW_HIDE);
		}
	} else {
		KillTimer(0);
		if(g_pTaskbarList)
			g_pTaskbarList->SetProgressState(this->m_hWnd, TBPF_NOPROGRESS);
	}
	return 0;
}
