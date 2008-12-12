// MAudioPage.cpp : implementation file
//

#include "stdafx.h"
#include "meditor2.h"
#include "MAudioPage.h"
#include "MConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMAudioPage dialog


CMAudioPage::CMAudioPage(CWnd* pParent /*=NULL*/)
		: CDialog(CMAudioPage::IDD, pParent)
		, m_alang(_T(""))
{
	m_cfg = NULL;
	m_audio_delay = _T("0");
	m_volume = _T("60");
	m_volnorm = _T("100");
	m_volnorm_s = 10;
	m_volume_s = 60;
	m_str_at = ResStr(IDS_PLAYER_AUTO);
	m_str_no = ResStr(IDS_PLAYER_NONE);
	m_str_low = ResStr(IDS_AUDIO_RELOW);
	m_str_int = ResStr(IDS_AUDIO_REINT);
	m_str_foa = ResStr(IDS_AUDIO_REFOA);
}


void CMAudioPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CHANNELS, m_channels);
	DDX_Text(pDX, IDC_EDIT_AUDIODELAY, m_audio_delay);
	DDV_MaxChars(pDX, m_audio_delay, 9);
	DDX_Text(pDX, IDC_EDIT_VOLUME, m_volume);
	DDX_Control(pDX, IDC_SLIDER_VOLNORM, m_volnorm_c);
	DDV_MaxChars(pDX, m_volume, 3);
	DDX_Text(pDX, IDC_EDIT_VOLNORM, m_volnorm);
	DDV_MaxChars(pDX, m_volnorm, 4);
	DDX_Control(pDX, IDC_LIST_AUDIO, m_List);
	DDX_Slider(pDX, IDC_SLIDER_VOLNORM, m_volnorm_s);
	DDX_Slider(pDX, IDC_SLIDER_VOLUME, m_volume_s);
	DDX_Control(pDX, IDC_COMBO_AUDIO, m_audio);
	DDX_Text(pDX, IDC_EDIT_ALANG, m_alang);
}


