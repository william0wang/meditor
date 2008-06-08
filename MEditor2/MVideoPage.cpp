// MVideoPage.cpp : implementation file
//

#include "stdafx.h"
#include "meditor2.h"
#include "MVideoPage.h"
#include "MConfig.h"
#include "MShowInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMVideoPage dialog


CMVideoPage::CMVideoPage(CWnd* pParent /*=NULL*/)
	: CDialog(CMVideoPage::IDD, pParent)
	, m_gamma(_T("1.0"))
	, m_gamma_s(10)
	, m_dr(FALSE)
{
	//{{AFX_DATA_INIT(CMVideoPage)
	m_noflash = TRUE;
	m_vosd = FALSE;
	m_soft = FALSE;
	m_color = _T("0xffffff");
	m_saturation = _T("0");
	m_saturation_s = 0;
	m_hue = _T("0");
	m_hue_s = 0;
	m_contrast = _T("0");
	m_contrast_s = 0;
	m_brightness = _T("0");
	m_brightness_s = 0;
	m_keepaspect = TRUE;
	m_framedrop = FALSE;
	m_idx = FALSE;
	//}}AFX_DATA_INIT
}


void CMVideoPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMVideoPage)
	DDX_Control(pDX, IDC_COMBO_MXCOLOR, m_mxcolor);
	DDX_Control(pDX, IDC_SLIDER_SATURATIONS, m_sc);
	DDX_Control(pDX, IDC_SLIDER_HUE, m_hc);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_cc);
	DDX_Control(pDX, IDC_SLIDER_BRINGHTNESS, m_bc);
	DDX_Control(pDX, IDC_COMBO_VO, m_vo);
	DDX_Check(pDX, IDC_CHECK_FLASH, m_noflash);
	DDX_Check(pDX, IDC_CHECK_OSD, m_vosd);
	DDX_Check(pDX, IDC_CHECK_SOFT, m_soft);
	DDX_Text(pDX, IDC_EDIT_COLOR, m_color);
	DDV_MaxChars(pDX, m_color, 8);
	DDX_Text(pDX, IDC_EDIT_SATURATIONS, m_saturation);
	DDX_Slider(pDX, IDC_SLIDER_SATURATIONS, m_saturation_s);
	DDX_Text(pDX, IDC_EDIT_HUE, m_hue);
	DDX_Slider(pDX, IDC_SLIDER_HUE, m_hue_s);
	DDX_Slider(pDX, IDC_SLIDER_CONTRAST, m_contrast_s);
	DDX_Text(pDX, IDC_EDIT_CONTRAST, m_contrast);
	DDX_Text(pDX, IDC_EDIT_BRINGHTNESS, m_brightness);
	DDX_Slider(pDX, IDC_SLIDER_BRINGHTNESS, m_brightness_s);
	DDX_Check(pDX, IDC_CHECK_KEEPAS, m_keepaspect);
	DDX_Check(pDX, IDC_CHECK_FRAMEDROP, m_framedrop);
	DDX_Check(pDX, IDC_CHECK_REINDEX, m_idx);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_COLOR, m_color_b);
	DDX_Control(pDX, IDC_LIST_VF, m_List);
	DDX_Text(pDX, IDC_EDIT_GAMMA, m_gamma);
	DDX_Slider(pDX, IDC_SLIDER_GAMMA, m_gamma_s);
	DDX_Control(pDX, IDC_SLIDER_GAMMA, m_gc);
	DDX_Check(pDX, IDC_CHECK_DR, m_dr);
}


BEGIN_MESSAGE_MAP(CMVideoPage, CDialog)
	//{{AFX_MSG_MAP(CMVideoPage)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_BRINGHTNESS, OnReleasedcaptureSliderBringhtness)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CONTRAST, OnReleasedcaptureSliderContrast)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_HUE, OnReleasedcaptureSliderHue)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SATURATIONS, OnReleasedcaptureSliderSaturations)
	ON_EN_CHANGE(IDC_EDIT_BRINGHTNESS, OnChangeEditBringhtness)
	ON_EN_CHANGE(IDC_EDIT_CONTRAST, OnChangeEditContrast)
	ON_EN_CHANGE(IDC_EDIT_HUE, OnChangeEditHue)
	ON_EN_CHANGE(IDC_EDIT_SATURATIONS, OnChangeEditSaturations)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELCHANGE,  OnSelChange)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_GAMMA, &CMVideoPage::OnNMReleasedcaptureSliderGamma)
	ON_EN_CHANGE(IDC_EDIT_GAMMA, &CMVideoPage::OnEnChangeEditGamma)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMVideoPage message handlers

