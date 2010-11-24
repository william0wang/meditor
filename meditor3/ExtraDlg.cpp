// ExtraDlg.cpp : implementation of the CExtraDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "shared.h"
#include "Reg.h"
#include "ExtraDlg.h"

CExtraDlg::CExtraDlg(UINT IDD_DLG)
{
	IDD = IDD_DLG;

}

BOOL CExtraDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CExtraDlg::OnIdle()
{
	return FALSE;
}

LRESULT CExtraDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	UIAddChildWindowContainer(m_hWnd);

	m_mplayer_ini = mconfig.GetOtherValue();
	
	DoDataExchange();
	
	return TRUE;
}

LRESULT CExtraDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	return 0;
}

void CExtraDlg::SaveConfig()
{
	DoDataExchange(TRUE);
	mconfig.SetValue_Other(m_mplayer_ini);
}
