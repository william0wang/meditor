// MDecodePage.cpp : implementation file
//

#include "stdafx.h"
#include "meditor2.h"
#include "MDecodePage.h"
#include "MConfig.h"
#include "reg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDecodePage dialog


CMDecodePage::CMDecodePage(CWnd* pParent /*=NULL*/)
	: CDialog(CMDecodePage::IDD, pParent)
	, m_auto_threads(TRUE)
{
	//{{AFX_DATA_INIT(CMDecodePage)
	//}}AFX_DATA_INIT
	m_vdecode.RemoveAll();
	m_adecode.RemoveAll();
	m_vdecode.Add(_T("coreavc"));
	m_vdecode.Add(_T("mpegpes"));
	m_vdecode.Add(_T("mpeg12"));
	m_vdecode.Add(_T("ffmpeg1"));
	m_vdecode.Add(_T("ffmpeg2"));
	m_vdecode.Add(_T("ffmpeg12"));
	m_vdecode.Add(_T("ffwmv1"));
	m_vdecode.Add(_T("ffwmv2"));
	m_vdecode.Add(_T("ffwmv3"));
	m_vdecode.Add(_T("ffvc1"));
	m_vdecode.Add(_T("ffh264"));
	m_vdecode.Add(_T("ffodivx"));
	m_vdecode.Add(_T("ffwv1f"));
	m_vdecode.Add(_T("camtasia"));
	m_vdecode.Add(_T("xvid"));
	m_vdecode.Add(_T("divx4vfw"));
	m_vdecode.Add(_T("divxds"));
	m_vdecode.Add(_T("divx"));
	m_vdecode.Add(_T("mpeg4ds"));
	m_vdecode.Add(_T("mpeg4"));
	m_vdecode.Add(_T("wmv11dmo"));
	m_vdecode.Add(_T("wmv9dmo"));
	m_vdecode.Add(_T("wmvdmo"));
	m_vdecode.Add(_T("wmv8"));
	m_vdecode.Add(_T("wmv7"));
	m_vdecode.Add(_T("wmvvc1dmo"));
	m_vdecode.Add(_T("wms10dmod"));
	m_vdecode.Add(_T("wmsdmod"));
	m_vdecode.Add(_T("wmvadmo"));
	m_vdecode.Add(_T("rv3040win"));
	m_vdecode.Add(_T("rv20winrp10"));
	m_vdecode.Add(_T("rv10winrp10"));
	m_vdecode.Add(_T("ffrv40"));
	m_vdecode.Add(_T("ffrv30"));
	m_vdecode.Add(_T("ffrv20"));
	m_vdecode.Add(_T("ffrv10"));
	m_vdecode.Add(_T("ffvp3"));
	m_vdecode.Add(_T("vp3"));
	m_vdecode.Add(_T("vp4"));
	m_vdecode.Add(_T("ffvp5"));
	m_vdecode.Add(_T("vp5"));
	m_vdecode.Add(_T("ffvp6"));
	m_vdecode.Add(_T("ffvp6f"));
	m_vdecode.Add(_T("vp6"));
	m_vdecode.Add(_T("vp7"));
	m_vdecode.Add(_T("null"));
	
	m_adecode.Add(_T("wmadmo"));
	m_adecode.Add(_T("wma9dmo"));
	m_adecode.Add(_T("wma9spdmo"));
	m_adecode.Add(_T("wma9spdshow"));
	m_adecode.Add(_T("ffcook"));
	m_adecode.Add(_T("ffatrc"));
	m_adecode.Add(_T("ra10cookwin"));
	m_adecode.Add(_T("racookwin"));
	m_adecode.Add(_T("ra10siprwin"));
	m_adecode.Add(_T("rasiprwin"));
	m_adecode.Add(_T("ra10atrcwin"));
	m_adecode.Add(_T("raatrcwin"));
	m_adecode.Add(_T("faad"));
	m_adecode.Add(_T("ffflac"));
	m_adecode.Add(_T("fftta"));
	m_adecode.Add(_T("ffwavpack"));
	m_adecode.Add(_T("ffape"));
	m_adecode.Add(_T("pcm"));
	m_adecode.Add(_T("divx"));
	m_adecode.Add(_T("mp3"));
	m_adecode.Add(_T("ffpcmdaud"));
	m_adecode.Add(_T("ffwmav1"));
	m_adecode.Add(_T("ffwmav2"));
	m_adecode.Add(_T("ffmp3"));
	m_adecode.Add(_T("ffmp3adu"));
	m_adecode.Add(_T("ffmp2"));
	m_adecode.Add(_T("mad"));
	m_adecode.Add(_T("mp3acm"));
	m_adecode.Add(_T("a52"));
	m_adecode.Add(_T("ffac3"));
	m_adecode.Add(_T("ffeac3"));
	m_adecode.Add(_T("dts"));
	m_adecode.Add(_T("ffdca"));
	m_adecode.Add(_T("ffamrnb"));
	m_adecode.Add(_T("ffamrwb"));
	m_adecode.Add(_T("hwac3"));
	m_adecode.Add(_T("hwdts"));
	m_adecode.Add(_T("ffvorbis"));
	m_adecode.Add(_T("vorbis"));

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
	m_last_extract = inner;
    	
	m_str_skip1 = ResStr(IDS_DECODE_SKIP1);
	m_str_skip2 = ResStr(IDS_DECODE_SKIP2);
	m_str_skip3 = ResStr(IDS_DECODE_SKIP3);
	m_str_skip4 = ResStr(IDS_DECODE_SKIP4);
	m_str_skip5 = ResStr(IDS_DECODE_SKIP5);

	use1 = ResStr(IDS_PLAYER_USE);
	pr1 = ResStr(IDS_DECODE_PR);
	use = ResStr(IDS_PLAYER_USE);
	name = ResStr(IDS_DECODE_NAME);
	op = ResStr(IDS_PLAYER_OPTION);
	inf = ResStr(IDS_DECODE_INF);

	m_str_avlist.Add(ResStr(IDS_DECODE_SKIPLOOP));
	m_str_avlist.Add(ResStr(IDS_DECODE_SKIPLOOP_INFO));
	m_str_avlist.Add(ResStr(IDS_DECODE_SKIPFRAME));
	m_str_avlist.Add(ResStr(IDS_DECODE_SKIPFRAME_INFO));
	m_str_avlist.Add(ResStr(IDS_DECODE_XY));
	m_str_avlist.Add(ResStr(IDS_DECODE_XY_INFO));
	m_str_avlist.Add(ResStr(IDS_DECODE_LOW));
	m_str_avlist.Add(ResStr(IDS_DECODE_LOW_INFO));
	m_str_avlist.Add(ResStr(IDS_DECODE_THREADS));
	m_str_avlist.Add(ResStr(IDS_DECODE_THREADS_INFO));

	m_str_codecs.Add(ResStr(IDS_DECODE_CODEFILE1));
	m_str_codecs.Add(ResStr(IDS_DECODE_CODEFILE2));
	m_str_codecs.Add(ResStr(IDS_DECODE_CODEFILE3));
}


void CMDecodePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMDecodePage)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_VC, m_vlist);
	DDX_Control(pDX, IDC_LIST_AC, m_alist);
	DDX_Control(pDX, IDC_LIST_AVCODEC, m_avlist);
	DDX_Control(pDX, IDC_COMBO_CODECS, m_codecs);
	DDX_Check(pDX, IDC_CHECK_THREAD, m_auto_threads);
}


BEGIN_MESSAGE_MAP(CMDecodePage, CDialog)
	//{{AFX_MSG_MAP(CMDecodePage)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_ADD2, OnButtonAdd2)
	ON_BN_CLICKED(IDC_BUTTON_DEL2, OnButtonDel2)
	ON_BN_CLICKED(IDC_BUTTON_UP2, OnButtonUp2)
	ON_BN_CLICKED(IDC_BUTTON_DOWN2, OnButtonDown2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDecodePage message handlers

int CMDecodePage::m_nColWidths[3] = { 12, 20};	// sixty-fourths
// InitListCtrl
void CMDecodePage::InitListCtrl(CXListCtrl * pList)
{
	ASSERT(pList);
	if (!pList)
		return;
	CRect rect;
	pList->GetWindowRect(&rect);
	int w = rect.Width() - 2;
	TCHAR *	lpszHeaders[] = { _tcsdup(use1.GetBuffer()) ,  _tcsdup(pr1.GetBuffer()), NULL };
	use1.ReleaseBuffer();
	pr1.ReleaseBuffer();
	int i;
	int total_cx = 0;
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	w = w - ::GetSystemMetrics(SM_CXVSCROLL);
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
void CMDecodePage::FillListCtrl(CXListCtrl * pList)
{
	// a list of some more of my favorite things

	pList->LockWindowUpdate();	// ***** lock window updates while filling list *****

	pList->DeleteAllItems();

	// insert the items and subitems into the list
	
	pList->UnlockWindowUpdate();	// ***** unlock window updates *****
}

int CMDecodePage::m_nColWidths_AV[5] = { 5, 15, 15, 50};
void CMDecodePage::InitListCtrlAV(CXListCtrl * pList)
{
	ASSERT(pList);
	if (!pList)
		return;
	CRect rect;
	pList->GetWindowRect(&rect);
	int w = rect.Width() - 2;
	TCHAR *	lpszHeaders[] = { _tcsdup(use.GetBuffer()) ,  _tcsdup(name.GetBuffer())
         ,_tcsdup(op.GetBuffer()) ,  _tcsdup(inf.GetBuffer()), NULL };
	use.ReleaseBuffer();
	name.ReleaseBuffer();
	op.ReleaseBuffer();
	inf.ReleaseBuffer();
	int i;
	int total_cx = 0;
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	w = w - ::GetSystemMetrics(SM_CXVSCROLL);
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = lpszHeaders[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = (lpszHeaders[i+1] == NULL) ? w - total_cx - 2 : (w * m_nColWidths_AV[i]) / 64;
		total_cx += lvcolumn.cx;
		pList->InsertColumn(i, &lvcolumn);
	}
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

void CMDecodePage::FillListCtrlAV(CXListCtrl * pList)
{
	pList->LockWindowUpdate();
	pList->DeleteAllItems();
	
	m_skip.RemoveAll();
	m_skip.Add(m_str_skip1);
	m_skip.Add(m_str_skip2);
	m_skip.Add(m_str_skip3);
	m_skip.Add(m_str_skip4);
	m_skip.Add(m_str_skip5);
	
	pList->InsertItem(skiploopfilter, _T(""));
	pList->SetCheckbox(skiploopfilter, 0, 0);
	pList->SetItemText(skiploopfilter, 1, m_str_avlist[skiploopfilter*2]);
	pList->SetComboBox(skiploopfilter, 2, TRUE,  &m_skip,  5,  4,  FALSE);
	pList->SetItemText(skiploopfilter, 3, m_str_avlist[skiploopfilter*2 + 1]);
	
	pList->InsertItem(skipframe, _T(""));
	pList->SetCheckbox(skipframe, 0, 0);
	pList->SetItemText(skipframe, 1, m_str_avlist[skipframe*2]);
	pList->SetComboBox(skipframe, 2, TRUE,  &m_skip,  5,  0,  FALSE);
	pList->SetItemText(skipframe, 3, m_str_avlist[skipframe*2 + 1]);

	pList->InsertItem(xy, _T(""));
	pList->SetCheckbox(xy, 0, 0);
	pList->SetItemText(xy, 1, m_str_avlist[xy*2]);
	pList->SetItemText(xy, 2, _T("720"));
	pList->SetEdit(xy, 2);
	pList->SetItemText(xy, 3,m_str_avlist[xy*2 + 1]);
	
	pList->InsertItem(lowres, _T(""));
	pList->SetCheckbox(lowres, 0, 0);
	pList->SetItemText(lowres, 1, m_str_avlist[lowres*2]);
	pList->SetItemText(lowres, 2, _T("1,1281"));
	pList->SetEdit(lowres, 2);
	pList->SetItemText(lowres, 3, m_str_avlist[lowres*2 + 1]);

	pList->InsertItem(threads, _T(""));
	pList->SetCheckbox(threads, 0, 0);
	pList->SetItemText(threads, 1, m_str_avlist[threads*2]);
	pList->SetItemText(threads, 2, _T("2"));
	pList->SetEdit(threads, 2);
	pList->SetItemText(threads, 3, m_str_avlist[threads*2 + 1]);

	pList->UnlockWindowUpdate();
}

BOOL CMDecodePage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_vlist.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_vlist.EnableToolTips(TRUE);
	InitListCtrl(&m_vlist);
	FillListCtrl(&m_vlist);
	
	m_alist.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_alist.EnableToolTips(TRUE);
	InitListCtrl(&m_alist);
	
	m_avlist.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_avlist.EnableToolTips(TRUE);
	InitListCtrlAV(&m_avlist);
	FillListCtrlAV(&m_avlist);

	for(int i = 0; i < m_str_codecs.GetCount(); i++)
		m_codecs.AddString(m_str_codecs[i]);
	m_codecs.SetCurSel(inner);
	
	InitFromConfig();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMDecodePage::SetNormal()
{
	m_avlist.SetCheckbox(skiploopfilter, 0, 0);
	m_avlist.SetComboBox(skiploopfilter, 2, TRUE,  &m_skip,  5,  4,  FALSE);
	m_avlist.SetCheckbox(skipframe, 0, 0);
	m_avlist.SetComboBox(skipframe, 2, TRUE,  &m_skip,  5,  0,  FALSE);
	m_avlist.SetCheckbox(threads, 0, 0);
	m_avlist.SetCheckbox(xy, 0, 0);
	m_avlist.SetItemText(xy, 2, _T("720"));
	m_avlist.SetCheckbox(lowres, 0, 0);
	m_avlist.SetItemText(lowres, 2, _T("1,1281"));
}

void CMDecodePage::SetHigh()
{
	SetNormal();
}

void CMDecodePage::SetLower()
{
	m_avlist.SetCheckbox(skiploopfilter, 0, 1);
	m_avlist.SetComboBox(skiploopfilter, 2, TRUE,  &m_skip,  5,  4,  FALSE);
	m_avlist.SetCheckbox(skipframe, 0, 0);
	m_avlist.SetCheckbox(xy, 0, 0);
}

void CMDecodePage::InitFromConfig()
{
	if(!m_cfg)
		return;
	int value_i;
	CString value_s;
	CString value_sub;

	if(m_cfg->GetValue_Integer(_T("auto_threads"),value_i,true))
	{
		m_auto_threads = value_i;
	}

	if(m_cfg->GetValue_Integer(_T("cofing_codecs"),value_i,true))
	{
		switch(value_i)
		{
		case 1:
			m_codecs.SetCurSel(external);
			break;
		case 2:
			m_codecs.SetCurSel(coreavc);
			break;
		default:
			m_codecs.SetCurSel(inner);
			break;
		}
	}
	else
		m_codecs.SetCurSel(inner);
	
	m_vlist.LockWindowUpdate();
	m_alist.LockWindowUpdate();
	m_avlist.LockWindowUpdate();

	if(m_cfg->GetValue_String(_T("vc"),value_s))
	{
		value_s += _T(",");
		int offset = value_s.Find(_T(","));
		while(offset > 0)
		{
			CString text = value_s.Left(offset);
			int index = -1;
			for(int i = 0; i < m_vdecode.GetSize() ; i ++)
			{
				if(m_vdecode[i] == text)
				{
					index = i;
					break;
				}
			}
			if(index >= 0)
			{
				int nItem = m_vlist.GetItemCount();
				m_vlist.InsertItem(nItem, _T(""));
				m_vlist.SetCheckbox(nItem, 0, 1);
				m_vlist.SetComboBox(nItem, 1, TRUE,  &m_vdecode,  15,  index,  FALSE);
			}
			value_s.Delete(0,offset + 1);
			offset = value_s.Find(_T(","));
		}
	}
	if(m_cfg->GetValue_String(_T("cofing_vc"),value_s,true))
	{
		value_s += _T(",");
		int offset = value_s.Find(_T(","));
		while(offset > 0)
		{
			CString text = value_s.Left(offset);
			int index = -1;
			for(int i = 0; i < m_vdecode.GetSize() ; i ++)
			{
				if(m_vdecode[i] == text)
				{
					index = i;
					break;
				}
			}
			if(index >= 0)
			{
				int nItem = m_vlist.GetItemCount();
				m_vlist.InsertItem(nItem, _T(""));
				m_vlist.SetCheckbox(nItem, 0, 0);
				m_vlist.SetComboBox(nItem, 1, TRUE,  &m_vdecode,  15,  index,  FALSE);
			}
			value_s.Delete(0,offset + 1);
			offset = value_s.Find(_T(","));
		}
	}
	
	if(m_cfg->GetValue_String(_T("ac"),value_s))
	{
		value_s += _T(",");
		int offset = value_s.Find(_T(","));
		while(offset > 0)
		{
			CString text = value_s.Left(offset);
			int index = -1;
			for(int i = 0; i < m_adecode.GetSize() ; i ++)
			{
				if(m_adecode[i] == text)
				{
					index = i;
					break;
				}
			}
			if(index >= 0)
			{
				int nItem = m_alist.GetItemCount();
				m_alist.InsertItem(nItem, _T(""));
				m_alist.SetCheckbox(nItem, 0, 1);
				m_alist.SetComboBox(nItem, 1, TRUE,  &m_adecode,  15,  index,  FALSE);
			}
			value_s.Delete(0,offset + 1);
			offset = value_s.Find(_T(","));
		}
	}
	if(m_cfg->GetValue_String(_T("cofing_ac"),value_s,true))
	{
		value_s += _T(",");
		int offset = value_s.Find(_T(","));
		while(offset > 0)
		{
			CString text = value_s.Left(offset);
			int index = -1;
			for(int i = 0; i < m_adecode.GetSize() ; i ++)
			{
				if(m_adecode[i] == text)
				{
					index = i;
					break;
				}
			}
			if(index >= 0)
			{
				int nItem = m_alist.GetItemCount();
				m_alist.InsertItem(nItem, _T(""));
				m_alist.SetCheckbox(nItem, 0, 0);
				m_alist.SetComboBox(nItem, 1, TRUE,  &m_adecode,  15,  index,  FALSE);
			}
			value_s.Delete(0,offset + 1);
			offset = value_s.Find(_T(","));
		}
	}

	if(m_cfg->GetValue_String(_T("lavdopts"),value_s))
	{
//		if(m_cfg->HaveSubValue(value_s,_T("fast"),_T(":")))
//			m_avlist.SetCheckbox(fast, 0, 1);
//		if(m_cfg->HaveSubValue(value_s,_T("gray"),_T(":")))
//			m_avlist.SetCheckbox(gray, 0, 1);
//		if(m_cfg->HaveSubValue(value_s,_T("sb"),_T(":")))
//			m_avlist.SetCheckbox(sb, 0, 1);
//		if(m_cfg->HaveSubValue(value_s,_T("st"),_T(":")))
//			m_avlist.SetCheckbox(st, 0, 1);
		if(m_cfg->GetSubValue(value_s,_T("skiploopfilter"),value_sub,0,_T(":")))
		{
			m_avlist.SetCheckbox(skiploopfilter, 0, 1);
			if(value_sub == _T("default"))
				m_avlist.SetComboBox(skiploopfilter, 2, TRUE,  &m_skip,  5,  0,  FALSE);
			else if(value_sub == _T("nonref"))
				m_avlist.SetComboBox(skiploopfilter, 2, TRUE,  &m_skip,  5,  1,  FALSE);
			else if(value_sub == _T("bidir"))
				m_avlist.SetComboBox(skiploopfilter, 2, TRUE,  &m_skip,  5,  2,  FALSE);
			else if(value_sub == _T("nonkey"))
				m_avlist.SetComboBox(skiploopfilter, 2, TRUE,  &m_skip,  5,  3,  FALSE);
			else
				m_avlist.SetComboBox(skiploopfilter, 2, TRUE,  &m_skip,  5,  4,  FALSE);
		}
		if(m_cfg->GetSubValue(value_s,_T("skipframe"),value_sub,0,_T(":")))
		{
			m_avlist.SetCheckbox(skipframe, 0, 1);
			if(value_sub == _T("all"))
				m_avlist.SetComboBox(skipframe, 2, TRUE,  &m_skip,  5,  4,  FALSE);
			else if(value_sub == _T("nonref"))
				m_avlist.SetComboBox(skipframe, 2, TRUE,  &m_skip,  5,  1,  FALSE);
			else if(value_sub == _T("bidir"))
				m_avlist.SetComboBox(skipframe, 2, TRUE,  &m_skip,  5,  2,  FALSE);
			else if(value_sub == _T("nonkey"))
				m_avlist.SetComboBox(skipframe, 2, TRUE,  &m_skip,  5,  3,  FALSE);
			else
				m_avlist.SetComboBox(skipframe, 2, TRUE,  &m_skip,  5,  0,  FALSE);
		}
		if(m_cfg->GetSubValue(value_s,_T("lowres"),value_sub,0,_T(":")))
		{
			m_avlist.SetCheckbox(lowres, 0, 1);
			m_avlist.SetItemText(lowres, 2, value_sub);
		}
		if(m_cfg->GetSubValue(value_s,_T("threads"),value_sub,0,_T(":")))
		{
			m_avlist.SetCheckbox(threads, 0, 1);
			m_avlist.SetItemText(threads, 2, value_sub);
		}
	}
	if(m_cfg->GetValue_String(_T("xy"),value_s))
	{
		m_avlist.SetCheckbox(xy, 0, 1);
		m_avlist.SetItemText(xy, 2, value_s);
	}

	m_vlist.UnlockWindowUpdate();
	m_alist.UnlockWindowUpdate();
	m_avlist.UnlockWindowUpdate();
	UpdateData(FALSE);
}

void CMDecodePage::RegCoreCodec()
{
	CString regfile = m_program_dir + _T("codecs\\CoreAVCDecoder.reg");
	if(!IsFileExist(regfile))
		return;
	CReg reg;
	CString SubKey = _T("SOFTWARE\\CoreCodec\\CoreAVC Pro");
	CString Name = _T("User");
	bool regIt = false;
	if(reg.ShowContent_STR(HKEY_LOCAL_MACHINE,SubKey,Name))
	{
		CString Content;
		Content.Format(_T("%s"),reg.content);
		if (Content.Find(_T("Registered User")) < 0)
			regIt = true;
	}
	else
		regIt = true;

	if(regIt)
		ShellExecute(0, _T("open"), _T("regedit"), _T(" /s \"")+regfile+_T("\""), NULL, SW_HIDE);

}

void CMDecodePage::SaveConfig()
{
	if(!m_cfg)
		return;
	UpdateData(TRUE);
	if(!IsFileExist(m_program_dir + _T("codecs.ini")))
		m_last_extract =  inner;
	int vcodecs = m_codecs.GetCurSel();
	switch (vcodecs)
	{
	case external:
		m_cfg->SetValue(_T("cofing_codecs"), _T("1"), true , ex_meditor);
		if(m_last_extract !=  external)
			ExtractResource(MAKEINTRESOURCE(IDZ_CODECS),TEXT("CODECS_INI")
				,m_program_dir + _T("codecs.ini"),true,true,_T("codecs.conf"));
		m_last_extract =  external;
		break;
	case coreavc:
		RegCoreCodec();
		m_cfg->SetValue(_T("cofing_codecs") , _T("2") , true , ex_meditor);
		if(m_last_extract !=  coreavc)
			ExtractResource(MAKEINTRESOURCE(IDZ_CODECS),TEXT("CODECS_INI")
				,m_program_dir + _T("codecs.ini"),true,true,_T("codecs.coreavc.conf"));
		m_last_extract =  coreavc;
		break;
	default:
		m_cfg->RemoveValue(_T("cofing_codecs"), true);
		DeleteFile(m_program_dir + _T("codecs.ini"));
		m_last_extract = inner;
		break;
	}

	if(m_auto_threads)
		m_cfg->RemoveValue(_T("auto_threads"), true);
	else
		m_cfg->SetValue(_T("auto_threads"), _T("0"), true , ex_option);

	m_vlist.LockWindowUpdate();
	m_alist.LockWindowUpdate();
	m_avlist.LockWindowUpdate();
	
	CString vc_str = _T("");
	CString vc_str_meditor = _T("");
	for(int i = 0; i < m_vlist.GetItemCount() ; i ++)
	{
		if(m_vlist.GetCheckbox(i, 0))
			vc_str +=  m_vlist.GetItemText(i, 1) + _T(",");
		else
			vc_str_meditor +=  m_vlist.GetItemText(i, 1) + _T(",");
	}
	if(vc_str.GetLength() > 2)
		m_cfg->SetValue(_T("vc"),  vc_str);
	else
	{
		m_cfg->SetValue(_T("vc"),  _T(""));
		m_cfg->RemoveValue(_T("vc"));
	}
	if(vc_str_meditor.GetLength() > 2)
		m_cfg->SetValue(_T("cofing_vc"),  vc_str_meditor,true , ex_meditor);
	else
		m_cfg->RemoveValue(_T("cofing_vc"),true);


	CString ac_str = _T("");
	CString ac_str_meditor = _T("");
	for(int j = 0; j < m_alist.GetItemCount() ; j ++)
	{
		if(m_alist.GetCheckbox(j, 0))
			ac_str +=  m_alist.GetItemText(j, 1) + _T(",");
		else
			ac_str_meditor +=  m_alist.GetItemText(j, 1) + _T(",");
	}	
	if(ac_str.GetLength() > 2)
		m_cfg->SetValue(_T("ac"),  ac_str);
	else
		m_cfg->RemoveValue(_T("ac"));
	if(ac_str_meditor.GetLength() > 2)
		m_cfg->SetValue(_T("cofing_ac"),  ac_str_meditor,true , ex_meditor);
	else
		m_cfg->RemoveValue(_T("cofing_ac"),true);

	if(m_avlist.GetCheckbox(xy, 0))
	{
		CString str = m_avlist.GetItemText(xy, 2);
		str.TrimLeft(_T(" "));
		str.TrimRight(_T(" "));
		int tr = _ttoi(str);
		if(tr > 0 && str.GetLength() >= 1)
			m_cfg->SetValue(_T("xy"),  str);
		else
			m_cfg->RemoveValue(_T("xy"));
	}
	else
		m_cfg->RemoveValue(_T("xy"));
	
	CString av_str = _T("");
	if(m_avlist.GetCheckbox(skiploopfilter, 0))
	{
		CString str = m_avlist.GetItemText(skiploopfilter, 2);
		str.TrimLeft(_T(" "));
		str.TrimRight(_T(" "));
		if(str == m_str_skip1)
			av_str += _T("skiploopfilter=default:") ;
		else if(str == m_str_skip2)
			av_str += _T("skiploopfilter=nonref:");
		else if(str == m_str_skip3)
			av_str += _T("skiploopfilter=bidir:");
		else if(str == m_str_skip4)
			av_str += _T("skiploopfilter=nonkey:");
		else
			av_str += _T("skiploopfilter=all:");
	}
	if(m_avlist.GetCheckbox(skipframe, 0))
	{
		CString str = m_avlist.GetItemText(skipframe, 2);
		str.TrimLeft(_T(" "));
		str.TrimRight(_T(" "));
		if(str == m_str_skip1)
			av_str += _T("skipframe=default:") ;
		else if(str == m_str_skip2)
			av_str += _T("skipframe=nonref:");
		else if(str == m_str_skip3)
			av_str += _T("skipframe=bidir:");
		else if(str == m_str_skip4)
			av_str += _T("skipframe=nonkey:");
		else
			av_str += _T("skipframe=all:");		
	}
	if(m_avlist.GetCheckbox(lowres, 0))
	{
		CString str = m_avlist.GetItemText(lowres, 2);
		str.TrimLeft(_T(" "));
		str.TrimRight(_T(" "));
		if(str.GetLength() > 0)
			av_str += _T("lowres=") + str + _T(":");
	}
	if(m_avlist.GetCheckbox(threads, 0))
	{
		CString str = m_avlist.GetItemText(threads, 2);
		str.TrimLeft(_T(" "));
		str.TrimRight(_T(" "));
		int tr = _ttoi(str);
		if(tr > 0 && tr < 9 && str.GetLength() == 1)
			av_str += _T("threads=") + str + _T(":");
	}
//	if(m_avlist.GetCheckbox(fast, 0))
//		av_str += _T("fast:");
//	if(m_avlist.GetCheckbox(gray, 0))
//		av_str += _T("gray:");
//	if(m_avlist.GetCheckbox(sb, 0))
//		av_str += _T("sb:");
//	if(m_avlist.GetCheckbox(st, 0))
//		av_str += _T("st:");
	av_str.TrimRight(_T(":"));
	if(av_str.GetLength() > 1)
		m_cfg->SetValue(_T("lavdopts"),  av_str);
	else
		m_cfg->RemoveValue(_T("lavdopts"));

	m_vlist.UnlockWindowUpdate();
	m_alist.UnlockWindowUpdate();
	m_avlist.UnlockWindowUpdate();

}

void CMDecodePage::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	m_vlist.LockWindowUpdate();
	int size = m_vlist.GetItemCount();
	m_vlist.InsertItem(size, _T(""));
	m_vlist.SetCheckbox(size, 0, 0);
	m_vlist.SetComboBox(size, 1, TRUE,  &m_vdecode,  15,  0,  FALSE);
	m_vlist.UnlockWindowUpdate();
}

void CMDecodePage::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	m_vlist.LockWindowUpdate();
	int nItem = m_vlist.GetCurSel();
	if(nItem >= 0)
		m_vlist.DeleteItem(nItem);
	m_vlist.UnlockWindowUpdate();	
}

void CMDecodePage::OnButtonUp() 
{
	// TODO: Add your control notification handler code here
	m_vlist.LockWindowUpdate();
	int nItem = m_vlist.GetCurSel();
	if(nItem >= 1)
	{
		int checked = m_vlist.GetCheckbox(nItem,0);
		CString text = m_vlist.GetComboText(nItem,1);
		int index = 0;
		for(int i = 0; i < m_vdecode.GetSize() ; i ++)
		{
			if(m_vdecode[i] == text)
			{
				index = i;
				break;
			}
		}
		m_vlist.DeleteItem(nItem);
		m_vlist.InsertItem(nItem - 1, _T(""));
		m_vlist.SetCheckbox(nItem - 1, 0, checked);
		m_vlist.SetComboBox(nItem - 1, 1, TRUE,  &m_vdecode,  15,  index,  FALSE);
		m_vlist.SetCurSel(nItem - 1);
	}
	m_vlist.UnlockWindowUpdate();
}

void CMDecodePage::OnButtonDown() 
{
	// TODO: Add your control notification handler code here
	m_vlist.LockWindowUpdate();
	int size = m_vlist.GetItemCount() - 1;
	int nItem = m_vlist.GetCurSel();
	if(nItem < size)
	{
		int checked = m_vlist.GetCheckbox(nItem,0);
		CString text = m_vlist.GetComboText(nItem,1);
		int index = 0;
		for(int i = 0; i < m_vdecode.GetSize() ; i ++)
		{
			if(m_vdecode[i] == text)
			{
				index = i;
				break;
			}
		}
		m_vlist.DeleteItem(nItem);
		m_vlist.InsertItem(nItem + 1, _T(""));
		m_vlist.SetCheckbox(nItem + 1, 0, checked);
		m_vlist.SetComboBox(nItem + 1, 1, TRUE,  &m_vdecode,  15,  index,  FALSE);
		m_vlist.SetCurSel(nItem + 1);
	}
	m_vlist.UnlockWindowUpdate();	
}

void CMDecodePage::OnButtonAdd2() 
{
	// TODO: Add your control notification handler code here
	m_alist.LockWindowUpdate();
	int size = m_alist.GetItemCount();
	m_alist.InsertItem(size, _T(""));
	m_alist.SetCheckbox(size, 0, 0);
	m_alist.SetComboBox(size, 1, TRUE,  &m_adecode,  15,  0,  FALSE);
	m_alist.UnlockWindowUpdate();	
}

void CMDecodePage::OnButtonDel2() 
{
	// TODO: Add your control notification handler code here
	m_alist.LockWindowUpdate();
	int nItem = m_alist.GetCurSel();
	if(nItem >= 0)
		m_alist.DeleteItem(nItem);
	m_alist.UnlockWindowUpdate();
}

void CMDecodePage::OnButtonUp2() 
{
	// TODO: Add your control notification handler code here
	m_alist.LockWindowUpdate();
	int nItem = m_alist.GetCurSel();
	if(nItem >= 1)
	{
		int checked = m_alist.GetCheckbox(nItem,0);
		CString text = m_alist.GetComboText(nItem,1);
		int index = 0;
		for(int i = 0; i < m_adecode.GetSize() ; i ++)
		{
			if(m_adecode[i] == text)
			{
				index = i;
				break;
			}
		}
		m_alist.DeleteItem(nItem);
		m_alist.InsertItem(nItem - 1, _T(""));
		m_alist.SetCheckbox(nItem - 1, 0, checked);
		m_alist.SetComboBox(nItem - 1, 1, TRUE,  &m_adecode,  15,  index,  FALSE);
		m_alist.SetCurSel(nItem - 1);
	}
	m_alist.UnlockWindowUpdate();
}

void CMDecodePage::OnButtonDown2() 
{
	// TODO: Add your control notification handler code here
	m_alist.LockWindowUpdate();
	int size = m_alist.GetItemCount() - 1;
	int nItem = m_alist.GetCurSel();
	if(nItem < size)
	{
		int checked = m_alist.GetCheckbox(nItem,0);
		CString text = m_alist.GetComboText(nItem,1);
		int index = 0;
		for(int i = 0; i < m_adecode.GetSize() ; i ++)
		{
			if(m_adecode[i] == text)
			{
				index = i;
				break;
			}
		}
		m_alist.DeleteItem(nItem);
		m_alist.InsertItem(nItem + 1, _T(""));
		m_alist.SetCheckbox(nItem + 1, 0, checked);
		m_alist.SetComboBox(nItem + 1, 1, TRUE,  &m_adecode,  15,  index,  FALSE);
		m_alist.SetCurSel(nItem + 1);
	}
	m_alist.UnlockWindowUpdate();	
}

BOOL CMDecodePage::PreTranslateMessage(MSG* pMsg) 
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

