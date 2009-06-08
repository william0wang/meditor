// MProfilePage.cpp : 实现文件
//

#include "stdafx.h"
#include "meditor2.h"
#include "MProfilePage.h"
#include "MConfig.h"
#include "MProfileDlg.h"


// CMProfilePage 对话框
enum Profile_list
{
	flvsync,
	slang,
	alang,
	adv_af,
	screenshot,
	ass,
	expand,
	aspect,
	eq2,
	hue,
	flip,
	rotate,
	crop,
	deinterlacing,
	deblocking,
	dering,
	autolevels,
	volnormal,
	resample,
	equalizer,
	audiodelay,
};

enum Deinterlacing_Profile
{
	di_lb,
	di_ci,
	di_md,
	di_fd,
	di_l5,
	di_yadif
};

enum Deblocking_Profile
{
	db_hb,
	db_vb,
	db_ha,
	db_va,
	db_b,
	db_a
};

IMPLEMENT_DYNAMIC(CMProfilePage, CDialog)

CMProfilePage::CMProfilePage(CWnd* pParent /*=NULL*/)
	: CDialog(CMProfilePage::IDD, pParent)
	, m_profile(_T(""))
{
	m_cfg = NULL;
	m_str_at = ResStr(IDS_PLAYER_AUTO);
	m_str_low = ResStr(IDS_AUDIO_RELOW);
	m_str_int = ResStr(IDS_AUDIO_REINT);
	m_str_foa = ResStr(IDS_AUDIO_REFOA);
	m_str_nco = ResStr(IDS_SUB_NORMAL_COL);
	use = ResStr(IDS_PLAYER_USE);
	nam = ResStr(IDS_DECODE_NAME);
	opt = ResStr(IDS_PLAYER_OPTION);
	inf = ResStr(IDS_DECODE_INF);
	m_str_vet = ResStr(IDS_VIDEO_EXPIT);
	m_str_cot = ResStr(IDS_VIDEO_CROIT);

	m_adv_af.Add(ResStr(IDS_AUDIO_AFAT));
	m_adv_af.Add(ResStr(IDS_AUDIO_AFH));
	m_adv_af.Add(ResStr(IDS_AUDIO_AFF));

	m_rotate.Add(ResStr(IDS_VIDEO_ROTA1));	
	m_rotate.Add(ResStr(IDS_VIDEO_ROTA2));
	m_rotate.Add(ResStr(IDS_VIDEO_ROTA3));
	m_rotate.Add(ResStr(IDS_VIDEO_ROTA4));

	m_deinterlacing.Add(ResStr(IDS_VIDEO_VD1));
	m_deinterlacing.Add(ResStr(IDS_VIDEO_VD2));
	m_deinterlacing.Add(ResStr(IDS_VIDEO_VD3));
	m_deinterlacing.Add(_T("FFmpeg"));
	m_deinterlacing.Add(_T("lowpass5"));

	m_deblocking.Add(ResStr(IDS_VIDEO_DB1));
	m_deblocking.Add(ResStr(IDS_VIDEO_DB2));
	m_deblocking.Add(ResStr(IDS_VIDEO_DB3));
	m_deblocking.Add(ResStr(IDS_VIDEO_DB4));
	m_deblocking.Add(ResStr(IDS_VIDEO_DB5));
	m_deblocking.Add(ResStr(IDS_VIDEO_DB6));


	m_volnormal.Add(ResStr(IDS_AUDIO_RS));
	m_volnormal.Add(ResStr(IDS_AUDIO_RM));

	m_resample.Add(_T("8kHz ") + m_str_at);
	m_resample.Add(_T("8kHz ") + m_str_low);
	m_resample.Add(_T("8kHz ") + m_str_int);
	m_resample.Add(_T("8kHz ") + m_str_foa);
	m_resample.Add(_T("16kHz ") + m_str_at);
	m_resample.Add(_T("16kHz ") + m_str_low);
	m_resample.Add(_T("16kHz ") + m_str_int);
	m_resample.Add(_T("16kHz ") + m_str_foa);
	m_resample.Add(_T("22.05kHz ") + m_str_at);
	m_resample.Add(_T("22.05kHz ") + m_str_low);
	m_resample.Add(_T("22.05kHz ") + m_str_int);
	m_resample.Add(_T("22.05kHz ") + m_str_foa);
	m_resample.Add(_T("32kHz ") + m_str_at);
	m_resample.Add(_T("32kHz ") + m_str_low);
	m_resample.Add(_T("32kHz ") + m_str_int);
	m_resample.Add(_T("32kHz ") + m_str_foa);
	m_resample.Add(_T("44.1kHz ") + m_str_at);
	m_resample.Add(_T("44.1kHz ") + m_str_low);
	m_resample.Add(_T("44.1kHz ") + m_str_int);
	m_resample.Add(_T("44.1kHz ") + m_str_foa);
	m_resample.Add(_T("48kHz ") + m_str_at);
	m_resample.Add(_T("48kHz ") + m_str_low);
	m_resample.Add(_T("48kHz ") + m_str_int);
	m_resample.Add(_T("48kHz ") + m_str_foa);
	m_resample.Add(_T("64kHz ") + m_str_at);
	m_resample.Add(_T("64kHz ") + m_str_low);
	m_resample.Add(_T("64kHz ") + m_str_int);
	m_resample.Add(_T("64kHz ") + m_str_foa);
	m_resample.Add(_T("96kHz ") + m_str_at);
	m_resample.Add(_T("96kHz ") + m_str_low);
	m_resample.Add(_T("96kHz ") + m_str_int);
	m_resample.Add(_T("96kHz ") + m_str_foa);

	m_str_profile.Add(ResStr(IDS_PROFILE_FLV));
	m_str_profile.Add(ResStr(IDS_PROFILE_FLV_INFO));
	m_str_profile.Add(ResStr(IDS_PROFILE_SLN));
	m_str_profile.Add(ResStr(IDS_PROFILE_SLN_INFO));
	m_str_profile.Add(ResStr(IDS_PROFILE_ALN));
	m_str_profile.Add(ResStr(IDS_PROFILE_ALN_INFO));
	m_str_profile.Add(ResStr(IDS_AUDIO_ADV));
	m_str_profile.Add(ResStr(IDS_AUDIO_ADV_INFO));
	m_str_profile.Add(ResStr(IDS_VIDEO_SST));
	m_str_profile.Add(ResStr(IDS_VIDEO_SSTI));
	m_str_profile.Add(ResStr(IDS_VIDEO_ASS));
	m_str_profile.Add(ResStr(IDS_VIDEO_ASSI));
	m_str_profile.Add(ResStr(IDS_VIDEO_EXP));
	m_str_profile.Add(ResStr(IDS_VIDEO_EXPI));
	m_str_profile.Add(ResStr(IDS_VIDEO_ASP));
	m_str_profile.Add(ResStr(IDS_VIDEO_ASPI));
	m_str_profile.Add(ResStr(IDS_VIDEO_EQ2));
	m_str_profile.Add(ResStr(IDS_VIDEO_EQ2I));
	m_str_profile.Add(ResStr(IDS_VIDEO_HUE));
	m_str_profile.Add(ResStr(IDS_VIDEO_HUEI));
	m_str_profile.Add(ResStr(IDS_VIDEO_FLP));
	m_str_profile.Add(ResStr(IDS_VIDEO_FLPI));
	m_str_profile.Add(ResStr(IDS_VIDEO_ROT));
	m_str_profile.Add(ResStr(IDS_VIDEO_ROTI));
	m_str_profile.Add(ResStr(IDS_VIDEO_CRO));
	m_str_profile.Add(ResStr(IDS_VIDEO_CROI));
	m_str_profile.Add(ResStr(IDS_VIDEO_DEI));
	m_str_profile.Add(ResStr(IDS_VIDEO_DEII));
	m_str_profile.Add(ResStr(IDS_VIDEO_DEB));
	m_str_profile.Add(ResStr(IDS_VIDEO_DEBI));
	m_str_profile.Add(ResStr(IDS_VIDEO_DER));
	m_str_profile.Add(ResStr(IDS_VIDEO_DERI));
	m_str_profile.Add(ResStr(IDS_VIDEO_ATL));
	m_str_profile.Add(ResStr(IDS_VIDEO_ATLI));
	m_str_profile.Add(ResStr(IDS_AUDIO_VNL));
	m_str_profile.Add(ResStr(IDS_AUDIO_VNL_INFO));
	m_str_profile.Add(ResStr(IDS_AUDIO_RE));
	m_str_profile.Add(ResStr(IDS_AUDIO_RE_INFO));
	m_str_profile.Add(ResStr(IDS_AUDIO_EQ));
	m_str_profile.Add(ResStr(IDS_AUDIO_EQ_INFO));
	m_str_profile.Add(ResStr(IDS_AUDIO_DELAY));
	m_str_profile.Add(ResStr(IDS_AUDIO_DELAY_INFO));

}

