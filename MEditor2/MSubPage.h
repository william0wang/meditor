#pragma once

#include "ColorButton.h"
#include "afxwin.h"
#include "AdvCombo\AdvComboBox.h"

// CMSubPage 对话框
enum align
{
	top,
	center,
	bottom
};

enum fuzziness
{
	exact,
	name,
	all
};


enum autoscale
{
	none,
	high,
	width,
	diagonal
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

class CMConfig;
class CMShowInfoDlg;

class CMSubPage : public CDialog
{
	DECLARE_DYNAMIC(CMSubPage)

public:
	CMSubPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMSubPage();
	CMConfig *m_cfg;
	void SaveConfig();
	void SetNormal();
	void SetHigh();
	void SetLower();

protected:
	CAdvComboBoxMod	m_autoscale;
	CAdvComboBoxMod	m_align;
	CAdvComboBoxMod	m_fuzziness;
	CAdvComboBoxMod	m_font2_c;
	CAdvComboBoxMod	m_font_c;
	CAdvComboBoxMod	m_size;
	CAdvComboBoxMod	m_osd_font_c;
	CAdvComboBoxMod	m_osdtime;
	CAdvComboBoxMod	m_osdsize;
	CAdvComboBoxMod	m_osdmode;
	CAdvComboBoxMod	m_ass_expand;
	CString	m_osd_font;
	CString	m_osdsize_s;
	BOOL	m_dvdsub;
	BOOL	m_ass;
	CString	m_sub_delay;
	CString	m_sub_error;
	CString	m_font;
	CString	m_font2;
	CString	m_size_s;
	CString	m_subpos;
	CString	m_slang;
	CString	m_subcp;
	BOOL m_ass_use_margins;
	BOOL m_noautosub;
	CString m_ass_expand_s;
	CString	m_str_at;
	CString	m_str_nco;
	CString	m_str_no;
	CColorButton m_color_pri;
	CColorButton m_color_sec;
	CColorButton m_color_out;
	CColorButton m_color_bak;
	BOOL m_osdpercent;
	BOOL m_colpri;
	BOOL m_colsec;
	BOOL m_colout;
	BOOL m_colbak;
	int m_colpria;
	int m_colseca;
	int m_colouta;
	int m_colbaka;
	CStringArray m_fuzziness_str;
	CStringArray m_align_str;
	CStringArray m_autoscale_str;
	CStringArray m_osdtime_str;
	CStringArray m_osdmode_str;

// 对话框数据
	enum { IDD = IDD_SUBTITLE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void InitFromConfig();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_outline;
	CString m_shadow;
	BOOL m_boutline;
	BOOL m_bshadow;
	BOOL m_fontconfig;
	BOOL m_bold;
	BOOL m_italic;
	CMShowInfoDlg *info;
	void SetInfoDlg(CMShowInfoDlg *infoDlg);
	void ShowInfo(int type);
	afx_msg void OnBnClickedCheckFontconfig();
	float m_ass_font_scale;
};
