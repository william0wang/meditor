// MIpnutPage.cpp : 实现文件
//

#include "stdafx.h"
#include "MEditor2.h"
#include "MInputPage.h"

// CMInputPage 对话框

IMPLEMENT_DYNAMIC(CMInputPage, CDialog)

CMInputPage::CMInputPage(CWnd* pParent /*=NULL*/)
	: CDialog(CMInputPage::IDD, pParent)
{
	m_str_none = ResStr(IDS_INPUT_NONE);
	m_skey.Add(_T("#NoneInput"),m_str_none);
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
	m_skey.Add(_T("LEFT"),_T("<- ") + ResStr(IDS_INPUT_LEFT));
	m_skey.Add(_T("RIGHT"),_T("-> ") + ResStr(IDS_INPUT_RIGHT));
	m_skey.Add(_T("UP"),_T("Up ") + ResStr(IDS_INPUT_UP));
	m_skey.Add(_T("DOWN"),_T("Down ") + ResStr(IDS_INPUT_DOWN));
	m_skey.Add(_T("PGUP"),_T("Page Up"));
	m_skey.Add(_T("PGDWN"),_T("Page Down"));
	m_skey.Add(_T("HOME"),_T("Home"));
	m_skey.Add(_T("END"),_T("End"));
	m_skey.Add(_T("INS"),_T("Insert ") + ResStr(IDS_INPUT_INS));
	m_skey.Add(_T("DEL"),_T("Delete ") + ResStr(IDS_INPUT_DEL));
	m_skey.Add(_T("SPACE"),_T("Space ") + ResStr(IDS_INPUT_SPACE));
	m_skey.Add(_T("ENTER"),_T("Enter ") + ResStr(IDS_INPUT_ENTER));
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

	m_skey.Add(_T("MOUSE_BTN0"),ResStr(IDS_INPUT_LC));
	m_skey.Add(_T("MOUSE_BTN0_DBL"),ResStr(IDS_INPUT_LD));
	m_skey.Add(_T("MOUSE_BTN2"),ResStr(IDS_INPUT_RC));
	m_skey.Add(_T("MOUSE_BTN2_DBL"),ResStr(IDS_INPUT_RD));
	m_skey.Add(_T("MOUSE_BTN1"),ResStr(IDS_INPUT_MC));
	m_skey.Add(_T("MOUSE_BTN1_DBL"),ResStr(IDS_INPUT_MD));
	m_skey.Add(_T("MOUSE_BTN3"),ResStr(IDS_INPUT_WF));
	m_skey.Add(_T("MOUSE_BTN4"),ResStr(IDS_INPUT_WB));
	m_skey.Add(_T("MOUSE_BTN5"),ResStr(IDS_INPUT_X1C));
	m_skey.Add(_T("MOUSE_BTN5_DBL"),ResStr(IDS_INPUT_X1D));
	m_skey.Add(_T("MOUSE_BTN6"),ResStr(IDS_INPUT_X2C));
	m_skey.Add(_T("MOUSE_BTN6_DBL"),ResStr(IDS_INPUT_X2D));
	m_skey.Add(_T("MOUSE_BTN8"),ResStr(IDS_INPUT_RF));
	m_skey.Add(_T("MOUSE_BTN9"),ResStr(IDS_INPUT_RB));
	
	m_skey.Add(_T("JOY_LEFT"),ResStr(IDS_INPUT_JL));
	m_skey.Add(_T("JOY_RIGHT"),ResStr(IDS_INPUT_JR));
	m_skey.Add(_T("JOY_UP"),ResStr(IDS_INPUT_JU));
	m_skey.Add(_T("JOY_DOWN"),ResStr(IDS_INPUT_JD));
	m_skey.Add(_T("JOY_BTN0"),ResStr(IDS_INPUT_J1));
	m_skey.Add(_T("JOY_BTN1"),ResStr(IDS_INPUT_J2));
	m_skey.Add(_T("JOY_BTN2"),ResStr(IDS_INPUT_J3));
	m_skey.Add(_T("JOY_BTN3"),ResStr(IDS_INPUT_J4));
	m_readonly_number = 0;
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);

}

