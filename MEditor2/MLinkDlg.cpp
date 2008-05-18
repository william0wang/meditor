// MLinkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "meditor2.h"
#include "MLinkDlg.h"


// CMLinkDlg 对话框

IMPLEMENT_DYNAMIC(CMLinkDlg, CDialog)

CMLinkDlg::CMLinkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMLinkDlg::IDD, pParent)
	, m_mplayer_desk(TRUE)
	, m_meditor_desk(FALSE)
	, m_mplayer_start(TRUE)
	, m_meditor_start(TRUE)
	, m_tools_start(TRUE)
	, m_flash_start(FALSE)
	, m_dshow_start(FALSE)
	, m_mpc_start(TRUE)
{

}

CMLinkDlg::~CMLinkDlg()
{
}

void CMLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_mplayer_desk);
	DDX_Check(pDX, IDC_CHECK2, m_meditor_desk);
	DDX_Check(pDX, IDC_CHECK3, m_mplayer_start);
	DDX_Check(pDX, IDC_CHECK4, m_meditor_start);
	DDX_Check(pDX, IDC_CHECK5, m_tools_start);
	DDX_Check(pDX, IDC_CHECK6, m_flash_start);
	DDX_Check(pDX, IDC_CHECK7, m_dshow_start);
	DDX_Check(pDX, IDC_C_MPC, m_mpc_start);
}


BEGIN_MESSAGE_MAP(CMLinkDlg, CDialog)
END_MESSAGE_MAP()


// CMLinkDlg 消息处理程序
