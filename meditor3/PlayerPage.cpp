// PlayerPage.cpp : implementation of the CPlayerPage class
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"

#include "PlayerPage.h"


BOOL CPlayerPage::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}


LRESULT CPlayerPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//CenterWindow(GetParent());

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);

	DoDataExchange(false);
	UIAddChildWindowContainer(m_hWnd);
	return TRUE;
}

LRESULT CPlayerPage::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	return 0;
}

void CPlayerPage::CloseDialog(int nVal)
{
	DestroyWindow();
	//if class is maindlg
	//::PostQuitMessage(nVal);
}
