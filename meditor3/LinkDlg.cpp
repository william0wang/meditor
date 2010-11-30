// LinkDlg.cpp : implementation of the CLinkDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "shared.h"
#include "Reg.h"
#include "LinkDlg.h"

CLinkDlg::CLinkDlg()
{
	m_mplayer_desk = TRUE;
	m_meditor_desk = FALSE;
	m_mplayer_start = TRUE;
	m_meditor_start = TRUE;
	m_tools_start = TRUE;
	m_flash_start = FALSE;
	m_dshow_start = FALSE;
	m_mpc_start = TRUE;
}

BOOL CLinkDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CLinkDlg::OnIdle()
{
	return FALSE;
}


LRESULT CLinkDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	UIAddChildWindowContainer(m_hWnd);

	SetWindowText(rStr.clink);
	::SetWindowText(GetDlgItem(IDC_STATIC_DESK), rStr.desktop);
	::SetWindowText(GetDlgItem(IDC_STATIC_MENU), rStr.startmenu);
	::SetWindowText(GetDlgItem(IDC_CHECK2), rStr.meditor);
	::SetWindowText(GetDlgItem(IDC_CHECK4), rStr.meditor);

	::SetWindowText(GetDlgItem(IDOK), rStr.btn_ok);
	::SetWindowText(GetDlgItem(IDCANCEL), rStr.btn_cancle);

	DoDataExchange();
	
	return TRUE;
}

LRESULT CLinkDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CLinkDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CLinkDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CLinkDlg::CloseDialog(int nVal)
{
	EndDialog(nVal);
}