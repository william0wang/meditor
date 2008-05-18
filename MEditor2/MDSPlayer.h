#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "CDXGraph.h"
#include "MInputList.h" 

// CMDSPlayer 对话框

class CMDSPlayer : public CDialog
{
	DECLARE_DYNAMIC(CMDSPlayer)

public:
	CMDSPlayer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMDSPlayer();
	void IninFileName(CString filename);

// 对话框数据
	enum { IDD = IDD_DIALOG_DSHOW };

protected:
	HICON m_hIcon;
	bool m_fs;
	bool m_fs_init;
	bool m_ontop;
	bool m_keydowning;
	int save_x;
	int save_y;
	int save_width;
	int save_height;
	int m_scr_width;
	int m_scr_height;
	bool TimerRuning;
	bool m_showctrl;
	CMenu *m_menu;
	CString m_program_dir;
	CMInputList m_inputs;
	CString m_filename;
	CSliderCtrl m_SliderGraph;
	CDXGraph* m_FilterGraph;     // Filter Graph封装
	void CreateGraph(const CString& strSourceFile);
	void DestroyGraph(void);
	void OnFullScreen(void);
	void Seek(double len);
	int GetKeyNumber(CString Key);
	bool GetKeyCommand(LONG KeyNumber,CString &cmd, CString &value);
	void LoadConfig();
	bool AnalyseLine(CString line, CString &key, CString &cmd, CString &val);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNMReleasedcaptureSliderCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDsOpen();
	afx_msg void OnDsPlay();
	afx_msg void OnDsStop();
	afx_msg void OnDsFullscreen();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LONG OnCmdKeyDown(UINT lParam, LONG wParam);
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
