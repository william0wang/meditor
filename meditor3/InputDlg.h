// InputDlg.h : interface of the CInputDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ListCtrl/ListCtrl.h"
#include "InputList.h"

#include <vector>
using namespace std;

class CInputDlg : public CDialogImpl<CInputDlg>, public CUpdateUI<CInputDlg>,
		public CMessageFilter, public CIdleHandler, public CWinDataExchange<CInputDlg>
{
public:
	UINT IDD;

	CKeyList m_skey;
	CInputList m_inputs;

	CListCtrl m_wndListCtrl;

	CInputDlg(UINT IDD_DLG);

	void GetCmdInfo(CString cmd ,CString &info);

	void LoadInputConfig();
	bool SaveInputConfig();
	
	void InitBasicList();

	int CheckReuse();

	bool AnalyseLine(CString line, CString &key, CString &cmd, CString &val, CString &abs);

	int m_def_number;
	int m_readonly_number;

	CString m_program_dir;
	vector<CString> m_str_list;
	vector<CString> m_str_input;

	BEGIN_DDX_MAP(CInputDlg)
	END_DDX_MAP();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CInputDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CInputDlg)
		COMMAND_HANDLER(IDC_BUTTON_ADD, BN_CLICKED, OnBnClickedAdd)
		COMMAND_HANDLER(IDC_BUTTON_DEL, BN_CLICKED, OnBnClickedDel)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);


	LRESULT OnBnClickedAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};