int CMVideoPage::m_nColWidths[5] = { 5, 15, 15, 50};	// sixty-fourths
// InitListCtrl
void CMVideoPage::InitListCtrl(CXListCtrl * pList)
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
void CMVideoPage::FillListCtrl(CXListCtrl * pList)
{
	// a list of some more of my favorite things

	pList->LockWindowUpdate();

	pList->DeleteAllItems();

	// insert the items and subitems into the list
	m_rotate.RemoveAll();
	m_rotate.Add(ResStr(IDS_VIDEO_ROTA1));	
	m_rotate.Add(ResStr(IDS_VIDEO_ROTA2));
	m_rotate.Add(ResStr(IDS_VIDEO_ROTA3));
	m_rotate.Add(ResStr(IDS_VIDEO_ROTA4));
	
	m_deinterlacing.RemoveAll();
	m_deinterlacing.Add(ResStr(IDS_VIDEO_VD1));
	m_deinterlacing.Add(ResStr(IDS_VIDEO_VD2));
	m_deinterlacing.Add(ResStr(IDS_VIDEO_VD3));
	m_deinterlacing.Add(_T("FFmpeg"));
	m_deinterlacing.Add(_T("lowpass5"));
	
	m_deblocking.RemoveAll();
	m_deblocking.Add(ResStr(IDS_VIDEO_DB1));
	m_deblocking.Add(ResStr(IDS_VIDEO_DB2));
	m_deblocking.Add(ResStr(IDS_VIDEO_DB3));
	m_deblocking.Add(ResStr(IDS_VIDEO_DB4));
	m_deblocking.Add(ResStr(IDS_VIDEO_DB5));
	m_deblocking.Add(ResStr(IDS_VIDEO_DB6));

	pList->InsertItem(screenshot, _T(""));
	pList->SetCheckbox(screenshot, 0, 0);
	pList->SetItemText(screenshot, 1, ResStr(IDS_VIDEO_SST));
	pList->SetItemText(screenshot, 2, _T(""));
	pList->SetItemText(screenshot, 3,ResStr(IDS_VIDEO_SSTI));
	
	pList->InsertItem(ass, _T(""));
	pList->SetCheckbox(ass, 0, 0);
	pList->SetItemText(ass, 1, ResStr(IDS_VIDEO_ASS));
	pList->SetItemText(ass, 2, _T(""));
	pList->SetItemText(ass, 3, ResStr(IDS_VIDEO_ASSI));

	pList->InsertItem(expand, _T(""));
	pList->SetCheckbox(expand, 0, 0);
	pList->SetItemText(expand, 1,ResStr(IDS_VIDEO_EXP));
	pList->SetItemText(expand, 2, _T(":::::4/3"));
	pList->SetEdit(expand, 2);
	pList->SetItemText(expand, 3, ResStr(IDS_VIDEO_EXPI));
	pList->SetItemToolTipText(expand, 2, ResStr(IDS_VIDEO_EXPIT));
	
	pList->InsertItem(aspect, _T(""));
	pList->SetCheckbox(aspect, 0, 0);
	pList->SetItemText(aspect, 1, ResStr(IDS_VIDEO_ASP));
	pList->SetItemText(aspect, 2, _T("4:3"));
	pList->SetEdit(aspect, 2);
	pList->SetItemText(aspect, 3, ResStr(IDS_VIDEO_ASPI));

	pList->InsertItem(eq2, _T(""));
	pList->SetCheckbox(eq2, 0, 0);
	pList->SetItemText(eq2, 1, ResStr(IDS_VIDEO_EQ2));
	pList->SetItemText(eq2, 2, _T(""));
	pList->SetItemText(eq2, 3, ResStr(IDS_VIDEO_EQ2I));
	
	pList->InsertItem(hue, _T(""));
	pList->SetCheckbox(hue, 0, 0);
	pList->SetItemText(hue, 1, ResStr(IDS_VIDEO_HUE));
	pList->SetItemText(hue, 3, ResStr(IDS_VIDEO_HUEI));
	
	pList->InsertItem(flip, _T(""));
	pList->SetCheckbox(flip, 0, 0);
	pList->SetItemText(flip, 1, ResStr(IDS_VIDEO_FLP));
	pList->SetItemText(flip, 2, _T(""));
	pList->SetItemText(flip, 3, ResStr(IDS_VIDEO_FLPI));
	
	pList->InsertItem(rotate, _T(""));
	pList->SetCheckbox(rotate, 0, 0);
	pList->SetItemText(rotate, 1, ResStr(IDS_VIDEO_ROT));
	pList->SetComboBox(rotate, 2, TRUE,  &m_rotate,  5,  0,  FALSE);
	pList->SetItemText(rotate, 3, ResStr(IDS_VIDEO_ROTI));
	
	pList->InsertItem(crop, _T(""));
	pList->SetCheckbox(crop, 0, 0);
	pList->SetItemText(crop, 1, ResStr(IDS_VIDEO_CRO));
	pList->SetItemText(crop, 2, _T("640:480"));
	pList->SetEdit(crop, 2);
	pList->SetItemText(crop, 3, ResStr(IDS_VIDEO_CROI));
	pList->SetItemToolTipText(crop, 2, ResStr(IDS_VIDEO_CROIT));
	
	pList->InsertItem(deinterlacing, _T(""));
	pList->SetCheckbox(deinterlacing, 0, 0);
	pList->SetItemText(deinterlacing, 1, ResStr(IDS_VIDEO_DEI));
	pList->SetComboBox(deinterlacing, 2, TRUE,  &m_deinterlacing,  5,  0,  FALSE);
	pList->SetItemText(deinterlacing, 3, ResStr(IDS_VIDEO_DEII));
	
	pList->InsertItem(deblocking, _T(""));
	pList->SetCheckbox(deblocking, 0, 0);
	pList->SetItemText(deblocking, 1, ResStr(IDS_VIDEO_DEB));
	pList->SetComboBox(deblocking, 2, TRUE,  &m_deblocking,  6,  0,  FALSE);
	pList->SetItemText(deblocking, 3, ResStr(IDS_VIDEO_DEBI));
	
	pList->InsertItem(dering, _T(""));
	pList->SetCheckbox(dering, 0, 0);
	pList->SetItemText(dering, 1, ResStr(IDS_VIDEO_DER));
	pList->SetItemText(dering, 2, _T(""));
	pList->SetItemText(dering, 3, ResStr(IDS_VIDEO_DERI));
	
	pList->InsertItem(tmpnoise, _T(""));
	pList->SetCheckbox(tmpnoise, 0, 0);
	pList->SetItemText(tmpnoise, 1, ResStr(IDS_VIDEO_TNO));
	pList->SetItemText(tmpnoise, 2, _T(""));
	pList->SetItemText(tmpnoise, 3, ResStr(IDS_VIDEO_TNOI));
	
	pList->InsertItem(autolevels, _T(""));
	pList->SetCheckbox(autolevels, 0, 0);
	pList->SetItemText(autolevels, 1, ResStr(IDS_VIDEO_ATL));
	pList->SetItemText(autolevels, 2, _T(""));
	pList->SetItemText(autolevels, 3, ResStr(IDS_VIDEO_ATLI));

	pList->InsertItem(vf_other, _T(""));
	pList->SetCheckbox(vf_other, 0, 0);
	pList->SetItemText(vf_other, 1, ResStr(IDS_VIDEO_OTH));
	pList->SetItemText(vf_other, 2, _T(""));
	pList->SetEdit(vf_other, 2);
	pList->SetItemText(vf_other, 3, ResStr(IDS_VIDEO_OTHI));

	pList->UnlockWindowUpdate();
}

