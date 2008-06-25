// MSubPage.cpp : 实现文件
//

#include "stdafx.h"
#include "meditor2.h"
#include "MSubPage.h"
#include "MConfig.h"


// CMSubPage 对话框

IMPLEMENT_DYNAMIC(CMSubPage, CDialog)

CMSubPage::CMSubPage(CWnd* pParent /*=NULL*/)
		: CDialog(CMSubPage::IDD, pParent)
		, m_ass_use_margins(FALSE)
		, m_noautosub(FALSE)
		, m_ass_expand_s(_T(""))
		, m_osdpercent(FALSE)
{
	m_cfg = NULL;
	m_dvdsub = FALSE;
	m_ass = FALSE;
	m_sub_delay = _T("0");
	m_sub_error = _T("8");
	m_font = _T("");
	m_font2 = _T("");
	m_size_s = _T("");
	m_color = _T("");
	m_bcolor = _T("");
	m_osd_font = _T("");
	m_osdsize_s = _T("");
	m_subpos = _T("90");
	m_slang = _T("zh,ch,chi,tw");
	m_subcp = _T("GBK,BIG5,UTF-8,UTF-16");
	m_str_at = ResStr(IDS_PLAYER_AUTO);
	m_str_no = ResStr(IDS_PLAYER_NONE);
	m_str_nco = ResStr(IDS_SUB_NORMAL_COL);

}

CMSubPage::~CMSubPage()
{
}

void CMSubPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_AUTOSCALE, m_autoscale);
	DDX_Control(pDX, IDC_COMBO_ALIGN, m_align);
	DDX_Control(pDX, IDC_COMBO_FUZZINESS, m_fuzziness);
	DDX_Control(pDX, IDC_COMBO_BCOLOR, m_bcolor_c);
	DDX_Control(pDX, IDC_COMBO_COLOR, m_color_c);
	DDX_Control(pDX, IDC_COMBO_F2, m_font2_c);
	DDX_Control(pDX, IDC_COMBO_F1, m_font_c);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_size);
	DDX_Check(pDX, IDC_CHECK_DVDSUB, m_dvdsub);
	DDX_Check(pDX, IDC_CHECK_ASS, m_ass);
	DDX_Text(pDX, IDC_EDIT_SUBDELAY, m_sub_delay);
	DDV_MaxChars(pDX, m_sub_delay, 9);
	DDX_Text(pDX, IDC_EDIT_ERROR, m_sub_error);
	DDV_MaxChars(pDX, m_sub_error, 3);
	DDX_CBString(pDX, IDC_COMBO_F1, m_font);
	DDX_CBString(pDX, IDC_COMBO_F2, m_font2);
	DDX_CBString(pDX, IDC_COMBO_SIZE, m_size_s);
	DDV_MaxChars(pDX, m_size_s, 3);
	DDX_CBString(pDX, IDC_COMBO_COLOR, m_color);
	DDX_CBString(pDX, IDC_COMBO_BCOLOR, m_bcolor);
	DDX_Text(pDX, IDC_EDIT_SUBPOS, m_subpos);
	DDV_MaxChars(pDX, m_subpos, 3);
	DDX_Text(pDX, IDC_EDIT_SLANG, m_slang);
	DDX_Text(pDX, IDC_EDIT_SUBCP, m_subcp);
	DDX_Check(pDX, IDC_CHECK_AUTOSUB, m_noautosub);
	DDX_Control(pDX, IDC_COMBO_ASSEXPAND, m_ass_expand);
	DDX_CBString(pDX, IDC_COMBO_ASSEXPAND, m_ass_expand_s);
	DDX_Control(pDX, IDC_COMBO_OSDFONT, m_osd_font_c);
	DDX_CBString(pDX, IDC_COMBO_OSDFONT, m_osd_font);
	DDX_Control(pDX, IDC_COMBO_OSD_SIZE, m_osdsize);
	DDX_CBString(pDX, IDC_COMBO_OSD_SIZE, m_osdsize_s);
	DDV_MaxChars(pDX, m_osdsize_s, 3);
	DDX_Control(pDX, IDC_COMBO_OSDMODE, m_osdmode);
	DDX_Control(pDX, IDC_COMBO_OSD_TIME, m_osdtime);
	DDX_Check(pDX, IDC_CHECK_PERCENT, m_osdpercent);
}


