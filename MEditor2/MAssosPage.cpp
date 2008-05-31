// MAssosPage.cpp : 实现文件
//
#include "stdafx.h"
#include "meditor2.h"
#include "MConfig.h"
#include "MAssosPage.h"
#include "reg.h"


// CMAssosPage 对话框

IMPLEMENT_DYNAMIC(CMAssosPage, CDialog)

CMAssosPage::CMAssosPage(CWnd* pParent /*=NULL*/)
	: CDialog(CMAssosPage::IDD, pParent)
	, m_rightmenu(TRUE)
	, m_rightmenu2(FALSE)
	, m_mpc(FALSE)
{
	
	m_str_list = ResStr(IDS_ASSOS_LIST);
	m_str_flash = ResStr(IDS_ASSOS_FLASH);
	m_str_ds = ResStr(IDS_ASSOS_DS);
	m_str_v = ResStr(IDS_ASSOS_V);
	m_str_a = ResStr(IDS_ASSOS_A);

	m_alist.Add( _T("3gp"), _T("3GP ") + ResStr(IDS_ASSOS_MV), m_str_v, _T("13"));
	m_alist.Add( _T("3gp2"), _T("3GP ") + ResStr(IDS_ASSOS_MV), m_str_v, _T("13"));
	m_alist.Add( _T("asf"), _T("Advanced Streaming ") + ResStr(IDS_ASSOS_V), m_str_v, _T("16"));
	m_alist.Add( _T("avi"), _T("AVI ") + ResStr(IDS_ASSOS_AVI), m_str_v, _T("1"));
	m_alist.Add( _T("dat"), _T("VCD ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("5"));
	m_alist.Add( _T("divx"), _T("DIVX ") + ResStr(IDS_ASSOS_V), m_str_v, _T("35"));
	m_alist.Add( _T("flv"), _T("Flash ") + ResStr(IDS_ASSOS_V), m_str_v, _T("2"));
	m_alist.Add( _T("m1v"), _T("MPEG-1 ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("7"));
	m_alist.Add( _T("m2v"), _T("MPEG-2 ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("7"));
	m_alist.Add( _T("m4b"), _T("MPEG-4 ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("6"));
	m_alist.Add( _T("m4p"), _T("MPEG-4 ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("6"));
	m_alist.Add( _T("m4v"), _T("MPEG-4 ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("6"));
	m_alist.Add( _T("mkv"), _T("Matroska ") + ResStr(IDS_ASSOS_V), m_str_v, _T("3"));
	m_alist.Add( _T("mov"), _T("QuickTime ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("12"));
	m_alist.Add( _T("mp4"), _T("MPEG-4 ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("6"));
	m_alist.Add( _T("mpeg"), _T("MPEG ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("7"));
	m_alist.Add( _T("mpg"), _T("MPEG ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("8"));
	m_alist.Add( _T("ogm"), _T("Ogg ") + ResStr(IDS_ASSOS_V), m_str_v, _T("4"));
	m_alist.Add( _T("pss"), _T("MPEG-1 ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("7"));
	m_alist.Add( _T("pva"), _T("MPEG-1 ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("7"));
	m_alist.Add( _T("tpr"), _T("MPEG-1 ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("7"));
	m_alist.Add( _T("qt"), _T("QuickTime ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("12"));
	m_alist.Add( _T("rm"), _T("RealMedia ") + ResStr(IDS_ASSOS_V), m_str_v, _T("14"));
	m_alist.Add( _T("rmvb"), _T("RealMedia VBR ") + ResStr(IDS_ASSOS_V), m_str_v, _T("15"));
	m_alist.Add( _T("tp"), _T("MPEG-2 ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("10"));
	m_alist.Add( _T("ts"), _T("MPEG-2 ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("9"));
	m_alist.Add( _T("vob"), _T("DVD ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("11"));
	m_alist.Add( _T("vp6"), _T("VP6/VP7 ") + ResStr(IDS_ASSOS_V), m_str_v, _T("35"));
	m_alist.Add( _T("wmv"), _T("Windows Media ") + ResStr(IDS_ASSOS_V), m_str_v, _T("17"));
	m_vedio_num = m_alist.m_type.GetSize();
	m_alist.Add( _T("aac"), _T("Advanced Audio Coding ") + ResStr(IDS_ASSOS_A),m_str_a, _T("18"));
	m_alist.Add( _T("ac3"), _T("Dolby AC3 ") + ResStr(IDS_ASSOS_A),m_str_a, _T("19"));
	m_alist.Add( _T("aiff"), _T("AIFF Mac") + ResStr(IDS_ASSOS_A),m_str_a, _T("36"));
	m_alist.Add( _T("amr"), _T("AMR ") + ResStr(IDS_ASSOS_MA),m_str_a, _T("20"));
	m_alist.Add( _T("dts"), _T("DTS DVD") + ResStr(IDS_ASSOS_A),m_str_a, _T("21"));
	m_alist.Add( _T("flac"), _T("FLAC ") + ResStr(IDS_ASSOS_LA),m_str_a, _T("22"));
	m_alist.Add( _T("m4a"), _T("MPEG-4 ") + ResStr(IDS_ASSOS_A),m_str_a, _T("23"));
	m_alist.Add( _T("mp2"), _T("MPEG ") + ResStr(IDS_ASSOS_A),m_str_a, _T("36"));
	m_alist.Add( _T("mp3"), _T("MPEG Laye-3 ") + ResStr(IDS_ASSOS_A),m_str_a, _T("24"));
	m_alist.Add( _T("mpa"), _T("MPEG ") + ResStr(IDS_ASSOS_A),m_str_a, _T("25"));
	m_alist.Add( _T("mpc"), _T("MusePack ") + ResStr(IDS_ASSOS_A),m_str_a, _T("26"));
	m_alist.Add( _T("mka"), _T("Matroska ") + ResStr(IDS_ASSOS_A),m_str_a, _T("27"));
	m_alist.Add( _T("ogg"), _T("Ogg ") + ResStr(IDS_ASSOS_A),m_str_a, _T("28"));
	m_alist.Add( _T("ra"), _T("RealMedia ") + ResStr(IDS_ASSOS_A),m_str_a, _T("29"));
	m_alist.Add( _T("wav"), _T("WAVE ") + ResStr(IDS_ASSOS_A),m_str_a, _T("30"));
	m_alist.Add( _T("wma"), _T("Windows Media ") + ResStr(IDS_ASSOS_A),m_str_a, _T("31"));
	m_audio_num = m_alist.m_type.GetSize();
	m_alist.Add( _T("m3u"), _T("M3U ") + ResStr(IDS_ASSOS_LISTFILE), m_str_list, _T("32"));
	m_alist.Add( _T("m3u8"), _T("M3U8 ") + ResStr(IDS_ASSOS_LISTFILE), m_str_list, _T("33"));
	m_alist.Add( _T("pls"), _T("PLS ") + ResStr(IDS_ASSOS_LISTFILE), m_str_list, _T("34"));
	m_alist.Add( _T("swf"), _T("Flash ") + ResStr(IDS_ASSOS_CT), m_str_flash, _T("0"));
	m_default_num = m_alist.m_type.GetSize();
	m_sa.RemoveAll();
	m_sa.Add(m_str_v);
	m_sa.Add(m_str_a);
	m_sa.Add(m_str_list);
	m_sa.Add(m_str_flash);
	m_sa.Add(m_str_ds);

	m_have_icons = false;
	m_special = false;
	
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
	
	m_icons_dll = m_program_dir + _T("micons.dll");
	m_player_exe = m_program_dir + _T("mplayer.exe");	
}

CMAssosPage::~CMAssosPage()
{
}

void CMAssosPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ASSOS, m_List);
	DDX_Check(pDX, IDC_CHECK_RMENU, m_rightmenu);
	DDX_Check(pDX, IDC_CHECK_RMENU2, m_rightmenu2);
	DDX_Check(pDX, IDC_CHECK_MPC, m_mpc);
	DDX_Control(pDX, IDC_CHECK_MPC, m_mpc_c);
}


BEGIN_MESSAGE_MAP(CMAssosPage, CDialog)
	ON_BN_CLICKED(IDC_ASSOS, OnBnClickedAssos)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DEL, OnBnClickedDel)
	ON_BN_CLICKED(IDC_ALL, OnBnClickedAll)
	ON_BN_CLICKED(IDC_NONE, OnBnClickedNone)
	ON_BN_CLICKED(IDC_RECOMMAND, OnBnClickedRecommand)
	
END_MESSAGE_MAP()


// CMAssosPage 消息处理程序


int CMAssosPage::m_nColWidths[4] = { 5, 10, 30, 10};	// sixty-fourths
// InitListCtrl
void CMAssosPage::InitListCtrl(CXListCtrl * pList)
{
	ASSERT(pList);
	if (!pList)
		return;

	// set column width according to window rect
	CRect rect;
	pList->GetWindowRect(&rect);

	int w = rect.Width() - 2;

	CString ass = ResStr(IDS_ASSOS_ASSOS);
	CString ex = ResStr(IDS_ASSOS_EX);
	CString inf = ResStr(IDS_ASSOS_INF);
	CString tp = ResStr(IDS_ASSOS_TYP);
	TCHAR *	lpszHeaders[] = { _tcsdup(ass.GetBuffer()) ,  _tcsdup(ex.GetBuffer()) 
		,  _tcsdup(inf.GetBuffer()) ,  _tcsdup(tp.GetBuffer()) , NULL };
	ass.ReleaseBuffer();
	ex.ReleaseBuffer();
	inf.ReleaseBuffer();
	tp.ReleaseBuffer();

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
void CMAssosPage::FillListCtrl(CXListCtrl * pList)
{
	// a list of some more of my favorite things

	pList->LockWindowUpdate();
	pList->DeleteAllItems();

	//m_sta.RemoveAll();
	//m_stv.RemoveAll();
	//m_stf.RemoveAll();
	//m_stl.RemoveAll();
	//m_sta.Add(m_str_a);
	//m_sta.Add(m_str_ds);
	//m_stv.Add(m_str_v);
	//m_stv.Add(m_str_ds);
	//m_stf.Add(m_str_flash);
	//m_stf.Add(m_str_ds);
	//m_stl.Add(m_str_list);
	//m_stl.Add(m_str_ds);

	CString str = _T("");
	int nItem, nSubItem;
	int num = m_alist.m_type.GetSize();
	int classnum;
	int checked;
	// insert the items and subitems into the list
	for (nItem = 0; nItem < num; nItem++)
	{
		if(m_alist.m_class[nItem].Find(m_str_v) >= 0)
			classnum = 0;
		else if(m_alist.m_class[nItem].Find(m_str_a) >= 0)
			classnum = 1;
		else if(m_alist.m_class[nItem].Find(m_str_list) >= 0)
			classnum = 2;
		else
			classnum = 3;

		if(IsAssosed(m_alist.m_type[nItem]))
			checked = 1;
		else
			checked = 0;
		for (nSubItem = 0; nSubItem < 4; nSubItem++)
		{
			str = _T("");

			if (nSubItem == 1)			// detiel
				str = m_alist.m_type[nItem];
			else if (nSubItem == 2)			// value
				str = m_alist.m_info[nItem];
			else if (nSubItem == 3)			// value
				str = m_alist.m_class[nItem];
			
			// show background coloring

			COLORREF color;
			if(classnum == 1)
				color = RGB(160,230,230);
			else if(classnum == 2)
				color = RGB(230,250,180);
			else if(classnum == 0)
				color = RGB(200,180,250);
			else
				color = RGB(255,120,120);
			if (nSubItem == 0)
			{
				pList->InsertItem(nItem, str,RGB(0,0,0), color);
				pList->SetCheckbox(nItem, nSubItem, checked);
			}
			else
				pList->SetItemText(nItem, nSubItem, str,RGB(0,0,0), color);
		}
		//if(m_mpc)
		//{
		//	if(classnum == 0)
		//		m_List.SetComboBox(nItem, 3,   TRUE,  &m_stv,  2,  0,  FALSE);
		//	else if(classnum == 1)
		//		m_List.SetComboBox(nItem, 3,   TRUE,  &m_sta,  2,  0,  FALSE);
		//	else if(classnum == 3)
		//		m_List.SetComboBox(nItem, 3,   TRUE,  &m_stf,  2,  0,  FALSE);
		//	else
		//		m_List.SetComboBox(nItem, 3,   TRUE,  &m_stl,  2,  0,  FALSE);
		//}
	}

	pList->UnlockWindowUpdate();
}

BOOL CMAssosPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	InitListCtrl(&m_List);
	FillListCtrl(&m_List);

	m_mpc_exe = m_program_dir + _T("tools\\mplayerc.exe");
	if( IsFileExist(m_mpc_exe))
	{
		m_mpc_c.EnableWindow(TRUE);
		m_mpc = TRUE;
	}

	if(IsFileExist(m_icons_dll))
		m_have_icons = true;

	if(IsFileExist(m_program_dir + _T("SpecialConfig")))
		m_special = true;
	
	m_List.LockWindowUpdate();
	if(m_cfg)
	{
		CString value_s;
		bool value_b;
		if(m_cfg->GetValue_Boolean(_T("assos_rightmenu"),value_b,true))
		{
			if(!value_b)
				m_rightmenu = FALSE;
		}
		if(m_cfg->GetValue_Boolean(_T("assos_rightmenu2"),value_b,true))
		{
			if(value_b)
				m_rightmenu2 = TRUE;
		}
		if(m_cfg->GetValue_String(_T("assos_video"),value_s,true))
		{
			value_s += _T(",");
			int offsets = value_s.Find(_T(","));
			CString s_type;
			CString s_info;
			bool checked;
			while(offsets > 0)
			{
				CString text = value_s.Left(offsets);
				int offset = text.Find(_T("="));
				if(offset > 0)
				{
					s_type = text.Left(offset);
					s_info = text.Mid(offset + 1, text.GetLength() - offset -1);
					if(IsAssosed(s_type))
						checked = 1;
					else
						checked = 0;
					int nItem = m_List.GetItemCount();
					m_List.InsertItem(nItem, _T(""));
					m_List.SetCheckbox(nItem, 0, checked);
					m_List.SetItemText(nItem, 1, s_type);
					m_List.SetEdit(nItem, 1);
					m_List.SetItemText(nItem, 2, s_info);
					m_List.SetEdit(nItem, 2);
					m_List.SetComboBox(nItem, 3,   TRUE,  &m_sa,  5,  0,  FALSE);
				}
				value_s.Delete(0,offsets + 1);
				offsets = value_s.Find(_T(","));
			}
		}
		if(m_cfg->GetValue_String(_T("assos_audio"),value_s,true))
		{
			value_s += _T(",");
			int offsets = value_s.Find(_T(","));
			CString s_type;
			CString s_info;
			bool checked;
			while(offsets > 0)
			{
				CString text = value_s.Left(offsets);
				int offset = text.Find(_T("="));
				if(offset > 0)
				{
					s_type = text.Left(offset);
					s_info = text.Mid(offset + 1, text.GetLength() - offset -1);
					if(IsAssosed(s_type))
						checked = 1;
					else
						checked = 0;
					int nItem = m_List.GetItemCount();
					m_List.InsertItem(nItem, _T(""));
					m_List.SetCheckbox(nItem, 0, checked);
					m_List.SetItemText(nItem, 1, s_type);
					m_List.SetEdit(nItem, 1);
					m_List.SetItemText(nItem, 2, s_info);
					m_List.SetEdit(nItem, 2);
					m_List.SetComboBox(nItem, 3,   TRUE,  &m_sa,  5,  1,  FALSE);
				}
				value_s.Delete(0,offsets + 1);
				offsets = value_s.Find(_T(","));
			}
		}
		if(m_cfg->GetValue_String(_T("assos_flash"),value_s,true))
		{
			value_s += _T(",");
			int offsets = value_s.Find(_T(","));
			CString s_type;
			CString s_info;
			bool checked;
			while(offsets > 0)
			{
				CString text = value_s.Left(offsets);
				int offset = text.Find(_T("="));
				if(offset > 0)
				{
					s_type = text.Left(offset);
					s_info = text.Mid(offset + 1, text.GetLength() - offset -1);
					if(IsAssosed(s_type))
						checked = 1;
					else
						checked = 0;
					int nItem = m_List.GetItemCount();
					m_List.InsertItem(nItem, _T(""));
					m_List.SetCheckbox(nItem, 0, checked);
					m_List.SetItemText(nItem, 1, s_type);
					m_List.SetEdit(nItem, 1);
					m_List.SetItemText(nItem, 2, s_info);
					m_List.SetEdit(nItem, 2);
					m_List.SetComboBox(nItem, 3,   TRUE,  &m_sa,  5,  3,  FALSE);
				}
				value_s.Delete(0,offsets + 1);
				offsets = value_s.Find(_T(","));
			}
		}
		if(m_cfg->GetValue_String(_T("assos_dshow"),value_s,true))
		{
			value_s += _T(",");
			int offsets = value_s.Find(_T(","));
			CString s_type;
			CString s_info;
			bool checked;
			while(offsets > 0)
			{
				CString text = value_s.Left(offsets);
				int offset = text.Find(_T("="));
				if(offset > 0)
				{
					s_type = text.Left(offset);
					s_info = text.Mid(offset + 1, text.GetLength() - offset -1);
					if(IsAssosed(s_type))
						checked = 1;
					else
						checked = 0;
					int nItem = m_List.GetItemCount();
					m_List.InsertItem(nItem, _T(""));
					m_List.SetCheckbox(nItem, 0, checked);
					m_List.SetItemText(nItem, 1, s_type);
					m_List.SetEdit(nItem, 1);
					m_List.SetItemText(nItem, 2, s_info);
					m_List.SetEdit(nItem, 2);
					m_List.SetComboBox(nItem, 3,   TRUE,  &m_sa,  5,  4,  FALSE);
				}
				value_s.Delete(0,offsets + 1);
				offsets = value_s.Find(_T(","));
			}
		}
		if(m_cfg->GetValue_String(_T("assos_playlist"),value_s,true))
		{
			value_s += _T(",");
			int offsets = value_s.Find(_T(","));
			CString s_type;
			CString s_info;
			bool checked;
			while(offsets > 0)
			{
				CString text = value_s.Left(offsets);
				int offset = text.Find(_T("="));
				if(offset > 0)
				{
					s_type = text.Left(offset);
					s_info = text.Mid(offset + 1, text.GetLength() - offset -1);
					if(IsAssosed(s_type))
						checked = 1;
					else
						checked = 0;
					int nItem = m_List.GetItemCount();
					m_List.InsertItem(nItem, _T(""));
					m_List.SetCheckbox(nItem, 0, checked);
					m_List.SetItemText(nItem, 1, s_type);
					m_List.SetEdit(nItem, 1);
					m_List.SetItemText(nItem, 2, s_info);
					m_List.SetEdit(nItem, 2);
					m_List.SetComboBox(nItem, 3,   TRUE,  &m_sa,  5,  2,  FALSE);
				}
				value_s.Delete(0,offsets + 1);
				offsets = value_s.Find(_T(","));
			}
		}
	}
	m_List.UnlockWindowUpdate();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMAssosPage::SaveConfig()
{
	if(!m_cfg)
		return;
	UpdateData(TRUE);

	if(!m_rightmenu)
		m_cfg->SetValue(_T("assos_rightmenu"),  _T("0") , true , ex_meditor);
	else
		m_cfg->RemoveValue(_T("assos_rightmenu") , true);

	if(m_rightmenu2)
		m_cfg->SetValue(_T("assos_rightmenu2"),  _T("1") , true , ex_meditor);
	else
		m_cfg->RemoveValue(_T("assos_rightmenu2") , true);

	m_List.LockWindowUpdate();

	CString video_str = _T(""), audio_str = _T("") , playlist_str = _T("") ,  flash_str = _T("") ,  dshow_str = _T("");
	CString s_type, s_info, s_classes;
	for(int i = m_default_num ; i < m_List.GetItemCount(); i ++)
	{
		s_type = m_List.GetItemText(i,1);
		if(s_type.GetLength() < 1 || s_type.GetLength() > 6)
			continue;
		s_info = m_List.GetItemText(i,2);
		s_classes = m_List.GetItemText(i,3);
		if(s_classes == m_str_list)
			playlist_str += s_type + _T("=")  + s_info  + _T(",");
		else if(s_classes ==m_str_a)
			audio_str += s_type + _T("=")  + s_info  + _T(",");
		else if(s_classes == m_str_flash)
			flash_str += s_type + _T("=")  + s_info  + _T(",");
		else if(s_classes == m_str_ds)
			dshow_str += s_type + _T("=")  + s_info  + _T(",");
		else
			video_str += s_type + _T("=")  + s_info  + _T(",");
	}
	
	video_str.TrimRight(_T(","));
	if(video_str.GetLength() > 1)
		m_cfg->SetValue(_T("assos_video"),  video_str , true , ex_meditor);
	else
		m_cfg->RemoveValue(_T("assos_video") , true);
	
	audio_str.TrimRight(_T(","));
	if(audio_str.GetLength() > 1)
		m_cfg->SetValue(_T("assos_audio"),  audio_str , true , ex_meditor);
	else
		m_cfg->RemoveValue(_T("assos_audio") , true);
	
	flash_str.TrimRight(_T(","));
	if(flash_str.GetLength() > 1)
		m_cfg->SetValue(_T("assos_flash"),  flash_str , true , ex_meditor);
	else
		m_cfg->RemoveValue(_T("assos_flash") , true);
	
	dshow_str.TrimRight(_T(","));
	if(dshow_str.GetLength() > 1)
		m_cfg->SetValue(_T("assos_dshow"),  dshow_str , true , ex_meditor);
	else
		m_cfg->RemoveValue(_T("assos_dshow") , true);
	
	playlist_str.TrimRight(_T(","));
	if(playlist_str.GetLength() > 1)
		m_cfg->SetValue(_T("assos_playlist"),  playlist_str , true , ex_meditor);
	else
		m_cfg->RemoveValue(_T("assos_playlist") , true);

	m_List.UnlockWindowUpdate();
}

bool CMAssosPage::IsAssosed(CString type)
{
	CReg reg;
	CString regstr = _T("mplayer.") + type;
	if(reg.ShowContent_STR(HKEY_CLASSES_ROOT,regstr,_T("")))
		return true;
	return false;
}

bool CMAssosPage::AssosType(CString type, CString info, CString icons, bool isplaylist, bool Assos)
{
	CReg reg;
	CString SubKey, Name, Content;
	if(Assos)
	{
		SubKey =  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + type + _T("\\OpenWithProgids");
		Name =  _T("mplayer.") + type;
		reg.SetValue_N_STR(HKEY_CURRENT_USER,SubKey, Name);

		SubKey =  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + type;
		Name =  _T("UserChoice");
		reg.DeleteKey_STR(HKEY_CURRENT_USER,SubKey, Name);

		Name =  _T("");
		SubKey =  _T("mplayer.") + type;
		reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,info);

		SubKey =  _T("mplayer.") + type + _T("\\DefaultIcon");
		if(IsFileExist(_T("icons\\")  +type +_T(".ico")))
			Content = m_program_dir +_T("icons\\") +type +_T(".ico");
		else if(IsFileExist(m_icons_dll) && icons.GetLength() > 0)
			Content = m_program_dir +_T("micons.dll,") + icons;
		else
			Content =  m_player_exe + _T(",0");
		reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name , Content);

		SubKey =  _T("mplayer.") + type + _T("\\shell");
		if(!reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,_T("open")))
			return false;

		SubKey =  _T("mplayer.") + type + _T("\\shell\\open");
		if(m_rightmenu)
			reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,ResStr(IDS_PLAYER_USE) + _T(" mplayer ") + ResStr(IDS_ASSOS_OP));
		else
			reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,_T(""));
		if(m_rightmenu2)
		{
			SubKey =  _T("mplayer.") + type + _T("\\shell\\play");
			reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,ResStr(IDS_PLAYER_USE) + _T(" mplayer ") + ResStr(IDS_ASSOS_PL));
			SubKey =  _T("mplayer.") + type + _T("\\shell\\play\\command");
			CString spini =_T("");

			if(m_special)
				spini =_T(" -include \"") + m_program_dir + _T("SpecialConfig\\") + type + _T(".ini\"");
			
			if(isplaylist)
				Content =  _T("\"") +m_player_exe +_T("\"") + spini + _T(" -playlist \"%1\"");
			else
				Content =  _T("\"") +m_player_exe +_T("\"") + spini +  _T(" \"%1\"");
			reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name , Content);
		}
		else
		{
			SubKey =  _T("mplayer.") + type + _T("\\shell\\play");
			reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, _T("command"));
			SubKey =  _T("mplayer.") + type + _T("\\shell");
			reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, _T("play"));
		}

		SubKey =  _T("mplayer.") + type + _T("\\shell\\open\\command");
		CString spini =_T("");

		if(m_special)
			spini =_T(" -include \"") + m_program_dir + _T("SpecialConfig\\") + type + _T(".ini\"");
		
		if(isplaylist)
			Content =  _T("\"") +m_player_exe +_T("\"") + spini + _T(" -playlist \"%1\"");
		else
			Content =  _T("\"") +m_player_exe +_T("\"") + spini +  _T(" \"%1\"");
		if(!reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name , Content))
			return false;

		SubKey = _T(".") + type;
		Content = _T("mplayer.") + type;
		if (reg.ShowContent_STR(HKEY_CLASSES_ROOT ,SubKey,_T("")))
		{
			CString BackSrt(reg.content);
			if(BackSrt.Find(_T("mplayer.")) != 0)
				reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, _T("mplayer backup"), BackSrt);
		}
		if(!reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, _T(""),Content))
			return false;
	}
	else
	{
		SubKey =  _T("mplayer.") + type;
		if (reg.ShowContent_STR(HKEY_CLASSES_ROOT ,SubKey,_T("mplayer backup")))
		{
			CString BackSrt(reg.content);
			if(BackSrt.Find(_T("mplayer."))  !=  0)
				reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, _T(""), BackSrt);
			reg.DeleteValue_STR(HKEY_CLASSES_ROOT,SubKey,_T("mplayer backup"));
		}

		SubKey =  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + type + _T("\\OpenWithProgids");
		Name =  _T("mplayer.") + type;
		reg.DeleteValue_STR(HKEY_CURRENT_USER,SubKey, Name);
		
		SubKey =  _T("mplayer.") + type + _T("\\shell\\open");
		Name =  _T("command");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + type + _T("\\shell");
		Name =  _T("open");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + type + _T("\\shell\\play");
		Name =  _T("command");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + type + _T("\\shell");
		Name =  _T("play");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + type;
		Name =  _T("shell");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + type;
		Name =  _T("DefaultIcon");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("");
		Name =  _T("mplayer.") + type;
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		
	}
	return true;
}

bool CMAssosPage::AssosTypeIner(CString type, CString info, CString icons, bool Assos ,bool isflash)
{
	CReg reg;
	CString SubKey, Name, Content;
	CString m_player_exe;
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	m_player_exe.Format(_T("%s"),szFilePath);
	if(Assos)
	{
		SubKey =  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + type + _T("\\OpenWithProgids");
		Name =  _T("mplayer.") + type;
		reg.SetValue_N_STR(HKEY_CURRENT_USER,SubKey, Name);

		SubKey =  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + type;
		Name =  _T("UserChoice");
		reg.DeleteKey_STR(HKEY_CURRENT_USER,SubKey, Name);

		Name =  _T("");
		SubKey =  _T("mplayer.") + type;
		reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,info);

		SubKey =  _T("mplayer.") + type + _T("\\DefaultIcon");
		if(IsFileExist(_T("icons\\")  +type +_T(".ico")))
			Content = m_program_dir +_T("icons\\") +type +_T(".ico");
		else if(IsFileExist(m_icons_dll) && icons.GetLength() > 0)
			Content = m_program_dir +_T("micons.dll,") + icons;
		else if(isflash)
			Content =  m_player_exe + _T(",1");
		else
			Content =  m_player_exe + _T(",2");
		reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name , Content);

		SubKey =  _T("mplayer.") + type + _T("\\shell");
		if(!reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,_T("open")))
			return false;

		SubKey =  _T("mplayer.") + type + _T("\\shell\\open");
		if(isflash)
		{
			if(m_rightmenu)
				reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,ResStr(IDS_PLAYER_USE) + _T(" MFlashPlayer ") + ResStr(IDS_ASSOS_OP));
			else
				reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,_T(""));
		}
		else
		{
			if(m_rightmenu && !m_mpc)
				reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,ResStr(IDS_PLAYER_USE) + _T(" MDSPlayer ") + ResStr(IDS_ASSOS_OP));
			else if(m_rightmenu && m_mpc)
				reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,ResStr(IDS_PLAYER_USE) + _T(" MPC ") + ResStr(IDS_ASSOS_OP));
			else
				reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name ,_T(""));
		}

		SubKey =  _T("mplayer.") + type + _T("\\shell\\open\\command");
		if(isflash)
			Content =  _T("\"") + m_player_exe +_T("\" --Open FlashPlayer \"%1\"");
		else	if(m_mpc)
			Content =  _T("\"") + m_mpc_exe +_T("\" \"%1\"");
		else
			Content =  _T("\"") + m_player_exe +_T("\" --Open MediaPlayer \"%1\"");
		if(!reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, Name , Content))
			return false;

		SubKey = _T(".") + type;
		Content = _T("mplayer.") + type;
		if (reg.ShowContent_STR(HKEY_CLASSES_ROOT ,SubKey,_T("")))
		{
			CString BackSrt(reg.content);
			if(BackSrt.Find(_T("mplayer.")) != 0)
				reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, _T("mplayer backup"), BackSrt);
		}
		if(!reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, _T(""),Content))
			return false;
	}
	else
	{
		SubKey =  _T("mplayer.") + type;
		if (reg.ShowContent_STR(HKEY_CLASSES_ROOT ,SubKey,_T("mplayer backup")))
		{
			CString BackSrt(reg.content);
			if(BackSrt.Find(_T("mplayer."))  !=  0)
				reg.SetValue_S_STR(HKEY_CLASSES_ROOT,SubKey, _T(""), BackSrt);
			reg.DeleteValue_STR(HKEY_CLASSES_ROOT,SubKey,_T("mplayer backup"));
		}

		SubKey =  _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.") + type + _T("\\OpenWithProgids");
		Name =  _T("mplayer.") + type;
		reg.DeleteValue_STR(HKEY_CURRENT_USER,SubKey, Name);

		SubKey =  _T("mplayer.") + type + _T("\\shell\\open");
		Name =  _T("command");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + type + _T("\\shell");
		Name =  _T("open");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + type + _T("\\shell\\play");
		Name =  _T("command");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + type + _T("\\shell");
		Name =  _T("play");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + type;
		Name =  _T("shell");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("mplayer.") + type;
		Name =  _T("DefaultIcon");
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		SubKey =  _T("");
		Name =  _T("mplayer.") + type;
		reg.DeleteKey_STR(HKEY_CLASSES_ROOT,SubKey, Name);
		
	}
	return true;
}

AssList::AssList()
{
	m_type.RemoveAll();
	m_info.RemoveAll();
	m_class.RemoveAll();
	m_icon.RemoveAll();
}
void AssList::RemoveAll()
{
	m_type.RemoveAll();
	m_info.RemoveAll();
	m_class.RemoveAll();
	m_icon.RemoveAll();
}

void AssList::Add(CString type, CString info, CString classes, CString icon)
{
	m_type.Add(type);
	m_info.Add(info);
	m_class.Add(classes);
	m_icon.Add(icon);
}

void CMAssosPage::OnBnClickedAssos()
{
	ApplyChange();
}

void CMAssosPage::OnBnClickedAdd()
{
	m_List.LockWindowUpdate();

	CString str = _T("");
	int nItem = m_List.GetItemCount();
	m_List.InsertItem(nItem, _T(""));
	m_List.SetCheckbox(nItem, 0, 0);
	m_List.SetItemText(nItem, 1, _T(""));
	m_List.SetEdit(nItem, 1);
	m_List.SetItemText(nItem, 2, ResStr(IDS_ASSOS_INPUT));
	m_List.SetEdit(nItem, 2);
	m_List.SetComboBox(nItem, 3,   TRUE,  &m_sa,  5,  0,  FALSE);
	
	m_List.UnlockWindowUpdate();
}

void CMAssosPage::OnBnClickedDel()
{
	m_List.LockWindowUpdate();
	int nItem = m_List.GetCurSel();
	if(nItem >= m_default_num)
		m_List.DeleteItem(nItem);
	else
		MessageBox(ResStr(IDS_ASSOS_DEL),ResStr(IDS_ASSOS_AS));
	m_List.UnlockWindowUpdate();
}

void CMAssosPage::OnBnClickedAll()
{
	m_List.LockWindowUpdate();
	CString str = _T("");
	int num = m_List.GetItemCount();
	int nItem;
	for (nItem = 0; nItem < num; nItem++)
	{
		m_List.SetCheckbox(nItem, 0, 1);
	}
	m_List.UnlockWindowUpdate();
}

void CMAssosPage::OnBnClickedNone()
{
	m_List.LockWindowUpdate();
	CString str = _T("");
	int num = m_List.GetItemCount();
	int nItem;
	for (nItem = 0; nItem < num; nItem++)
	{
		m_List.SetCheckbox(nItem, 0, 0);
	}
	m_List.UnlockWindowUpdate();
}


void CMAssosPage::OnBnClickedRecommand()
{
	m_List.LockWindowUpdate();
	CString str = _T("");
	int num = m_List.GetItemCount();
	for (int nItem = 0; nItem < num; nItem++)
	{
		if(nItem < m_vedio_num || nItem == m_audio_num + 2)
			m_List.SetCheckbox(nItem, 0, 1);
		else
			m_List.SetCheckbox(nItem, 0, 0);
	}
	m_List.UnlockWindowUpdate();
}

void CMAssosPage::ApplyChange(bool quiet)
{
	UpdateData(TRUE);
	if(!IsFileExist(m_player_exe))
	{
		if(!quiet)
			MessageBox(ResStr(IDS_PLAYER_SAMEDIR));
		return;
	}
	CString type, info, icon ,classes;
	bool checked, isplaylist , isflash, isdshow;
	//m_olist.RemoveAll();
	m_List.LockWindowUpdate();
	int num = m_List.GetItemCount();
	for (int nItem = 0; nItem < num; nItem++)
	{
		isflash = false;
		isplaylist = false;
		isdshow = false;
		if(nItem < m_default_num)
		{
			type = m_alist.m_type[nItem];
			info = m_alist.m_info[nItem];
			icon = m_alist.m_icon[nItem];
			if(m_alist.m_class[nItem] == m_str_list)
				isplaylist = true;
			else if(m_alist.m_class[nItem] == m_str_flash)
				 isflash = true;
			else if(m_alist.m_class[nItem] == m_str_ds)
				 isdshow = true;
		}
		else
		{
			type = m_List.GetItemText(nItem,1);
			if(type.GetLength() < 1 || type.GetLength() > 6)
				continue;
			
			info = m_List.GetItemText(nItem,2);
			classes = m_List.GetItemText(nItem,3);
			if(classes == m_str_list)
			{
				icon = _T("37");
				isplaylist = true;
			}
			else if(classes == m_str_flash)
			{
				 isflash = true;
			}
			else if(classes == m_str_ds)
			{
				 isdshow = true;
			}
			else
			{
				if(classes == m_str_v)
					icon = _T("35");
				else
					icon = _T("36");
				isplaylist = false;
			}
			//m_olist.Add(type,info,classes,icon);
		}
		if(m_List.GetCheckbox(nItem, 0))
			checked = true;
		else
			checked = false;
		if(isflash)
			AssosTypeIner(type,info,icon,checked);
		else if(isdshow)
			AssosTypeIner(type,info,icon,checked,false);
		else
			AssosType(type,info,icon,isplaylist,checked);
	}
	m_List.UnlockWindowUpdate();

	if(!quiet)
	{
		//Rebuild   Icon   Cache
		HKEY   hKey;  
		DWORD   dwType=REG_SZ;
		DWORD   dwLength=256;
		LPBYTE   SetContent_S;
		BYTE   content[256];
		RegOpenKeyEx(HKEY_CURRENT_USER,_T("Control   Panel\\Desktop\\WindowMetrics"),0,KEY_READ,&hKey);
		RegQueryValueExW(hKey,_T("Shell   Icon   Size")	,NULL,&dwType,content,&dwLength);
		RegCloseKey(hKey);

		SetContent_S=LPBYTE("21");
		RegOpenKeyEx(HKEY_CURRENT_USER , _T("Control   Panel\\Desktop\\WindowMetrics") ,0,KEY_WRITE,&hKey);
		RegSetValueEx(hKey,_T("Shell   Icon   Size")	,NULL,REG_SZ,SetContent_S,CString(SetContent_S).GetLength());
		RegCloseKey(hKey);
		SendMessageTimeout(HWND_BROADCAST,WM_SETTINGCHANGE, SPI_SETNONCLIENTMETRICS,0,SMTO_ABORTIFHUNG,500,NULL);
		
		SetContent_S=LPBYTE(content);
		RegOpenKeyEx(HKEY_CURRENT_USER,_T("Control   Panel\\Desktop\\WindowMetrics"),0,KEY_WRITE,&hKey);
		RegSetValueEx(hKey,_T("Shell   Icon   Size"),NULL,REG_SZ,SetContent_S,CString(SetContent_S).GetLength());
		RegCloseKey(hKey);
		SendMessageTimeout(HWND_BROADCAST,WM_SETTINGCHANGE, SPI_SETNONCLIENTMETRICS,0,SMTO_ABORTIFHUNG,1500,NULL);
	}
	SHChangeNotify(SHCNE_ASSOCCHANGED,   SHCNF_IDLIST|SHCNF_FLUSH,   0,   0);
}

BOOL CMAssosPage::PreTranslateMessage(MSG* pMsg) 
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