BOOL CMVideoPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List.EnableToolTips(TRUE);
	InitListCtrl(&m_List);
	FillListCtrl(&m_List);
	
	m_bc.SetRange(0,200);
	m_sc.SetRange(0,200);
	m_cc.SetRange(0,200);
	m_hc.SetRange(0,200);
	m_gc.SetRange(1,100);
	m_hue_s = 100;
	m_contrast_s = 100;
	m_saturation_s = 100;
	m_brightness_s = 100;
	m_gamma_s = 10;

	m_vo.AddString(ResStr(IDS_VIDEO_VO1));
	m_vo.AddString(ResStr(IDS_VIDEO_VO2));
	m_vo.AddString(ResStr(IDS_VIDEO_VO3));
	m_vo.AddString(ResStr(IDS_VIDEO_VO4));
	m_vo.AddString(ResStr(IDS_VIDEO_VO5));
	m_vo.AddString(ResStr(IDS_VIDEO_VO6));
	m_vo.AddString(ResStr(IDS_VIDEO_VO7));
	m_vo.AddString(ResStr(IDS_VIDEO_VO8));
	m_vo.AddString(ResStr(IDS_VIDEO_VO9));
	m_vo.AddString(ResStr(IDS_VIDEO_VO10));
	m_vo.SetCurSel(directx);
	
	m_mxcolor.AddString(ResStr(IDS_VIDEO_MX1));
	m_mxcolor.AddString(ResStr(IDS_VIDEO_MX2));
	m_mxcolor.AddString(ResStr(IDS_VIDEO_MX3));
	m_mxcolor.AddString(ResStr(IDS_VIDEO_MX4));
	m_mxcolor.SetCurSel(mx_g);
	
	InitFromConfig();
	
	int b = _tcstoul(m_color.Right(2), 0, 16);
	int g = _tcstoul(m_color.Right(4).Left(2), 0, 16);
	int r = _tcstoul(m_color.Right(6).Left(2), 0, 16);
	m_color_b.Color = RGB(r,g,b);
	m_color_b.DefaultColor =  ::GetSysColor(COLOR_APPWORKSPACE);
	m_color_b.TrackSelection= TRUE;
	m_color_b.CustomText= ResStr(IDS_VIDEO_COLMORE);
	m_color_b.DefaultText = ResStr(IDS_VIDEO_COLAT);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMVideoPage::SetNormal()
{
	m_noflash = TRUE;
	m_keepaspect = TRUE;
	m_vosd = FALSE;
	m_soft = FALSE;
	m_idx = FALSE;
	m_dr = FALSE;
	m_framedrop = FALSE;
	m_color = _T("0xffffff");
	m_saturation = _T("0");
	m_hue = _T("0");
	m_contrast = _T("0");
	m_brightness = _T("0");
	m_gamma = _T("1.0");
	m_hue_s = 100;
	m_contrast_s = 100;
	m_saturation_s = 100;
	m_brightness_s = 100;
	m_gamma_s = 10;
	m_vo.SetCurSel(directx);
	m_mxcolor.SetCurSel(mx_g);

	m_List.SetCheckbox(screenshot, 0, 1);
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
	m_List.SetCheckbox(tmpnoise, 0, 0);
	m_List.SetCheckbox(autolevels, 0, 0);
	m_List.SetCheckbox(vf_other, 0, 0);
	m_List.SetItemText(vf_other, 2, _T(""));
}

