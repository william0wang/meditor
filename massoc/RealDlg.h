#pragma once


// CRealDlg 对话框

class CRealDlg : public CDialog
{
	DECLARE_DYNAMIC(CRealDlg)

public:
	CRealDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRealDlg();

// 对话框数据
	enum { IDD = IDD_REAL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnNotifyReg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyDReg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyCheck(WPARAM wParam, LPARAM lParam);
};