BEGIN_MESSAGE_MAP(CMAudioPage, CDialog)
	//{{AFX_MSG_MAP(CMAudioPage)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_VOLUME, OnReleasedcaptureSliderVolume)
	ON_EN_CHANGE(IDC_EDIT_VOLUME, OnChangeEditVolume)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_VOLNORM, OnReleasedcaptureSliderVolnorm)
	ON_EN_CHANGE(IDC_EDIT_VOLNORM, OnChangeEditVolnorm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMAudioPage message handlers

enum Filters
{
	adv_af,
	volnormal,
	scaletempo,
	resample,
	equalizer,
};


int CMAudioPage::m_nColWidths[5] = { 5, 15, 20, 50};	// sixty-fourths
// InitListCtrl
void CMAudioPage::InitListCtrl(CXListCtrl * pList)
{
	ASSERT(pList);
	if (!pList)
		return;

	// set column width according to window rect
	CRect rect;
	pList->GetWindowRect(&rect);

	int w = rect.Width() - 2;

	CString use = ResStr(IDS_PLAYER_USE);
	CString nam = ResStr(IDS_PLAYER_FNAME);
	CString opt = ResStr(IDS_PLAYER_OPTION);
	CString inf = ResStr(IDS_PLAYER_FINFO);
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

///////////////////////////////////////////////////////////////////////////////
// FillListCtrl
void CMAudioPage::FillListCtrl(CXListCtrl * pList)
{
	// a list of some more of my favorite things

	pList->LockWindowUpdate();
	pList->DeleteAllItems();

	// insert the items and subitems into the list
	m_adv_af.RemoveAll();
	m_volnormal.RemoveAll();
	m_equalizer.RemoveAll();
	m_resample.RemoveAll();

	m_adv_af.Add(ResStr(IDS_AUDIO_AFAT));
	m_adv_af.Add(ResStr(IDS_AUDIO_AFH));
	m_adv_af.Add(ResStr(IDS_AUDIO_AFF));

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

	pList->InsertItem(adv_af, _T(""));
	pList->SetCheckbox(adv_af, 0, 0);
	pList->SetItemText(adv_af, 1, ResStr(IDS_AUDIO_ADV));
	pList->SetComboBox(adv_af, 2, TRUE,  &m_adv_af,  5,  1,  FALSE);
	pList->SetItemText(adv_af, 3, ResStr(IDS_AUDIO_ADV_INFO));

	pList->InsertItem(volnormal, _T(""));
	pList->SetCheckbox(volnormal, 0, 0);
	pList->SetItemText(volnormal, 1, ResStr(IDS_AUDIO_VNL));
	pList->SetComboBox(volnormal, 2, TRUE,  &m_volnormal,  5,  0,  FALSE);
	pList->SetItemText(volnormal, 3, ResStr(IDS_AUDIO_VNL_INFO));

	pList->InsertItem(scaletempo, _T(""));
	pList->SetCheckbox(scaletempo, 0, 0);
	pList->SetItemText(scaletempo, 1, ResStr(IDS_AUDIO_STM));
	pList->SetItemText(scaletempo, 2, _T(""));
	pList->SetItemText(scaletempo, 3, ResStr(IDS_AUDIO_STM_INFO));
	
	pList->InsertItem(resample, _T(""));
	pList->SetCheckbox(resample, 0, 0);
	pList->SetItemText(resample, 1, ResStr(IDS_AUDIO_RE));
	pList->SetComboBox(resample, 2, TRUE,  &m_resample,  5,  16,  FALSE);
	pList->SetItemText(resample, 3, ResStr(IDS_AUDIO_RE_INFO));
	
	pList->InsertItem(equalizer, _T(""));
	pList->SetCheckbox(equalizer, 0, 0);
	pList->SetItemText(equalizer, 1, ResStr(IDS_AUDIO_EQ));
	pList->SetItemText(equalizer, 2, _T("0:0:0:0:0:0:0:0:0:0"));
	pList->SetEdit(equalizer, 2);
	pList->SetItemText(equalizer, 3, ResStr(IDS_AUDIO_EQ_INFO));


	pList->UnlockWindowUpdate();	// ***** unlock window updates *****
}

void CMAudioPage::OnReleasedcaptureSliderVolume(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_volume.Format(_T("%d"),m_volume_s);
	UpdateData(FALSE);
	*pResult = 0;
}

void CMAudioPage::OnChangeEditVolume() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int x = _ttoi(m_volume);
	if(x >= 0 && x <= 100)
		m_volume_s = x;
	else if(x < 0)
		m_volume_s = 0;
	else
		m_volume_s = 100;
	UpdateData(FALSE);
}

void CMAudioPage::OnChangeEditVolnorm() 
{
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int x = _ttoi(m_volnorm);
	if(x >= 100 && x <= 1000)
		m_volnorm_s = x/10;
	else if(x < 100)
		m_volnorm_s = 10;
	else
		m_volnorm_s = 100;
	UpdateData(FALSE);
}

void CMAudioPage::OnReleasedcaptureSliderVolnorm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_volnorm.Format(_T("%d"),m_volnorm_s * 10);
	UpdateData(FALSE);
	*pResult = 0;
}

