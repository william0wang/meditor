#pragma once
#include "explorer_html.h"


// CMediaInfoDlg 对话框

class CMediaInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CMediaInfoDlg)

public:
	CMediaInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMediaInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MEDIAINFO };

protected:
	HICON m_hIcon;
	CString URL;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CExplorer_html m_html;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	DECLARE_EVENTSINK_MAP()
	void DownloadCompleteExplorerHtml();
	void NavigateComplete2ExplorerHtml(LPDISPATCH pDisp, VARIANT* URL);
};