CMInputPage::~CMInputPage()
{
}

void CMInputPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INPUT, m_List);
}


BEGIN_MESSAGE_MAP(CMInputPage, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMInputPage::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CMInputPage::OnBnClickedButtonDel)
END_MESSAGE_MAP()

// CMInputPage 消息处理程序

int CMInputPage::m_nColWidths[4] = { 15, 25, 10, 15};	// sixty-fourths
// InitListCtrl
void CMInputPage::InitListCtrl(CXListCtrl * pList)
{
	ASSERT(pList);
	if (!pList)
		return;

	// set column width according to window rect
	CRect rect;
	pList->GetWindowRect(&rect);

	int w = rect.Width() - 2;
	CString cmd = ResStr(IDS_INPUT_CMD);
	CString inf = ResStr(IDS_DECODE_INF);
	CString val	 = ResStr(IDS_INPUT_VAL);
	CString key = ResStr(IDS_INPUT_KEY);

	TCHAR *	lpszHeaders[] = {_tcsdup(cmd.GetBuffer()),_tcsdup(inf.GetBuffer())
		, _tcsdup(val.GetBuffer()), _tcsdup(key.GetBuffer()), NULL };
	cmd.ReleaseBuffer();
	val.ReleaseBuffer();
	key.ReleaseBuffer();
	inf.ReleaseBuffer();
	int i;
	int total_cx = 0;
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	w = w - ::GetSystemMetrics(SM_CXVSCROLL);	// width of vertical scroll bar

	// add columns
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = lpszHeaders[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = (lpszHeaders[i+1] == NULL) ? w - total_cx - 2 : (w * m_nColWidths[i]) / 64;
		total_cx += lvcolumn.cx;
		pList->InsertColumn(i, &lvcolumn);
	}

	// iterate through header items and attach the image list
	HDITEM hditem;

	for (i = 0; i < pList->m_HeaderCtrl.GetItemCount(); i++)
	{
		hditem.mask = HDI_IMAGE | HDI_FORMAT;
		pList->m_HeaderCtrl.GetItem(i, &hditem);
		hditem.fmt |=  HDF_IMAGE;
		hditem.iImage = XHEADERCTRL_NO_IMAGE;
		pList->m_HeaderCtrl.SetItem(i, &hditem);
	}

	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// set the format again - must do this twice or first column does not get set
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.iSubItem = i;
		pList->SetColumn(i, &lvcolumn);
	}
}