void CMVideoPage::SetHigh()
{
	SetNormal();
	m_vo.SetCurSel(gl);
	m_List.SetCheckbox(eq2, 0, 1);
	m_List.SetCheckbox(hue, 0, 1);
}

void CMVideoPage::SetLower()
{
	SetNormal();
	m_framedrop = TRUE;
}

void CMVideoPage::InitFromConfig()
{
	if(!m_cfg)
		return;
	int value_i;
	bool value_b;
	bool is_directx = false;
	CString value_s;
	CString value_sub;

	m_List.LockWindowUpdate();
	m_cfg->GetValue_Boolean(_T("autoexpand"),value_b);
	if(m_cfg->GetValue_Boolean(_T("keepaspect"),value_b))
	{
		if(!value_b)
			m_keepaspect = FALSE;
		else
			m_keepaspect = TRUE;
	}
	if(m_cfg->GetValue_Boolean(_T("idx"),value_b))
	{
		if(value_b)
			m_idx = TRUE;
		else
			m_idx = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("dr"),value_b))
	{
		if(!value_b)
			m_dr = FALSE;
		else
			m_dr = TRUE;
	}
	if(m_cfg->GetValue_Boolean(_T("framedrop"),value_b))
	{
		if(value_b)
			m_framedrop = TRUE;
		else
			m_framedrop = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("gl_fs_flash"),value_b,true))
	{
		if(!value_b)
			m_noflash = TRUE;
		else
			m_noflash = FALSE;
	}
	if(m_cfg->GetValue_Integer(_T("brightness"),value_i))
	{
		m_brightness_s = value_i + 100;
		//if(is_directx && !(m_List.GetCheckbox(eq2, 0)))
		//		m_brightness_s += 100;
		m_brightness.Format(_T("%d"),m_brightness_s - 100);
	}
	else if(m_cfg->GetValue_Integer(_T("cofing_brightness"),value_i,true))
	{
		m_brightness_s = value_i + 100;
		m_brightness.Format(_T("%d"),value_i);
	}
	if(m_cfg->GetValue_Integer(_T("contrast"),value_i))
	{
		m_contrast_s = value_i + 100;
		m_contrast.Format(_T("%d"),value_i);
	}
	else if(m_cfg->GetValue_Integer(_T("cofing_contrast"),value_i,true))
	{
		m_contrast_s = value_i + 100;
		m_contrast.Format(_T("%d"),value_i);
	}
	if(m_cfg->GetValue_Integer(_T("saturation"),value_i))
	{
		m_saturation_s = value_i + 100;
		m_saturation.Format(_T("%d"),value_i);
	}
	else if(m_cfg->GetValue_Integer(_T("cofing_saturation"),value_i,true))
	{
		m_saturation_s = value_i + 100;
		m_saturation.Format(_T("%d"),value_i);
	}
	if(m_cfg->GetValue_Integer(_T("cofing_gamma"),value_i,true))
	{
		m_gamma_s = value_i;
		m_gamma.Format(_T("%0.1f"),(double)value_i / 10.0);
	}
	if(m_cfg->GetValue_Integer(_T("hue"),value_i))
	{
		m_hue_s = value_i + 100;
		m_hue.Format(_T("%d"),value_i);
	}
	else if(m_cfg->GetValue_Integer(_T("cofing_hue"),value_i,true))
	{
		m_hue_s = value_i + 100;
		m_hue.Format(_T("%d"),value_i);
	}
	if(m_cfg->GetValue_String(_T("vo"),value_s))
	{
		if(value_s == _T("gl2") || value_s.Find(_T("gl2:")) == 0)
			m_vo.SetCurSel(gl2);
		else if(value_s == _T("gl") || value_s.Find(_T("gl:")) == 0)
		{
			m_vo.SetCurSel(gl);
			if(value_s.Find(_T(":yuv=4")) > 0)
				m_soft = 0;
			else
				m_soft = 1;
			if(value_s.Find(_T(":scaled-osd")) > 0)
				m_vosd = 1;
			else
				m_vosd = 0;
			int offset = value_s.Find(_T(":osdcolor="));
			value_s += _T(":");
			if(offset > 0)
			{
				offset += 10;
				int offsetend = value_s.Find(_T(":"),offset);
				if(offsetend > offset)
					m_color = value_s.Mid(offset,offsetend - offset);
			}
		}
		else if(value_s == _T("matrixview") || value_s.Find(_T("matrixview:")) == 0)
		{
			if(value_s.Find(_T("matrixview:cols=320:")) == 0)
				m_vo.SetCurSel(mx_h);
			else if(value_s.Find(_T("matrixview:cols=40:")) == 0)
				m_vo.SetCurSel(mx_l);
			else
				m_vo.SetCurSel(mx);
			if(value_s.Find(_T("color=1")) > 0)
				m_mxcolor.SetCurSel(mx_b);
			else if(value_s.Find(_T("color=2")) > 0)
				m_mxcolor.SetCurSel(mx_r);
			else if(value_s.Find(_T("color=3")) > 0)
				m_mxcolor.SetCurSel(mx_a);
			else
				m_mxcolor.SetCurSel(mx_g);
		}
		else if(value_s == _T("jpeg"))
			m_vo.SetCurSel(jpeg);
		else if(value_s == _T("png") || value_s.Find(_T("png:")) == 0)
			m_vo.SetCurSel(png);
		else if(value_s == _T("yuv4mpeg"))
			m_vo.SetCurSel(yuv4mpeg);
		else if(value_s == _T("null"))
			m_vo.SetCurSel(null);
		else
		{
			m_vo.SetCurSel(directx);
			is_directx = true;
		}
	}
	if(m_cfg->GetValue_Boolean(_T("flip"),value_b))
	{
		if(value_b)
			m_List.SetCheckbox(flip, 0, 1);
	}
	if(m_cfg->GetValue_String(_T("aspect"),value_s))
	{
		m_List.SetCheckbox(aspect, 0, 1);
		m_List.SetItemText(aspect, 2,value_s);
	}
	if(m_cfg->GetValue_String(_T("vf"),value_s))
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
			if(m_cfg->GetSubValue(value_s,_T("eq2"), value_sub))
			{
				if(value_sub.GetLength() >= 1)
				{
					int gamma_i =  (int)(StringToDouble(value_sub) * 10);
					if(gamma_i != 10)
					{
						m_gamma = value_sub;
						m_gamma_s = gamma_i;
					}
				}
			}
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
			if(m_cfg->HaveSubValue(value_sub,_T("tmpnoise") , _T("/"))
				|| m_cfg->HaveSubValue(value_sub,_T("tn") , _T("/")))
				m_List.SetCheckbox(tmpnoise, 0, 1);
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
		
		value_vf.TrimLeft(_T(","));
		if(value_vf.GetLength() > 1)
		{
			m_List.SetCheckbox(vf_other, 0, 1);
			m_List.SetItemText(vf_other, 2, value_vf);
		}
	}
	else
	{
		if(m_cfg->GetValue_Boolean(_T("cofing_eq2") , value_b, true))
		{
			if(value_b)
				m_List.SetCheckbox(eq2, 0, 1);
		}
		if(m_cfg->GetValue_Boolean(_T("cofing_use_hue") , value_b, true))
		{
			if(value_b)
				m_List.SetCheckbox(hue, 0, 1);
		}
		if(m_cfg->GetValue_String(_T("cofing_expand"),value_s,true))
			m_List.SetItemText(expand, 2,value_s);
		if(m_cfg->GetValue_String(_T("cofing_crop"),value_s,true))
			m_List.SetItemText(crop, 2,value_s);
	}
	m_List.UnlockWindowUpdate();
	UpdateData(FALSE);
}