BOOL CMAudioPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	InitListCtrl(&m_List);
	FillListCtrl(&m_List);

	m_volnorm_c.SetRange(10,100);
	m_volnorm_c.SetPos(0);
	
	m_audio.AddString(ResStr(IDS_AUDIO_DS));
	m_audio.AddString(ResStr(IDS_AUDIO_WIN));
	m_audio.AddString(ResStr(IDS_AUDIO_FILE));
	m_audio.AddString(ResStr(IDS_AUDIO_NULL));
	m_audio.SetCurSel(dsound);

	m_channels.AddString(ResStr(IDS_RESUME_NORMAL));
	m_channels.AddString(_T("2.0") + ResStr(IDS_AUDIO_CHAN));
	m_channels.AddString(_T("4.0") + ResStr(IDS_AUDIO_CHAN));
	m_channels.AddString(_T("5.1") + ResStr(IDS_AUDIO_CHAN));
	m_channels.SetCurSel(ch_auto);

	InitFromConfig();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMAudioPage::SetNormal()
{
	m_volnorm_s = 0;
	m_volnorm = _T("100");
	m_audio_delay = _T("0");
	m_volume = _T("60");
	m_volume_s = 60;
	m_alang = _T("");
	m_audio.SetCurSel(dsound);
	m_channels.SetCurSel(ch_auto);
	m_List.SetCheckbox(equalizer, 0, 0);
	m_List.SetItemText(equalizer, 2,_T("0:0:0:0:0:0:0:0:0:0"));
	m_List.SetCheckbox(resample, 0, 0);
	m_List.SetComboBox(resample, 2, TRUE,  &m_resample,  8,  16,  FALSE);
	m_List.SetCheckbox(adv_af, 0, 0);
	m_List.SetComboBox(adv_af, 2, TRUE,  &m_adv_af,  5,  1,  FALSE);
	m_List.SetCheckbox(volnormal, 0, 0);
	m_List.SetComboBox(volnormal, 2, TRUE,  &m_volnormal,  5,  0,  FALSE);
	m_List.SetCheckbox(scaletempo, 0, 0);
}

void CMAudioPage::SetHigh()
{
	SetNormal();
	m_List.SetCheckbox(adv_af, 0, 1);
	m_List.SetComboBox(adv_af, 2, TRUE,  &m_adv_af,  5,  1,  FALSE);
}

void CMAudioPage::SetLower()
{
	SetNormal();
	m_List.SetCheckbox(adv_af, 0, 0);
	m_List.SetComboBox(adv_af, 2, TRUE,  &m_adv_af,  5,  2,  FALSE);
}

