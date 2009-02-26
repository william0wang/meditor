#pragma once


// CMProfilePage 对话框
#ifndef XLISTCTRLLIB_STATIC
#define XLISTCTRLLIB_STATIC
#endif
#include "XListCtrl.h"
#include "afxwin.h"
#include "AdvCombo\AdvComboBox.h"
class CMConfig;

class CMProfilePage : public CDialog
{
	DECLARE_DYNAMIC(CMProfilePage)

public:
	CMProfilePage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMProfilePage();
	CMConfig *m_cfg;
	void SaveConfig();

// 对话框数据
	enum { IDD = IDD_PROFILE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CStringArray m_rotate;
	CStringArray m_deinterlacing;
	CStringArray m_deblocking;
	CStringArray m_adv_af;
	CStringArray m_volnormal;
	CStringArray m_resample;
	CXListCtrl m_List;
	CString m_pre_profile;
	CString	m_str_at;
	CString	m_str_low;
	CString	m_str_int;
	CString	m_str_foa;
	CString	m_str_nco;
	CString use;
	CString nam;
	CString opt;
	CString inf;
	CString m_str_vet;
	CString m_str_cot;
	CStringArray m_str_profile;
	static int m_nColWidths[];
	void InitListCtrl(CXListCtrl * pList);
	void FillListCtrl(CXListCtrl * pList);
	void CleanConfig();
	void SetCurConfig();
	void SavePreConfig();
	void RemoveSubValue(CString &line ,CString subname ,CString sp = _T(","));


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CAdvComboBoxMod m_profile_list;
	CString m_profile;
	afx_msg void OnCbnSelchangeComboExtensino();
	afx_msg void OnBnClickedDel();
	afx_msg void OnBnClickedAdd();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