void CMVideoPage::RemoveSubValue(CString &line ,CString subname ,CString sp)
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

void CMVideoPage::SaveConfig()
{
	if(!m_cfg)
		return;
	UpdateData(TRUE);
	
	m_List.LockWindowUpdate();
	if(!m_keepaspect)
		m_cfg->SetValue(_T("keepaspect"),_T("0"));
	else
		m_cfg->RemoveValue(_T("keepaspect"));
	
	if(m_framedrop)
		m_cfg->SetValue(_T("framedrop"),_T("1"));
	else
		m_cfg->RemoveValue(_T("framedrop"));
	
	if(m_idx)
		m_cfg->SetValue(_T("idx"),_T("1"));
	else
		m_cfg->RemoveValue(_T("idx"));

	if(m_dr)
		m_cfg->SetValue(_T("dr"),_T("1"));
	else
		m_cfg->RemoveValue(_T("dr"));
	
	if(m_noflash)
		m_cfg->RemoveValue(_T("gl_fs_flash"),true);
	else
		m_cfg->SetValue(_T("gl_fs_flash"),_T("1"),true, ex_option);
	
	
	if(m_List.GetCheckbox(flip, 0))
		m_cfg->SetValue(_T("flip"),_T("1"));
	else
		m_cfg->RemoveValue(_T("flip"));
	
	if(m_List.GetCheckbox(aspect, 0))
	{
		CString str = m_List.GetItemText(aspect, 2);
		str.TrimLeft(_T(" "));
		if(str.GetLength() > 0)
			m_cfg->SetValue(_T("aspect"),str);
		else
			m_cfg->RemoveValue(_T("aspect"));
	}
	else
		m_cfg->RemoveValue(_T("aspect"));

	int vvo = m_vo.GetCurSel();
	int vmxcolor = m_mxcolor.GetCurSel();
	CString glstr =  _T("gl");
	CString mxcolorstr;
	if(vmxcolor > 0 && vmxcolor <=3)
		mxcolorstr.Format(_T(":color=%d"),vmxcolor);
	else
		mxcolorstr = _T(":color=0");
	switch (vvo)
	{
	case gl:
		if(!m_soft)
			glstr += _T(":yuv=4") ;
		if(m_vosd)
			glstr += _T(":scaled-osd") ;
		if( m_color != _T("0xffffff"))
			glstr += _T(":osdcolor=") + m_color ;	
		m_cfg->SetValue(_T("vo") , glstr );
		break;
	case gl2:
		m_cfg->SetValue(_T("vo") , _T("gl2") );
		break;
	case mx:
		m_cfg->SetValue(_T("vo") , _T("matrixview") + mxcolorstr );
		break;
	case mx_l:
		m_cfg->SetValue(_T("vo") , _T("matrixview:cols=40:rows=30") + mxcolorstr);
		break;
	case mx_h:
		m_cfg->SetValue(_T("vo") , _T("matrixview:cols=320:rows=240") + mxcolorstr);
		break;
	case jpeg:
		m_cfg->SetValue(_T("vo") , _T("jpeg"));
		break;
	case png:
		m_cfg->SetValue(_T("vo") , _T("png:z=1"));
		break;
	case yuv4mpeg:
		m_cfg->SetValue(_T("vo") , _T("yuv4mpeg"));
		break;
	case null:
		m_cfg->SetValue(_T("vo") , _T("null"));
		break;
	default:
		m_cfg->SetValue(_T("vo") ,_T("directx"));
	}
	CString vf_str = _T("");
	if(m_List.GetCheckbox(ass, 0))
		vf_str += _T("ass,");
	if(m_List.GetCheckbox(screenshot, 0))
		vf_str += _T("screenshot,");

	bool use_coreavc = false;
	if(! m_cfg->IsRemoved(_T("vc")))
	{
		CString value_s;
		if(m_cfg->GetValue_String(_T("vc"),value_s))
		{
			value_s += _T(",");
			if(value_s.Find(_T("coreavc,")) >= 0)
				use_coreavc = true;
		}
	}
	if(! m_cfg->IsRemoved(_T("cofing_codecs"),true))
	{
		int value_i;
		if(m_cfg->GetValue_Integer(_T("cofing_codecs"),value_i,true))
		{
			if(value_i == 2)
				use_coreavc = true;
		}
	}
	
	bool use_ass_expand = false;
	if(! m_cfg->IsRemoved(_T("ass")))
	{
		bool value_b;
		if(m_cfg->GetValue_Boolean(_T("ass"),value_b))
		{
			if(value_b && ! m_cfg->IsRemoved(_T("ass_auto_expand"), true))
			{
				if(m_cfg->GetValue_Boolean(_T("ass_auto_expand"),value_b,true))
				{
					if(value_b)
						use_ass_expand = true;
				}
			}
		}
	}
	bool use_expand = false;
	if(!use_ass_expand)
	{
		if(m_List.GetCheckbox(expand, 0))
			use_expand = true;
		else
			use_coreavc = false;
	}
	else if(m_List.GetCheckbox(expand, 0))
	{
		ShowInfo(type_ass_expand);
		m_List.SetCheckbox(expand, 0, 0);
	}


	int veq = m_List.GetCheckbox(eq2, 0);
	int vhue = m_List.GetCheckbox(hue, 0);
	CString value;
	m_cfg->RemoveValue(_T("cofing_eq2"), true);
	m_cfg->RemoveValue(_T("cofing_use_hue"), true);
	m_cfg->RemoveValue(_T("cofing_brightness"), true);
	m_cfg->RemoveValue(_T("cofing_contrast"), true);
	m_cfg->RemoveValue(_T("cofing_saturation"), true);
	m_cfg->RemoveValue(_T("cofing_hue"), true);
	m_cfg->RemoveValue(_T("cofing_gamma"), true);
	m_cfg->RemoveValue(_T("brightness"));
	m_cfg->RemoveValue(_T("contrast"));
	m_cfg->RemoveValue(_T("saturation"));
	m_cfg->RemoveValue(_T("hue"));
	if(veq || vvo == gl || vvo == gl2 || use_coreavc)
	//	|| m_brightness_s > 100
	//	|| m_contrast_s > 100
	//	|| m_saturation_s > 100)
	{
		int adjust = 0;
		if(vvo == directx)
		{
			if(veq)
			{
				if(m_gamma_s == 10)
					vf_str += _T("eq2,");
				else
					vf_str += _T("eq2=") + m_gamma + _T(",");
			}
			else if(use_coreavc)
			{
				vf_str += _T("eq2,");
				ShowInfo(type_coreavc);
				m_List.SetCheckbox(eq2, 0 , 1);
			}
//			else
//				adjust = 100;
		}
		else if(veq)
		{
			if(m_gamma_s == 10)
			{
				if(use_coreavc && use_expand)
				{
					vf_str += _T("eq2,");
					ShowInfo(type_coreavc);
					m_List.SetCheckbox(eq2, 0 , 1);
				}
				else
					m_cfg->SetValue(_T("cofing_eq2"), _T("1") ,true ,ex_meditor);
			}
			else
				vf_str += _T("eq2=") + m_gamma + _T(",");
		}
		else
		{
			if(use_coreavc && use_expand)
			{
				vf_str += _T("eq2,");
				ShowInfo(type_coreavc);
				m_List.SetCheckbox(eq2, 0 , 1);
			}
			value.Format(_T("%d") , m_gamma_s);
			m_cfg->SetValue(_T("cofing_gamma"), value, true , ex_meditor);
		}

		if(m_brightness_s != 100)
		{
			value.Format(_T("%d") , m_brightness_s - 100 - adjust);
			m_cfg->SetValue(_T("brightness"), value);
		}
		if(m_contrast_s != 100)
		{
			value.Format(_T("%d") , m_contrast_s - 100);
			m_cfg->SetValue(_T("contrast"), value);
		}
		if(m_saturation_s != 100)
		{
			value.Format(_T("%d") , m_saturation_s - 100);
			m_cfg->SetValue(_T("saturation"), value);
		}
	}
	else
	{
		if(m_brightness_s != 100)
		{
			value.Format(_T("%d") , m_brightness_s - 100);
			m_cfg->SetValue(_T("cofing_brightness"), value , true , ex_meditor);
		}
		if(m_contrast_s != 100)
		{
			value.Format(_T("%d") , m_contrast_s - 100);
			m_cfg->SetValue(_T("cofing_contrast"), value, true , ex_meditor);
		}
		if(m_saturation_s != 100)
		{
			value.Format(_T("%d") , m_saturation_s - 100);
			m_cfg->SetValue(_T("cofing_saturation"), value, true , ex_meditor);
		}
		if(m_gamma_s != 10)
		{
			value.Format(_T("%d") , m_gamma_s);
			m_cfg->SetValue(_T("cofing_gamma"), value, true , ex_meditor);
		}
	}
	
	if(vhue || vvo == gl || vvo == gl2)//	|| m_hue_s > 100)
	{
		if(vvo == directx && vhue)
		{
			vf_str += _T("hue,");
		}
		else if(vhue)
			m_cfg->SetValue(_T("cofing_use_hue"), _T("1") ,true ,ex_meditor);

		if(m_hue_s != 100)
		{
			value.Format(_T("%d") , m_hue_s - 100);
			m_cfg->SetValue(_T("hue"), value);
		}
	}
	else
	{
		if(m_hue_s != 100)
		{
			value.Format(_T("%d") , m_hue_s - 100);
			m_cfg->SetValue(_T("cofing_hue"), value , true , ex_meditor);
		}
	}

	CString str_expand = m_List.GetItemText(expand, 2);
	str_expand.TrimLeft(_T(" "));
	str_expand.TrimRight(_T(" "));
	if(use_expand)
	{
		if(str_expand.GetLength() > 0)
		{
			vf_str += _T("expand=") + str_expand + _T(",");
			m_cfg->RemoveValue(_T("autoexpand"));
		}
		else
		{
			vf_str += _T("expand,");
			m_cfg->SetValue(_T("autoexpand"), _T("1"));
		}
		m_cfg->RemoveValue(_T("cofing_expand"),true);
	}
	else if(str_expand.GetLength() > 0)
		m_cfg->SetValue(_T("cofing_expand"),  str_expand, true , ex_meditor);
	else
	{
		m_cfg->RemoveValue(_T("autoexpand"));
		m_cfg->RemoveValue(_T("cofing_expand"),true);
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
		m_cfg->RemoveValue(_T("cofing_crop"),true);
	}
	else if(str_crop.GetLength() > 0)
		m_cfg->SetValue(_T("cofing_crop"),  str_crop, true , ex_meditor);
	else
		m_cfg->RemoveValue(_T("cofing_crop"),true);


	int vdi = m_List.GetCheckbox(deinterlacing, 0);
	int vdb = m_List.GetCheckbox(deblocking, 0);
	int vdr = m_List.GetCheckbox(dering, 0);
	int vtn = m_List.GetCheckbox(tmpnoise, 0);
	int val = m_List.GetCheckbox(autolevels, 0);
	
	if( vdi || vdb || vdr || vtn || val)
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
		if(vtn)
			vf_str += _T("tn/");
		if(val)
			vf_str += _T("al/");
		vf_str.TrimRight(_T("/"));
	}
	int bvf_other = m_List.GetCheckbox(vf_other, 0);
	if(bvf_other)
	{
		CString str_vf_other = m_List.GetItemText(vf_other, 2);
		str_vf_other.TrimLeft(_T(","));
		str_vf_other.TrimRight(_T(","));
		str_vf_other.TrimLeft();
		str_vf_other.TrimRight();
		if(str_vf_other.GetLength() > 1)
			vf_str += str_vf_other;
	}

	vf_str.TrimRight(_T(","));
	if(vf_str.GetLength() > 1)
		m_cfg->SetValue(_T("vf"),  vf_str);
	else
		m_cfg->RemoveValue(_T("vf"));
	m_List.UnlockWindowUpdate();
}