CMProfilePage::~CMProfilePage()
{
}

void CMProfilePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROFILE, m_List);
	DDX_Control(pDX, IDC_COMBO_EXTENSINO, m_profile_list);
	DDX_AMCBString(pDX, IDC_COMBO_EXTENSINO, m_profile);
}


BEGIN_MESSAGE_MAP(CMProfilePage, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_EXTENSINO, &CMProfilePage::OnCbnSelchangeComboExtensino)
	ON_BN_CLICKED(IDC_DEL, &CMProfilePage::OnBnClickedDel)
	ON_BN_CLICKED(IDC_ADD, &CMProfilePage::OnBnClickedAdd)
END_MESSAGE_MAP()


// CMProfilePage 消息处理程序
int CMProfilePage::m_nColWidths[5] = { 5, 15, 15, 50};	// sixty-fourths

void CMProfilePage::InitListCtrl(CXListCtrl * pList)
{
	ASSERT(pList);
	if (!pList)
		return;

	// set column width according to window rect
	CRect rect;
	pList->GetWindowRect(&rect);

	int w = rect.Width() - 2;

	TCHAR *	lpszHeaders[] = { _tcsdup(use.GetBuffer()) ,  _tcsdup(nam.GetBuffer())
		,_tcsdup(opt.GetBuffer()) ,  _tcsdup(inf.GetBuffer()),  NULL };
	use.ReleaseBuffer();
	nam.ReleaseBuffer();
	opt.ReleaseBuffer();
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

void CMProfilePage::FillListCtrl(CXListCtrl * pList)
{
	// a list of some more of my favorite things

	pList->LockWindowUpdate();

	pList->DeleteAllItems();

	// insert the items and subitems into the list
	pList->InsertItem(flvsync, _T(""));
	pList->SetCheckbox(flvsync, 0, 0);
	pList->SetItemText(flvsync, 1, m_str_profile[flvsync*2]);
	pList->SetItemText(flvsync, 2, _T(""));
	pList->SetItemText(flvsync, 3, m_str_profile[flvsync*2 + 1]);

	pList->InsertItem(slang, _T(""));
	pList->SetCheckbox(slang, 0, 0);
	pList->SetItemText(slang, 1, m_str_profile[slang*2]);
	pList->SetItemText(slang, 2, _T("zh,ch,tw"));
	pList->SetEdit(slang, 2);
	pList->SetItemText(slang, 3, m_str_profile[slang*2 + 1]);

	pList->InsertItem(alang, _T(""));
	pList->SetCheckbox(alang, 0, 0);
	pList->SetItemText(alang, 1, m_str_profile[alang*2]);
	pList->SetItemText(alang, 2, _T("zh,ch,tw"));
	pList->SetEdit(alang, 2);
	pList->SetItemText(alang, 3, m_str_profile[alang*2 + 1]);

	pList->InsertItem(adv_af, _T(""));
	pList->SetCheckbox(adv_af, 0, 0);
	pList->SetItemText(adv_af, 1, m_str_profile[adv_af*2]);
	pList->SetComboBox(adv_af, 2, TRUE,  &m_adv_af,  5,  1,  FALSE);
	pList->SetItemText(adv_af, 3, m_str_profile[adv_af*2 + 1]);

	pList->InsertItem(screenshot, _T(""));
	pList->SetCheckbox(screenshot, 0, 0);
	pList->SetItemText(screenshot, 1, m_str_profile[screenshot*2]);
	pList->SetItemText(screenshot, 2, _T(""));
	pList->SetItemText(screenshot, 3, m_str_profile[screenshot*2 + 1]);

	pList->InsertItem(ass, _T(""));
	pList->SetCheckbox(ass, 0, 0);
	pList->SetItemText(ass, 1, m_str_profile[ass*2]);
	pList->SetItemText(ass, 2, _T(""));
	pList->SetItemText(ass, 3, m_str_profile[ass*2 + 1]);

	pList->InsertItem(expand, _T(""));
	pList->SetCheckbox(expand, 0, 0);
	pList->SetItemText(expand, 1,m_str_profile[expand*2]);
	pList->SetItemText(expand, 2, _T(":::::4/3"));
	pList->SetEdit(expand, 2);
	pList->SetItemText(expand, 3, m_str_profile[expand*2 + 1]);
	pList->SetItemToolTipText(expand, 2, m_str_vet);

	pList->InsertItem(aspect, _T(""));
	pList->SetCheckbox(aspect, 0, 0);
	pList->SetItemText(aspect, 1, m_str_profile[aspect*2]);
	pList->SetItemText(aspect, 2, _T("4:3"));
	pList->SetEdit(aspect, 2);
	pList->SetItemText(aspect, 3, m_str_profile[aspect*2 + 1]);

	pList->InsertItem(eq2, _T(""));
	pList->SetCheckbox(eq2, 0, 0);
	pList->SetItemText(eq2, 1, m_str_profile[eq2*2]);
	pList->SetItemText(eq2, 2, _T(""));
	pList->SetItemText(eq2, 3, m_str_profile[eq2*2 + 1]);

	pList->InsertItem(hue, _T(""));
	pList->SetCheckbox(hue, 0, 0);
	pList->SetItemText(hue, 1, m_str_profile[hue*2]);
	pList->SetItemText(hue, 3, m_str_profile[hue*2 + 1]);

	pList->InsertItem(flip, _T(""));
	pList->SetCheckbox(flip, 0, 0);
	pList->SetItemText(flip, 1, m_str_profile[flip*2]);
	pList->SetItemText(flip, 2, _T(""));
	pList->SetItemText(flip, 3, m_str_profile[flip*2 + 1]);

	pList->InsertItem(rotate, _T(""));
	pList->SetCheckbox(rotate, 0, 0);
	pList->SetItemText(rotate, 1, m_str_profile[rotate*2]);
	pList->SetComboBox(rotate, 2, TRUE,  &m_rotate,  5,  0,  FALSE);
	pList->SetItemText(rotate, 3, m_str_profile[rotate*2 + 1]);

	pList->InsertItem(crop, _T(""));
	pList->SetCheckbox(crop, 0, 0);
	pList->SetItemText(crop, 1, m_str_profile[crop*2]);
	pList->SetItemText(crop, 2, _T("640:480"));
	pList->SetEdit(crop, 2);
	pList->SetItemText(crop, 3, m_str_profile[crop*2 + 1]);
	pList->SetItemToolTipText(crop, 2, m_str_cot);

	pList->InsertItem(deinterlacing, _T(""));
	pList->SetCheckbox(deinterlacing, 0, 0);
	pList->SetItemText(deinterlacing, 1, m_str_profile[deinterlacing*2]);
	pList->SetComboBox(deinterlacing, 2, TRUE,  &m_deinterlacing,  5,  0,  FALSE);
	pList->SetItemText(deinterlacing, 3, m_str_profile[deinterlacing*2 + 1]);

	pList->InsertItem(deblocking, _T(""));
	pList->SetCheckbox(deblocking, 0, 0);
	pList->SetItemText(deblocking, 1, m_str_profile[deblocking*2]);
	pList->SetComboBox(deblocking, 2, TRUE,  &m_deblocking,  6,  0,  FALSE);
	pList->SetItemText(deblocking, 3, m_str_profile[deblocking*2 + 1]);

	pList->InsertItem(dering, _T(""));
	pList->SetCheckbox(dering, 0, 0);
	pList->SetItemText(dering, 1, m_str_profile[dering*2]);
	pList->SetItemText(dering, 2, _T(""));
	pList->SetItemText(dering, 3, m_str_profile[dering*2 + 1]);

	pList->InsertItem(autolevels, _T(""));
	pList->SetCheckbox(autolevels, 0, 0);
	pList->SetItemText(autolevels, 1, m_str_profile[autolevels*2]);
	pList->SetItemText(autolevels, 2, _T(""));
	pList->SetItemText(autolevels, 3, m_str_profile[autolevels*2 + 1]);

	pList->InsertItem(volnormal, _T(""));
	pList->SetCheckbox(volnormal, 0, 0);
	pList->SetItemText(volnormal, 1, m_str_profile[volnormal*2]);
	pList->SetComboBox(volnormal, 2, TRUE,  &m_volnormal,  5,  0,  FALSE);
	pList->SetItemText(volnormal, 3, m_str_profile[volnormal*2 + 1]);

	pList->InsertItem(resample, _T(""));
	pList->SetCheckbox(resample, 0, 0);
	pList->SetItemText(resample, 1, m_str_profile[resample*2]);
	pList->SetComboBox(resample, 2, TRUE,  &m_resample,  5,  16,  FALSE);
	pList->SetItemText(resample, 3, m_str_profile[resample*2 + 1]);

	pList->InsertItem(equalizer, _T(""));
	pList->SetCheckbox(equalizer, 0, 0);
	pList->SetItemText(equalizer, 1, m_str_profile[equalizer*2]);
	pList->SetItemText(equalizer, 2, _T("0:0:0:0:0:0:0:0:0:0"));
	pList->SetEdit(equalizer, 2);
	pList->SetItemText(equalizer, 3, m_str_profile[equalizer*2 + 1]);

	pList->InsertItem(audiodelay, _T(""));
	pList->SetCheckbox(audiodelay, 0, 0);
	pList->SetItemText(audiodelay, 1, m_str_profile[audiodelay*2]);
	pList->SetItemText(audiodelay, 2, _T(""));
	pList->SetEdit(audiodelay, 2);
	pList->SetItemText(audiodelay, 3, m_str_profile[audiodelay*2 + 1]);

	pList->UnlockWindowUpdate();
}

BOOL CMProfilePage::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.EnableToolTips(TRUE);
	InitListCtrl(&m_List);
	FillListCtrl(&m_List);

	if(m_cfg)
	{
		CString type;
		size_t size = m_cfg->GetSizeSp();
		for(size_t i = 0 ; i < size ; i++)
		{
			if(m_cfg->GetTypeSp(i , type))
				m_profile_list.AddString(type);
		}

		if(m_profile_list.GetCount() > 0)
		{
			m_profile_list.SetCurSel(0);
			SetCurConfig();
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMProfilePage::CleanConfig()
{
	m_List.LockWindowUpdate();
	m_List.SetCheckbox(screenshot, 0, 0);
	m_List.SetCheckbox(ass, 0, 0);
	m_List.SetCheckbox(expand, 0, 0);
	m_List.SetItemText(expand, 2, _T(":::::4/3"));	
	m_List.SetCheckbox(aspect, 0, 0);
	m_List.SetItemText(aspect, 2, _T("4:3"));
	m_List.SetCheckbox(eq2, 0, 0);	
	m_List.SetCheckbox(hue, 0, 0);	
	m_List.SetCheckbox(flip, 0, 0);	
	m_List.SetCheckbox(rotate, 0, 0);
	m_List.SetComboBox(rotate, 2, TRUE,  &m_rotate,  5,  0,  FALSE);	
	m_List.SetCheckbox(crop, 0, 0);
	m_List.SetItemText(crop, 2, _T("640:480"));	
	m_List.SetCheckbox(deinterlacing, 0, 0);
	m_List.SetComboBox(deinterlacing, 2, TRUE,  &m_deinterlacing,  5,  0,  FALSE);	
	m_List.SetCheckbox(deblocking, 0, 0);
	m_List.SetComboBox(deblocking, 2, TRUE,  &m_deblocking,  6,  0,  FALSE);	
	m_List.SetCheckbox(dering, 0, 0);	
	m_List.SetCheckbox(autolevels, 0, 0);
	m_List.SetCheckbox(adv_af, 0, 0);
	m_List.SetComboBox(adv_af, 2, TRUE,  &m_adv_af,  5,  1,  FALSE);
	m_List.SetCheckbox(volnormal, 0, 0);
	m_List.SetComboBox(volnormal, 2, TRUE,  &m_volnormal,  5,  0,  FALSE);
	m_List.SetCheckbox(alang, 0, 0);
	m_List.SetItemText(alang, 2, _T("zh,ch,tw"));
	m_List.SetCheckbox(slang, 0, 0);
	m_List.SetItemText(slang, 2, _T("zh,ch,tw"));
	m_List.SetCheckbox(flvsync, 0, 0);
	m_List.SetCheckbox(resample, 0, 0);
	m_List.SetComboBox(resample, 2, TRUE,  &m_resample,  5,  16,  FALSE);
	m_List.SetCheckbox(equalizer, 0, 0);
	m_List.SetItemText(equalizer, 2, _T("0:0:0:0:0:0:0:0:0:0"));
	m_List.SetCheckbox(audiodelay, 0, 0);
	m_List.SetItemText(audiodelay, 2, _T(""));
	m_List.UnlockWindowUpdate();
}

void CMProfilePage::SetCurConfig()
{
	int cur = m_profile_list.GetCurSel();
	CString curtype;
	m_profile_list.GetLBText(cur , curtype);
	m_pre_profile = curtype;

	CleanConfig();
	m_List.LockWindowUpdate();

	if(!m_cfg)
		return;
	bool value_b;
	int value_i;
	CString value_s;
	CString value_sub;
	if(m_cfg->GetValueSP_Boolean(curtype,_T("flip"),value_b))
	{
		if(value_b)
			m_List.SetCheckbox(flip, 0, 1);
	}
	if(m_cfg->GetValueSP_Integer(curtype,_T("autosync"),value_i))
	{
		if(value_i == 0)
			m_List.SetCheckbox(flvsync, 0, 1);
	}
	if(m_cfg->GetValueSP_String(curtype,_T("slang"),value_s))
	{
		m_List.SetCheckbox(slang, 0, 1);
		m_List.SetItemText(slang, 2,value_s);
	}
	if(m_cfg->GetValueSP_String(curtype,_T("alang"),value_s))
	{
		m_List.SetCheckbox(alang, 0, 1);
		m_List.SetItemText(alang, 2,value_s);
	}
	if(m_cfg->GetValueSP_String(curtype,_T("delay"),value_s))
	{
		m_List.SetCheckbox(audiodelay, 0, 1);
		m_List.SetItemText(audiodelay, 2,value_s);
	}
	if(m_cfg->GetValueSP_String(curtype,_T("aspect"),value_s))
	{
		m_List.SetCheckbox(aspect, 0, 1);
		m_List.SetItemText(aspect, 2,value_s);
	}
	if(m_cfg->GetValueSP_String(curtype,_T("af-adv"),value_s))
	{
		if(m_cfg->GetSubValue(value_s,_T("force"), value_sub))
		{
			m_List.SetCheckbox(adv_af, 0, 1);
			if(value_sub == _T("2"))
				m_List.SetComboBox(adv_af, 2, TRUE,  &m_adv_af,  5,  2,  FALSE);
			else if(value_sub == _T("1"))
				m_List.SetComboBox(adv_af, 2, TRUE,  &m_adv_af,  5,  1,  FALSE);
			else
				m_List.SetComboBox(adv_af, 2, TRUE,  &m_adv_af,  5,  0,  FALSE);
		}
	}
	if(m_cfg->GetValueSP_String(curtype,_T("af"),value_s))
	{
		if(m_cfg->HaveSubValue(value_s,_T("volnorm")))
		{
			m_List.SetCheckbox(volnormal, 0, 1);
			if(m_cfg->GetSubValue(value_s,_T("volnorm"), value_sub))
			{
				if(value_sub == _T("2"))
					m_List.SetComboBox(volnormal, 2, TRUE,  &m_volnormal,  5,  1,  FALSE);
			}
		}
		if(m_cfg->GetSubValue(value_s,_T("equalizer"), value_sub))
		{
			m_List.SetCheckbox(equalizer, 0, 1);
			m_List.SetItemText(equalizer, 2,value_sub);
		}
		if(m_cfg->GetSubValue(value_s,_T("resample"), value_sub,1))
		{
			m_List.SetCheckbox(resample, 0, 1);
			int type = 0, rp = 16;
			if(m_cfg->GetSubValue(value_s,_T("resample"), value_sub,3))
			{
				if(value_sub  == _T("0"))
					type = 1;
				else if(value_sub  == _T("2"))
					type = 3;
				else
					type = 2;
			}
			if(value_sub  == _T("8000"))
				rp = 0;
			else if(value_sub  == _T("16000"))
				rp = 4;
			else if(value_sub  == _T("22050"))
				rp = 8;
			else if(value_sub  == _T("32000"))
				rp = 12;
			else if(value_sub  == _T("48000"))
				rp = 20;
			else if(value_sub  == _T("64000"))
				rp = 24;
			else if(value_sub  == _T("96000"))
				rp = 28;
			else
				rp = 16;
			rp += type;
			m_List.SetComboBox(resample, 2, TRUE,  &m_resample,  8,  rp,  FALSE);
		}
	}
	if(m_cfg->GetValueSP_String(curtype,_T("vf"),value_s))
	{
		CString value_vf = value_s;
		if(m_cfg->HaveSubValue(value_s,_T("ass")))
		{
			m_List.SetCheckbox(ass, 0, 1);
			RemoveSubValue(value_vf , _T("ass"));
		}
		if(m_cfg->HaveSubValue(value_s,_T("screenshot")))
		{
			m_List.SetCheckbox(screenshot, 0, 1);
			RemoveSubValue(value_vf , _T("screenshot"));
		}
		if(m_cfg->HaveSubValue(value_s,_T("eq")))
		{
			m_List.SetCheckbox(eq2, 0, 1);
			RemoveSubValue(value_vf , _T("eq"));
		}
		if(m_cfg->HaveSubValue(value_s,_T("eq2")))
		{
			m_List.SetCheckbox(eq2, 0, 1);
			RemoveSubValue(value_vf , _T("eq2"));
		}
		else if(m_cfg->GetValue_Boolean(_T("cofing_eq2") , value_b, true))
		{
			if(value_b)
				m_List.SetCheckbox(eq2, 0, 1);
		}
		if(m_cfg->HaveSubValue(value_s,_T("hue")))
		{
			m_List.SetCheckbox(hue, 0, 1);
			RemoveSubValue(value_vf , _T("hue"));
		}
		else if(m_cfg->GetValue_Boolean(_T("cofing_use_hue") , value_b, true))
		{
			if(value_b)
				m_List.SetCheckbox(hue, 0, 1);
		}
		if(m_cfg->HaveSubValue(value_s,_T("expand")))
		{
			m_List.SetCheckbox(expand, 0, 1);
			m_List.SetItemText(expand, 2,_T(""));
			if(m_cfg->GetSubValue(value_s,_T("expand"), value_sub))
				m_List.SetItemText(expand, 2,value_sub);
			RemoveSubValue(value_vf , _T("expand"));
		}
		else if(m_cfg->GetValue_String(_T("cofing_expand"),value_sub,true))
			m_List.SetItemText(expand, 2,value_sub);
			
		if(m_cfg->GetSubValue(value_s,_T("crop"), value_sub))
		{
			m_List.SetCheckbox(crop, 0, 1);
			m_List.SetItemText(crop, 2,value_sub);
			RemoveSubValue(value_vf , _T("crop"));
		}
		else if(m_cfg->GetValue_String(_T("cofing_crop"),value_sub,true))
			m_List.SetItemText(crop, 2,value_sub);
		
		if(m_cfg->HaveSubValue(value_s,_T("rotate")))
		{
			m_List.SetCheckbox(rotate, 0, 1);
			if(m_cfg->GetSubValue(value_s,_T("rotate"), value_sub))
			{
				if(value_sub == _T("0"))
					m_List.SetComboBox(rotate, 2, TRUE,  &m_rotate,  5,  0,  FALSE);
				if(value_sub == _T("1"))
					m_List.SetComboBox(rotate, 2, TRUE,  &m_rotate,  5,  1,  FALSE);
				if(value_sub == _T("2"))
					m_List.SetComboBox(rotate, 2, TRUE,  &m_rotate,  5,  2,  FALSE);
				if(value_sub == _T("3"))
					m_List.SetComboBox(rotate, 2, TRUE,  &m_rotate,  5,  3,  FALSE);
			}
			RemoveSubValue(value_vf , _T("rotate"));
		}
		if(m_cfg->GetSubValue(value_s,_T("pp"), value_sub))
		{
			RemoveSubValue(value_vf , _T("pp"));
			if(m_cfg->HaveSubValue(value_sub,_T("dering") , _T("/"))
				|| m_cfg->HaveSubValue(value_sub,_T("dr") , _T("/")))
				m_List.SetCheckbox(dering, 0, 1);
			if(m_cfg->HaveSubValue(value_sub,_T("autolevels") , _T("/"))
				|| m_cfg->HaveSubValue(value_sub,_T("al") , _T("/")))
				m_List.SetCheckbox(autolevels, 0, 1);
			
			if(m_cfg->HaveSubValue(value_sub,_T("linblenddeint") , _T("/")) 
				|| m_cfg->HaveSubValue(value_sub,_T("lb") , _T("/")) )
			{
				m_List.SetCheckbox(deinterlacing, 0, 1);
				m_List.SetComboBox(deinterlacing, 2, TRUE,  &m_deinterlacing,  5,  di_lb,  FALSE);
			}
			else if(m_cfg->HaveSubValue(value_sub,_T("cubicipoldeint") , _T("/"))
				|| m_cfg->HaveSubValue(value_sub,_T("ci") , _T("/")))
			{
				m_List.SetCheckbox(deinterlacing, 0, 1);
				m_List.SetComboBox(deinterlacing, 2, TRUE,  &m_deinterlacing ,  5,  di_ci,  FALSE);
			}
			else if(m_cfg->HaveSubValue(value_sub,_T("mediandeint") , _T("/"))
				|| m_cfg->HaveSubValue(value_sub,_T("md") , _T("/")))
			{
				m_List.SetCheckbox(deinterlacing, 0, 1);
				m_List.SetComboBox(deinterlacing, 2, TRUE,  &m_deinterlacing ,  5,  di_md,  FALSE);
			}
			else if(m_cfg->HaveSubValue(value_sub,_T("ffmpegdeint") , _T("/"))
				|| m_cfg->HaveSubValue(value_sub,_T("fd") , _T("/")))
			{
				m_List.SetCheckbox(deinterlacing, 0, 1);
				m_List.SetComboBox(deinterlacing, 2, TRUE,  &m_deinterlacing ,  5,  di_fd,  FALSE);
			}
			else if(m_cfg->HaveSubValue(value_sub,_T("lowpass5") , _T("/"))
				|| m_cfg->HaveSubValue(value_sub,_T("l5") , _T("/")))
			{
				m_List.SetCheckbox(deinterlacing, 0, 1);
				m_List.SetComboBox(deinterlacing, 2, TRUE,  &m_deinterlacing ,  5,  di_l5,  FALSE);
			}
			bool d_hb = false,d_vb = false,d_ha = false,d_va = false;
			if(m_cfg->HaveSubValue(value_sub,_T("hdeblock") , _T("/")) 
				|| m_cfg->HaveSubValue(value_sub,_T("hb") , _T("/")) )
				d_hb = true;
			if(m_cfg->HaveSubValue(value_sub,_T("vdeblock") , _T("/")) 
				|| m_cfg->HaveSubValue(value_sub,_T("vb") , _T("/")))
				d_vb = true;
			if(m_cfg->HaveSubValue(value_sub,_T("hadeblock") , _T("/")) 
				|| m_cfg->HaveSubValue(value_sub,_T("ha") , _T("/")))
				d_ha = true;
			if(m_cfg->HaveSubValue(value_sub,_T("vadeblock") , _T("/")) 
				|| m_cfg->HaveSubValue(value_sub,_T("va") , _T("/")))
				d_va = true;
			if(d_hb || d_vb || d_ha || d_va)
			{
				m_List.SetCheckbox(deblocking, 0, 1);
				int index = db_hb;
				if(d_hb && d_vb)
					index = db_b;
				else if(d_ha && d_va)
					index = db_a;
				else if(d_hb)
					index = db_hb;
				else if(d_vb)
					index = db_vb;
				else if(d_ha)
					index = db_ha;
				else if(d_va)
					index = db_va;
				m_List.SetComboBox(deblocking, 2, TRUE,  &m_deblocking,  6,  index,  FALSE);
			}
		}
	}
	m_List.UnlockWindowUpdate();
}

void CMProfilePage::SavePreConfig()
{
	if(!m_cfg || m_pre_profile.GetLength() < 2)
		return;
	
	m_List.LockWindowUpdate();

	if(m_List.GetCheckbox(flvsync, 0))
	{
		m_cfg->SetValueSP(m_pre_profile,_T("autosync"),_T("0"));
		m_cfg->SetValueSP(m_pre_profile,_T("mc"),_T("0.1"));
	}
	else
	{
		m_cfg->RemoveValueSP(m_pre_profile,_T("autosync"));
		m_cfg->RemoveValueSP(m_pre_profile,_T("mc"));
	}

	if(m_List.GetCheckbox(alang, 0))
	{
		CString str = m_List.GetItemText(alang, 2);
		str.TrimLeft(_T(" "));
		str.TrimRight(_T(" "));
		if(str.GetLength() > 1)
			m_cfg->SetValueSP(m_pre_profile,_T("alang"),str);
		else
			m_cfg->RemoveValueSP(m_pre_profile,_T("alang"));
	}
	else
		m_cfg->RemoveValueSP(m_pre_profile,_T("alang"));


	if(m_List.GetCheckbox(slang, 0))
	{
		CString str = m_List.GetItemText(slang, 2);
		str.TrimLeft(_T(" "));
		str.TrimRight(_T(" "));
		if(str.GetLength() > 1)
			m_cfg->SetValueSP(m_pre_profile,_T("slang"),str);
		else
			m_cfg->RemoveValueSP(m_pre_profile,_T("slang"));
	}
	else
		m_cfg->RemoveValueSP(m_pre_profile,_T("slang"));

	if(m_List.GetCheckbox(audiodelay, 0))
	{
		CString str = m_List.GetItemText(audiodelay, 2);
		str.TrimLeft(_T(" "));
		str.TrimRight(_T(" "));
		if(IsDigit(str))
			m_cfg->SetValueSP(m_pre_profile,_T("delay"),str);
		else
			m_cfg->RemoveValueSP(m_pre_profile,_T("delay"));
	}
	else
		m_cfg->RemoveValueSP(m_pre_profile,_T("delay"));

	if(m_List.GetCheckbox(adv_af, 0))
	{
		CString str = m_List.GetItemText(adv_af, 2);
		if(str == ResStr(IDS_AUDIO_AFAT))
			m_cfg->SetValueSP(m_pre_profile,_T("af-adv"), _T("force=0"));
		else if(str == ResStr(IDS_AUDIO_AFH))
			m_cfg->SetValueSP(m_pre_profile,_T("af-adv"), _T("force=1"));
		else
			m_cfg->SetValueSP(m_pre_profile,_T("af-adv"), _T("force=2"));
	}
	else
		m_cfg->RemoveValueSP(m_pre_profile,_T("af-adv"));


	if(m_List.GetCheckbox(flip, 0))
		m_cfg->SetValueSP(m_pre_profile,_T("flip"),_T("1"));
	else
		m_cfg->RemoveValueSP(m_pre_profile,_T("flip"));
	
	if(m_List.GetCheckbox(aspect, 0))
	{
		CString str = m_List.GetItemText(aspect, 2);
		str.TrimLeft(_T(" "));
		if(str.GetLength() > 0)
			m_cfg->SetValueSP(m_pre_profile,_T("aspect"),str);
		else
			m_cfg->RemoveValueSP(m_pre_profile,_T("aspect"));
	}
	else
		m_cfg->RemoveValueSP(m_pre_profile,_T("aspect"));

	CString vf_str = _T("");
	if(m_List.GetCheckbox(ass, 0))
		vf_str += _T("ass,");
	if(m_List.GetCheckbox(screenshot, 0))
		vf_str += _T("screenshot,");
	if(m_List.GetCheckbox(eq2, 0))
		vf_str += _T("eq2,");
	if(m_List.GetCheckbox(eq2, 0))
		vf_str += _T("hue,");

	CString str_expand = m_List.GetItemText(expand, 2);
	str_expand.TrimLeft(_T(" "));
	str_expand.TrimRight(_T(" "));
	if(m_List.GetCheckbox(expand, 0))
	{
		if(str_expand.GetLength() > 0)
			vf_str += _T("expand=") + str_expand + _T(",");
		else
			vf_str += _T("expand,");
	}
	
	if(m_List.GetCheckbox(rotate, 0))
	{
		CString str = m_List.GetItemText(rotate, 2);
		if(str == ResStr(IDS_VIDEO_ROTA2))
			vf_str += _T("rotate=1,") ;
		else if(str == ResStr(IDS_VIDEO_ROTA3))
			vf_str += _T("rotate=2,") ;
		else if(str == ResStr(IDS_VIDEO_ROTA4))
			vf_str += _T("rotate=2,") ;
		else
			vf_str += _T("rotate,") ;
	}
	
	CString str_crop = m_List.GetItemText(crop, 2);
	str_crop.TrimLeft(_T(" "));
	str_crop.TrimRight(_T(" "));
	if(m_List.GetCheckbox(crop, 0))
	{
		if(str_crop.GetLength() > 0)
			vf_str += _T("crop=") + str_crop + _T(",");
	}

	int vdi = m_List.GetCheckbox(deinterlacing, 0);
	int vdb = m_List.GetCheckbox(deblocking, 0);
	int vdr = m_List.GetCheckbox(dering, 0);
	int val = m_List.GetCheckbox(autolevels, 0);
	
	if( vdi || vdb || vdr || val)
	{
		vf_str += _T("pp=");
		if(vdi)
		{
			CString str = m_List.GetItemText(deinterlacing, 2);
			if(str == ResStr(IDS_VIDEO_VD2))
				vf_str += _T("ci/");
			else if(str == ResStr(IDS_VIDEO_VD3))
				vf_str += _T("md/");
			else if(str == _T("FFmpeg"))
				vf_str += _T("fd/");
			else if(str == _T("lowpass5"))
				vf_str += _T("l5/");
			else
				vf_str += _T("lb/");
		}
		if(vdb)
		{
			CString str = m_List.GetItemText(deblocking, 2);
			if(str == ResStr(IDS_VIDEO_DB2))
				vf_str += _T("vb/");
			else if(str == ResStr(IDS_VIDEO_DB3))
				vf_str += _T("ha/");
			else if(str == ResStr(IDS_VIDEO_DB4))
				vf_str += _T("va/");
			else if(str == ResStr(IDS_VIDEO_DB5))
				vf_str += _T("hb/vb/");
			else if(str == ResStr(IDS_VIDEO_DB6))
				vf_str += _T("ha/va/");
			else
				vf_str += _T("hb/");
		}
		if(vdr)
			vf_str += _T("dr/");
		if(val)
			vf_str += _T("al/");
		vf_str.TrimRight(_T("/"));
	}

	vf_str.TrimRight(_T(","));
	if(vf_str.GetLength() > 1)
		m_cfg->SetValueSP(m_pre_profile,_T("vf"),  vf_str);
	else
		m_cfg->RemoveValueSP(m_pre_profile,_T("vf"));

	CString af_str = _T("");
	if(m_List.GetCheckbox(volnormal, 0))
	{
		CString str= m_List.GetItemText(volnormal, 2);
		if(str == ResStr(IDS_AUDIO_RM))
			af_str +=  _T("volnorm=2,");
		else
			af_str +=  _T("volnorm,");
	}
	if(m_List.GetCheckbox(equalizer, 0))
	{
		CString str= m_List.GetItemText(equalizer, 2);
		str.TrimLeft(_T(" "));
		str.TrimRight(_T(" "));
		if(str != _T(""))
			af_str +=  _T("equalizer=") + str  +  _T(",");
	}
	if(m_List.GetCheckbox(resample, 0))
	{
		CString str= m_List.GetItemText(resample, 2);
		if(str == _T("8kHz ") + m_str_at)
			af_str +=  _T("resample=8000,");
		else if(str == _T("8kHz ") + m_str_low)
			af_str +=  _T("resample=8000:1:0,");
		else if(str == _T("8kHz ") + m_str_int)
			af_str +=  _T("resample=8000:1:1,");
		else if(str == _T("8kHz ") + m_str_foa)
			af_str +=  _T("resample=8000:1:2,");
		else if(str == _T("16kHz ") + m_str_at)
			af_str +=  _T("resample=16000,");
		else if(str == _T("16kHz ") + m_str_low)
			af_str +=  _T("resample=16000:1:0,");
		else if(str == _T("16kHz ") + m_str_int)
			af_str +=  _T("resample=16000:1:1,");
		else if(str == _T("16kHz ") + m_str_foa)
			af_str +=  _T("resample=16000:1:2,");
		else if(str == _T("22.05kHz ") + m_str_at)
			af_str +=  _T("resample=22050,");
		else if(str == _T("22.05kHz ") + m_str_low)
			af_str +=  _T("resample=22050:1:0,");
		else if(str == _T("22.05kHz ") + m_str_int)
			af_str +=  _T("resample=22050:1:1,");
		else if(str == _T("22.05kHz ") + m_str_foa)
			af_str +=  _T("resample=22050:1:2,");
		else if(str == _T("32kHz ") + m_str_at)
			af_str +=  _T("resample=32000,");
		else if(str == _T("32kHz ") + m_str_low)
			af_str +=  _T("resample=32000:1:0,");
		else if(str == _T("32kHz ") + m_str_int)
			af_str +=  _T("resample=32000:1:1,");
		else if(str == _T("32kHz ") + m_str_foa)
			af_str +=  _T("resample=32000:1:2,");
		else if(str == _T("44.1kHz ") + m_str_low)
			af_str +=  _T("resample=44100:1:0,");
		else if(str == _T("44.1kHz ") + m_str_int)
			af_str +=  _T("resample=44100:1:1,");
		else if(str == _T("44.1kHz ") + m_str_foa)
			af_str +=  _T("resample=44100:1:2,");
		else if(str == _T("48kHz ") + m_str_at)
			af_str +=  _T("resample=48000,");
		else if(str == _T("48kHz ") + m_str_low)
			af_str +=  _T("resample=48000:1:0,");
		else if(str == _T("48kHz ") + m_str_int)
			af_str +=  _T("resample=48000:1:1,");
		else if(str == _T("48kHz ") + m_str_foa)
			af_str +=  _T("resample=48000:1:2,");
		else if(str == _T("64kHz ") + m_str_at)
			af_str +=  _T("resample=64000,");
		else if(str == _T("64kHz ") + m_str_low)
			af_str +=  _T("resample=64000:1:0,");
		else if(str == _T("64kHz ") + m_str_int)
			af_str +=  _T("resample=64000:1:1,");
		else if(str == _T("64kHz ") + m_str_foa)
			af_str +=  _T("resample=64000:1:2,");
		else if(str == _T("96kHz ") + m_str_at)
			af_str +=  _T("resample=96000,");
		else if(str == _T("96kHz ") + m_str_low)
			af_str +=  _T("resample=96000:1:0,");
		else if(str == _T("96kHz ") + m_str_int)
			af_str +=  _T("resample=96000:1:1,");
		else if(str == _T("96kHz ") + m_str_foa)
			af_str +=  _T("resample=96000:1:2,");
		else
			af_str +=  _T("resample=44100,");
	}
	af_str.TrimRight(_T(","));
	if(af_str.GetLength() > 1)
		m_cfg->SetValueSP(m_pre_profile,_T("af"),  af_str);
	else
		m_cfg->RemoveValueSP(m_pre_profile,_T("af"));
	m_List.UnlockWindowUpdate();
}

void CMProfilePage::RemoveSubValue(CString &line ,CString subname ,CString sp)
{
	line += sp;
	int offset = line.Find(subname);
	if(offset < 0)
		return;
	int offset1 = line.Find(sp,offset);
	if(offset1 <= offset)
		return;
	line.Delete(offset,offset1 - offset + 1);
	line.TrimRight(sp);
}

void CMProfilePage::OnCbnSelchangeComboExtensino()
{
	SavePreConfig();
	SetCurConfig();
}

void CMProfilePage::SaveConfig()
{
	SavePreConfig();
}
void CMProfilePage::OnBnClickedDel()
{
	if(MessageBox(ResStr(IDS_PROFILE_DEL),ResStr(IDS_PROFILE_DEL_INF),MB_OKCANCEL) != IDOK)
		return;

	int cur = m_profile_list.GetCurSel();
	CString curtype;
	m_profile_list.GetLBText(cur , curtype);
	if(!m_cfg)
		return;
	m_cfg->RemoveConfigSP(curtype);
	m_pre_profile = _T("");
	m_profile_list.DeleteString(cur);
	m_profile_list.SetCurSel(0);
	SetCurConfig();
}

void CMProfilePage::OnBnClickedAdd()
{
	CMProfileDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString add_profile = dlg.m_profile_add;
		int Type = -1;
		CString curtype;
		for(int i = 0 ; i < m_profile_list.GetCount() ; i++)
		{
			m_profile_list.GetLBText(i , curtype);
			if(curtype == add_profile)
			{
				Type = i;
				break;
			}
		}
		if(Type >= 0)
		{
			SavePreConfig();
			m_pre_profile = _T("");
			m_profile_list.SetCurSel(Type);
			SetCurConfig();
		}
		else
		{
			SavePreConfig();
			m_pre_profile = _T("");
			m_profile_list.AddString(add_profile);
			Type = m_profile_list.GetCount();
			m_profile_list.SetCurSel(Type - 1);
			CleanConfig();
			m_pre_profile = add_profile;
		}
	}
}

BOOL CMProfilePage::PreTranslateMessage(MSG* pMsg)
{
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
