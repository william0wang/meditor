// GuiDlg.cpp : implementation of the CGuiDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "shared.h"
#include "Reg.h"
#include "GuiDlg.h"

CGuiDlg::CGuiDlg(UINT IDD_DLG)
{
	IDD = IDD_DLG;


}

BOOL CGuiDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CGuiDlg::OnIdle()
{
	return FALSE;
}

void CGuiDlg::InitCheckBox()
{
	if(mconfig.GetBoolean(_T("idle")))
		m_show = TRUE;
	else
		m_show = FALSE;

	if(mconfig.GetBoolean(_T("skinned_player"), true, true))
		m_bskin = TRUE;
	else
		m_bskin = FALSE;

	if(mconfig.GetBoolean(_T("skin_controlbar"), true, true))
		m_ctlskin = TRUE;
	else
		m_ctlskin = FALSE;

	if(mconfig.GetBoolean(_T("auto_hide_control"), true, true))
		m_alpha_control = TRUE;
	else
		m_alpha_control = FALSE;
	
	if(mconfig.GetBoolean(_T("show_menubar"), false, true))
		m_menu = TRUE;
	else
		m_menu = FALSE;

	if(mconfig.GetBoolean(_T("use-filename-title"), true))
		m_filename = TRUE;
	else
		m_filename = FALSE;

	if(mconfig.GetBoolean(_T("use_rightmenu"), false, true))
		m_rightmenu = TRUE;
	else
		m_rightmenu = FALSE;
}

void CGuiDlg::SaveConfig()
{
	DoDataExchange(TRUE);
	CString value_s;
	int value_i;

	if(m_show)
		mconfig.SetValue(_T("idle"),_T("1"));
	else
		mconfig.RemoveValue(_T("idle"));

	if(m_bskin)
		mconfig.RemoveValue(_T("skinned_player"), true);
	else
		mconfig.SetValue(_T("skinned_player"), _T("0"), true, ex_gui);

	if(m_ctlskin)
		mconfig.RemoveValue(_T("skin_controlbar"),true);
	else
		mconfig.SetValue(_T("skin_controlbar"), _T("0"), true, ex_gui);

	if(m_alpha_control)
		mconfig.RemoveValue(_T("auto_hide_control"), true);
	else
		mconfig.SetValue(_T("auto_hide_control"), _T("0"), true, ex_gui);
	
	if(m_menu)
		mconfig.SetValue(_T("show_menubar"), _T("1"), true, ex_gui);
	else
		mconfig.RemoveValue(_T("show_menubar"), true);

	if(m_filename)
		mconfig.RemoveValue(_T("use-filename-title"));
	else
		mconfig.SetValue(_T("use-filename-title"), _T("0"));

	if(m_rightmenu)
		mconfig.SetValue(_T("use_rightmenu"), _T("1"), true, ex_gui);
	else
		mconfig.RemoveValue(_T("use_rightmenu"), true);
	
	//Skin
	value_s = aComboSkin.GetSelectedString();
	value_i = aComboSkin.GetSelectedIndex();
	if(value_i > 1) {
		mconfig.RemoveValue(_T("show_controlbar"), true);
		mconfig.SetValue(_T("skin"), value_s, true, ex_gui);
	} else if(value_i == 1) {
		mconfig.RemoveValue(_T("show_controlbar") , true);
		mconfig.SetValue(_T("skin_controlbar"), _T("0"), true, ex_gui);
		mconfig.SetValue(_T("skinned_player"), _T("0"), true, ex_gui);
		mconfig.SetValue(_T("skin"), _T("inner"), true, ex_gui);
	} else {
		mconfig.SetValue(_T("show_controlbar"), _T("0"), true, ex_gui);
	}

	//Alpha
	value_i = aComboAlpha.GetSelectedIndex();
	if(value_i > 0) {
		value_s.Format(_T("%d"), value_i);
		mconfig.SetValue(_T("controlbar_alpha"), value_s, true, ex_gui);
	} else
		mconfig.RemoveValue(_T("controlbar_alpha"), true);

	//Tray
	switch (aComboTray.GetSelectedIndex())
	{
	case 1:
		mconfig.SetValue(_T("systray"), _T("1"));
		break;
	case 2:
		mconfig.SetValue(_T("systray"), _T("2"));
		break;
	default:
		mconfig.RemoveValue(_T("systray"));
	}

	//Hide
	switch (aComboHide.GetSelectedIndex())
	{
	case 1:
		mconfig.SetValue(_T("switchview"), _T("1"));
		break;
	case 2:
		mconfig.SetValue(_T("switchview"), _T("2"));
		break;
	default:
		mconfig.RemoveValue(_T("switchview"));
	}

	//Top
	switch (aComboTop.GetSelectedIndex())
	{
	case 1:
		mconfig.SetValue(_T("ontop"), _T("1"));
		break;
	case 2:
		mconfig.SetValue(_T("ontop"), _T("2"));
		break;
	default:
		mconfig.RemoveValue(_T("ontop"));
	}

	//Lang
	value_i = aComboLang.GetSelectedIndex();
	if(value_i < 0 || value_i > 4)
		value_i = 0;
	value_s.Format(_T("%d"), value_i);
	mconfig.SetValue(_T("Language"), value_s, true, ex_option);

	//ColorKey
	value_s = aComboColorkey.GetSelectedString();
	if(value_s == rStr.nos) {
		mconfig.SetValue(_T("nocolorkey"),_T("1"));
		mconfig.RemoveValue(_T("colorkey"));
	} else {
		mconfig.RemoveValue(_T("nocolorkey"));
		mconfig.SetValue(_T("colorkey"), value_s);
	}
}

