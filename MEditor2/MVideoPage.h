#if !defined(AFX_MVIDEOPAGE_H__C936F918_FDF3_4D73_A465_BAADEADF18A3__INCLUDED_)
#define AFX_MVIDEOPAGE_H__C936F918_FDF3_4D73_A465_BAADEADF18A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MVideoPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMVideoPage dialog
#ifndef XLISTCTRLLIB_STATIC
#define XLISTCTRLLIB_STATIC
#endif
#include "XListCtrl.h"
#include "ColorButton.h"
#include "afxcmn.h"
#include "AdvCombo\AdvComboBox.h"

enum Filters
{
	screenshot,
	ass,
	expand,
	aspect,
	eq2,
	hue,
	crop,
	scale,
	deinterlacing,
	deblocking,
	dering,
	tmpnoise,
	flip,
	rotates,
	autolevels,
	vf_other
};

enum Deinterlacing
{
	di_l5,
	di_lb,
	di_ci,
	di_md,
	di_fd,
	di_yadif
};

enum Deblocking
{
	db_hb,
	db_vb,
	db_ha,
	db_va,
	db_b,
	db_a
};

enum vo
{
	direct3d,
	direct3d_nolevel,
	directx,
	gl,
	glauto,
	glyuv3,
	glyuv4,
	glnv,
	glati,
	glyuv6,
	glyuv0,
	gl2,
	mx,
	mx_l,
	mx_h,
	//sdl,
	jpeg,
	png,
	yuv4mpeg,
	null
};

enum mx_color
{
	mx_g,
	mx_b,
	mx_r,
	mx_a
};

class CMConfig;
class CMShowInfoDlg;

class CMVideoPage : public CDialog
{
// Construction
public:
	CMVideoPage(CWnd* pParent = NULL);   // standard constructor
	CMConfig *m_cfg;
	void SaveConfig();
	void SetNormal();
	void SetHigh();
	void SetLower();

protected:
// Dialog Data
	enum { IDD = IDD_VIDEO_DIALOG };
	CAdvComboBoxMod	m_vo;
	CSliderCtrl	m_sc;
	CSliderCtrl	m_hc;
	CSliderCtrl	m_cc;
	CSliderCtrl	m_bc;
	BOOL	m_noflash;
	BOOL	m_forcepbo;
	CString	m_color;
	CString	m_saturation;
	int		m_saturation_s;
	CString	m_hue;
	int		m_hue_s;
	int		m_contrast_s;
	CString	m_contrast;
	CString	m_brightness;
	int		m_brightness_s;
	CString m_gamma;
	int m_gamma_s;
	CSliderCtrl m_gc;
	BOOL	m_keepaspect;
	BOOL	m_framedrop;
	BOOL	m_idx;
	CMShowInfoDlg *info;
	
	CStringArray m_rotate;
	CStringArray m_deinterlacing;
	CStringArray m_deblocking;
	CStringArray m_vo_str;
	CStringArray m_str_vf;

	CString use;
	CString nam;
	CString opt;
	CString inf;
	CString m_str_vet;
	CString m_str_cot;

	CColorButton m_color_b;
	CXListCtrl m_List;
	static int m_nColWidths[];
	void FillListCtrl(CXListCtrl * pList);
	void InitListCtrl(CXListCtrl * pList);
	void ShowInfo(int type);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMVideoPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitFromConfig();
	void RemoveSubValue(CString &line ,CString subname ,CString sp = _T(","));

	// Generated message map functions
	//{{AFX_MSG(CMVideoPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedcaptureSliderBringhtness(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderContrast(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderHue(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderSaturations(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMReleasedcaptureSliderGamma(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeEditBringhtness();
	afx_msg void OnChangeEditContrast();
	afx_msg void OnChangeEditHue();
	afx_msg void OnChangeEditSaturations();
	//}}AFX_MSG
	afx_msg LRESULT OnSelChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditGamma();
	BOOL m_dr;
	void SetInfoDlg(CMShowInfoDlg *infoDlg);
	BOOL m_vista_fs;
	BOOL m_aero_directx;
	BOOL m_d3dfix;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MVIDEOPAGE_H__C936F918_FDF3_4D73_A465_BAADEADF18A3__INCLUDED_)
