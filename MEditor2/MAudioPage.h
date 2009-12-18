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
#include "afxwin.h"
#include "AdvCombo\AdvComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CMAudioPage dialog

enum ao
{
	dsound,
	win32,
	ao_sdl,
	pcm,
	ao_null
};

enum channels
{
	ch_auto,
	ch_2,
	ch_4,
	ch_6,
	ch_7,
	ch_8
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
	enum { IDD = IDD_AUDIO_DIALOG };
	CAdvComboBoxMod	m_channels;
	CAdvComboBoxMod	m_audio;
	CString	m_audio_delay;
	CString m_alang;
	CString	m_volume;
	CString	m_volnorm;
	int		m_volnorm_s;
	int		m_volume_s;
	CString	m_str_no;
	CString	m_str_at;
	CString	m_str_low;
	CString	m_str_int;
	CString	m_str_foa;
	CSliderCtrl	m_volnorm_c;
	CStringArray m_adv_af;
	CStringArray m_volnormal;
	CStringArray m_equalizer;
	CStringArray m_resample;
	CStringArray m_wadsp;
	CXListCtrl m_List;
	static int m_nColWidths[];
	void FillListCtrl(CXListCtrl * pList);
	void InitListCtrl(CXListCtrl * pList);

	CString use;
	CString nam;
	CString opt;
	CString inf;
	CString str_af1;
	CString str_af2;
	CString str_vn1;
	CString str_vn2;
	CString str_st1;
	CString str_st2;
	CString str_re1;
	CString str_re2;
	CString str_eq1;
	CString str_eq2;
	CString str_dsp1;
	CString str_dsp2;

	CStringArray m_str_audio;
	CStringArray m_str_channels;

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
