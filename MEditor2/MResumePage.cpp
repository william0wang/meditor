// MResumePage.cpp : 实现文件
//

#include "stdafx.h"
#include "meditor2.h"
#include "MResumePage.h"
#include "MConfig.h"


// CMResumePage 对话框

IMPLEMENT_DYNAMIC(CMResumePage, CDialog)

CMResumePage::CMResumePage(CWnd* pParent /*=NULL*/)
	: CDialog(CMResumePage::IDD, pParent)
	, m_auto_resume(FALSE)
{
	no = ResStr(IDS_RESUME_NO);
	time = ResStr(IDS_RESUME_TIME);
	file = ResStr(IDS_RESUME_FILE);
	nol = ResStr(IDS_RESUME_NORMAL);
	m_maxnum = _T("10");
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
	m_configs_ex.Format(_T("%skk.ini"),szFilePath);
	m_number = -1;
}

CMResumePage::~CMResumePage()
{
}

void CMResumePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESUME, m_list);
	DDX_Check(pDX, IDC_CHECK_RESUME, m_auto_resume);
	DDX_Text(pDX, IDC_EDIT_NUM, m_maxnum);
	DDV_MaxChars(pDX, m_maxnum, 3);
	DDX_Control(pDX, IDC_RADIOAUTO, m_auto);
	DDX_Control(pDX, IDC_RADIOMANUAL, m_manual);
}


BEGIN_MESSAGE_MAP(CMResumePage, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_CLA, OnBnClickedButtonCla)
	ON_BN_CLICKED(IDC_RADIOAUTO, OnBnClickedRadioAuto)
	ON_BN_CLICKED(IDC_RADIOMANUAL, OnBnClickedRadioManual)
END_MESSAGE_MAP()


// CMResumePage 消息处理程序

int CMResumePage::m_nColWidths[4] = { 5, 8, 50};	// sixty-fourths
// InitListCtrl
void CMResumePage::InitListCtrl(CXListCtrl * pList)
{
	ASSERT(pList);
	if (!pList)
		return;

	// set column width according to window rect
	CRect rect;
	pList->GetWindowRect(&rect);

	int w = rect.Width() - 2;

	TCHAR *	lpszHeaders[] = { _tcsdup(no.GetBuffer()) , _tcsdup(time.GetBuffer()) , _tcsdup(file.GetBuffer()) , NULL };
	no.ReleaseBuffer();
	time.ReleaseBuffer();
	file.ReleaseBuffer();
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

BOOL CMResumePage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.EnableToolTips(TRUE);
	InitListCtrl(&m_list);

	m_auto.SetCheck(0);
	m_manual.SetCheck(1);

	m_list.LockWindowUpdate();
	m_list.DeleteAllItems();
	if(m_cfg)
	{
		bool value_b;
		int value_i;
		CString value_s;
		CString index;
		if(m_cfg->GetValue_Integer(_T("auto_resume"),value_i,true))
		{
			if(value_i == 1)
			{
				m_auto_resume = TRUE;
				m_auto.SetCheck(1);
				m_manual.SetCheck(0);
			}
			else if(value_i == 2)
			{
				m_auto_resume = TRUE;
				m_auto.SetCheck(0);
				m_manual.SetCheck(1);
			}
			else
				m_auto_resume = FALSE;
		}
		if(m_cfg->GetValue_String(_T("save_stats_num"),value_s,true))
		{
			m_maxnum = value_s;
		}
		if(m_cfg->GetValue_String(_T("Resume"),reinfo[0].Resume,true))
		{
			m_cfg->GetValue_String(_T("Time"),reinfo[0].Time,true);
			m_cfg->GetValue_String(_T("Audio"),reinfo[0].Audio,true);
			m_cfg->GetValue_String(_T("DVDSub"),reinfo[0].DVDSub,true);
			m_cfg->GetValue_String(_T("VOBSub"),reinfo[0].VOBSub,true);
			m_cfg->GetValue_String(_T("TXTSub"),reinfo[0].TXTSub,true);
			m_cfg->GetValue_String(_T("SubPos"),reinfo[0].SubPos,true);
			m_cfg->GetValue_String(_T("Filename"),reinfo[0].Filename,true);
			m_list.InsertItem(0, nol);
			m_list.SetItemText(0, 1, reinfo[0].Time);
			m_list.SetItemText(0, 2,reinfo[0].Filename);
			m_number = 0;
			for(int i = 1; i <= 10 ; i ++)
			{
				index.Format(_T("%02d"),i);
				if(m_cfg->GetValue_Boolean(_T("Resume") + index , value_b,true))
				{
					if(!value_b)
						break;
				}
				else
						break;
				m_cfg->GetValue_String(_T("Resume") + index,reinfo[i].Resume,true);
				m_cfg->GetValue_String(_T("Time") + index,reinfo[i].Time,true);
				m_cfg->GetValue_String(_T("Audio") + index,reinfo[i].Audio,true);
				m_cfg->GetValue_String(_T("DVDSub") + index,reinfo[i].DVDSub,true);
				m_cfg->GetValue_String(_T("VOBSub") + index,reinfo[i].VOBSub,true);
				m_cfg->GetValue_String(_T("TXTSub") + index,reinfo[i].TXTSub,true);
				m_cfg->GetValue_String(_T("SubPos") + index,reinfo[i].SubPos,true);
				m_cfg->GetValue_String(_T("Filename") + index,reinfo[i].Filename,true);
				m_number = i;
				m_list.InsertItem(i, index);
				m_list.SetItemText(i, 1, reinfo[i].Time);
				m_list.SetItemText(i, 2,reinfo[i].Filename);
			}
		}
	}
	m_list.UnlockWindowUpdate();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CMResumePage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
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

void CMResumePage::OnBnClickedButtonPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	int cur = m_list.GetCurSel();
	if(cur < 0)
		return;
	if(cur > 0)
	{
		reinfo[11] = reinfo[0];
		reinfo[0] = reinfo[cur];
		reinfo[cur] = reinfo[11];
		m_list.SetItemText(0, 1, reinfo[0].Time);
		m_list.SetItemText(0, 2,reinfo[0].Filename);
		m_list.SetItemText(cur, 1, reinfo[cur].Time);
		m_list.SetItemText(cur, 2,reinfo[cur].Filename);
		m_list.SetCurSel(0);
		reinfo[0].WritePrivateProfile(0, m_configs_ex);
		reinfo[cur].WritePrivateProfile(cur, m_configs_ex);
	}
	WritePrivateProfileInt(_T("Option"), _T("auto_resume") , 1, m_configs_ex);
	if(!IsFileExist(m_program_dir + _T("mplayer.exe")))
	{
		MessageBox(ResStr(IDS_PLAYER_SAMEDIR));
		return;
	}
	ShellExecute(0, _T("open"), m_program_dir +_T("mplayer.exe"), _T(""), NULL, SW_SHOW);
}

