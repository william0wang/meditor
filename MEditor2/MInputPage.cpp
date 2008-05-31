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
	m_skey.Add(_T("#None"),_T("无快捷键"));
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
	m_skey.Add(_T("LEFT"),_T("<- 左"));
	m_skey.Add(_T("RIGHT"),_T("-> 右"));
	m_skey.Add(_T("UP"),_T("Up 上"));
	m_skey.Add(_T("DOWN"),_T("Down 下"));
	m_skey.Add(_T("PGUP"),_T("Page Up"));
	m_skey.Add(_T("PGDWN"),_T("Page Down"));
	m_skey.Add(_T("HOME"),_T("Home"));
	m_skey.Add(_T("END"),_T("End"));
	m_skey.Add(_T("INS"),_T("Insert 插入"));
	m_skey.Add(_T("DEL"),_T("Delete 删除"));
	m_skey.Add(_T("SPACE"),_T("Space 空格"));
	m_skey.Add(_T("ENTER"),_T("Enter 回车"));
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

	m_skey.Add(_T("MOUSE_BTN0"),_T("单击左键"));
	m_skey.Add(_T("MOUSE_BTN0_DBL"),_T("双击左键"));
	m_skey.Add(_T("MOUSE_BTN2"),_T("单击右键"));
	m_skey.Add(_T("MOUSE_BTN2_DBL"),_T("双击右键"));
	m_skey.Add(_T("MOUSE_BTN1"),_T("单击中键"));
	m_skey.Add(_T("MOUSE_BTN1_DBL"),_T("双击中键"));
	m_skey.Add(_T("MOUSE_BTN3"),_T("向前滚动"));
	m_skey.Add(_T("MOUSE_BTN4"),_T("向后滚动"));
	m_skey.Add(_T("MOUSE_BTN5"),_T("单击X1键"));
	m_skey.Add(_T("MOUSE_BTN5_DBL"),_T("双击X1键"));
	m_skey.Add(_T("MOUSE_BTN6"),_T("单击X2键"));
	m_skey.Add(_T("MOUSE_BTN6_DBL"),_T("双击X2键"));
	m_skey.Add(_T("MOUSE_BTN8"),_T("按住右键并前滚"));
	m_skey.Add(_T("MOUSE_BTN9"),_T("按住右键并后滚"));
	
	m_skey.Add(_T("JOY_RIGHT"),_T("手柄－左"));
	m_skey.Add(_T("JOY_LEFT"),_T("手柄－右"));
	m_skey.Add(_T("JOY_UP"),_T("手柄－上"));
	m_skey.Add(_T("JOY_DOWN"),_T("手柄－下"));
	m_skey.Add(_T("JOY_BTN0"),_T("手柄 1键"));
	m_skey.Add(_T("JOY_BTN1"),_T("手柄 2键"));
	m_skey.Add(_T("JOY_BTN2"),_T("手柄 3键"));
	m_skey.Add(_T("JOY_BTN3"),_T("手柄 4键"));
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

	TCHAR *	lpszHeaders[] = { _T("命令"),_T("功能说明"), _T("参数"), _T("快捷键"), NULL };
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
	m_List.InsertItem(index, _T("内置快捷键"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, _T("增大字幕尺寸"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, _T("增大 0.1"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("c 或 C"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, _T("内置快捷键"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, _T("减小字幕尺寸"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, _T("减小 0.1"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("v 或 V"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, _T("内置快捷键"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, _T("调整单文件循环次数"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, _T("1~10、始终、停止"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("b 或 B"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, _T("内置快捷键"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, _T("全屏时调节画面尺寸"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, _T("放大/缩小/变形"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("右 Ctrl + 1~9"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, _T("内置快捷键"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, _T("全屏时调节画面位置"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, _T("上下左右移动"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("左 Ctrl + 1~9"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_List.InsertItem(index, _T("内置快捷键"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, _T("全屏幕"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, _T(""),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("Alt + 回车"),RGB(0,0,0),RGB(200,255,200));
	++index;
	
	m_List.InsertItem(index, _T("内置鼠标操作"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 1, _T("全屏幕下右键菜单"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 2, _T("鼠标移动到屏幕最上方"),RGB(0,0,0),RGB(200,255,200));
	m_List.SetItemText(index, 3, _T("单击鼠标右键"),RGB(0,0,0),RGB(200,255,200));
	++index;

	m_readonly_number = index;

	int nItem, nSubItem;
	m_def_number = m_inputs.GetSize() + m_readonly_number;

	// insert the items and subitems into the list
	CString info, command, value, key;
	int keynum = 0;
	for (nItem = m_readonly_number; nItem < m_def_number; nItem++)
	{
		m_inputs.GetAt(nItem - m_readonly_number,key,command,value);
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
				str = value;
			else if (nSubItem == 3)			// key
				str = key;
			
			COLORREF color;
			if(nItem % 2 == 0)
				color = RGB(250,230,200);
			else
				color = RGB(240,230,250);

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
		CString line,command,value,key;
		while(inputcfg.ReadString(line))
		{
			line = LocalToUnicode(line);
			if(AnalyseLine(line,key,command,value))
			{
				m_inputs.Add(key,command,value);
			}
		}
		inputcfg.Close();
	}
	if(m_inputs.GetSize() < 1)
	{
		if(MessageBox(_T("未找到快捷键设置，是否使用默认快捷键设置？"),_T("MEditor2 - 快捷键设置"),MB_OKCANCEL|MB_TOPMOST) != IDOK)
			return;
		ExtractResource(MAKEINTRESOURCE(IDZ_INPUT),TEXT("INPUT_INI")
			,m_program_dir + _T("input.ini"),true,true,_T("input.ini"));
		if(inputcfg.Open(m_program_dir + _T("input.ini"),CFile::modeRead))
		{
			CString line,command,value,key;
			while(inputcfg.ReadString(line))
			{
				line = LocalToUnicode(line);
				if(AnalyseLine(line,key,command,value))
				{
					m_inputs.Add(key,command,value);
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
		msg.Format(_T("重复使用快捷键：%s 、 %s 、 %s。"), m_List.GetItemText(mute,0)
			, m_List.GetItemText(mute,2), m_List.GetItemText(mute,3));
		m_List.UnlockWindowUpdate();
		MessageBox(msg,_T("快捷键冲突！"));
		return false;
	}
	CStdioFile inputcfg;
	CString key, cmd, value, keyinfo;
	if(inputcfg.Open(m_program_dir + _T("input.ini"),CFile::modeCreate|CFile::modeWrite))
	{
		inputcfg.WriteString(_T("## MPlayer input control file\n\n"));
		for(int i = 0; i < m_List.GetItemCount(); i++)
		{
			cmd = m_List.GetItemText(i,0);
			value = m_List.GetItemText(i,2);
			keyinfo = m_List.GetItemText(i,3);
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

//int CMInputPage::GetKeyNumber(CString Key)
//{
//	if(Key.GetLength() == 1)
//	{
//		TCHAR x = Key.GetAt(0);
//		int xi =  x - _T('a');
//		if(xi < 26 && xi >= 0)
//			return xi + 1;
//		xi =   x - _T('A');
//		if(xi < 26 && xi >= 0)
//			return xi + 37;
//		xi =   x - _T('0');
//		if(xi < 10 && xi >= 0)
//			return xi + 27;
//	}
//
//	if(Key == _T("LEFT"))
//		return 63;
//	else if(Key == _T("RIGHT"))
//		return 64;
//	else if(Key == _T("UP"))
//		return 65;
//	else if(Key == _T("DOWN"))
//		return 66;
//	else if(Key == _T("PGUP"))
//		return 67;
//	else if(Key == _T("PGDWN"))
//		return 68;
//	else if(Key == _T("HOME"))
//		return 69;
//	else if(Key == _T("END"))
//		return 70;
//	else if(Key == _T("INS"))
//		return 71;
//	else if(Key == _T("DEL"))
//		return 72;
//	else if(Key == _T("SPACE"))
//		return 73;
//	else if(Key == _T("ENTER"))
//		return 74;
//	else if(Key == _T("ESC"))
//		return 75;
//
//	else if(Key == _T("<"))
//		return 76;
//	else if(Key == _T(">"))
//		return 77;
//	else if(Key == _T("["))
//		return 78;
//	else if(Key == _T("]"))
//		return 79;
//	else if(Key == _T("{"))
//		return 80;
//	else if(Key == _T("}"))
//		return 81;
//	else if(Key == _T("\\"))
//		return 82;
//	else if(Key == _T(";"))
//		return 83;
//	else if(Key == _T("'"))
//		return 84;
//	else if(Key == _T(","))
//		return 85;
//	else if(Key == _T("."))
//		return 86;
//	else if(Key == _T("|"))
//		return 87;
//	else if(Key == _T(":"))
//		return 88;
//	else if(Key == _T("\""))
//		return 89;
//	else if(Key == _T("="))
//		return 90;
//	else if(Key == _T("-"))
//		return 91;
//	else if(Key == _T("+"))
//		return 92;
//	else if(Key == _T("/"))
//		return 93;
//	else if(Key == _T("*"))
//		return 94;
//
//	else if(Key == _T("F1"))
//		return 95;
//	else if(Key == _T("F2"))
//		return 96;
//	else if(Key == _T("F3"))
//		return 97;
//	else if(Key == _T("F4"))
//		return 98;
//	else if(Key == _T("F5"))
//		return 99;
//	else if(Key == _T("F6"))
//		return 100;
//	else if(Key == _T("F7"))
//		return 101;
//	else if(Key == _T("F8"))
//		return 102;
//	else if(Key == _T("F9"))
//		return 103;
//	else if(Key == _T("F10"))
//		return 104;
//	else if(Key == _T("F11"))
//		return 105;
//	else if(Key == _T("F12"))
//		return 106;
//
//	else if(Key == _T("MOUSE_BTN0"))
//		return 107;
//	else if(Key == _T("MOUSE_BTN0_DBL"))
//		return 108;
//	else if(Key == _T("MOUSE_BTN2"))
//		return 109;
//	else if(Key == _T("MOUSE_BTN2_DBL"))
//		return 110;
//	else if(Key == _T("MOUSE_BTN1"))
//		return 111;
//	else if(Key == _T("MOUSE_BTN3"))
//		return 112;
//	else if(Key == _T("MOUSE_BTN4"))
//		return 113;
//
//	else if(Key == _T("JOY_RIGHT"))
//		return 114;
//	else if(Key == _T("JOY_LEFT"))
//		return 115;
//	else if(Key == _T("JOY_UP"))
//		return 116;
//	else if(Key == _T("JOY_DOWN"))
//		return 117;
//	else if(Key == _T("JOY_BTN0"))
//		return 118;
//	else if(Key == _T("JOY_BTN1"))
//		return 119;
//	else if(Key == _T("JOY_BTN2"))
//		return 120;
//	else if(Key == _T("JOY_BTN3"))
//		return 121;
//
//	return 0;
//}

void CMInputPage::GetCmdInfo(CString cmd ,CString &info)
{
	info = _T("");
	if(cmd == _T("seek"))
		info = _T("快进/快退  单位:秒");	
	else if(cmd == _T("sub_step"))
		info = _T("上/下n句字幕  n为参数值");
	else if(cmd == _T("pt_up_step"))
		info = _T("上/下n项  n为参数值");
	else if(cmd == _T("pt_step"))
		info = _T("上/下n个视频  n为参数值");
	else if(cmd == _T("alt_src_step"))
		info = _T("上/下n个(asx)源  n为参数值");
	else if(cmd == _T("panscan"))
		info = _T("增加/减少过扫描");
	else if(cmd == _T("pause"))
		info = _T("暂停/播放");
	else if(cmd == _T("screenshot"))
		info = _T("视频截图");
	else if(cmd == _T("keep_aspect"))
		info = _T("是否保持视频比例");
	else if(cmd == _T("frame_drop"))
		info = _T("丢帧 状态切换");
	else if(cmd == _T("osd"))
		info = _T("OSD 状态切换");
	else if(cmd == _T("quit"))
		info = _T("退出播放器");
	else if(cmd == _T("mute"))
		info = _T("静音 状态切换");
	else if(cmd == _T("frame_drop"))
		info = _T("丢帧 状态切换");
	else if(cmd == _T("switch_audio"))
		info = _T("音轨/声道切换");
	else if(cmd == _T("switch_font"))
		info = _T("字体切换");
	else if(cmd == _T("sub_select"))
		info = _T("字幕切换");
	else if(cmd == _T("sub_alignment"))
		info = _T("字幕校正");
	else if(cmd == _T("switch_view"))
		info = _T("视图切换");
	else if(cmd == _T("vo_fullscreen"))
		info = _T("全屏幕 状态切换");
	else if(cmd == _T("vo_ontop"))
		info = _T("前端显示 状态切换");
	else if(cmd == _T("reload"))
		info = _T("重新加载视频");
	else if(cmd == _T("frame_step"))
		info = _T("单帧播放");
	else if(cmd == _T("speed_set"))
		info = _T("设置播放速度  1.0为正常速度");
	else if(cmd == _T("speed_incr"))
		info = _T("加速/减速播放  1.0为正常速度");

	else if(cmd == _T("volume"))
		info = _T("音量调节  0~100");
	else if(cmd == _T("sub_pos"))
		info = _T("字幕位置  0~100");
	else if(cmd == _T("contrast"))
		info = _T("对比度调节  -100~100");
	else if(cmd == _T("brightness"))
		info = _T("亮度调节  -100~100");
	else if(cmd == _T("saturation"))
		info = _T("饱和度调节  -100~100");
	else if(cmd == _T("gamma"))
		info = _T("伽玛值调节  -100~100");
	else if(cmd == _T("hue"))
		info = _T("色度调节  -100~100");

	else if(cmd == _T("audio_delay"))
		info = _T("音频延迟  单位:秒");
	else if(cmd == _T("sub_delay"))
		info = _T("字幕延迟  单位:秒");


}

bool CMInputPage::AnalyseLine(CString line, CString &key, CString &cmd, CString &val)
{
	line += _T(" ");
	line.TrimLeft(_T(" "));
	int sp = -1;
	if(line.Find(_T("#")) == 0)
	{
		sp = line.Find(_T("#None"));
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
	m_List.SetItemText(nItem, 1, _T("自定义快捷键"));
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
		msg.Format(_T("确定删除快捷键：%s   %s ？")
			, m_List.GetItemText(nItem,3),  m_List.GetItemText(nItem,1));
		if( MessageBox(msg,_T("删除快捷键"),MB_OKCANCEL) == IDOK)
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
			if(keyinfo == m_keyInfoList[j] && keyinfo != _T("无快捷键"))
				return i;
		}
		m_keyInfoList.Add(keyinfo);
	}
	return -1;
}