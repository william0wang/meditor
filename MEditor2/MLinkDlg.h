#pragma once


// CMLinkDlg 对话框

class CMLinkDlg : public CDialog
{
	DECLARE_DYNAMIC(CMLinkDlg)

public:
	CMLinkDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMLinkDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LINK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_mplayer_desk;
	BOOL m_meditor_desk;
	BOOL m_mplayer_start;
	BOOL m_meditor_start;
	BOOL m_tools_start;
	BOOL m_flash_start;
	BOOL m_dshow_start;
	BOOL m_mpc_start;
};