void CMVideoPage::OnReleasedcaptureSliderBringhtness(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_brightness.Format(_T("%d"),m_brightness_s - 100);
	UpdateData(FALSE);
	*pResult = 0;
}

void CMVideoPage::OnReleasedcaptureSliderContrast(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_contrast.Format(_T("%d"),m_contrast_s - 100);
	UpdateData(FALSE);
	
	*pResult = 0;
}

void CMVideoPage::OnReleasedcaptureSliderHue(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_hue.Format(_T("%d"),m_hue_s - 100);
	UpdateData(FALSE);
	
	*pResult = 0;
}

void CMVideoPage::OnReleasedcaptureSliderSaturations(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_saturation.Format(_T("%d"),m_saturation_s - 100);
	UpdateData(FALSE);
	
	*pResult = 0;
}

void CMVideoPage::OnChangeEditBringhtness() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int x = _ttoi(m_brightness);
	if(x >= -100 && x <= 100)
		m_brightness_s = x + 100;
	else if(x < -100)
		m_brightness_s = 0;
	else
		m_brightness_s = 100;
	UpdateData(FALSE);
}

void CMVideoPage::OnChangeEditContrast() 
{
	
	// TODO: Add your control notification handler code here
		UpdateData(TRUE);
	int x = _ttoi(m_contrast);
	if(x >= -100 && x <= 100)
		m_contrast_s = x + 100;
	else if(x < -100)
		m_contrast_s = 0;
	else
		m_contrast_s = 100;
	UpdateData(FALSE);
}

