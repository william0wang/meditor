#pragma once
#include "ProgressDlg.h"
#include "AdvCombo\AdvComboBox.h"


// CPreviewDlg 对话框

class CPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreviewDlg)

public:
	CPreviewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPreviewDlg();

// 对话框数据
	enum { IDD = IDD_PREVIEW_DIALOG };

protected:
	BOOL	m_have_preview;
	CString m_player_exe;
	CString m_program_dir;

	void GetPreview(int index, int resolution, int time);

	void GenerateThumbnails(CString ThumbName);

	BOOL EnumerateFonts();
	void AddFont(CString strName);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	static BOOL CALLBACK AFX_EXPORT EnumFamScreenCallBackEx(
		ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
		LPVOID pThis);

	int		m_percent;
	int		m_parti;
	int		m_partj;
	int		m_width;
	int		m_time;
	long	ltime;
	BOOL	m_show;
	double	m_aspect;
	CString m_filename;
	CString m_savename;
	CString m_font;
	CProgressDlg	m_pdlg;
	CSpinButtonCtrl m_spinr;
	CSpinButtonCtrl m_spinv;
	CSpinButtonCtrl m_spinw;
	CAdvComboBoxMod m_font_c;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonB();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
};
