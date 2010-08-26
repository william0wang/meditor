// MediaInfoDlg.cpp : implementation of the CMediaInfoDlg class
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"

#include "MediaInfoDlg.h"
#include "AtlStdioFile.h"
#include "MediaInfo/MediaInfoDLL.h"

CMediaInfoDlg::CMediaInfoDlg()
{
	m_wndListCtrl.RegisterClass();
}

BOOL CMediaInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

void CMediaInfoDlg::SetItemColor(int nItem, COLORREF color)
{
	m_wndListCtrl.SetItemColours(nItem, 0, color, RGB(0, 0, 0));
	m_wndListCtrl.SetItemColours(nItem, 1, color, RGB(0, 0, 0));
	m_wndListCtrl.SetItemColours(nItem, 2, color, RGB(0, 0, 0));
}

LRESULT CMediaInfoDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	m_wndListCtrl.SubclassWindow( GetDlgItem( IDC_LISTCTRL ) );
	m_wndListCtrl.ShowHeaderSort(FALSE);
	m_wndListCtrl.SetFocusSubItem(FALSE);
	m_wndListCtrl.SetSingleSelect(TRUE);
	m_wndListCtrl.AddColumn( _T(""), 80);
	m_wndListCtrl.AddColumn( _T(""), 180);
	m_wndListCtrl.AddColumn( _T(""), 320);

	DoDataExchange(false);
	return TRUE;
}

void CMediaInfoDlg::GetMediaInfo(LPCTSTR filename)
{
	MediaInfoLib::MediaInfo MI;
	CString info, line, str;

	m_wndListCtrl.DeleteAllItems();

	if (MI.Open(filename)) {
		MI.Option(_T("Complete"));
		info = MI.Inform().c_str();

		int index = 0;
		while(true) {
			index = info.Find(_T("\n"));
			if(index < 0)
				break;

			line = info.Left(index);
			info.Delete(0, index+1);

			line.Trim();
			if(line.IsEmpty())
				continue;

			index = line.Find(_T(" : "));
			if(index <= 0) {
				int nItem = m_wndListCtrl.AddItem(line);

				LOGFONT lf;
				CFontHandle font = m_wndListCtrl.GetItemFont(nItem, 0);
				font.GetLogFont(lf);
				lf.lfWeight = FW_BOLD;
				HFONT hfont = CreateFontIndirect(&lf);
				m_wndListCtrl.SetItemFont(nItem, 0, hfont);

			} else {
				int nItem = m_wndListCtrl.AddItem(_T(""));

				str = line.Left(index);
				line.Delete(0, index+3);

				str.Trim();
				line.Trim();

				m_wndListCtrl.SetItemText(nItem, 1, str);
				m_wndListCtrl.SetItemText(nItem, 2, line);
			}
		}

		MI.Close();
	}
}

void CMediaInfoDlg::loadInfoFile(LPCTSTR filename)
{
	CAtlStdioFile file;
	CString str, str1;

	m_wndListCtrl.DeleteAllItems();

	int	codepage = CP_ACP;
	LANGID   _SysLangId   =   GetSystemDefaultLangID();

	if(PRIMARYLANGID(_SysLangId)   ==   LANG_CHINESE) {
		if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_SIMPLIFIED)
			codepage = 936;		//Simplified Chinese GBK
		else if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_TRADITIONAL)
			codepage = 950;		//Traditional Chinese Big5
		else if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_HONGKONG)
			codepage = 950;		//Traditional Chinese Big5
	} else if(PRIMARYLANGID(_SysLangId)   ==   LANG_JAPANESE)
		codepage = 932;			//Japanese Shift-JIS
	else if(PRIMARYLANGID(_SysLangId)   ==   LANG_KOREAN)
		codepage = 949;			//Korean

	if(SUCCEEDED(file.Create(filename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING))) {
		while(file.ReadLine(str, codepage)) {
			int index = str.Find(_T(",_,"));
			if(index <= 0)
				continue;
			str1 = str.Left(index);
			int nItem = m_wndListCtrl.AddItem(str1);

			str.Delete(0, index + 3);

			index = str.Find(_T(",_,"));
			if(index <= 0)
				continue;

			LOGFONT lf;
			CFontHandle font = m_wndListCtrl.GetItemFont(nItem, 0);
			font.GetLogFont(lf);
			lf.lfWeight = FW_BOLD;
			HFONT hfont = CreateFontIndirect(&lf);
			m_wndListCtrl.SetItemFont(nItem, 0, hfont);

			str1 = str.Left(index);
			str.Delete(0, index + 3);

			m_wndListCtrl.SetItemText(nItem, 1, str1);
			m_wndListCtrl.SetItemText(nItem, 2, str);
		}
		file.Close();
	}

	DeleteFile(filename);
}

LRESULT CMediaInfoDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CMediaInfoDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMediaInfoDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	//if class is maindlg
	//::PostQuitMessage(nVal);
}


LRESULT CMediaInfoDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//UINT nType = (UINT)wParam;
	//CSize size = _WTYPES_NS::CSize(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	m_wndListCtrl.MoveWindow(0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	return 0;
}

LRESULT CMediaInfoDlg::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDROP hDropInfo = (HDROP)wParam;

	int iFileNumber;
	TCHAR szFileName[MAX_PATH*2];
	iFileNumber = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	if(iFileNumber > 0) 
	{
		DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH*2);
		GetMediaInfo(szFileName);
	}


	return 0;
}
