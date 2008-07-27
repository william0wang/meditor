// MediaInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "meditor2.h"
#include "MediaInfoDlg.h"


// CMediaInfoDlg 对话框

IMPLEMENT_DYNAMIC(CMediaInfoDlg, CDialog)

CMediaInfoDlg::CMediaInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMediaInfoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DS);
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	URL.Format(_T("%s"),szFilePath);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	URL.Format(_T("%smedia_info.html"),szFilePath);
}

CMediaInfoDlg::~CMediaInfoDlg()
{
}

void CMediaInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER_HTML, m_html);
}


BEGIN_MESSAGE_MAP(CMediaInfoDlg, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMediaInfoDlg 消息处理程序

BOOL CMediaInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(!IsFileExist(URL))
		OnOK();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	m_html.Navigate(URL,NULL,NULL,NULL,NULL);
	return TRUE; 
}

BOOL CMediaInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
	case WM_RBUTTONUP:
	case WM_RBUTTONDOWN:
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CMediaInfoDlg::OnDestroy()
{
	CDialog::OnDestroy();
	if(IsFileExist(URL))
		DeleteFile(URL);
	
}
BEGIN_EVENTSINK_MAP(CMediaInfoDlg, CDialog)
	ON_EVENT(CMediaInfoDlg, IDC_EXPLORER_HTML, 104, CMediaInfoDlg::DownloadCompleteExplorerHtml, VTS_NONE)
END_EVENTSINK_MAP()

void CMediaInfoDlg::DownloadCompleteExplorerHtml()
{
	DeleteFile(URL);
	::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
}
