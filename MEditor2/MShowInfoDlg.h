#pragma once


// CMShowInfoDlg 对话框
enum Info_Type
{
	type_reload,
	type_coreavc,
	type_ass_expand,
	type_boost,
	type_realtime,
};

class CMConfig;

class CMShowInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CMShowInfoDlg)

public:
	CMShowInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMShowInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_info_nshow;
	CString m_info;
	CMConfig *m_cfg;
	int m_type;
	BYTE unshow;
	void set_bit(BYTE &date , UINT bit);
	void clear_bit(BYTE &date , UINT bit);
	bool get_bit(BYTE date , UINT bit);
public:
	void SetConfig(CMConfig *cfg);
	bool IsShow(int type);
	void ResetInfo();
	afx_msg void OnBnClickedOk();

};