BEGIN_MESSAGE_MAP(CMSubPage, CDialog)
END_MESSAGE_MAP()


// CMSubPage 消息处理程序

BOOL CMSubPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_fuzziness.AddString(ResStr(IDS_SUB_FUZZ1));
	m_fuzziness.AddString(ResStr(IDS_SUB_FUZZ2));
	m_fuzziness.AddString(ResStr(IDS_SUB_FUZZ3));
	m_fuzziness.SetCurSel(name);

	m_ass_expand.AddString(m_str_no);
	m_ass_expand.AddString(m_str_at);
	m_ass_expand.AddString(_T("4:3"));
	m_ass_expand.AddString(_T("16:11"));
	m_ass_expand.AddString(_T("16:10"));
	m_ass_expand.AddString(_T("16:9"));
	m_ass_expand_s = m_str_at;

	m_align.AddString(ResStr(IDS_SUB_ALIG1));
	m_align.AddString(ResStr(IDS_SUB_ALIG2));
	m_align.AddString(ResStr(IDS_SUB_ALIG3));
	m_align.SetCurSel(center);

	m_autoscale.AddString(ResStr(IDS_SUB_SCAL1));
	m_autoscale.AddString(ResStr(IDS_SUB_SCAL2));
	m_autoscale.AddString(ResStr(IDS_SUB_SCAL3));
	m_autoscale.AddString(ResStr(IDS_SUB_SCAL4));
	m_autoscale.SetCurSel(diagonal);

	m_size.AddString(_T("2"));
	m_size.AddString(_T("2.5"));
	m_size.AddString(_T("3"));
	m_size.AddString(_T("3.5"));
	m_size.AddString(_T("4"));
	m_size.AddString(_T("4.5"));
	m_size.AddString(_T("5"));
	m_size_s = _T("3");

	m_color_c.AddString(m_str_nco);
	m_color_c.AddString(_T("ffffff00"));
	m_color_c.AddString(_T("ff000000"));
	m_color_c.AddString(_T("00ff0000"));
	m_color_c.AddString(_T("0000ff00"));
	m_color_c.AddString(_T("ffffff50"));

	m_bcolor_c.AddString(m_str_nco);
	m_bcolor_c.AddString(_T("ffffff00"));
	m_bcolor_c.AddString(_T("ff000000"));
	m_bcolor_c.AddString(_T("00ff0000"));
	m_bcolor_c.AddString(_T("0000ff00"));
	m_bcolor_c.AddString(_T("00000050"));

	m_osdsize.AddString(_T("2"));
	m_osdsize.AddString(_T("2.5"));
	m_osdsize.AddString(_T("3"));
	m_osdsize.AddString(_T("3.5"));
	m_osdsize.AddString(_T("4"));
	m_osdsize.AddString(_T("4.5"));
	m_osdsize.AddString(_T("5"));
	m_osdsize_s = _T("3");

	m_osdtime.AddString(ResStr(IDS_PLAYER_NOTIME));
	m_osdtime.AddString(ResStr(IDS_PLAYER_T1));
	m_osdtime.AddString(ResStr(IDS_PLAYER_T2));
	m_osdtime.AddString(ResStr(IDS_PLAYER_T3));
	m_osdtime.AddString(ResStr(IDS_PLAYER_T4));
	m_osdtime.AddString(ResStr(IDS_PLAYER_T5));
	m_osdtime.AddString(ResStr(IDS_PLAYER_T6));
	m_osdtime.AddString(ResStr(IDS_PLAYER_T7));
	m_osdtime.AddString(ResStr(IDS_PLAYER_T8));
	m_osdtime.AddString(ResStr(IDS_PLAYER_T9));
	m_osdtime.SetCurSel(time_none);

	m_osdmode.AddString(ResStr(IDS_PLAYER_NORMAL_MODE));
	m_osdmode.AddString(ResStr(IDS_PLAYER_SHOWTIME));
	m_osdmode.AddString(ResStr(IDS_PLAYER_SHOWREMAIN));
	m_osdmode.AddString(ResStr(IDS_PLAYER_NOOSD));
	m_osdmode.SetCurSel(osd_normal);

	m_bcolor = m_str_nco;
	m_color = m_str_nco;
	TCHAR szCurPath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szCurPath);

	TCHAR szFontPath[MAX_PATH + 1];
	SHGetSpecialFolderPath(NULL, szFontPath , CSIDL_FONTS , FALSE);

	::SetCurrentDirectory(szFontPath);

	CFileFind finder;
	if(finder.FindFile(_T("*.ttf"),0))
	{
		while(finder.FindNextFile())
		{
			m_font_c.AddString(finder.GetFileName());
			m_font2_c.AddString(finder.GetFileName());
			m_osd_font_c.AddString(finder.GetFileName());
		}
		CString str = finder.GetFileName();
		if(str.GetLength() > 1)
		{
			m_font_c.AddString(str);
			m_font2_c.AddString(str);
			m_osd_font_c.AddString(str);
		}
	}
	//if(finder.FindFile(_T("*.ttc"),0))
	//{
	//	while(finder.FindNextFile())
	//	{
	//		m_font_c.AddString(finder.GetFileName());
	//		m_font2_c.AddString(finder.GetFileName());
	//		m_osd_font_c.AddString(finder.GetFileName());
	//	}
	//	CString str = finder.GetFileName();
	//	if(str.GetLength() > 1)
	//	{
	//		m_font_c.AddString(str);
	//		m_font2_c.AddString(str);
	//		m_osd_font_c.AddString(str);
	//	}
	//}
	::SetCurrentDirectory(szCurPath);
	m_font2_c.AddString(_T(""));
	m_font = _T("simhei.ttf");
	m_osd_font = _T("simhei.ttf");

	InitFromConfig();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMSubPage::SetNormal()
{
	m_ass = TRUE;
	m_dvdsub = TRUE;
	m_size_s = _T("3");
	m_bcolor = m_str_nco;
	m_color = m_str_nco;
	m_ass_expand_s = m_str_at;
	m_sub_delay = _T("0");
	m_sub_error = _T("8");
	m_subpos = _T("90");
	m_slang = _T("zh,ch,chi,tw");
	m_osdsize_s = _T("3");
	m_osdtime.SetCurSel(time_none);	
	m_osdmode.SetCurSel(osd_normal);
	m_subcp = _T("GBK,BIG-5,CP932,CP949,UTF-8,UTF-16");
	m_fuzziness.SetCurSel(name);
	m_align.SetCurSel(center);
	m_autoscale.SetCurSel(diagonal);
}

