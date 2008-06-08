// MShowInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "meditor2.h"
#include "MConfig.h"
#include "MShowInfoDlg.h"


// CMShowInfoDlg 对话框

IMPLEMENT_DYNAMIC(CMShowInfoDlg, CDialog)

CMShowInfoDlg::CMShowInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMShowInfoDlg::IDD, pParent)
	, m_info_nshow(FALSE)
	, m_info(_T(""))
{
	unshow = 0;
	m_type = 0;
	m_cfg = NULL;
}

CMShowInfoDlg::~CMShowInfoDlg()
{
}

void CMShowInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_INFO, m_info_nshow);
	DDX_Text(pDX, IDC_STATIC_INFO, m_info);
}


BEGIN_MESSAGE_MAP(CMShowInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMShowInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMShowInfoDlg 消息处理程序

void CMShowInfoDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if(m_info_nshow && m_cfg && m_type)
	{
		set_bit(unshow,m_type);
		CString str;
		str.Format(_T("%d"),unshow);
		m_cfg->SetValue(_T("meditor_hide_info"),str,true,ex_meditor);
	}
	m_info_nshow = FALSE;
	UpdateData(FALSE);
	OnOK();
}

void CMShowInfoDlg::SetConfig(CMConfig *cfg)
{
	m_cfg = cfg;
	if(m_cfg)
	{
		int value;
		if(m_cfg->GetValue_Integer(_T("meditor_hide_info") , value , true))
			unshow = (BYTE)value;
	}
}

bool CMShowInfoDlg::IsShow(int type)
{
	if(get_bit(unshow,type + 1))
		return false;
	switch(type)
	{
	case type_reload:
		m_info = ResStr(IDS_INFO_RELOAD);
		break;
	case type_coreavc:
		m_info = ResStr(IDS_INFO_COREAVC);
		break;
	case type_ass_expand:
		m_info = ResStr(IDS_INFO_ASSEXP);
		break;
	default:
		return false;
		break;
	}
	m_type = type + 1;

	return true;
}

void CMShowInfoDlg::ResetInfo()
{
	unshow = 0;
}

void CMShowInfoDlg::set_bit(BYTE &date , UINT bit)
{
	if( bit < 1 && bit > 7)
		return;
	date |= (0x1 << bit); 
}

void CMShowInfoDlg::clear_bit(BYTE &date , UINT bit)
{
	if( bit < 1 && bit > 7)
		return;
	date &= ~(0x1 << bit); 
}

bool CMShowInfoDlg::get_bit(BYTE date , UINT bit)
{
	if( bit < 1 && bit > 7)
		return false;
	date &= (0x1 << bit);
	if(date)
		return true;
	return false;
}
