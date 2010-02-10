// MAssosPage.cpp : 实现文件
//
#include "stdafx.h"
#include "meditor2.h"
#include "MAssosPage.h"
#include "reg.h"


// CMAssosPage 对话框

IMPLEMENT_DYNAMIC(CMAssosPage, CDialog)

CMAssosPage::CMAssosPage(CWnd* pParent /*=NULL*/)
	: CDialog(CMAssosPage::IDD, pParent)
{
	m_is_vista = FALSE;

	OSVERSIONINFO version;

	version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&version)) {
		if(version.dwMajorVersion >= 6)
			m_is_vista = TRUE;
	}

	m_str_list = ResStr(IDS_ASSOS_LIST);
	m_str_flash = ResStr(IDS_ASSOS_FLASH);
	m_str_v = ResStr(IDS_ASSOS_V);
	m_str_a = ResStr(IDS_ASSOS_A);

	m_alist.Add( _T("3gp"), _T("3GP ") + ResStr(IDS_ASSOS_MV), m_str_v, _T("13"));
	m_alist.Add( _T("3gp2"), _T("3GP ") + ResStr(IDS_ASSOS_MV), m_str_v, _T("13"));
	m_alist.Add( _T("asf"), _T("Advanced Streaming ") + ResStr(IDS_ASSOS_V), m_str_v, _T("16"));
	m_alist.Add( _T("avi"), _T("AVI ") + ResStr(IDS_ASSOS_AVI), m_str_v, _T("1"));
	m_alist.Add( _T("avs"), ResStr(IDS_ASSOS_AVS), m_str_v, _T("35"));
	m_alist.Add( _T("dat"), _T("VCD ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("5"));
	m_alist.Add( _T("divx"), _T("DIVX ") + ResStr(IDS_ASSOS_V), m_str_v, _T("35"));
	m_alist.Add( _T("evo"), _T("EVOB ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("35"));
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
	m_alist.Add( _T("pmp"), _T("PMP ") + ResStr(IDS_ASSOS_MOV), m_str_v, _T("38"));
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
	m_llist.Add( _T("m3u"), _T("M3U ") + ResStr(IDS_ASSOS_LISTFILE), m_str_list, _T("32"));
	m_llist.Add( _T("m3u8"), _T("M3U8 ") + ResStr(IDS_ASSOS_LISTFILE), m_str_list, _T("33"));
	m_llist.Add( _T("pls"), _T("PLS ") + ResStr(IDS_ASSOS_LISTFILE), m_str_list, _T("34"));

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);

	m_assoc_exe = m_program_dir + _T("\\codecs\\massoc.exe");

	if(!IsFileExist(m_assoc_exe))
		m_assoc_exe = m_program_dir + _T("\\tools\\massoc.exe");

	if(!IsFileExist(m_assoc_exe))
		m_assoc_exe = m_program_dir + _T("\\massoc.exe");

	CString right = m_program_dir.Right(8);
	if(right == _T("\\codecs\\"))
		m_program_dir = m_program_dir.Left(m_program_dir.GetLength() - 7);

	m_icons_dll = m_program_dir + _T("micons.dll");
	m_player_exe = m_program_dir + _T("mplayer.exe");
	m_editor_exe = m_program_dir + _T("meditor2.exe");
	if(!IsFileExist(m_editor_exe))
		m_editor_exe = m_program_dir + _T("meditor.exe");
}

CMAssosPage::~CMAssosPage()
{
}

void CMAssosPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMAssosPage, CDialog)
	ON_BN_CLICKED(IDC_ASSOS, OnBnClickedAssos)
	
	ON_BN_CLICKED(IDC_ASSOS_DEF, &CMAssosPage::OnBnClickedAssosDef)
END_MESSAGE_MAP()


// CMAssosPage 消息处理程序

BOOL CMAssosPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

bool CMAssosPage::AssosTypeDef(CString type, CString info, CString icons, bool isplaylist)
{
	CReg reg;
	CString SubKey, Name, Content;
	{
		SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites\\FileAssociations");
		Name =  _T(".") + type;
		Content = _T("MPlayer.AssocFile.") + type;
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

		Name =  _T("");
		SubKey =  _T("SOFTWARE\\Classes\\MPlayer.AssocFile.") + type;
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,info);

		Name =  _T("FriendlyTypeName");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,info);

		Name =  _T("");
		SubKey =  _T("SOFTWARE\\Classes\\MPlayer.AssocFile.") + type + _T("\\DefaultIcon");
		if(IsFileExist(_T("icons\\")  +type +_T(".ico")))
			Content = m_program_dir +_T("icons\\") +type +_T(".ico");
		else if(IsFileExist(m_icons_dll) && icons.GetLength() > 0)
			Content = m_icons_dll +_T(",") + icons;
		else
			Content =  m_player_exe + _T(",0");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name , Content);

		SubKey =  _T("SOFTWARE\\Classes\\MPlayer.AssocFile.") + type + _T("\\shell");
		if(!reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name ,_T("open")))
			return false;

		SubKey =  _T("SOFTWARE\\Classes\\MPlayer.AssocFile.") + type + _T("\\shell\\open");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name ,_T(""));

		SubKey =  _T("SOFTWARE\\Classes\\MPlayer.AssocFile.") + type + _T("\\shell\\open\\command");

		if(isplaylist)
			Content =  _T("\"") +m_player_exe +_T("\" -playlist \"%1\"");
		else
			Content =  _T("\"") +m_player_exe +_T("\" \"%1\"");

		if(!reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name , Content))
			return false;

	}
	return true;
}

