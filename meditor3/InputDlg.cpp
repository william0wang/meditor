// InputDlg.cpp : implementation of the CInputDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include <vector>
using namespace std;

#include "AtlStdioFile.h"
#include "InputDlg.h"
#include "shared.h"

CInputDlg::CInputDlg(UINT IDD_DLG)
{
	IDD = IDD_DLG;

	m_wndListCtrl.RegisterClass();
	
	m_skey.Add(_T("#NoneInput"),rStr.INPUT_NONE);
	m_skey.Add(_T("a"),_T("a"));
	m_skey.Add(_T("b"),_T("b"));
	m_skey.Add(_T("c"),_T("c"));
	m_skey.Add(_T("d"),_T("d"));
	m_skey.Add(_T("e"),_T("e"));
	m_skey.Add(_T("f"),_T("f"));
	m_skey.Add(_T("g"),_T("g"));
	m_skey.Add(_T("h"),_T("h"));
	m_skey.Add(_T("i"),_T("i"));
	m_skey.Add(_T("j"),_T("j"));
	m_skey.Add(_T("k"),_T("k"));
	m_skey.Add(_T("l"),_T("l"));
	m_skey.Add(_T("m"),_T("m"));
	m_skey.Add(_T("n"),_T("n"));
	m_skey.Add(_T("o"),_T("o"));
	m_skey.Add(_T("p"),_T("p"));
	m_skey.Add(_T("q"),_T("q"));
	m_skey.Add(_T("r"),_T("r"));
	m_skey.Add(_T("s"),_T("s"));
	m_skey.Add(_T("t"),_T("t"));
	m_skey.Add(_T("u"),_T("u"));
	m_skey.Add(_T("v"),_T("v"));
	m_skey.Add(_T("w"),_T("w"));
	m_skey.Add(_T("x"),_T("x"));
	m_skey.Add(_T("y"),_T("y"));
	m_skey.Add(_T("z"),_T("z"));
	m_skey.Add(_T("0"),_T("0"));
	m_skey.Add(_T("1"),_T("1"));
	m_skey.Add(_T("2"),_T("2"));
	m_skey.Add(_T("3"),_T("3"));
	m_skey.Add(_T("4"),_T("4"));
	m_skey.Add(_T("5"),_T("5"));
	m_skey.Add(_T("6"),_T("6"));
	m_skey.Add(_T("7"),_T("7"));
	m_skey.Add(_T("8"),_T("8"));
	m_skey.Add(_T("9"),_T("9"));
	m_skey.Add(_T("A"),_T("A"));
	m_skey.Add(_T("B"),_T("B"));
	m_skey.Add(_T("C"),_T("C"));
	m_skey.Add(_T("D"),_T("D"));
	m_skey.Add(_T("E"),_T("E"));
	m_skey.Add(_T("F"),_T("F"));
	m_skey.Add(_T("G"),_T("G"));
	m_skey.Add(_T("H"),_T("H"));
	m_skey.Add(_T("I"),_T("I"));
	m_skey.Add(_T("J"),_T("J"));
	m_skey.Add(_T("K"),_T("K"));
	m_skey.Add(_T("L"),_T("L"));
	m_skey.Add(_T("M"),_T("M"));
	m_skey.Add(_T("N"),_T("N"));
	m_skey.Add(_T("O"),_T("O"));
	m_skey.Add(_T("P"),_T("P"));
	m_skey.Add(_T("Q"),_T("Q"));
	m_skey.Add(_T("R"),_T("R"));
	m_skey.Add(_T("S"),_T("S"));
	m_skey.Add(_T("T"),_T("T"));
	m_skey.Add(_T("U"),_T("U"));
	m_skey.Add(_T("V"),_T("V"));
	m_skey.Add(_T("W"),_T("W"));
	m_skey.Add(_T("X"),_T("X"));
	m_skey.Add(_T("Y"),_T("Y"));
	m_skey.Add(_T("Z"),_T("Z"));
	m_skey.Add(_T("LEFT"),_T("<- ") + rStr.INPUT_LEFT);
	m_skey.Add(_T("RIGHT"),_T("-> ") + rStr.INPUT_RIGHT);
	m_skey.Add(_T("UP"),_T("Up ") + rStr.INPUT_UP);
	m_skey.Add(_T("DOWN"),_T("Down ") + rStr.INPUT_DOWN);
	m_skey.Add(_T("PGUP"),_T("Page Up"));
	m_skey.Add(_T("PGDWN"),_T("Page Down"));
	m_skey.Add(_T("HOME"),_T("Home"));
	m_skey.Add(_T("END"),_T("End"));
	m_skey.Add(_T("INS"),_T("Insert ") + rStr.INPUT_INS);
	m_skey.Add(_T("DEL"),_T("Delete ") + rStr.INPUT_DEL);
	m_skey.Add(_T("SPACE"),_T("Space ") + rStr.INPUT_SPACE);
	m_skey.Add(_T("ENTER"),_T("Enter ") + rStr.INPUT_ENTER);
	m_skey.Add(_T("TAB"),_T("Tab"));
	m_skey.Add(_T("ESC"),_T("ESC"));
	m_skey.Add(_T("<"),_T("<"));
	m_skey.Add(_T(">"),_T(">"));
	m_skey.Add(_T("["),_T("["));
	m_skey.Add(_T("]"),_T("]"));
	m_skey.Add(_T("{"),_T("{"));
	m_skey.Add(_T("}"),_T("}"));
	m_skey.Add(_T("\\"),_T("\\"));
	m_skey.Add(_T(";"),_T(";"));
	m_skey.Add(_T("'"),_T("'"));
	m_skey.Add(_T(","),_T(","));
	m_skey.Add(_T("."),_T("."));
	m_skey.Add(_T("|"),_T("|"));
	m_skey.Add(_T(":"),_T(":"));
	m_skey.Add(_T("\""),_T("\""));
	m_skey.Add(_T("="),_T("="));
	m_skey.Add(_T("-"),_T("-"));
	m_skey.Add(_T("+"),_T("+"));
	m_skey.Add(_T("/"),_T("/"));
	m_skey.Add(_T("*"),_T("*"));
	m_skey.Add(_T("!"),_T("!"));
	m_skey.Add(_T("@"),_T("@"));
	m_skey.Add(_T("$"),_T("$"));
	m_skey.Add(_T("%"),_T("%"));
	m_skey.Add(_T("^"),_T("^"));
	m_skey.Add(_T("`"),_T("`"));
	m_skey.Add(_T("~"),_T("~"));
	m_skey.Add(_T("&"),_T("&"));
	m_skey.Add(_T("("),_T("("));
	m_skey.Add(_T(")"),_T(")"));
	m_skey.Add(_T("F1"),_T("F1"));
	m_skey.Add(_T("F2"),_T("F2"));
	m_skey.Add(_T("F3"),_T("F3"));
	m_skey.Add(_T("F4"),_T("F4"));
	m_skey.Add(_T("F5"),_T("F5"));
	m_skey.Add(_T("F6"),_T("F6"));
	m_skey.Add(_T("F7"),_T("F7"));
	m_skey.Add(_T("F8"),_T("F8"));
	m_skey.Add(_T("F9"),_T("F9"));
	m_skey.Add(_T("F10"),_T("F10"));
	m_skey.Add(_T("F11"),_T("F11"));
	m_skey.Add(_T("F12"),_T("F12"));

	m_skey.Add(_T("MOUSE_BTN0"),rStr.INPUT_LC);
	m_skey.Add(_T("MOUSE_BTN0_DBL"),rStr.INPUT_LD);
	m_skey.Add(_T("MOUSE_BTN2"),rStr.INPUT_RC);
	m_skey.Add(_T("MOUSE_BTN2_DBL"),rStr.INPUT_RD);
	m_skey.Add(_T("MOUSE_BTN1"),rStr.INPUT_MC);
	m_skey.Add(_T("MOUSE_BTN1_DBL"),rStr.INPUT_MD);
	m_skey.Add(_T("MOUSE_BTN3"),rStr.INPUT_WF);
	m_skey.Add(_T("MOUSE_BTN4"),rStr.INPUT_WB);
	m_skey.Add(_T("MOUSE_BTN5"),rStr.INPUT_X1C);
	m_skey.Add(_T("MOUSE_BTN5_DBL"),rStr.INPUT_X1D);
	m_skey.Add(_T("MOUSE_BTN6"),rStr.INPUT_X2C);
	m_skey.Add(_T("MOUSE_BTN6_DBL"),rStr.INPUT_X2D);
	m_skey.Add(_T("MOUSE_BTN8"),rStr.INPUT_RF);
	m_skey.Add(_T("MOUSE_BTN9"),rStr.INPUT_RB);

	m_skey.Add(_T("JOY_LEFT"),rStr.INPUT_JL);
	m_skey.Add(_T("JOY_RIGHT"),rStr.INPUT_JR);
	m_skey.Add(_T("JOY_UP"),rStr.INPUT_JU);
	m_skey.Add(_T("JOY_DOWN"),rStr.INPUT_JD);
	m_skey.Add(_T("JOY_BTN0"),rStr.INPUT_J1);
	m_skey.Add(_T("JOY_BTN1"),rStr.INPUT_J2);
	m_skey.Add(_T("JOY_BTN2"),rStr.INPUT_J3);
	m_skey.Add(_T("JOY_BTN3"),rStr.INPUT_J4);
		
	m_str_list.push_back(rStr.INPUT_INNER);
	m_str_list.push_back(rStr.INPUT_ADD + rStr.INPUT_SUBSZ);
	m_str_list.push_back(rStr.INPUT_ADD + _T(" 0.1"));
	m_str_list.push_back(_T("c / C"));

	m_str_list.push_back(rStr.INPUT_INNER);
	m_str_list.push_back(rStr.INPUT_DEC + rStr.INPUT_SUBSZ);
	m_str_list.push_back(rStr.INPUT_DEC + _T(" 0.1"));
	m_str_list.push_back(_T("v / V"));

	m_str_list.push_back(rStr.INPUT_INNER);
	m_str_list.push_back(rStr.INPUT_LOOP);
	m_str_list.push_back(rStr.INPUT_LOOPIF);
	m_str_list.push_back(_T("b / B"));

	m_str_list.push_back(rStr.INPUT_INNER);
	m_str_list.push_back(rStr.INPUT_WINRESIZE);
	m_str_list.push_back(_T("+-25%, 100%"));
	m_str_list.push_back(_T("alt + 1~3"));

	m_str_list.push_back(rStr.INPUT_INNER);
	m_str_list.push_back(rStr.INPUT_RESIZE);
	m_str_list.push_back(rStr.INPUT_RESIZE);
	m_str_list.push_back(rStr.INPUT_RIGHT + _T(" Ctrl + 1~9"));

	m_str_list.push_back(rStr.INPUT_INNER);
	m_str_list.push_back(rStr.INPUT_POS);
	m_str_list.push_back(rStr.INPUT_POSIF);
	m_str_list.push_back(rStr.INPUT_LEFT + _T(" Ctrl + 1~9"));

	m_str_list.push_back(rStr.INPUT_INNER);
	m_str_list.push_back(rStr.INPUT_FS);
	m_str_list.push_back(_T(""));
	m_str_list.push_back(_T("Alt + ")+ rStr.INPUT_ENTER);

	m_str_list.push_back(rStr.INPUT_INNER);
	m_str_list.push_back(rStr.INPUT_SYS);
	m_str_list.push_back(_T(""));
	m_str_list.push_back(_T("Alt + H/S"));

	m_str_list.push_back(rStr.INPUT_INNER);
	m_str_list.push_back(rStr.INPUT_DVDDIR);
	m_str_list.push_back(_T(""));
	m_str_list.push_back(_T("Ctrl + ")+ rStr.INPUT_DVDDIR2);

	m_str_list.push_back(rStr.INPUT_INNER);
	m_str_list.push_back(rStr.INPUT_DVDSEL);
	m_str_list.push_back(_T(""));
	m_str_list.push_back(_T("Ctrl + ")+ rStr.INPUT_ENTER);

	m_str_list.push_back(rStr.INPUT_INNER);
	m_str_list.push_back(rStr.INPUT_DVDROOT);
	m_str_list.push_back(_T(""));
	m_str_list.push_back(_T("Ctrl + Home"));

	m_str_list.push_back(rStr.INPUT_INNER);
	m_str_list.push_back(rStr.INPUT_DVDPRE);
	m_str_list.push_back(_T(""));
	m_str_list.push_back(_T("Ctrl + End"));

	m_str_list.push_back(rStr.INPUT_INNER);
	m_str_list.push_back(rStr.INPUT_FSR);
	m_str_list.push_back(rStr.INPUT_FSRIF);
	m_str_list.push_back(rStr.INPUT_RC);
	
	m_str_input.push_back(rStr.INPUT_SEEK);
	m_str_input.push_back(rStr.INPUT_SUBT);
	m_str_input.push_back(rStr.INPUT_PTUS);
	m_str_input.push_back(rStr.INPUT_PTS);
	m_str_input.push_back(rStr.INPUT_ALTS);
	m_str_input.push_back(rStr.INPUT_PANS);
	m_str_input.push_back(rStr.INPUT_PAUS);
	m_str_input.push_back(rStr.INPUT_SCRS);
	m_str_input.push_back(rStr.INPUT_KEEP);
	m_str_input.push_back(rStr.INPUT_FRAD);
	m_str_input.push_back(rStr.INPUT_OSD);
	m_str_input.push_back(rStr.INPUT_QUIT);
	m_str_input.push_back(rStr.INPUT_MUTE);

	m_str_input.push_back(rStr.INPUT_SWIA);
	m_str_input.push_back(rStr.INPUT_SWIF);
	m_str_input.push_back(rStr.INPUT_CHAP);
	m_str_input.push_back(rStr.INPUT_TITLE);
	m_str_input.push_back(rStr.INPUT_ANGLE);

	m_str_input.push_back(rStr.INPUT_SUBS);
	m_str_input.push_back(rStr.INPUT_SUBA);
	m_str_input.push_back(rStr.INPUT_SWIV);
	m_str_input.push_back(rStr.INPUT_VOFS);
	m_str_input.push_back(rStr.INPUT_VOOT);
	m_str_input.push_back(rStr.INPUT_RELO);
	m_str_input.push_back(rStr.INPUT_FRAS);
	m_str_input.push_back(rStr.INPUT_SPES);
	m_str_input.push_back(rStr.INPUT_SPEI);
	m_str_input.push_back(rStr.INPUT_BALA);

	m_str_input.push_back(rStr.INPUT_VOLU);
	m_str_input.push_back(rStr.INPUT_SUBP);
	m_str_input.push_back(rStr.INPUT_CONT);
	m_str_input.push_back(rStr.INPUT_BRIG);
	m_str_input.push_back(rStr.INPUT_SATU);
	m_str_input.push_back(rStr.INPUT_GAMM);
	m_str_input.push_back(rStr.INPUT_HUE);

	m_str_input.push_back(rStr.INPUT_AUDD);
	m_str_input.push_back(rStr.INPUT_SUBD);

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
}

