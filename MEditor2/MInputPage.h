#pragma once
#ifndef XLISTCTRLLIB_STATIC
#define XLISTCTRLLIB_STATIC
#endif
#include "XListCtrl.h" 
#include "MInputList.h" 
// CMInputPage 对话框

class CMInputPage : public CDialog
{
	DECLARE_DYNAMIC(CMInputPage)

public:
	CMInputPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMInputPage();

// 对话框数据
	enum { IDD = IDD_INPUT_DIALOG };

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	CXListCtrl m_List;
	int m_def_number;
	int m_readonly_number;
	static int m_nColWidths[];
	CMKeyList m_skey;
	CString m_program_dir;
	CMInputList m_inputs;
	int CheckMute();
	int GetKeyNumber(CString Key);
	void FillListCtrl(CXListCtrl * pList);
	void InitListCtrl(CXListCtrl * pList);
	void LoadInputConfig();
	void GetCmdInfo(CString cmd ,CString &info);
	bool AnalyseLine(CString line, CString &key, CString &cmd, CString &val);
public:
	virtual BOOL OnInitDialog();
	bool SaveInputConfig();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
};
