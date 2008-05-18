#pragma once

#include "real_player.h"
#include "MInputList.h"

// CMRealPlayer 对话框

class CMRealPlayer : public CDialog
{
	DECLARE_DYNAMIC(CMRealPlayer)

public:
	CMRealPlayer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMRealPlayer();
	void IninFileName(CString filename);

// 对话框数据
	enum { IDD = IDD_DIALOG_REAL };

protected:
	HICON m_hIcon;
	CString m_program_dir;
	CMInputList m_inputs;
	bool m_ontop;
	bool m_fs_init;
	bool m_fs;
	bool m_rmenu;
	bool m_first_play;
	bool m_keydowning;
	CString m_filename;
	CMenu *m_menu;

	int GetKeyNumber(CString Key);
	bool GetKeyCommand(LONG KeyNumber,CString &cmd, CString &value);
	void LoadConfig();
	bool AnalyseLine(CString line, CString &key, CString &cmd, CString &val);
	afx_msg LONG OnCmdKeyDown(UINT lParam, LONG wParam);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CReal_player m_real;
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_EVENTSINK_MAP()
	void OnStateChangeRealPlayer(long lOldState, long lNewState);
	afx_msg void OnRealOpen();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnRplay();
	afx_msg void OnRightmenu();
	afx_msg void OnFullscreen();
	afx_msg void OnRstop();
};
