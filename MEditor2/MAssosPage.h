#pragma once

#include "afxwin.h"

// CMAssosPage 对话框

class AssList
{
public:
	CStringArray m_type;
	CStringArray m_info;
	CStringArray m_class;
	CStringArray m_icon;
	AssList();
	void RemoveAll();
	void Add(CString type, CString info, CString classes,  CString icon);
	int GetInfo(CString type);
};

class CMAssosPage : public CDialog
{
	DECLARE_DYNAMIC(CMAssosPage)

public:
	CMAssosPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMAssosPage();

// 对话框数据
	enum { IDD = IDD_ASSOS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CString m_assoc_exe;
	BOOL	m_is_vista;
	AssList m_alist;
	AssList m_llist;

	CString m_str_list;
	CString m_str_flash;
	CString m_str_v;
	CString m_str_a;
	CString m_editor_exe;
	CString m_player_exe;
	CString m_icons_org;
	CString m_icons_dll;
	CString	m_micons_str;
	CString m_program_dir;

	bool AssosTypeDef(CString type, CString info, CString icons, bool isplaylist = false);

	DECLARE_MESSAGE_MAP()
public:
	//AssList m_olist;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAssos();
	afx_msg void OnBnClickedAssosDef();
};