void CMAssosPage::OnBnClickedAssos()
{
	if(m_is_vista) {
		CReg reg;
		CString SubKey, Name, Content;

		Name =  _T("MPlayer");
		Content = _T("SOFTWARE\\MPlayer\\Capabilites");
		SubKey =  _T("SOFTWARE\\RegisteredApplications");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

		Name =  _T("");
		Content = _T("");
		SubKey =  _T("SOFTWARE\\MPlayer");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

		SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
		Name =  _T("ApplicationDescription");
		Content = _T("MPlayer WW - The Movie Player.");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

		SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
		Name =  _T("ApplicationName");
		Content = _T("MPlayer WW");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

		SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
		Name =  _T("ApplicationIcon");
		Content = m_player_exe + _T(",0");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);


		for(int i =0; i < m_alist.m_type.GetSize(); i++) {
			AssosTypeDef(m_alist.m_type[i], m_alist.m_info[i], m_alist.m_icon[i]);
		}
		for(int i =0; i < m_llist.m_type.GetSize(); i++) {
			AssosTypeDef(m_llist.m_type[i], m_llist.m_info[i], m_llist.m_icon[i], true);
		}
	}
	ShellExecute(0, _T("open"), m_assoc_exe , NULL, NULL, SW_SHOW);
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

void CMAssosPage::OnBnClickedAssosDef()
{
	if(m_is_vista) {
		CReg reg;
		CString SubKey, Name, Content;

		Name =  _T("MPlayer");
		Content = _T("SOFTWARE\\MPlayer\\Capabilites");
		SubKey =  _T("SOFTWARE\\RegisteredApplications");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

		Name =  _T("");
		Content = _T("");
		SubKey =  _T("SOFTWARE\\MPlayer");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

		SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
		Name =  _T("ApplicationDescription");
		Content = _T("MPlayer WW - The Movie Player.");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

		SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
		Name =  _T("ApplicationName");
		Content = _T("MPlayer WW");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);

		SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
		Name =  _T("ApplicationIcon");
		Content = m_player_exe + _T(",0");
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE, SubKey, Name ,Content);


		for(int i =0; i < m_alist.m_type.GetSize(); i++) {
			AssosTypeDef(m_alist.m_type[i], m_alist.m_info[i], m_alist.m_icon[i]);
		}
		for(int i =0; i < m_llist.m_type.GetSize(); i++) {
			AssosTypeDef(m_llist.m_type[i], m_llist.m_info[i], m_llist.m_icon[i], true);
		}
		IApplicationAssociationRegistrationUI* pAARUI = NULL;
		HRESULT hr = ::CoCreateInstance( CLSID_ApplicationAssociationRegistrationUI
			, NULL, CLSCTX_INPROC, __uuidof( IApplicationAssociationRegistrationUI )
			, reinterpret_cast< void** >( &pAARUI ) );

		if ( SUCCEEDED( hr ) && pAARUI != NULL ) {
			hr = pAARUI->LaunchAdvancedAssociationUI( _T( "MPlayer" ) );
			pAARUI->Release();
		}
	}
}
