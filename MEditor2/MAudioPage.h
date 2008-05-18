#if !defined(AFX_MAUDIOPAGE_H__2A541CDC_A24D_4214_9BA9_0638879E5337__INCLUDED_)
#define AFX_MAUDIOPAGE_H__2A541CDC_A24D_4214_9BA9_0638879E5337__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MAudioPage.h : header file
//

class CMConfig;
#ifndef XLISTCTRLLIB_STATIC
#define XLISTCTRLLIB_STATIC
#endif
#include "XListCtrl.h" 
/////////////////////////////////////////////////////////////////////////////
// CMAudioPage dialog

enum ao
{
	dsound,
	win32,
	pcm,
	ao_null
};

enum fuzziness
{
	exact,
	name,
	all
};

enum align
{
	top,
	center,
	bottom
};

enum autoscale
{
	none,
	high,
	width,
	diagonal
};

enum channels
{
	ch_auto,
	ch_2,
	ch_4,
	ch_6
};

class CMAudioPage : public CDialog
{
// Construction
public:
	CMAudioPage(CWnd* pParent = NULL);   // standard constructor
	CMConfig *m_cfg;
	void SaveConfig();
	void SetNormal();
	void SetHigh();
	void SetLower();

protected:
// Dialog Data
	//{{AFX_DATA(CMAudioPage)
	enum { IDD = IDD_AUDIO_DIALOG };
	CComboBox	m_autoscale;
	CComboBox	m_align;
	CComboBox	m_fuzziness;
	CComboBox	m_bcolor_c;
	CComboBox	m_color_c;
	CComboBox	m_channels;
	CComboBox	m_font2_c;
	CComboBox	m_font_c;
	CSliderCtrl	m_volnorm_c;
	CComboBox	 m_size;
	CComboBox	 m_audio;
	BOOL	m_dvdsub;
	BOOL	m_ass;
	CString	m_audio_delay;
	CString	m_sub_delay;
	CString	m_sub_error;
	CString	m_volume;
	CString	m_volnorm;
	int		m_volnorm_s;
	int		m_volume_s;
	CString	m_font;
	CString	m_font2;
	CString	m_size_s;
	CString	m_color;
	CString	m_bcolor;
	CString	m_subpos;
	CString	m_slang;
	CString	m_subcp;
	//}}AFX_DATA
	CStringArray m_adv_af;
	CStringArray m_volnormal;
	CStringArray m_equalizer;
	CStringArray m_resample;
	CXListCtrl m_List;
	static int m_nColWidths[];
	void FillListCtrl(CXListCtrl * pList);
	void InitListCtrl(CXListCtrl * pList);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMAudioPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitFromConfig();

	// Generated message map functions
	//{{AFX_MSG(CMAudioPage)
	afx_msg void OnReleasedcaptureSliderVolume(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditVolume();
	afx_msg void OnReleasedcaptureSliderVolnorm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditVolnorm();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAUDIOPAGE_H__2A541CDC_A24D_4214_9BA9_0638879E5337__INCLUDED_)
