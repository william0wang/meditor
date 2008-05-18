#if !defined(AFX_MMEDIALIST_H__7607751D_0063_41BA_BD98_41A763B05DF5__INCLUDED_)
#define AFX_MMEDIALIST_H__7607751D_0063_41BA_BD98_41A763B05DF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MMediaList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMMediaList dialog
#include "wmpplaylist.h"
#include "wmpmedia.h"
#include "wmplayer.h"
#include "wmpcontrols.h"

class CMMediaList : public CDialog
{
// Construction
public:
	CMMediaList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMMediaList)
	enum { IDD = IDD_DIALOG_MLIST };
	CListBox	m_Dlist;
	//}}AFX_DATA
	CWMPlayer	*m_wmp;
	CWMPPlaylist m_wmplist;
	void ReFlashList();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMMediaList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg void OnDropFiles(HDROP hDropInfo);

// Implementation
protected:
	CWnd* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CMMediaList)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonClean();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnDblclkPlaylist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MMEDIALIST_H__7607751D_0063_41BA_BD98_41A763B05DF5__INCLUDED_)
