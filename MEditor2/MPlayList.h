#if !defined(AFX_MPLAYLIST_H__6E7D3D0C_60A0_4511_8726_61D824497ADF__INCLUDED_)
#define AFX_MPLAYLIST_H__6E7D3D0C_60A0_4511_8726_61D824497ADF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MPlayList.h : header file
//

//#include "DragAndDropListBox.h"
/////////////////////////////////////////////////////////////////////////////
// CMPlayList dialog

#define PLS_SELCHANGE     WM_USER + 1110

class CMPlayList : public CDialog
{
// Construction
public:
	CMPlayList(CWnd* pParent = NULL);   // standard constructor
	CStringArray *m_playlist;
	void ReFlashList();

// Dialog Data
	//{{AFX_DATA(CMPlayList)
	enum { IDD = IDD_DIALOG_LIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPlayList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg void OnDropFiles(HDROP hDropInfo);

// Implementation
protected:
	CListBox m_Dlist;
	CWnd* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CMPlayList)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonClean();
	afx_msg void OnDblclkPlaylist();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPLAYLIST_H__6E7D3D0C_60A0_4511_8726_61D824497ADF__INCLUDED_)
