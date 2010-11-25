// ShowInfoDlg.cpp : implementation of the CShowInfoDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "shared.h"
#include "resource.h"

#include "ShowInfoDlg.h"

BOOL CShowInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CShowInfoDlg::OnIdle()
{
	return FALSE;
}

LRESULT CShowInfoDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	SetWindowText(rStr.warning);
	SetDlgItemText(IDC_CHECK_INFO, rStr.notshow);
	SetDlgItemText(IDOK, rStr.btn_ok);

	DoDataExchange();

	return TRUE;
}

LRESULT CShowInfoDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

bool CShowInfoDlg::IsShow(int type)
{
	m_type = 0;
	unshow = (BYTE)mconfig.GetInteger(_T("meditor_hide_info"), 0, true);
	if(get_bit(unshow, type + 1))
		return false;

	switch(type)
	{
	case type_reload:
	case type_coreavc:
	case type_ass_expand:
	case type_boost:
	case type_realtime:
		m_info = rStr.winfo[type];
		break;
	default:
		return false;
		break;
	}
	m_type = type + 1;

	return true;
}

void CShowInfoDlg::ResetInfo()
{
	unshow = 0;
	mconfig.SetValue(_T("meditor_hide_info"), _T("0"), true, ex_meditor);
	mconfig.RemoveValue(_T("meditor_hide_info"), true);
}

void CShowInfoDlg::set_bit(BYTE &date , UINT bit)
{
	if( bit < 1 && bit > 7)
		return;
	date |= (0x1 << bit); 
}

void CShowInfoDlg::clear_bit(BYTE &date , UINT bit)
{
	if( bit < 1 && bit > 7)
		return;
	date &= ~(0x1 << bit); 
}

bool CShowInfoDlg::get_bit(BYTE date , UINT bit)
{
	if( bit < 1 && bit > 7)
		return false;
	date &= (0x1 << bit);
	if(date)
		return true;
	return false;
}


LRESULT CShowInfoDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(TRUE);
	CString str;
	if(m_type) {
		set_bit(unshow, m_type);
		str.Format(_T("%d"), unshow);
		mconfig.SetValue(_T("meditor_hide_info"), str, true, ex_meditor);
	}
	EndDialog(0);

	return 0;
}

void ShowInfoDlg(int type)
{
	CShowInfoDlg dlg;
	if(dlg.IsShow(type))
		dlg.DoModal();
}
