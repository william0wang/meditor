// ProgressDlg.cpp : implementation of the CProgressDlg class
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "Win7ShellApi.h"

#include "ProgressDlg.h"

static ChangeWindowMessageFilterFunction ChangeWindowMessageFilterDLL = NULL;

CProgressDlg::CProgressDlg()
{
	g_pTaskbarList = NULL;
	s_uTBBC = WM_NULL;

}

LRESULT CProgressDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);

	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);
	
	m_progress.Attach(GetDlgItem(IDC_PROGRESS));

	m_progress.SetRange(0,100);
	m_progress.SetPos(0);
	SetTimer(0, 100, 0);

	DoDataExchange(false);
		
	s_uTBBC = RegisterWindowMessage(L"TaskbarButtonCreated");
	HINSTANCE user32 = GetModuleHandle(L"user32.dll");
	if(user32) ChangeWindowMessageFilterDLL = (ChangeWindowMessageFilterFunction)GetProcAddress(user32, "ChangeWindowMessageFilter");
	if(ChangeWindowMessageFilterDLL) ChangeWindowMessageFilterDLL(s_uTBBC, MSGFLT_ADD);

	SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	return TRUE;
}

LRESULT CProgressDlg::OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CString percent;
	int p = m_progress.GetPos();
	if(p >= 100) {
		KillTimer(0);
		if (g_pTaskbarList)
		{
			g_pTaskbarList->Release();
			g_pTaskbarList = NULL;
		}
		EndDialog(IDOK);
		return 0;
	}
	percent.Format(_T("%d%%"), p);
	SetWindowText(percent);
	if(g_pTaskbarList)
		g_pTaskbarList->SetProgressValue(this->m_hWnd, p, 100);

	return 0;
}

LRESULT CProgressDlg::OnTBBC(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(uMsg == WM_NULL)
		return 0;

	if (!g_pTaskbarList)
	{
		HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pTaskbarList));
		if (SUCCEEDED(hr))
		{
			hr = g_pTaskbarList->HrInit();
			if (FAILED(hr))
			{
				g_pTaskbarList->Release();
				g_pTaskbarList = NULL;
			}

			if(g_pTaskbarList) g_pTaskbarList->SetProgressState(this->m_hWnd, TBPF_NORMAL);
		}
	}

	return 0;
}

LRESULT CProgressDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CProgressDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