void CMVideoPage::OnChangeEditHue() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int x = _ttoi(m_hue);
	if(x >= -100 && x <= 100)
		m_hue_s = x + 100;
	else if(x < -100)
		m_hue_s = 0;
	else
		m_hue_s = 100;

	UpdateData(FALSE);
}

void CMVideoPage::OnChangeEditSaturations() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int x = _ttoi(m_saturation);
	if(x >= -100 && x <= 100)
		m_saturation_s = x + 100;
	else if(x < -100)
		m_saturation_s = 0;
	else
		m_saturation_s = 100;
	UpdateData(FALSE);
}

LONG CMVideoPage::OnSelChange(UINT /*lParam*/, LONG /*wParam*/)
{
	UpdateData(TRUE);
	COLORREF Color = m_color_b.GetColor();
	m_color.Format(_T("0x%02x%02x%02x"), GetRValue(Color), GetGValue(Color), GetBValue(Color));
	UpdateData(FALSE);
	return TRUE;
}

BOOL CMVideoPage::PreTranslateMessage(MSG* pMsg) 
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

void CMVideoPage::OnNMReleasedcaptureSliderGamma(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_gamma.Format(_T("%0.1f"),(double)m_gamma_s / 10.0);
	UpdateData(FALSE);
	*pResult = 0;
}

void CMVideoPage::OnEnChangeEditGamma()
{
	UpdateData(TRUE);
	double x = StringToDouble(m_gamma);
	if(x >= 0.1 && x <= 10)
		m_gamma_s = (int)(x * 10);
	else
		m_gamma_s = 10;
	UpdateData(FALSE);
}

void CMVideoPage::SetInfoDlg(CMShowInfoDlg *infoDlg)
{
	info = infoDlg;
}

void CMVideoPage::ShowInfo(int type)
{
	if(info)
	{
		if(info->IsShow(type))
			info->DoModal();
	}
}
