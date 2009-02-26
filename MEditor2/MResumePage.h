#pragma once


#ifndef XLISTCTRLLIB_STATIC
#define XLISTCTRLLIB_STATIC
#endif
#include "XListCtrl.h"
// CMResumePage 对话框
class CMConfig;

struct resume_info
{
		CString Resume;
		CString Time;
		CString Audio;
		CString DVDSub;
		CString VOBSub;
		CString TXTSub;
		CString SubPos;
		CString Filename;
		void WritePrivateProfile(int index, CString m_configs_ex);
};

class CMResumePage : public CDialog
{
	DECLARE_DYNAMIC(CMResumePage)

public:
	CMResumePage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMResumePage();
	CMConfig *m_cfg;
	void SaveConfig();

// 对话框数据
	enum { IDD = IDD_RESUME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CString m_configs_ex;
	CString m_program_dir;
	CString m_maxnum;
	resume_info reinfo[12];
	int m_number;
	CXListCtrl m_list;
	CButton	m_auto;
	CButton	m_manual;
	static int m_nColWidths[];
	void InitListCtrl(CXListCtrl * pList);

	CString no;
	CString time;
	CString file;
	CString nol;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonCla();
	BOOL m_auto_resume;
	afx_msg void OnBnClickedRadioAuto();
	afx_msg void OnBnClickedRadioManual();
};
