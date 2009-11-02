// ProgressDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "meditor2.h"
#include "ProgressDlg.h"

static ChangeWindowMessageFilterFunction ChangeWindowMessageFilterDLL = NULL;

// CProgressDlg 对话框

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{
	g_pTaskbarList = NULL;

}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProgressDlg 消息处理程序

BOOL CProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	s_uTBBC = RegisterWindowMessage(L"TaskbarButtonCreated");
	HINSTANCE user32 = GetModuleHandle(L"user32.dll");
	if(user32) ChangeWindowMessageFilterDLL = (ChangeWindowMessageFilterFunction)GetProcAddress(user32, "ChangeWindowMessageFilter");
	if(ChangeWindowMessageFilterDLL) ChangeWindowMessageFilterDLL(s_uTBBC, MSGFLT_ADD);

	m_progress.SetRange(0,100);
	m_progress.SetPos(0);
	SetTimer(0, 100, 0);

	::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CProgressDlg::OnTimer(UINT_PTR nIDEvent)
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
		OnOK();
		return;
	}
	percent.Format(_T("%d%%"), p);
	SetWindowText(percent);
	if(g_pTaskbarList) g_pTaskbarList->SetProgressValue(this->m_hWnd, p, 100);
	CDialog::OnTimer(nIDEvent);
}

BOOL CProgressDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == s_uTBBC)
	{
		// Once we get the TaskbarButtonCreated message, we can call methods
		// specific to our window on a TaskbarList instance. Note that it's
		// possible this message can be received multiple times over the lifetime
		// of this window (if explorer terminates and restarts, for example).
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
	}

	return CDialog::PreTranslateMessage(pMsg);
}
