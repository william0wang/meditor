#pragma once


// CRealDlg 对话框

class CRealDlg : public CDialog
{
	DECLARE_DYNAMIC(CRealDlg)

public:
	CRealDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRealDlg();

	CWinThread *CheckRealThread;
	CString m_cmdline;
	BOOL m_reg_ok;
	BOOL CheckRealOnline();
	BOOL RegRealOnline();
	BOOL DRegRealOnline();

// 对话框数据
	enum { IDD = IDD_REAL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