BOOL CInputDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CInputDlg::OnIdle()
{
	return FALSE;
}


LRESULT CInputDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	UIAddChildWindowContainer(m_hWnd);

	m_wndListCtrl.SubclassWindow( GetDlgItem( IDC_LISTCTRL ) );

	::SetWindowText(GetDlgItem(IDC_BUTTON_ADD), rStr.add);
	::SetWindowText(GetDlgItem(IDC_BUTTON_DEL), rStr.del);

	LoadInputConfig();

	InitBasicList();
	
	DoDataExchange();
	
	return TRUE;
}

LRESULT CInputDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	return 0;
}

void CInputDlg::InitBasicList()
{
	m_wndListCtrl.SetFocusSubItem( TRUE );
	m_wndListCtrl.ShowHeaderSort(FALSE);
	m_wndListCtrl.SetSingleSelect(TRUE);

	m_wndListCtrl.AddColumn(_T(""), 35, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_NONE);
	m_wndListCtrl.AddColumn(rStr.INPUT_CMD, 100, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_NONE, ITEM_FLAGS_CENTRE);
	m_wndListCtrl.AddColumn(rStr.DECODE_INF, 160, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_NONE, ITEM_FLAGS_CENTRE);
	m_wndListCtrl.AddColumn(rStr.INPUT_VAL, 120, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_NONE, ITEM_FLAGS_CENTRE);
	m_wndListCtrl.AddColumn(rStr.INPUT_KEY, 120, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_COMBO, ITEM_FLAGS_CENTRE);

	COLORREF color_font = RGB( 250, 250, 250 );
	m_readonly_number = m_str_list.size()/4;
	for(int index = 0; index < m_readonly_number; index++) {
		COLORREF color0 = RGB(130,150,180);
		COLORREF color = index%2 ? RGB(220,200,195): RGB(200,195,220);
		CString sIndex;
		sIndex.Format(_T("%03d"), index+1);

		int nNewItem = m_wndListCtrl.AddItem( sIndex );
		m_wndListCtrl.SetItemText( nNewItem, 1, m_str_list[index*4] );
		m_wndListCtrl.SetItemText( nNewItem, 2, m_str_list[index*4 + 1] );
		m_wndListCtrl.SetItemText( nNewItem, 3, m_str_list[index*4 + 2] );
		m_wndListCtrl.SetItemText( nNewItem, 4, m_str_list[index*4 + 3] );
		m_wndListCtrl.SetItemColours( nNewItem, 0, color, color_font);
		m_wndListCtrl.SetItemColours( nNewItem, 1, color0, color_font);
		m_wndListCtrl.SetItemColours( nNewItem, 2, color, color_font);
		m_wndListCtrl.SetItemColours( nNewItem, 3, color, color_font);
		m_wndListCtrl.SetItemColours( nNewItem, 4, color, color_font);
	}
	
	m_def_number = m_inputs.GetSize();

	CString key, command, vals, abs, info;

	for (int i = 0; i < m_def_number; i++) {
		m_inputs.GetAt(i, key, command, vals, abs);
		int keynum = m_skey.GetKeyInfo(key);
		GetCmdInfo(command, info);
		UINT Style = ITEM_FLAGS_EDIT_FLOAT|ITEM_FLAGS_RIGHT;
		if(abs.GetLength() > 0) {
			vals = _T("=") + vals;
			Style = ITEM_FLAGS_RIGHT;
		}
		CString sIndex;
		sIndex.Format(_T("%03d"), i+m_readonly_number+1);

		int nNewItem = m_wndListCtrl.AddItem( sIndex );

		m_wndListCtrl.SetItemText( nNewItem, 1, command );
		m_wndListCtrl.SetItemText( nNewItem, 2, info );
		m_wndListCtrl.SetItemText( nNewItem, 3, vals );
		m_wndListCtrl.SetItemText( nNewItem, 4, m_skey.m_keyInfo[keynum]);
		m_wndListCtrl.SetItemFormat( nNewItem, 3, ITEM_FORMAT_EDIT, Style);
		m_wndListCtrl.SetItemFormat( nNewItem, 4, ITEM_FORMAT_COMBO, ITEM_FLAGS_NONE, m_skey.m_keyInfo);

		COLORREF color;
		if(i % 2 == 0)
			color = RGB(255,255,240);
		else
			color = RGB(220,255,240);

		m_wndListCtrl.SetItemColours( nNewItem, 0, color, RGB( 0, 0, 0 ) );
		m_wndListCtrl.SetItemColours( nNewItem, 1, color, RGB( 0, 0, 0 ) );
		m_wndListCtrl.SetItemColours( nNewItem, 2, color, RGB( 0, 0, 0 ) );
		m_wndListCtrl.SetItemColours( nNewItem, 3, color, RGB( 0, 0, 0 ) );
		m_wndListCtrl.SetItemColours( nNewItem, 4, color, RGB( 0, 0, 0 ) );

	}

	m_def_number += m_readonly_number;		
}