LRESULT CGuiDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	UIAddChildWindowContainer(m_hWnd);

	int width = 120;
	CString values;

	InitCheckBox();

	aComboLang.SetInfoWidth(width);
	aComboSkin.SetInfoWidth(width);
	aComboAlpha.SetInfoWidth(width);
	aComboTop.SetInfoWidth(width);
	aComboHide.SetInfoWidth(width);
	aComboTray.SetInfoWidth(width);
	aComboColorkey.SetInfoWidth(width);

	aComboLang.InitCtrl(GetDlgItem(IDC_LISTCTRL_LANG));
	aComboSkin.InitCtrl(GetDlgItem(IDC_LISTCTRL_SKIN));
	aComboAlpha.InitCtrl(GetDlgItem(IDC_LISTCTRL_ALPHA));
	aComboTop.InitCtrl(GetDlgItem(IDC_LISTCTRL_TOP));
	aComboHide.InitCtrl(GetDlgItem(IDC_LISTCTRL_HIDE));
	aComboTray.InitCtrl(GetDlgItem(IDC_LISTCTRL_TRAY));
	aComboColorkey.InitCtrl(GetDlgItem(IDC_LISTCTRL_COLORKEY));

	//Lang
	aComboLang.SetTitle(rStr.lang);
	aComboLang.AddString(rStr.autos);
	aComboLang.AddString(_T("简体中文"));
	aComboLang.AddString(_T("English"));
	aComboLang.AddString(_T("繁體中文"));
	aComboLang.AddString(_T("繁體中文 BIG-5"));
	aComboLang.SelecteIndex(mconfig.GetInteger(_T("Language"), 0, true));

	//Skin
	aComboSkin.SetTitle(rStr.skin);
	aComboSkin.AddString(rStr.nos);
	aComboSkin.AddString(rStr.systheme);

	CString m_skin_dir;
	TCHAR szCurPath[MAX_PATH + 1];
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szCurPath);
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_skin_dir.Format(_T("%sskin"),szFilePath);
	::SetCurrentDirectory(m_skin_dir);

	CFindFile finder;
	if(finder.FindFile(_T("*.*"))) {
		while(finder.FindNextFile()) {
			if(finder.IsDirectory() && !finder.IsDots())
				aComboSkin.AddString(finder.GetFileName().MakeLower());
		}
		CString str = finder.GetFileName();
		if(finder.IsDirectory() && !finder.IsDots() && str.GetLength() > 1) {
			aComboSkin.AddString(str);
		}
	}
	::SetCurrentDirectory(szCurPath);

	if(mconfig.GetBoolean(_T("show_controlbar"), true, true)) {
		if(mconfig.GetString(_T("skin"), values, true)) {
			int index = aComboSkin.FindStringExact(0, values);
			if(index < 0) {
				values = _T("default");
				index = aComboSkin.FindStringExact(0, values);
			}
			if(!m_ctlskin && values == _T("inner"))
				aComboSkin.SelecteIndex(1);
			else if(index > 0)
				aComboSkin.SelecteIndex(index);
			else
				aComboSkin.SelecteIndex(0);
		} else {
			int index = aComboSkin.FindStringExact(0, _T("default"));
			if(index > 0)
				aComboSkin.SelecteIndex(index);
			else
				aComboSkin.SelecteIndex(0);
		}
	} else {
		int indexCtrl = aComboSkin.FindStringExact(0, _T("default"));
		if(indexCtrl < 1)
			indexCtrl = 1;
		aComboSkin.SelecteIndex(indexCtrl);
	}

	//Alpha
	aComboAlpha.SetTitle(rStr.ctlalpha);
	aComboAlpha.AddString(rStr.nos);
	CString tmp;
	for (int i = 1; i < 256; i++) {
		tmp.Format(_T("%d"), i);
		aComboAlpha.AddString(tmp);
	}
	int value_i = mconfig.GetInteger(_T("controlbar_alpha"), 0, true);
	if(value_i < 0 || value_i > 255)
		value_i = 0;
	aComboAlpha.SelecteIndex(value_i);

	//OnTop
	aComboTop.SetTitle(rStr.ontop);
	aComboTop.AddString(rStr.ontop_none);
	aComboTop.AddString(rStr.ontop_always);
	aComboTop.AddString(rStr.ontop_playing);
	value_i = mconfig.GetInteger(_T("ontop"));
	if(value_i < 0 || value_i > 1)
		value_i = 2;
	aComboTop.SelecteIndex(value_i);

	//Hide
	aComboHide.SetTitle(rStr.autohide);
	aComboHide.AddString(rStr.hide_no);
	aComboHide.AddString(rStr.hide_m);
	aComboHide.AddString(rStr.hide_a);
	value_i = mconfig.GetInteger(_T("switchview"));
	if(value_i < 0 || value_i > 2)
		value_i = 0;
	aComboHide.SelecteIndex(value_i);

	//Tray
	aComboTray.SetTitle(rStr.mintray);
	aComboTray.AddString(rStr.tray_ta);
	aComboTray.AddString(rStr.tray_tr);
	aComboTray.AddString(rStr.tray_trs);
	value_i = mconfig.GetInteger(_T("systray"));
	if(value_i < 0 || value_i > 2)
		value_i = 0;
	aComboTray.SelecteIndex(value_i);

	//ColorKey
	aComboColorkey.SetTitle(_T("ColorKey:"));
	aComboColorkey.AddString(rStr.nos);
	aComboColorkey.AddString(_T("0x010203"));
	aComboColorkey.SelecteText(_T("0x010203"));
	if(mconfig.GetBoolean(_T("nocolorkey"))) {
		aComboColorkey.SelecteIndex(0);
	} else if(mconfig.GetString(_T("colorkey"), values)) {
		if(values.Compare(_T("0x010203"))) {
			aComboColorkey.AddString(values);
			aComboColorkey.SelecteText(values);
		}
	}

	DoDataExchange();
	
	return TRUE;
}

LRESULT CGuiDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	return 0;
}
