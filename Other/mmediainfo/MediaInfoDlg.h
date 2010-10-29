// MediaInfoDlg.h : interface of the CMediaInfoDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ListCtrl/ListCtrl.h"

class CMediaInfoDlg : public CDialogImpl<CMediaInfoDlg>,
	public CWinDataExchange<CMediaInfoDlg>
{
public:
	enum { IDD = IDD_DIALOG_INFO };

	CListCtrl				m_wndListCtrl;

	void SetItemColor(int nItem, COLORREF color);
	void loadInfoFile(LPCTSTR filename);

	void GetMediaInfo(LPCTSTR filename);

	CMediaInfoDlg();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_DDX_MAP(CMediaInfoDlg)
	END_DDX_MAP()

	BEGIN_MSG_MAP(CMediaInfoDlg)
		MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_BUTTON_CLOSE, BN_CLICKED, OnBnClickedButtonClose)
		COMMAND_HANDLER(IDC_BUTTON_SAVE, BN_CLICKED, OnBnClickedButtonSave)
	END_MSG_MAP()


	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void CloseDialog(int nVal);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBnClickedButtonClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
