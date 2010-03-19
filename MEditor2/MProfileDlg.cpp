// MProfileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "meditor2.h"
#include "MProfileDlg.h"


// CMProfileDlg 对话框

IMPLEMENT_DYNAMIC(CMProfileDlg, CDialog)

CMProfileDlg::CMProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMProfileDlg::IDD, pParent)
{

}

CMProfileDlg::~CMProfileDlg()
{
}

void CMProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PROFILE, m_plist);
}


BEGIN_MESSAGE_MAP(CMProfileDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMProfileDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMProfileDlg 消息处理程序

void CMProfileDlg::OnBnClickedOk()
{
	int cur = m_plist.GetCurSel();
	m_plist.GetLBText(cur , m_profile_add);
	OnOK();
}

BOOL CMProfileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_plist.AddString(_T("extension.avi"));
	m_plist.AddString(_T("extension.rm"));
	m_plist.AddString(_T("extension.rmvb"));
	m_plist.AddString(_T("extension.mkv"));
	m_plist.AddString(_T("extension.wmv"));
	m_plist.AddString(_T("extension.asf"));
	m_plist.AddString(_T("extension.flv"));
	m_plist.AddString(_T("extension.dat"));
	m_plist.AddString(_T("extension.mpg"));
	m_plist.AddString(_T("extension.mpeg"));
	m_plist.AddString(_T("extension.mp4"));
	m_plist.AddString(_T("extension.m4v"));
	m_plist.AddString(_T("extension.vob"));
	m_plist.AddString(_T("extension.ts"));
	m_plist.AddString(_T("extension.tp"));
	m_plist.AddString(_T("extension.mov"));
	m_plist.AddString(_T("extension.qt"));
	m_plist.AddString(_T("extension.ogm"));
	m_plist.AddString(_T("extension.3gp"));
	m_plist.AddString(_T("extension.vp6"));
	m_plist.AddString(_T("protocol.dvd"));
	m_plist.AddString(_T("protocol.dvdnav"));
	m_plist.AddString(_T("protocol.rtsp"));
	m_plist.AddString(_T("protocol.mms"));
	m_plist.AddString(_T("protocol.http"));
	m_plist.AddString(_T("protocol.ftp"));
	m_plist.AddString(_T("protocol.vod"));
	m_plist.AddString(_T("vo.direct"));
	m_plist.AddString(_T("vo.gl"));
	m_plist.AddString(_T("vo.gl2"));
	m_plist.AddString(_T("ao.dsound"));
	m_plist.AddString(_T("ao.win32"));
	m_plist.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CMProfileDlg::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
	case   WM_KEYDOWN:
		switch(pMsg->wParam)
		{
		case   VK_RETURN:
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
