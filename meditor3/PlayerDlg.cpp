// PlayerDlg.cpp : implementation of the CPlayerDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "shared.h"
#include "Reg.h"
#include "PlayerDlg.h"

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
	//if(mconfig.GetBoolean(_T("seek_realtime"), true, true))
	//	m_seekrt = TRUE;
	//else
	//	m_seekrt = FALSE;
}

void CPlayerDlg::SaveConfig()
{
	//if(m_seekrt)
	//	mconfig.RemoveValue(_T("seek_realtime"), true);
	//else
	//	mconfig.SetValue(_T("seek_realtime"), _T("0"), true, ex_setting);
}

LRESULT CPlayerDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	UIAddChildWindowContainer(m_hWnd);

	DoDataExchange();
	
	return TRUE;
}

LRESULT CPlayerDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}
