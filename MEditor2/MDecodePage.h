#if !defined(AFX_MDECODEPAGE_H__3F171818_60EC_45EF_9301_BBC9F39CF3DF__INCLUDED_)
#define AFX_MDECODEPAGE_H__3F171818_60EC_45EF_9301_BBC9F39CF3DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MDecodePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMDecodePage dialog

class CMConfig;
#ifndef XLISTCTRLLIB_STATIC
#define XLISTCTRLLIB_STATIC
#endif
#include "XListCtrl.h"
#include "afxwin.h"

enum codecs_ini
{
	inner,
	external,
	coreavc
};

enum Avcodec
{
	skiploopfilter,
	skipframe,
//	fast,
//	gray,
//	sb,
//	st,
//	threads,
	xy,
	lowres
};

class CMDecodePage : public CDialog
{
// Construction
public:
	CMDecodePage(CWnd* pParent = NULL);   // standard constructor
	CMConfig *m_cfg;
	void SaveConfig();
	void SetNormal();
	void SetHigh();
	void SetLower();


// Dialog Data
	//{{AFX_DATA(CMDecodePage)
	enum { IDD = IDD_DECODE_DIALOG };
	//}}AFX_DATA
protected:
	CXListCtrl m_vlist;
	CXListCtrl m_alist;
	CXListCtrl m_avlist;
	CStringArray m_vdecode;
	CStringArray m_adecode;
	CStringArray m_skip;
	CStringArray m_threads;
	static int m_nColWidths[];
	static int m_nColWidths_AV[];
	void FillListCtrl(CXListCtrl * pList);
	void InitListCtrl(CXListCtrl * pList);
	void FillListCtrlAV(CXListCtrl * pList);
	void InitListCtrlAV(CXListCtrl * pList);
	CString m_program_dir;
	int m_last_extract;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDecodePage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void InitFromConfig();
	// Generated message map functions
	//{{AFX_MSG(CMDecodePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonAdd2();
	afx_msg void OnButtonDel2();
	afx_msg void OnButtonUp2();
	afx_msg void OnButtonDown2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_codecs;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDECODEPAGE_H__3F171818_60EC_45EF_9301_BBC9F39CF3DF__INCLUDED_)
