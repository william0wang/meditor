// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"
#include "InputDlg.h"
#include "AssocDlg.h"
#include "shared.h"
#include "AboutDlg.h"
#include "PlayerPage.h"

enum 
{
	TAB_PAGE_INPUT = 0,
	TAB_PAGE_ASSOC,
};

CMainDlg::CMainDlg()
{
	m_tablist.ShowBorder(FALSE);
	m_tablist.RegisterClass();

	m_InputDlg = NULL;
	m_AssocDlg = NULL;
	
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
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

	m_Images.CreateFromImage(IDB_BITMAP_TAB, 16, 0, RGB( 255, 0, 255 ), IMAGE_BITMAP, LR_CREATEDIBSECTION );
	
	m_tablist.SetImageList(m_Images);
	m_tablist.AddColumn(_T(""), 141, ITEM_IMAGE_NONE, FALSE);
	m_tablist.AddItem(rStr.hotkey, 0);
	m_tablist.AddItem(rStr.assoc, 1);
	//m_tablist.AddItem( _T("TEST2"), 3);
	//m_tablist.AddItem( _T("TEST2"), 4);
	//m_tablist.AddItem( _T("TEST2"), 5);
	m_tablist.SelectItem(0, 0);

	RECT rc;
	POINT pos;
	::GetWindowRect(GetDlgItem(IDC_STATIC_PAGE), &rc);
	pos.x = rc.left;
	pos.y = rc.top;
	rc.bottom -= rc.top;
	rc.right -= rc.left;
	ScreenToClient(&pos);

	m_InputDlg = new CInputDlg(IDD_DIALOG_PAGE_INPUT);
	m_InputDlg->Create(m_hWnd);
	m_InputDlg->MoveWindow(pos.x, pos.y, rc.right, rc.bottom);
	m_InputDlg->ShowWindow(SW_SHOW);

	m_AssocDlg = new CAssocDlg(IDD_DIALOG_PAGE_ASSOC);
	m_AssocDlg->Create(m_hWnd);
	m_AssocDlg->MoveWindow(pos.x, pos.y, rc.right, rc.bottom);
	//m_AssocDlg->ShowWindow(SW_SHOW);
	
	//CPlayerPage *page = new CPlayerPage;
	//page->Create(this->m_hWnd);
	//page->MoveWindow(pos.x, pos.y, rc.right, rc.bottom);
	//page->ShowWindow(SW_SHOW);


	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	if(m_InputDlg)
		m_InputDlg->SendMessage(WM_CLOSE);

	if(m_AssocDlg)
		m_InputDlg->SendMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
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

LRESULT CMainDlg::OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(m_InputDlg)
		m_InputDlg->SaveInputConfig();
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

LRESULT CMainDlg::OnTabSelected( LPNMHDR lpNMHDR )
{
	CListNotify *pListNotify = reinterpret_cast<CListNotify *>( lpNMHDR );

	if(!m_InputDlg || !m_AssocDlg)
		return 0;

	switch(pListNotify->m_nItem)
	{
	case TAB_PAGE_INPUT:
		m_InputDlg->ShowWindow(SW_SHOW);
		m_AssocDlg->ShowWindow(SW_HIDE);
		break;
	case TAB_PAGE_ASSOC:
		m_InputDlg->ShowWindow(SW_HIDE);
		m_AssocDlg->ShowWindow(SW_SHOW);
		break;
	default:
		ATLTRACE(_T("User Selected: %d\n"), pListNotify->m_nItem);
		break;
	}


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