bool CInputDlg::AnalyseLine(CString line, CString &key, CString &cmd, CString &val, CString &abs)
{
	line += _T(" ");
	line.TrimLeft(_T(" "));
	int sp = -1;
	if(line.Find(_T("#")) == 0)
	{
		sp = line.Find(_T("#NoneInput"));
		if(sp < 0)
			return false;
		key = line.Left(sp);
		line.Delete(0,sp);
	}
	sp = line.Find(_T(" "));
	if(sp <= 0)
		return false;
	key = line.Left(sp);
	line.Delete(0,sp);

	line.TrimLeft(_T(" "));
	sp = line.Find(_T(" "));
	if(sp <= 0)
		return false;
	cmd = line.Left(sp);
	line.Delete(0,sp);

	line.TrimLeft(_T(" "));
	sp = line.Find(_T(" "));
	if(sp <= 0)
		val = _T("");
	else
		val = line.Left(sp);
	line.Delete(0,sp);

	line.TrimLeft(_T(" "));
	sp = line.Find(_T(" "));
	if(sp <= 0)
		abs = _T("");
	else
	{
		abs = line.Left(sp);
		if(_ttoi(abs) == 0)
			abs = _T("");
	}
	return true;
}

void CInputDlg::LoadInputConfig()
{
	CAtlStdioFile inputcfg;
	CString inifile = m_program_dir + _T("input.ini");

	if(!FileExist(inifile)) {
		if(MessageBox(rStr.INPUT_SETNONE, rStr.INPUT_SET, MB_YESNO|MB_TOPMOST) != IDYES)
			return;
		ExtractResource(MAKEINTRESOURCE(IDZ_INPUT),TEXT("INPUT_INI")
			,m_program_dir + _T("input.ini"), true, true, _T("input.ini"));
	}
	if(SUCCEEDED(inputcfg.OpenFile(inifile.GetBuffer(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING))) {
		CString line, command, value, key, abs;
		CStringA lineA;
		while(inputcfg.ReadLineA(lineA)) {
			line = local2unicode(lineA.GetBuffer()).c_str();
			lineA.ReleaseBuffer();
			if(AnalyseLine(line, key, command, value, abs))
				m_inputs.Add(key, command, value, abs);
		}
		inputcfg.Close();
	}
	inifile.ReleaseBuffer();
}

BOOL CInputDlg::CheckReuse()
{
	CString keyinfo;
	CListArray<CString> m_keyInfoList;
	for(int i = m_readonly_number; i < m_wndListCtrl.GetItemCount(); i++)
	{
		CString keyinfo = m_wndListCtrl.GetItemText(i,4);
		if(m_keyInfoList.Find(keyinfo) >= 0 && keyinfo != rStr.INPUT_NONE)
			return i;
		m_keyInfoList.Add(keyinfo);
	}
	return -1;
}

bool CInputDlg::SaveInputConfig()
{
	int resue = CheckReuse();
	if(resue >= 0) {
		CString msg;
		msg.Format(_T("%s\n\n%03d: %s  %s  %s"), rStr.INPUT_REUS, resue+1, m_wndListCtrl.GetItemText(resue, 1)
			, m_wndListCtrl.GetItemText(resue, 3), m_wndListCtrl.GetItemText(resue, 4));
		if(MessageBox(msg, rStr.INPUT_CONF, MB_YESNO) != IDYES)
			return false;
	}
	CAtlStdioFile inputcfg;
	CString key, cmd, vals, keyinfo;
	CString inifile = m_program_dir + _T("input.ini");
	if(SUCCEEDED(inputcfg.OpenFile(inifile.GetBuffer(), GENERIC_WRITE, 0, CREATE_ALWAYS))) {
		inputcfg.WriteLineA("## MPlayer input control file\r\n");
		int number = m_wndListCtrl.GetItemCount();

		for(int i = m_readonly_number; i < number; i++)	{
			cmd = m_wndListCtrl.GetItemText(i, 1);
			vals = m_wndListCtrl.GetItemText(i, 3);
			keyinfo = m_wndListCtrl.GetItemText(i, 4);
			int offset = vals.Find(_T("="));
			if(offset >= 0)	{
				vals.Delete(0,offset + 1);
				vals += _T(" 1");
			}
			if(m_skey.GetKeyName(keyinfo,key))	{
				if(i >= m_def_number) {
					cmd.TrimLeft(_T(" "));
					cmd.TrimRight(_T(" "));
					if(cmd.GetLength() < 2)
						continue;
				}
				keyinfo = key + _T(" ") + cmd;
				if(vals.GetLength() > 0)
					keyinfo += _T(" ") + vals;
				inputcfg.WriteLineA(unicode2local(keyinfo.GetBuffer()).c_str());
				keyinfo.ReleaseBuffer();
			}
		}
		inputcfg.Close();
	}

	inifile.ReleaseBuffer();
	return true;
}

void CInputDlg::GetCmdInfo(CString cmd ,CString &info)
{
	info = _T("");
	if(cmd == _T("seek"))
		info = m_str_input[0];
	else if(cmd == _T("sub_step"))
		info = m_str_input[1];
	else if(cmd == _T("pt_up_step"))
		info = m_str_input[2];
	else if(cmd == _T("pt_step"))
		info = m_str_input[3];
	else if(cmd == _T("alt_src_step"))
		info = m_str_input[4];
	else if(cmd == _T("panscan"))
		info = m_str_input[5];
	else if(cmd == _T("pause"))
		info = m_str_input[6];
	else if(cmd == _T("screenshot"))
		info = m_str_input[7];
	else if(cmd == _T("keep_aspect"))
		info = m_str_input[8];
	else if(cmd == _T("frame_drop"))
		info = m_str_input[9];
	else if(cmd == _T("osd"))
		info = m_str_input[10];
	else if(cmd == _T("quit"))
		info = m_str_input[11];
	else if(cmd == _T("mute"))
		info = m_str_input[12];

	else if(cmd == _T("switch_audio"))
		info = m_str_input[13];
	else if(cmd == _T("switch_font"))
		info = m_str_input[14];
	else if(cmd == _T("seek_chapter"))
		info = m_str_input[15];
	else if(cmd == _T("switch_title"))
		info = m_str_input[16];
	else if(cmd == _T("switch_angle"))
		info = m_str_input[17];

	else if(cmd == _T("sub_select"))
		info = m_str_input[18];
	else if(cmd == _T("sub_alignment"))
		info = m_str_input[19];
	else if(cmd == _T("switch_view"))
		info = m_str_input[20];
	else if(cmd == _T("vo_fullscreen"))
		info = m_str_input[21];
	else if(cmd == _T("vo_ontop"))
		info = m_str_input[22];
	else if(cmd == _T("reload"))
		info = m_str_input[23];
	else if(cmd == _T("frame_step"))
		info = m_str_input[24];
	else if(cmd == _T("speed_set"))
		info = m_str_input[25];
	else if(cmd == _T("speed_incr"))
		info = m_str_input[26];
	else if(cmd == _T("balance"))
		info = m_str_input[27];

	else if(cmd == _T("volume"))
		info = m_str_input[28];
	else if(cmd == _T("sub_pos"))
		info = m_str_input[29];
	else if(cmd == _T("contrast"))
		info = m_str_input[30];
	else if(cmd == _T("brightness"))
		info = m_str_input[31];
	else if(cmd == _T("saturation"))
		info = m_str_input[32];
	else if(cmd == _T("gamma"))
		info = m_str_input[33];
	else if(cmd == _T("hue"))
		info = m_str_input[34];

	else if(cmd == _T("audio_delay"))
		info = m_str_input[35];
	else if(cmd == _T("sub_delay"))
		info = m_str_input[36];

}


LRESULT CInputDlg::OnBnClickedAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

	CString str;
	int nNewItem = m_wndListCtrl.AddItem(_T(""));

	str.Format(_T("%03d"), nNewItem+1);
	m_wndListCtrl.SetItemText( nNewItem, 0, str );
	m_wndListCtrl.SetItemText( nNewItem, 2, rStr.INPUT_CUST );
	m_wndListCtrl.SetItemText( nNewItem, 3, _T("") );
	m_wndListCtrl.SetItemText( nNewItem, 4, m_skey.m_keyInfo[0]);
	m_wndListCtrl.SetItemFormat( nNewItem, 1, ITEM_FORMAT_EDIT, ITEM_FLAGS_CENTRE);
	m_wndListCtrl.SetItemFormat( nNewItem, 2, ITEM_FORMAT_EDIT, ITEM_FLAGS_CENTRE);
	m_wndListCtrl.SetItemFormat( nNewItem, 3, ITEM_FORMAT_EDIT, ITEM_FLAGS_RIGHT);
	m_wndListCtrl.SetItemFormat( nNewItem, 4, ITEM_FORMAT_COMBO, ITEM_FLAGS_NONE, m_skey.m_keyInfo);
	
	return 0;
}