void CMAudioPage::InitFromConfig()
{
	if(!m_cfg)
		return;
	int value_i;
	bool value_b;
	double value_d;
	CString value_s;
	CString value_sub;
	m_List.LockWindowUpdate();

	m_cfg->GetValue_Boolean(_T("softvol"),value_b);
	if(m_cfg->GetValue_Integer(_T("softvol-max"),value_i))
	{
		if(value_i > 100 && value_i < 1000)
		{
			m_volnorm_s = value_i  / 10;
			m_volnorm.Format(_T("%d"),value_i);
		}
		else
			m_volnorm = _T("100");
	}
	if(m_cfg->GetValue_Double(_T("delay"),value_d))
	{
		m_audio_delay.Format(_T("%0.3f"),value_d);
	}
	if(m_cfg->GetValue_Integer(_T("Volume"),value_i,true))
	{
		if(value_i >= 0 && value_i <= 100)
		{
			m_volume_s = value_i;
			m_volume.Format(_T("%d"),value_i);
		}
	}
	if(m_cfg->GetValue_Integer(_T("channels"),value_i))
	{
		switch (value_i)
		{
		case 2:
			m_channels.SetCurSel(ch_2);
			break;
		case 4:
			m_channels.SetCurSel(ch_4);
			break;
		case 6:
			m_channels.SetCurSel(ch_6);
			break;
		default:
			m_channels.SetCurSel(ch_auto);
		}
	}
	if(m_cfg->GetValue_String(_T("ao"),value_s))
	{
		if(value_s == _T("win32"))
			m_audio.SetCurSel(win32);
		else if(value_s == _T("pcm"))
			m_audio.SetCurSel(pcm);
		else if(value_s == _T("null"))
			m_audio.SetCurSel(ao_null);
		else
			m_audio.SetCurSel(dsound);
	}
	if(m_cfg->GetValue_String(_T("af"),value_s))
	{
		if(m_cfg->GetSubValue(value_s,_T("equalizer"), value_sub))
		{
			m_List.SetCheckbox(equalizer, 0, 1);
			m_List.SetItemText(equalizer, 2,value_sub);
		}
		if(m_cfg->HaveSubValue(value_s,_T("volnorm")))
		{
			m_List.SetCheckbox(volnormal, 0, 1);
			if(m_cfg->GetSubValue(value_s,_T("volnorm"), value_sub))
			{
				if(value_sub == _T("2"))
					m_List.SetComboBox(volnormal, 2, TRUE,  &m_volnormal,  5,  1,  FALSE);
			}
		}
		if(m_cfg->HaveSubValue(value_s,_T("scaletempo")))
		{
			m_List.SetCheckbox(scaletempo, 0, 1);
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
	if(m_cfg->GetValue_String(_T("af-adv"),value_s))
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
	if(m_cfg->GetValue_String(_T("alang"),value_s))
		m_alang = value_s;
	m_List.UnlockWindowUpdate();
	UpdateData(FALSE);
}

void CMAudioPage::SaveConfig()
{
	if(!m_cfg)
		return;
	UpdateData(TRUE);
	
	m_List.LockWindowUpdate();
	
	int vao = m_audio.GetCurSel();
	switch (vao)
	{
	case win32:
		m_cfg->SetValue(_T("ao") ,_T("win32") );
		break;
	case pcm:
		m_cfg->SetValue(_T("ao") ,_T("pcm") );
		break;
	case ao_null:
		m_cfg->SetValue(_T("ao") ,_T("null") );
		break;
	default:
		m_cfg->SetValue(_T("ao") ,_T("dsound") );
	}
	
	int vchannels = m_channels.GetCurSel();
	switch (vchannels)
	{
	case ch_2:
		m_cfg->SetValue(_T("channels") ,_T("2") );
		break;
	case ch_4:
		m_cfg->SetValue(_T("channels") ,_T("4") );
		break;
	case ch_6:
		m_cfg->SetValue(_T("channels") ,_T("6") );
		break;
	default:
		m_cfg->RemoveValue(_T("channels"));
	}	
	
	if(m_volnorm_s > 10 && m_volnorm_s <= 100)
	{
		m_cfg->SetValue(_T("softvol"), _T("1"));
		m_volnorm.Format(_T("%d"),m_volnorm_s * 10);
		m_cfg->SetValue(_T("softvol-max"), m_volnorm);
	}
	else
	{
		m_cfg->RemoveValue(_T("softvol"));
		m_cfg->RemoveValue(_T("softvol-max"));
	}

	if(m_alang != _T(""))
		m_cfg->SetValue(_T("alang"),  m_alang );
	else
		m_cfg->RemoveValue(_T("alang"));

	if(m_volume_s >= 0 && m_volume_s <= 100)
	{
		m_volume.Format(_T("%d"),m_volume_s);
		m_cfg->SetValue(_T("Volume"), m_volume,true, ex_status);
	}
	else
		m_cfg->SetValue(_T("Volume"), _T("60"),true, ex_status);
	
	if(StringToDouble(m_audio_delay) != 0.0)
		m_cfg->SetValue(_T("delay"), m_audio_delay);
	else
		m_cfg->RemoveValue(_T("delay"));

	if(m_List.GetCheckbox(adv_af, 0))
	{
		CString str = m_List.GetItemText(adv_af, 2);
		if(str == ResStr(IDS_AUDIO_AFAT))
			m_cfg->SetValue(_T("af-adv"), _T("force=0"));
		else if(str == ResStr(IDS_AUDIO_AFH))
			m_cfg->SetValue(_T("af-adv"), _T("force=1"));
		else
			m_cfg->SetValue(_T("af-adv"), _T("force=2"));
	}
	else
		m_cfg->RemoveValue(_T("af-adv"));
	
	CString af_str = _T("");
	if(m_List.GetCheckbox(volnormal, 0))
	{
		CString str= m_List.GetItemText(volnormal, 2);
		if(str == ResStr(IDS_AUDIO_RM))
			af_str +=  _T("volnorm=2,");
		else
			af_str +=  _T("volnorm,");
	}
	if(m_List.GetCheckbox(scaletempo, 0))
	{
		af_str +=  _T("scaletempo,");
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
		m_cfg->SetValue(_T("af"),  af_str);
	else
		m_cfg->RemoveValue(_T("af"));
	m_List.UnlockWindowUpdate();
}
	
BOOL CMAudioPage::PreTranslateMessage(MSG* pMsg) 
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
