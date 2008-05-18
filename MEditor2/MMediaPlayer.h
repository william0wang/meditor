//{{AFX_INCLUDES()
#include "wmplayer.h"
//}}AFX_INCLUDES
#if !defined(AFX_MMEDAIPLAYER_H__57FE590B_3E28_4D47_81BA_7AADF996D456__INCLUDED_)
#define AFX_MMEDAIPLAYER_H__57FE590B_3E28_4D47_81BA_7AADF996D456__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MMedaiPlayer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMMediaPlayer dialog
#include "MInputList.h" 
#include "wmpcontrols.h"
#include "wmpsettings.h"
#include "wmpplaylist.h"
#include "wmpmedia.h"
class CMMediaList;

class CMMediaPlayer : public CDialog
{
// Construction
public:
	CMMediaPlayer(CWnd* pParent = NULL);   // standard constructor
	void IninFileName(CString filename);

// Dialog Data
	//{{AFX_DATA(CMMediaPlayer)
	enum { IDD = IDD_DIALOG_MEDIA };
	CSliderCtrl	m_control;
	CWMPlayer	m_wmp;
	//}}AFX_DATA
	CWMPControls m_wmpctrl;
	CWMPSettings m_wmpset;
	CWMPPlaylist m_wmplist;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMMediaPlayer)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg void OnDropFiles(HDROP hDropInfo);

// Implementation
protected:
	HICON m_hIcon;
	CString m_filename;
	bool m_inited;
	long m_status;
	bool m_keydowning;
	bool m_ontop;
	bool m_fs_init;
	bool m_fs;
	bool m_rmenu;
	bool m_changing;
	bool m_showctrl;
	double m_length;
	int save_x;
	int save_y;
	int save_width;
	int save_height;
	int m_scr_width;
	int m_scr_height;
	CMenu *m_menu;
	CMInputList m_inputs;
	CString m_program_dir;
	CMMediaList *m_list;
	void FullScreen(bool init = false);
	int GetKeyNumber(CString Key);
	bool GetKeyCommand(LONG KeyNumber,CString &cmd, CString &value);
	void LoadConfig();
	bool AnalyseLine(CString line, CString &key, CString &cmd, CString &val);
	// Generated message map functions
	//{{AFX_MSG(CMMediaPlayer)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnMediaOpen();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnPlayStateChange(long NewState);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPrev();
	afx_msg void OnNext();
	afx_msg void OnFullscreen();
	afx_msg void OnRightmenu();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnReleasedcapture(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPlaylist();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LONG OnCmdKeyDown(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MMEDAIPLAYER_H__57FE590B_3E28_4D47_81BA_7AADF996D456__INCLUDED_)