LRESULT CInputDlg::OnBnClickedDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int iRefresh = 0xffffff;
	CListArray < int > aSelectedItems;
	if(m_wndListCtrl.GetSelectedItems(aSelectedItems)) {
		for(int i = aSelectedItems.GetSize() - 1; i >= 0; i--) {
			int nItem = aSelectedItems[i];
			if(nItem >= m_def_number) {
				m_wndListCtrl.DeleteItem(nItem);
				if(nItem < iRefresh)
					iRefresh = nItem;
			} else if(nItem >= m_readonly_number)	{
				CString msg;
				msg.Format(_T("%s %03d: %s  %s £¿") , rStr.INPUT_CDEL, nItem+1
					, m_wndListCtrl.GetItemText(nItem, 4),  m_wndListCtrl.GetItemText(nItem, 1));
				if( MessageBox(msg,ResStr(IDS_INPUT_CDEL),MB_YESNO) == IDYES)	{
					m_wndListCtrl.DeleteItem(nItem);
					m_def_number--;
					if(nItem < iRefresh)
						iRefresh = nItem;
				}
			}
		}
	}

	if(iRefresh < m_wndListCtrl.GetItemCount()) {
		CString str;
		for(int i = iRefresh; i < m_wndListCtrl.GetItemCount(); i++){
			str.Format(_T("%03d"), i+1);
			m_wndListCtrl.SetItemText(i, 0, str);
		}
	}

	return 0;
}