void CMSubPage::SetHigh()
{
	SetNormal();
}

void CMSubPage::SetLower()
{
	SetNormal();
	m_ass = FALSE;
	m_dvdsub = FALSE;
	m_ass_expand_s = m_str_no;
}

void CMSubPage::InitFromConfig()
{
	if(!m_cfg)
		return;
	int value_i;
	bool value_b;
	double value_d;
	CString value_s;
	CString value_sub;

	if(m_cfg->GetValue_Boolean(_T("ass"),value_b))
	{
		if(value_b)
			m_ass = TRUE;
		else
			m_ass = FALSE;
	}
	if(m_cfg->GetValue_Boolean(_T("ass-use-margins"),value_b))
	{
		if(value_b)
			m_ass_expand_s = m_str_at;
		else
			m_ass_expand_s = m_str_no;
	}
	else
		m_ass_expand_s = m_str_no;
	if(m_cfg->GetValue_Boolean(_T("ass_auto_expand"),value_b,true))
	{
		if(value_b)
		{
			m_ass_expand_s = m_str_at;
			if(m_cfg->GetValue_String(_T("meditor_ass_aspect"),value_s,true))
				m_ass_expand_s = value_s;
			else
			{
				CString width,height;
				if(m_cfg->GetValue_String(_T("ass_expand_width"),value_s,true))
				{
					width = value_s;
					m_cfg->RemoveValue(_T("ass_expand_width"),true);
				}
				if(m_cfg->GetValue_String(_T("ass_expand_height"),value_s,true))
				{
					height = value_s;
					m_cfg->RemoveValue(_T("ass_expand_height"),true);
				}
				if(IsDigit(width) && IsDigit(height))
					m_ass_expand_s.Format(_T("%s:%s"),width,height);
			}
		}
		else
			m_ass_expand_s = m_str_no;
	}
	else
		m_ass_expand_s = m_str_no;

	if(m_cfg->GetValue_Boolean(_T("noautosub"),value_b))
	{
		if(value_b)
			m_noautosub = TRUE;
		else
			m_noautosub = FALSE;
	}
	if(m_cfg->GetValue_Integer(_T("osdlevel"),value_i))
	{
		switch (value_i)
		{
		case 0:
			m_osdmode.SetCurSel(osd_none);
			break;
		case 3:
			m_osdmode.SetCurSel(osd_play_time);
			break;
		case 4:
			m_osdmode.SetCurSel(osd_left_time);
			break;
		default:
			m_osdmode.SetCurSel(osd_normal);
		}
	}
	if(m_cfg->GetValue_Boolean(_T("osd_percent"),value_b,true))
	{
		if(value_b)
			m_osdpercent = TRUE;
		else
			m_osdpercent = FALSE;
	}
	if(m_cfg->GetValue_Integer(_T("osd_systime"),value_i,true))
	{
		if(value_i >= 1 && value_i <= 9)
			m_osdtime.SetCurSel(value_i);
		else
			m_osdtime.SetCurSel(time_none);
	}
	if(m_cfg->GetValue_String(_T("font"),value_s))
	{
		value_s.TrimLeft(_T('"'));
		value_s.TrimRight(_T('"'));
		m_osd_font = value_s;
	}
	if(m_cfg->GetValue_String(_T("subfont-osd-scale"),value_s))
	{
		m_osdsize_s = value_s;
	}
	if(m_cfg->GetValue_Integer(_T("spuaa"),value_i))
	{
		if(value_i  == 4)
			m_dvdsub = TRUE;
		else
			m_dvdsub = FALSE;
	}
	if(m_cfg->GetValue_Integer(_T("sub-ignore-errors"),value_i))
	{
		if(value_i >= 0 && value_i <= 500)
			m_sub_error.Format(_T("%d"),value_i);
	}
	if(m_cfg->GetValue_Integer(_T("sub-fuzziness"),value_i))
		m_fuzziness.SetCurSel(value_i);
	if(m_cfg->GetValue_Integer(_T("spualign"),value_i))
		m_align.SetCurSel(value_i);
	if(m_cfg->GetValue_Integer(_T("subalign"),value_i))
		m_align.SetCurSel(value_i);
	if(m_cfg->GetValue_Integer(_T("subfont-autoscale"),value_i))
		m_autoscale.SetCurSel(value_i);
	if(m_cfg->GetValue_Integer(_T("subpos"),value_i))
	{
		if(value_i >= 0 && value_i <= 100)
			m_subpos.Format(_T("%d"),value_i);
	}
	if(m_cfg->GetValue_Double(_T("subdelay"),value_d))
	{
		m_sub_delay.Format(_T("%0.3f"),value_d);
	}
	if(m_cfg->GetValue_String(_T("subfont"),value_s))
	{
		value_s.TrimLeft(_T('"'));
		value_s.TrimRight(_T('"'));

		int first = value_s.Find(_T(","));
		if( first > 0)
		{
			m_font = value_s.Left(first);
			m_font2 = value_s.Right(value_s.GetLength() - first - 1);
		}
		else
			m_font = value_s;
	}
	if(m_cfg->GetValue_String(_T("ass-color"),value_s))
		m_color = value_s;
	if(m_cfg->GetValue_String(_T("ass-border-color"),value_s))
		m_bcolor = value_s;
	if(m_cfg->GetValue_String(_T("slang"),value_s))
		m_slang = value_s;
	if(m_cfg->GetValue_String(_T("subcp"),value_s))
		m_subcp = value_s;
	if(m_cfg->GetValue_String(_T("subfont-text-scale"),value_s))
	{
		m_size_s = value_s;
	}
	UpdateData(FALSE);
}

