// MPlayList.cpp : implementation file
//

#include "stdafx.h"
#include "meditor2.h"
#include "MPlayList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPlayList dialog


CMPlayList::CMPlayList(CWnd* pParent /*=NULL*/)
	: CDialog(CMPlayList::IDD, pParent)
{
	m_pParent = pParent;
	//{{AFX_DATA_INIT(CMPlayList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMPlayList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAYLIST, m_Dlist);
	//{{AFX_DATA_MAP(CMPlayList)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMPlayList, CDialog)
	//{{AFX_MSG_MAP(CMPlayList)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, OnButtonClean)
	ON_LBN_DBLCLK(IDC_PLAYLIST, OnDblclkPlaylist)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPlayList message handlers

void CMPlayList::OnDropFiles(HDROP hDropInfo)
{
    TCHAR szFileName[MAX_PATH];
    int iFileNumber;
    // 得到拖拽操作中的文件个数
    iFileNumber = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
    for (int i = 0; i < iFileNumber; i++) 
    {
        // 得到每个文件名
        DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);
        // 把文件名添加到list中
        m_Dlist.AddString(szFileName);
		m_playlist->Add(szFileName);
    }
	CDialog::OnDropFiles(hDropInfo);
}

BOOL CMPlayList::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Dlist.SetHorizontalExtent(1000);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMPlayList::ReFlashList()
{
	for(int j = m_Dlist.GetCount() - 1; j >= 0 ; j--)
	{
		m_Dlist.DeleteString(j);
	}
	for(int i = 0; i < m_playlist->GetSize(); i++)
	{
		m_Dlist.AddString(m_playlist->GetAt(i));
	}
}

void CMPlayList::OnButtonAdd() 
{
	
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT
		,_T("Flash 动画(*.swf)|*.swf|所有文件(*.*)|*.*||"));

	dlg.m_ofn.lpstrFile = new TCHAR[102400];
	memset(dlg.m_ofn.lpstrFile,0,sizeof(TCHAR)   *   102400);
	dlg.m_ofn.nMaxFile = 102400;
	dlg.m_ofn.lpstrTitle=_T("添加Flash文件");
	if(dlg.DoModal()==IDOK)
	{
		POSITION pos = dlg.GetStartPosition();
		CString szFileName,szFilePath;
		int indxe=dlg.GetPathName().GetLength();
		while(pos)
		{
			szFilePath = dlg.GetNextPathName(pos); 
			if((szFileName = dlg.GetFileName()) == "")
				szFileName = szFilePath.Right(szFilePath.GetLength()- indxe -1);
			m_Dlist.AddString(szFilePath);
			//m_playlist->Add(szFilePath);
		}
	}
	dlg.DestroyWindow();
	::SetCurrentDirectory(szFilePath);
}

void CMPlayList::OnButtonDel() 
{
	int cr = m_Dlist.GetCurSel();
	if(cr < 0)
		return;
	m_Dlist.DeleteString(cr);
	if(cr == m_Dlist.GetCount())
		m_Dlist.SetCurSel(cr -1);
	else
		m_Dlist.SetCurSel(cr);

	m_playlist->RemoveAll();
	CString value;
	for(int i =  0; i < m_Dlist.GetCount() ; i++)
	{
		m_Dlist.GetText(i,value);
		m_playlist->Add(value);
	}
}

void CMPlayList::OnButtonClean() 
{
	for(int i = m_Dlist.GetCount() - 1; i >= 0 ; i--)
	{
		m_Dlist.DeleteString(i);
	}
	m_playlist->RemoveAll();
}

BOOL CMPlayList::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case   WM_KEYDOWN:
		switch(pMsg->wParam)
		{
		case   VK_DELETE:
			OnButtonDel();
			break;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMPlayList::OnDblclkPlaylist() 
{
	int cr = m_Dlist.GetCurSel();
	if(cr >= 0 && m_pParent)
		m_pParent->SendMessage(PLS_SELCHANGE, (WPARAM) cr, 0);
}

void CMPlayList::OnButtonUp() 
{
	int cr = m_Dlist.GetCurSel();
	if(cr > 0)
	{
		CString value;
		m_Dlist.GetText(cr,value);
		m_Dlist.DeleteString(cr);
		m_Dlist.InsertString(cr - 1,value);
		m_Dlist.SetCurSel(cr - 1);

		m_playlist->RemoveAll();
		for(int i =  0; i < m_Dlist.GetCount() ; i++)
		{
			m_Dlist.GetText(i,value);
			m_playlist->Add(value);
		}
	}
}

void CMPlayList::OnButtonDown() 
{
	int cr = m_Dlist.GetCurSel();
	if(cr < m_Dlist.GetCount() - 1)
	{
		CString value;
		m_Dlist.GetText(cr,value);
		m_Dlist.DeleteString(cr);
		m_Dlist.InsertString(cr + 1,value);
		m_Dlist.SetCurSel(cr + 1);

		m_playlist->RemoveAll();
		for(int i =  0; i < m_Dlist.GetCount() ; i++)
		{
			m_Dlist.GetText(i,value);
			m_playlist->Add(value);
		}
	}
}