void CMResumePage::OnBnClickedButtonUp()
{
	// TODO: 在此添加控件通知处理程序代码
	int cur = m_list.GetCurSel();
	if(cur <= 0)
		return;
	reinfo[11] = reinfo[cur - 1];
	reinfo[cur - 1] = reinfo[cur];
	reinfo[cur] = reinfo[11];
	m_list.SetItemText(cur - 1, 1, reinfo[cur - 1].Time);
	m_list.SetItemText(cur - 1, 2,reinfo[cur - 1].Filename);
	m_list.SetItemText(cur, 1, reinfo[cur].Time);
	m_list.SetItemText(cur, 2,reinfo[cur].Filename);
	m_list.SetCurSel(cur - 1);
}

void CMResumePage::OnBnClickedButtonDown()
{
	// TODO: 在此添加控件通知处理程序代码
	int cur = m_list.GetCurSel();
	if(cur < 0 || cur >= m_list.GetItemCount() - 1)
		return;
	reinfo[11] = reinfo[cur + 1];
	reinfo[cur + 1] = reinfo[cur];
	reinfo[cur] = reinfo[11];
	m_list.SetItemText(cur + 1, 1, reinfo[cur + 1].Time);
	m_list.SetItemText(cur + 1, 2,reinfo[cur + 1].Filename);
	m_list.SetItemText(cur, 1, reinfo[cur].Time);
	m_list.SetItemText(cur, 2,reinfo[cur].Filename);
	m_list.SetCurSel(cur + 1);
}

void CMResumePage::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int cur = m_list.GetCurSel();
	if(cur < 0)
		return;
	for(int i = cur; i < m_list.GetItemCount() ; i++)
	{
		reinfo[i] = reinfo[i + 1];
		m_list.SetItemText(i, 1, reinfo[i].Time);
		m_list.SetItemText(i, 2,reinfo[i].Filename);
	}
	m_list.DeleteItem(m_number);
	m_number--;
}

void CMResumePage::OnBnClickedButtonCla()
{
	// TODO: 在此添加控件通知处理程序代码
	m_number = -1;
	m_list.DeleteAllItems();
}

