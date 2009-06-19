#include "afxwin.h"
#if !defined(AFX_MPLAYERPAGE_H__8314A3DD_EE1B_4563_B7FB_79EB59179287__INCLUDED_)
#define AFX_MPLAYERPAGE_H__8314A3DD_EE1B_4563_B7FB_79EB59179287__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MPlayerPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMPlayerPage dialog

class CMConfig;
class CMShowInfoDlg;
#include "AdvCombo\AdvComboBox.h"

enum language
{
	lang_auto,
	lang_sc,
	lang_en,
	lang_tc,
	lang_big5
};
enum ontop
{
	top_none,
	top_always,
	top_playing
};
enum autoplay
{
	auto_none,
	auto_normal,
	auto_ex
};
enum systray
{
	sys_task,
	sys_tray,
	sys_tray_stop
};
enum autosync
{
	sync_none,
	sync_normal,
	sync_fast,
	sync_flv,
	sync_flvex,
	sync_avi
};
enum priority
{
	prio_auto,
	prio_realtime,
	prio_high,
	prio_abovenormal,
	prio_normal,
	prio_belownormal,
	prio_idle
};
enum loop
{
	loop_none,
	loop_single,
	loop_list
};
enum log
{
	log_none,
	log_normal,
	log_v1,
	log_v2,
	log_v3,
	log_v4,
	log_v5
};


enum switch_view
{
	switch_none,
	switch_manual,
	switch_auto
};

class CMPlayerPage : public CDialog
{
// Construction
public:
	CMPlayerPage(CWnd* pParent = NULL);   // standard constructor
	CMConfig *m_cfg;
	void SaveConfig();
	void SetNormal();
	void SetHigh();
	void SetLower();

	enum { IDD = IDD_PLAYER_DIALOG };
	CAdvComboBoxMod	m_autoplay;
	CAdvComboBoxMod	m_systray;
	CAdvComboBoxMod	m_switchview;
	CAdvComboBoxMod	m_cache;
	CAdvComboBoxMod	m_colorkey;
	CAdvComboBoxMod m_autosync;
	CAdvComboBoxMod	m_language;
	CAdvComboBoxMod	m_log;
	CAdvComboBoxMod m_loop;
	CAdvComboBoxMod m_monitor;
	CAdvComboBoxMod m_ontop;
	CAdvComboBoxMod m_priority;
	CAdvComboBoxMod m_ctrlbar;
	CAdvComboBoxMod m_alpha;
	CEdit	m_fuzziness;
	BOOL	m_control;
	BOOL	m_framedrop;
	BOOL	m_fullscreen;
	BOOL	m_guithread;
	BOOL	m_menu;
	BOOL	m_oneplayer;
	BOOL	m_quit;
	BOOL	m_show;
	BOOL	m_url;
	BOOL	m_htimer;
	BOOL	m_rightmenu;
	BOOL	m_gui_high;
	BOOL	m_fixedvo;
	BOOL	m_reload;
	BOOL	m_no_dvdnav;
	BOOL	m_quiet;
	BOOL	m_double;
	BOOL	m_conf;
	BOOL	m_filename;
	CString	m_ctrlbar_s;
	CString	m_colorkey_s;
	CString m_def;
	CString	m_auto_fuzziness;
	CString	m_dvd;
	CString	m_end;
	CString	m_png;
	CString	m_start;
	CString	m_monitor_s;
	CString	m_auto_s;
	CString	m_no_s;
	CMShowInfoDlg *info;

	CStringArray m_str_language;
	CStringArray m_str_ontop;
	CStringArray m_str_autosync;
	CStringArray m_str_priority;
	CStringArray m_str_loop;
	CStringArray m_str_log;
	CStringArray m_str_switchview;
	CStringArray m_str_systray;
	CStringArray m_str_autoplay;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPlayerPage)
	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		void SetInfoDlg(CMShowInfoDlg *infoDlg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void InitFromConfig();
	void ShowInfo(int type);
	// Generated message map functions
	//{{AFX_MSG(CMPlayerPage)
	afx_msg void OnButtonPng();
	afx_msg void OnButtonDvd();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeAutoplay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDef();
	BOOL m_console;
	BOOL m_noskin;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPLAYERPAGE_H__8314A3DD_EE1B_4563_B7FB_79EB59179287__INCLUDED_)
