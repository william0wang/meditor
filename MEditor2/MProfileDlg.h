#pragma once
#include "afxwin.h"


// CMProfileDlg 对话框

class CMProfileDlg : public CDialog
{
	DECLARE_DYNAMIC(CMProfileDlg)

public:
	CMProfileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMProfileDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROFILE_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_plist;
	CString m_profile_add;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