void CMSubPage::SaveConfig()
{
	if(!m_cfg)
		return;
	UpdateData(TRUE);

	if(m_ass)
		m_cfg->SetValue(_T("ass"),_T("1"));
	else
		m_cfg->RemoveValue(_T("ass"));

	if(m_ass_expand_s != m_str_no)
	{
		m_cfg->SetValue(_T("ass-use-margins"),_T("1"));
		m_cfg->SetValue(_T("ass_auto_expand"),_T("1"),true);
		if(m_ass_expand_s != m_str_at)
		{
			int index = m_ass_expand_s.Find(_T(":"));
			int rightlen = m_ass_expand_s.GetLength() - index - 1;
			if(index > 0 &&rightlen > 0)
			{
				CString width_s = m_ass_expand_s.Left(index);
				CString height_s = m_ass_expand_s.Right(rightlen);
				if(IsDigit(width_s) && IsDigit(height_s))
				{
					int width = _ttoi(width_s);
					int height = _ttoi(height_s);
					if(height > 0)
					{
						CString ass_aspect;
						ass_aspect.Format(_T("%d") ,width * 100000 / height);
						m_cfg->SetValue(_T("ass_expand_aspect"),ass_aspect,true);
						m_cfg->SetValue(_T("meditor_ass_aspect"),m_ass_expand_s,true,ex_meditor);
					}
				}
			}
		}
		else
		{
			m_cfg->RemoveValue(_T("ass_expand_aspect"),true);
			m_cfg->RemoveValue(_T("meditor_ass_aspect"),true);
		}
	}
	else
	{
		m_cfg->RemoveValue(_T("ass-use-margins"));
		m_cfg->RemoveValue(_T("ass_auto_expand"),true);
		m_cfg->RemoveValue(_T("ass_expand_aspect"),true);
		m_cfg->RemoveValue(_T("meditor_ass_aspect"),true);
	}

	if(m_noautosub)
		m_cfg->SetValue(_T("noautosub"),_T("1"));
	else
		m_cfg->RemoveValue(_T("noautosub"));

	if(m_dvdsub)
		m_cfg->SetValue(_T("spuaa"),_T("4"));
	else
		m_cfg->RemoveValue(_T("spuaa"));

	int vsub_error = _ttoi(m_sub_error);
	if(vsub_error > 0 && vsub_error <= 500)
		m_cfg->SetValue(_T("sub-ignore-errors"), m_sub_error);
	else
		m_cfg->RemoveValue(_T("sub-ignore-errors"));

	int vsubpos = _ttoi(m_subpos);
	if(vsubpos >= 0 && vsubpos <= 100)
		m_cfg->SetValue(_T("subpos"), m_subpos);
	else
		m_cfg->RemoveValue(_T("subpos"));

	if(StringToDouble(m_sub_delay) != 0.0)
		m_cfg->SetValue(_T("subdelay"), m_sub_delay);
	else
		m_cfg->RemoveValue(_T("subdelay"));

	if(m_font2 == _T(""))
		m_cfg->SetValue(_T("subfont"), _T("\"") + m_font + _T("\""));
	else
		m_cfg->SetValue(_T("subfont"), _T("\"") + m_font + _T(",") + m_font2 +  _T("\""));

	if(m_size_s != _T(""))
		m_cfg->SetValue(_T("subfont-text-scale"),  m_size_s );
	else
		m_cfg->RemoveValue(_T("subfont-text-scale"));

	m_cfg->SetValue(_T("font"), _T("\"") + m_osd_font + _T("\""));
	m_cfg->SetValue(_T("subfont-osd-scale"),m_osdsize_s );

	if(m_color != m_str_nco)
		m_cfg->SetValue(_T("ass-color"),  m_color );
	else
	m_cfg->RemoveValue(_T("ass-color"));

	if(m_bcolor != m_str_nco)
		m_cfg->SetValue(_T("ass-border-color"),  m_bcolor );
	else
	m_cfg->RemoveValue(_T("ass-border-color"));

	if(m_slang != _T(""))
		m_cfg->SetValue(_T("slang"),  m_slang );
	else
		m_cfg->RemoveValue(_T("slang"));

	if(m_subcp != _T(""))
		m_cfg->SetValue(_T("subcp"),  m_subcp );
	else
		m_cfg->SetValue(_T("subcp"),  _T("GBK,BIG-5,CP932,CP949,UTF-8,UTF-16") );

	int vosdmode = m_osdmode.GetCurSel();
	switch (vosdmode)
	{
	case osd_play_time:
		m_cfg->SetValue(_T("osdlevel") ,_T("3") );
		break;
	case osd_left_time:
		m_cfg->SetValue(_T("osdlevel") ,_T("4") );
		break;
	case osd_none:
		m_cfg->SetValue(_T("osdlevel") ,_T("0") );
		break;
	default:
		m_cfg->RemoveValue(_T("osdlevel"));
	}

	if(m_osdpercent)
		m_cfg->SetValue(_T("osd_percent") ,_T("1") ,true, ex_osd);
	else
		m_cfg->RemoveValue(_T("osd_percent") ,true);


	int vosdtime = m_osdtime.GetCurSel();
	switch (vosdtime)
	{
	case time_t1:
		m_cfg->SetValue(_T("osd_systime") ,_T("1") ,true, ex_osd);
		break;
	case time_t2:
		m_cfg->SetValue(_T("osd_systime") ,_T("2") ,true, ex_osd);
		break;
	case time_t3:
		m_cfg->SetValue(_T("osd_systime") ,_T("3") ,true, ex_osd);
		break;
	case time_t4:
		m_cfg->SetValue(_T("osd_systime") ,_T("4") ,true, ex_osd);
		break;
	case time_dt1:
		m_cfg->SetValue(_T("osd_systime") ,_T("5") ,true, ex_osd);
		break;
	case time_dt2:
		m_cfg->SetValue(_T("osd_systime") ,_T("6") ,true, ex_osd);
		break;
	case time_dt3:
		m_cfg->SetValue(_T("osd_systime") ,_T("7") ,true, ex_osd);
		break;
	case time_dt4:
		m_cfg->SetValue(_T("osd_systime") ,_T("8") ,true, ex_osd);
		break;
	case time_ot:
		m_cfg->SetValue(_T("osd_systime") ,_T("9") ,true, ex_osd);
		break;
	default:
		m_cfg->RemoveValue(_T("osd_systime"),true);
	}

	int vautoscale = m_autoscale.GetCurSel();
	switch (vautoscale)
	{
	case 0:
		m_cfg->SetValue(_T("subfont-autoscale") ,_T("0") );
		break;
	case 1:
		m_cfg->SetValue(_T("subfont-autoscale") ,_T("1") );
		break;
	case 2:
		m_cfg->SetValue(_T("subfont-autoscale") ,_T("2") );
		break;
	default:
		m_cfg->SetValue(_T("subfont-autoscale") ,_T("3") );
	}

	int vfuzziness = m_fuzziness.GetCurSel();
	switch (vfuzziness)
	{
	case 0:
		m_cfg->SetValue(_T("sub-fuzziness") ,_T("0") );
		break;
	case 2:
		m_cfg->SetValue(_T("sub-fuzziness") ,_T("2") );
		break;
	default:
		m_cfg->SetValue(_T("sub-fuzziness") ,_T("1") );
	}

	int valign = m_align.GetCurSel();
	switch (valign)
	{
	case 0:
		m_cfg->SetValue(_T("subalign") ,_T("0") );
		m_cfg->SetValue(_T("spualign") ,_T("0") );
		break;
	case 2:
		m_cfg->SetValue(_T("subalign") ,_T("2") );
		m_cfg->SetValue(_T("spualign") ,_T("2") );
		break;
	default:
		m_cfg->SetValue(_T("subalign") ,_T("1") );
		m_cfg->SetValue(_T("spualign") ,_T("1") );
	}
}

BOOL CMSubPage::PreTranslateMessage(MSG* pMsg)
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