void CMResumePage::SaveConfig()
{
	UpdateData(TRUE);
	if(m_auto_resume)
	{
		if (m_manual.GetCheck() == 1)
			m_cfg->SetValue(_T("auto_resume") ,_T("2") , true , ex_option);
		else
			m_cfg->SetValue(_T("auto_resume") ,_T("1") , true , ex_option);
	}
	else
		m_cfg->RemoveValue(_T("auto_resume") ,true);

	m_maxnum.TrimLeft();
	m_maxnum.TrimRight();
	if(m_maxnum != _T("10"))
		m_cfg->SetValue(_T("save_stats_num") ,m_maxnum, true , ex_setting);
	else
		m_cfg->RemoveValue(_T("save_stats_num") ,true);

	m_cfg->RemoveValue(_T("Resume") ,true);
	m_cfg->RemoveValue(_T("Time"),true);
	m_cfg->RemoveValue(_T("Audio") ,true);
	m_cfg->RemoveValue(_T("DVDSub"),true);
	m_cfg->RemoveValue(_T("VOBSub"),true);
	m_cfg->RemoveValue(_T("TXTSub") ,true);
	m_cfg->RemoveValue(_T("SubPos") ,true);
	m_cfg->RemoveValue(_T("Filename") ,true);
	
	CString index;
	for(int j = 1; j <= 10 ; j++)
	{
		index.Format(_T("%02d"),j);
		m_cfg->RemoveValue(_T("Resume")  + index,true);
		m_cfg->RemoveValue(_T("Time") + index,true);
		m_cfg->RemoveValue(_T("Audio") + index ,true);
		m_cfg->RemoveValue(_T("DVDSub") + index,true);
		m_cfg->RemoveValue(_T("VOBSub") + index,true);
		m_cfg->RemoveValue(_T("TXTSub") + index ,true);
		m_cfg->RemoveValue(_T("SubPos")  + index,true);
		m_cfg->RemoveValue(_T("Filename") + index ,true);
	}
	if(m_number >= 0)
	{
		m_cfg->SetValue(_T("Resume") ,reinfo[0].Resume, true , ex_status);
		m_cfg->SetValue(_T("Time") ,reinfo[0].Time, true , ex_status);
		m_cfg->SetValue(_T("Audio") ,reinfo[0].Audio, true , ex_status);
		m_cfg->SetValue(_T("DVDSub") ,reinfo[0].DVDSub, true , ex_status);
		m_cfg->SetValue(_T("VOBSub") ,reinfo[0].VOBSub, true , ex_status);
		m_cfg->SetValue(_T("TXTSub") ,reinfo[0].TXTSub, true , ex_status);
		m_cfg->SetValue(_T("SubPos") ,reinfo[0].SubPos, true , ex_status);
		m_cfg->SetValue(_T("Filename") ,reinfo[0].Filename,true , ex_status);
	}
	for(int i = 1; i <= m_number ; i++)
	{
		index.Format(_T("%02d"),i);
		m_cfg->SetValue(_T("Resume") + index ,reinfo[i].Resume, true , ex_status);
		m_cfg->SetValue(_T("Time") + index,reinfo[i].Time, true , ex_status);
		m_cfg->SetValue(_T("Audio") + index,reinfo[i].Audio, true , ex_status);
		m_cfg->SetValue(_T("DVDSub") + index,reinfo[i].DVDSub, true , ex_status);
		m_cfg->SetValue(_T("VOBSub") + index,reinfo[i].VOBSub, true , ex_status);
		m_cfg->SetValue(_T("TXTSub") + index,reinfo[i].TXTSub, true , ex_status);
		m_cfg->SetValue(_T("SubPos") + index,reinfo[i].SubPos, true , ex_status);
		m_cfg->SetValue(_T("Filename") + index,reinfo[i].Filename,true , ex_status);
	}
}

void resume_info::WritePrivateProfile(int index, CString m_configs_ex)
{
	if( index < 0 || index > 10)
		return;
	CString str = _T("");
	if(index != 0)
			str.Format(_T("%02d"),index);
	WritePrivateProfileInt(_T("Status"), _T("Resume") + str	, 1, m_configs_ex);
	WritePrivateProfileString(_T("Status"), _T("Time") + str	, Time , m_configs_ex);
	WritePrivateProfileString(_T("Status"), _T("Audio") + str	, Audio , m_configs_ex);
	WritePrivateProfileString(_T("Status"), _T("DVDSub") + str	, DVDSub , m_configs_ex);
	WritePrivateProfileString(_T("Status"), _T("VOBSub") + str, VOBSub, m_configs_ex);
	WritePrivateProfileString(_T("Status"), _T("TXTSub") + str, TXTSub , m_configs_ex);
	WritePrivateProfileString(_T("Status"), _T("SubPos") + str, SubPos, m_configs_ex);
	WritePrivateProfileString(_T("Status"), _T("Filename") + str	, Filename, m_configs_ex);
}

void CMResumePage::OnBnClickedRadioAuto()
{
	m_manual.SetCheck(0);
}

void CMResumePage::OnBnClickedRadioManual()
{
	m_auto.SetCheck(0);
}
