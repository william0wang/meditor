#pragma once

#ifndef XLISTCTRLLIB_STATIC
#define XLISTCTRLLIB_STATIC
#endif
#include "XListCtrl.h" 
#include "afxwin.h"
#include "AdvCombo\AdvComboBox.h"
class CMConfig;

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
	CMConfig *m_cfg;
	void SaveConfig();

// 对话框数据
	enum { IDD = IDD_ASSOS_DIALOG };

	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	bool IsAssosed(CString type);
	bool AssosTypeIner(CString type, CString info, CString icons, bool Assos = true, bool isflash = true);
	bool AssosType(CString type, CString info, CString icons, bool isplaylist, bool Assos = true);
	AssList m_alist;
	CAdvComboBoxMod	m_micons;
	CXListCtrl m_List;
	CStringArray m_sa;
	CStringArray m_stv;
	CStringArray m_stf;
	CStringArray m_sta;
	CStringArray m_stl;
	CString m_player_exe;
	CString m_mpc_exe;
	CString m_icons_org;
	CString m_icons_dll;
	CString	m_micons_str;
	CString m_program_dir;
	CString m_str_list;
	CString m_str_flash;
	CString m_str_ds;
	CString m_str_v;
	CString m_str_a;
	CString ass;
	CString ex;
	CString inf;
	CString tp;

	bool m_dll_getted;
	bool m_have_icons;
	bool m_special;
	int m_default_num;
	int m_vedio_num;
	int m_audio_num;
	static int m_nColWidths[];
	void FillListCtrl(CXListCtrl * pList);
	void InitListCtrl(CXListCtrl * pList);

	DECLARE_MESSAGE_MAP()
public:
	//AssList m_olist;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAssos();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDel();
	afx_msg void OnBnClickedAll();
	afx_msg void OnBnClickedNone();
	afx_msg void OnBnClickedRecommand();
	void ApplyChange(bool quiet = false);
	BOOL m_rightmenu;
	BOOL m_rightmenu2;
	BOOL m_mpc;
	CButton m_mpc_c;
};
