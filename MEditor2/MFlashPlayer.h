//{{AFX_INCLUDES()
#include "shockwaveflash.h"
//}}AFX_INCLUDES
#if !defined(AFX_MFLASHPLAYER_H__F3E8ACAE_0892_4DE7_A5C8_5439EF100E51__INCLUDED_)
#define AFX_MFLASHPLAYER_H__F3E8ACAE_0892_4DE7_A5C8_5439EF100E51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MFlashPlayer.h : header file
//
class CMPlayList;
#include "MInputList.h" 
/////////////////////////////////////////////////////////////////////////////
// CMFlashPlayer dialog

class CMFlashPlayer : public CDialog
{
// Construction
public:
	CMFlashPlayer(CWnd* pParent = NULL);   // standard constructor
	void IninFileName(CString filename);

// Dialog Data
	//{{AFX_DATA(CMFlashPlayer)
	enum { IDD = IDD_DIALOG_FLASH };
	CSliderCtrl	m_control;
	CShockwaveFlash	m_flash;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFlashPlayer)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg void OnDropFiles(HDROP hDropInfo);

// Implementation
protected:
	HICON m_hIcon;
	int m_current;
	long m_fnumber;
	bool m_inited;
	bool m_keydowning;
	bool m_ontop;
	bool m_fs_init;
	bool m_fs;
	bool m_rmenu;
	bool m_showctrl;
	bool m_changing;
	int save_x;
	int save_y;
	int save_width;
	int save_height;
	int m_scr_width;
	int m_scr_height;
	int now_state;
	CMPlayList *m_list;
	CStringArray m_playlist;
	CMInputList m_inputs;
	CString m_program_dir;
	CMenu *m_menu;
	void PlayFile(bool init = false);
	void FullScreen(bool init = false);
	int GetKeyNumber(CString Key);
	bool GetKeyCommand(LONG KeyNumber,CString &cmd, CString &value);
	void LoadConfig();
	bool AnalyseLine(CString line, CString &key, CString &cmd, CString &val);

	// Generated message map functions
	//{{AFX_MSG(CMFlashPlayer)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFlashOpen();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnProgressflash(long percentDone);
	afx_msg void OnFlashCall(LPCTSTR request);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnReleasedcaptureSliderControl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMplaylist();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnOnReadyStateChange(long newState);
	afx_msg void OnNext();
	afx_msg void OnPre();
	afx_msg void OnFSCommand(LPCTSTR command, LPCTSTR args);
	afx_msg void OnRmenu();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LONG OnSelChange(UINT lParam, LONG wParam);
	afx_msg LONG OnCmdKeyDown(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFLASHPLAYER_H__F3E8ACAE_0892_4DE7_A5C8_5439EF100E51__INCLUDED_)
