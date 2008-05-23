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

enum osd_type
{
	osd_normal,
	osd_play_time,
	osd_left_time,
	osd_none
};

enum osd_time
{
	time_none,
	time_t1,
	time_t2,
	time_t3,
	time_t4,
	time_dt1,
	time_dt2,
	time_dt3,
	time_dt4,
	time_ot
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

// Dialog Data
	//{{AFX_DATA(CMPlayerPage)
	enum { IDD = IDD_PLAYER_DIALOG };
	CEdit	m_fuzziness;
	CComboBox	m_autoplay;
	CComboBox	m_systray;
	CButton	m_bottom_c;
	CComboBox	m_switchview;
	CComboBox	m_cache;
	CComboBox	m_colorkey;
	CComboBox m_autosync;
	CComboBox	 m_language;
	CComboBox	 m_log;
	CComboBox m_loop;
	CComboBox m_monitor;
	CComboBox m_ontop;
	CComboBox m_priority;
	CComboBox	m_osd_font_c;
	CComboBox	 m_osdtime;
	CComboBox	 m_osdsize;
	CComboBox	 m_osdmode;
	BOOL	m_control;
	BOOL	m_framedrop;
	BOOL	m_fullscreen;
	BOOL	m_guithread;
	BOOL	m_menu;
	BOOL	m_oneplayer;
	BOOL	m_quit;
	BOOL	m_show;
	BOOL	m_url;
	CString	m_osd_font;
	CString	m_osdsize_s;
	CString	m_dvd;
	CString	m_end;
	CString	m_png;
	CString	m_start;
	CString	m_monitor_s;
	BOOL	m_quiet;
	BOOL	m_double;
	CString	m_colorkey_s;
	BOOL	m_conf;
	BOOL	m_bottom;
	CString	m_auto_fuzziness;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPlayerPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void InitFromConfig();
	// Generated message map functions
	//{{AFX_MSG(CMPlayerPage)
	afx_msg void OnButtonPng();
	afx_msg void OnButtonDvd();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckCtrl();
	afx_msg void OnSelchangeAutoplay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_htimer;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPLAYERPAGE_H__8314A3DD_EE1B_4563_B7FB_79EB59179287__INCLUDED_)
