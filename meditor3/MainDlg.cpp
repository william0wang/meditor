// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "PlayerPage.h"

CMainDlg::CMainDlg()
{
	m_tablist.RegisterClass();
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

	m_Images.CreateFromImage(IDB_BITMAP_TAB, 16, 0, RGB( 255, 0, 255 ), IMAGE_BITMAP, LR_CREATEDIBSECTION );

	m_tablist.SetImageList(m_Images);
	m_tablist.AddColumn(  _T("TEST1"), 141, ITEM_IMAGE_NONE, FALSE);
	m_tablist.AddItem( _T("TEST1"), 0);
	m_tablist.AddItem( _T("TEST2"), 1);
	m_tablist.AddItem( _T("TEST2"), 3);
	m_tablist.AddItem( _T("TEST2"), 4);
	m_tablist.AddItem( _T("TEST2"), 5);
	m_tablist.SelectItem(0, 0);

	RECT rc;
	POINT pos;
	::GetWindowRect(GetDlgItem(IDC_STATIC_PAGE), &rc);
	pos.x = rc.left;
	pos.y = rc.top;
	rc.bottom -= rc.top;
	rc.right -= rc.left;
	ScreenToClient(&pos);

	CPlayerPage *page = new CPlayerPage;
	page->Create(this->m_hWnd);
	page->MoveWindow(pos.x, pos.y, rc.right, rc.bottom);
	page->ShowWindow(SW_SHOW);


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

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
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

LRESULT CMainDlg::OnTabSelected( LPNMHDR lpNMHDR )
{
	CListNotify *pListNotify = reinterpret_cast<CListNotify *>( lpNMHDR );

	ATLTRACE(_T("User Selected: %d\n"), pListNotify->m_nItem);

	return 0;
}