///////////////////////////////////////////////////////////////////////////////
// FillListCtrl
void CMInputPage::FillListCtrl(CXListCtrl * pList)
{
	// a list of some more of my favorite things

	pList->LockWindowUpdate();	// ***** lock window updates while filling list *****
	pList->DeleteAllItems();

	CString str = _T("");

	int index = 0;
	m_List.InsertItem(index, ResStr(IDS_INPUT_INNER),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1,ResStr(IDS_INPUT_ADD)+ResStr(IDS_INPUT_SUBSZ),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2,ResStr(IDS_INPUT_ADD)+_T(" 0.1"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("c / C"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, ResStr(IDS_INPUT_INNER),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, ResStr(IDS_INPUT_DEC)+ResStr(IDS_INPUT_SUBSZ),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, ResStr(IDS_INPUT_DEC)+_T(" 0.1"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("v / V"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, ResStr(IDS_INPUT_INNER),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, ResStr(IDS_INPUT_LOOP),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, ResStr(IDS_INPUT_LOOPIF),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("b / B"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, ResStr(IDS_INPUT_INNER),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1,ResStr(IDS_INPUT_RESIZE),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2,ResStr(IDS_INPUT_SIZEIF),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3,ResStr(IDS_INPUT_RIGHT) + _T(" Ctrl + 1~9"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, ResStr(IDS_INPUT_INNER),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1,ResStr(IDS_INPUT_POS),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2,ResStr(IDS_INPUT_POSIF),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3,ResStr(IDS_INPUT_LEFT)+ _T(" Ctrl + 1~9"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, ResStr(IDS_INPUT_INNER),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, ResStr(IDS_INPUT_FS),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, _T(""),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("Alt + ")+ ResStr(IDS_INPUT_ENTER),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, ResStr(IDS_INPUT_INNER),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, ResStr(IDS_INPUT_SYS),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, _T(""),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("Alt + H/S"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, ResStr(IDS_INPUT_INNER),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, ResStr(IDS_INPUT_DVDDIR),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, _T(""),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("Ctrl + ")+ ResStr(IDS_INPUT_DVDDIR2),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, ResStr(IDS_INPUT_INNER),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, ResStr(IDS_INPUT_DVDSEL),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, _T(""),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("Ctrl + ")+ ResStr(IDS_INPUT_ENTER),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, ResStr(IDS_INPUT_INNER),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, ResStr(IDS_INPUT_DVDROOT),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, _T(""),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("Ctrl + Home"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, ResStr(IDS_INPUT_INNER),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, ResStr(IDS_INPUT_DVDPRE),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, _T(""),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("Ctrl + End"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, ResStr(IDS_INPUT_INMOS),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, ResStr(IDS_INPUT_FSR),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, ResStr(IDS_INPUT_FSRIF),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, ResStr(IDS_INPUT_RC),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_readonly_number = index;

	int nItem, nSubItem;
	m_def_number = m_inputs.GetSize() + m_readonly_number;

	// insert the items and subitems into the list
	CString info, command, value, key, abs;
	int keynum = 0;
	for (nItem = m_readonly_number; nItem < m_def_number; nItem++)
	{
		m_inputs.GetAt(nItem - m_readonly_number,key,command,value,abs);
		keynum = m_skey.GetKeyInfo(key);
		GetCmdInfo(command,info);
		for (nSubItem = 0; nSubItem < 4; nSubItem++)
		{
			str = _T("");

			if (nSubItem == 0)				// command
				str = command;
			else if (nSubItem == 1)			// detiel
				str = info;
			else if (nSubItem == 2)			// value
			{
				str = value;
				if(abs.GetLength() > 0)
					str = _T("=") + str;
			}
			else if (nSubItem == 3)			// key
				str = key;
			
			COLORREF color;
			if(nItem % 2 == 0)
				color = RGB(255,255,240);
			else
				color = RGB(220,240,255);

			// show background coloring
			if (nSubItem == 0)
				pList->InsertItem(nItem, str,RGB(0,0,0),color);
			else
				pList->SetItemText(nItem, nSubItem, str,RGB(0,0,0),color);

			if (nSubItem == 2 && value != _T(""))
				pList->SetEdit(nItem, nSubItem);
			if (nSubItem == 3)
				pList->SetComboBox(nItem, nSubItem, 
								   TRUE,		// enable combobox
								   &m_skey.m_keyInfo,		// pointer to persistent CStringArray
								   10,			// size of dropdown
								   keynum,			// initial combo selection
								   FALSE);		// sort CStringArray
		}
	}

	pList->UnlockWindowUpdate();	// ***** unlock window updates *****
}


BOOL CMInputPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	LoadInputConfig();
	// TODO:  在此添加额外的初始化
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	InitListCtrl(&m_List);
	FillListCtrl(&m_List);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMInputPage::LoadInputConfig()
{
	CStdioFile inputcfg;
	if(inputcfg.Open(m_program_dir + _T("input.ini"),CFile::modeRead))
	{
		CString line, command, value, key, abs;
		while(inputcfg.ReadString(line))
		{
			line = LocalToUnicode(line);
			if(AnalyseLine(line, key, command, value, abs))
			{
				m_inputs.Add(key, command, value, abs);
			}
		}
		inputcfg.Close();
	}
	if(m_inputs.GetSize() < 1)
	{
		if(MessageBox(ResStr(IDS_INPUT_SETNONE),ResStr(IDS_INPUT_SET),MB_OKCANCEL|MB_TOPMOST) != IDOK)
			return;
		ExtractResource(MAKEINTRESOURCE(IDZ_INPUT),TEXT("INPUT_INI")
			,m_program_dir + _T("input.ini"),true,true,_T("input.ini"));
		if(inputcfg.Open(m_program_dir + _T("input.ini"),CFile::modeRead))
		{
			CString line,command,value,key,abs;
			while(inputcfg.ReadString(line))
			{
				line = LocalToUnicode(line);
				if(AnalyseLine(line,key,command,value,abs))
				{
					m_inputs.Add(key,command,value,abs);
				}
			}
			inputcfg.Close();
		}
	}
}

bool CMInputPage::SaveInputConfig()
{
	UpdateData(TRUE);
	m_List.LockWindowUpdate();
	int mute = CheckMute();
	if(mute >= 0)
	{
		CString msg;
		msg.Format(_T("%s：%s 、 %s 、 %s。"), ResStr(IDS_INPUT_REUS)
			, m_List.GetItemText(mute,0), m_List.GetItemText(mute,2), m_List.GetItemText(mute,3));
		m_List.UnlockWindowUpdate();
		MessageBox(msg, ResStr(IDS_INPUT_CONF));
		return false;
	}
	CStdioFile inputcfg;
	CString key, cmd, value, keyinfo;
	if(inputcfg.Open(m_program_dir + _T("input.ini"),CFile::modeCreate|CFile::modeWrite))
	{
		inputcfg.WriteString(_T("## MPlayer input control file\n\n"));
		for(int i = m_readonly_number; i < m_List.GetItemCount(); i++)
		{
			cmd = m_List.GetItemText(i,0);
			value = m_List.GetItemText(i,2);
			keyinfo = m_List.GetItemText(i,3);
			int offset = value.Find(_T("="));
			if(offset >= 0)
			{
				value.Delete(0,offset + 1);
				value += _T(" 1");
			}
			if(m_skey.GetKeyName(keyinfo,key))
			{
				if(i >= m_def_number)
				{
					cmd.TrimLeft(_T(" "));
					cmd.TrimRight(_T(" "));
					if(cmd.GetLength() < 2)
						continue;
				}
				keyinfo = key + _T(" ") + cmd;
				if(value.GetLength() > 0)
					keyinfo += _T(" ") + value;
				keyinfo +=_T("\n");
				inputcfg.WriteString(keyinfo);
			}
		}
		inputcfg.Close();
	}
	m_List.UnlockWindowUpdate();
	return true;
}

void CMInputPage::GetCmdInfo(CString cmd ,CString &info)
{
	info = _T("");
	if(cmd == _T("seek"))
		info = ResStr(IDS_INPUT_SEEK);
	else if(cmd == _T("sub_step"))
		info = ResStr(IDS_INPUT_SUBT);
	else if(cmd == _T("pt_up_step"))
		info = ResStr(IDS_INPUT_PTUS);
	else if(cmd == _T("pt_step"))
		info = ResStr(IDS_INPUT_PTS);
	else if(cmd == _T("alt_src_step"))
		info = ResStr(IDS_INPUT_ALTS);
	else if(cmd == _T("panscan"))
		info = ResStr(IDS_INPUT_PANS);
	else if(cmd == _T("pause"))
		info = ResStr(IDS_INPUT_PAUS);
	else if(cmd == _T("screenshot"))
		info = ResStr(IDS_INPUT_SCRS);
	else if(cmd == _T("keep_aspect"))
		info = ResStr(IDS_INPUT_KEEP);
	else if(cmd == _T("frame_drop"))
		info = ResStr(IDS_INPUT_FRAD);
	else if(cmd == _T("osd"))
		info = ResStr(IDS_INPUT_OSD);
	else if(cmd == _T("quit"))
		info = ResStr(IDS_INPUT_QUIT);
	else if(cmd == _T("mute"))
		info = ResStr(IDS_INPUT_MUTE);

	else if(cmd == _T("switch_audio"))
		info = ResStr(IDS_INPUT_SWIA);
	else if(cmd == _T("switch_font"))
		info = ResStr(IDS_INPUT_SWIF);
	else if(cmd == _T("seek_chapter"))
		info = ResStr(IDS_INPUT_CHAP);
	else if(cmd == _T("switch_title"))
		info = ResStr(IDS_INPUT_TITLE);
	else if(cmd == _T("switch_angle"))
		info = ResStr(IDS_INPUT_ANGLE);

	else if(cmd == _T("sub_select"))
		info = ResStr(IDS_INPUT_SUBS);
	else if(cmd == _T("sub_alignment"))
		info = ResStr(IDS_INPUT_SUBA);
	else if(cmd == _T("switch_view"))
		info = ResStr(IDS_INPUT_SWIV);
	else if(cmd == _T("vo_fullscreen"))
		info = ResStr(IDS_INPUT_VOFS);
	else if(cmd == _T("vo_ontop"))
		info = ResStr(IDS_INPUT_VOOT);
	else if(cmd == _T("reload"))
		info = ResStr(IDS_INPUT_RELO);
	else if(cmd == _T("frame_step"))
		info = ResStr(IDS_INPUT_FRAS);
	else if(cmd == _T("speed_set"))
		info = ResStr(IDS_INPUT_SPES);
	else if(cmd == _T("speed_incr"))
		info = ResStr(IDS_INPUT_SPEI);
	else if(cmd == _T("balance"))
		info = ResStr(IDS_INPUT_BALA);

	else if(cmd == _T("volume"))
		info = ResStr(IDS_INPUT_VOLU);
	else if(cmd == _T("sub_pos"))
		info = ResStr(IDS_INPUT_SUBP);
	else if(cmd == _T("contrast"))
		info = ResStr(IDS_INPUT_CONT);
	else if(cmd == _T("brightness"))
		info = ResStr(IDS_INPUT_BRIG);
	else if(cmd == _T("saturation"))
		info = ResStr(IDS_INPUT_SATU);
	else if(cmd == _T("gamma"))
		info = ResStr(IDS_INPUT_GAMM);
	else if(cmd == _T("hue"))
		info = ResStr(IDS_INPUT_HUE);

	else if(cmd == _T("audio_delay"))
		info = ResStr(IDS_INPUT_AUDD);
	else if(cmd == _T("sub_delay"))
		info = ResStr(IDS_INPUT_SUBD);

}

bool CMInputPage::AnalyseLine(CString line, CString &key, CString &cmd, CString &val, CString &abs)
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

BOOL CMInputPage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	switch(pMsg->message)
	{
	case   WM_KEYDOWN:
		switch(pMsg->wParam)
		{
		case   VK_RETURN:
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMInputPage::OnBnClickedButtonAdd()
{
	m_List.LockWindowUpdate();

	CString str = _T("");
	int nItem = m_List.GetItemCount();
	m_List.InsertItem(nItem, _T(""));
	m_List.SetEdit(nItem, 0);
	m_List.SetItemText(nItem, 1, ResStr(IDS_INPUT_CUST));
	m_List.SetItemText(nItem, 2, _T(""));
	m_List.SetEdit(nItem, 2);
	m_List.SetItemText(nItem, 3, _T(""));
	m_List.SetComboBox(nItem, 3, TRUE, &m_skey.m_keyInfo, 10, 0, FALSE);

	m_List.UnlockWindowUpdate();
}

void CMInputPage::OnBnClickedButtonDel()
{
	m_List.LockWindowUpdate();
	int nItem = m_List.GetCurSel();
	if(nItem >= m_def_number)
		m_List.DeleteItem(nItem);
	else if(nItem >= m_readonly_number)
	{
		CString msg;
		msg.Format(_T("%s：%s   %s ？") , ResStr(IDS_INPUT_CDEL)
			, m_List.GetItemText(nItem,3),  m_List.GetItemText(nItem,1));
		if( MessageBox(msg,ResStr(IDS_INPUT_CDEL),MB_OKCANCEL) == IDOK)
		{
			m_List.DeleteItem(nItem);
			m_def_number--;
		}
	}
	m_List.UnlockWindowUpdate();
}

int CMInputPage::CheckMute()
{
	CString keyinfo;
	CStringArray m_keyInfoList;
	m_keyInfoList.RemoveAll();
	for(int i = m_readonly_number; i < m_List.GetItemCount(); i++)
	{
		CString keyinfo = m_List.GetItemText(i,3);
		for(int j = 0; j < m_keyInfoList.GetSize() ; j++)
		{
			if(keyinfo == m_keyInfoList[j] && keyinfo != m_str_none)
				return i;
		}
		m_keyInfoList.Add(keyinfo);
	}
	return -1;
}
