#pragma once
#include "afxcmn.h"
#include "ProgressDlg.h"


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

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	int		m_percent;
	int		m_parti;
	int		m_partj;
	int		m_width;
	int		m_time;
	long	ltime;
	BOOL	m_show;
	CString m_filename;
	CString m_savename;
	CProgressDlg	m_pdlg;
	CSpinButtonCtrl m_spinr;
	CSpinButtonCtrl m_spinv;
	CSpinButtonCtrl m_spinw;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonB();
};
