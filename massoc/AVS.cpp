#include "StdAfx.h"
#include "AVS.h"
#include "Reg.h"
#include "shared.h"

CAVS::CAVS(void)
{
}

CAVS::~CAVS(void)
{
}

void CAVS::Install(CString path)
{
	CReg reg;
	CString SubKey, Name, Content;

	SubKey = _T("SOFTWARE\\AviSynth");
	Name =  _T("plugindir2_5");
	Content = path + _T("codecs\\AviSynth\\plugins");
	if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE,SubKey,Name))
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name, Content);
	else{
		Content = reg.content;
		if(!IsFileExist(Content + _T("\\DirectShowSource.dll")))
			CopyFile(path + _T("codecs\\AviSynth\\plugins\\DirectShowSource.dll")
			, Content + _T("\\DirectShowSource.dll"), TRUE);
	}

	SubKey = _T("SOFTWARE\\AviSynth");
	Name =  _T("");
	Content = path + _T("codecs\\AviSynth");;
	if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE,SubKey,Name))
		reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name, Content);

	SubKey =  _T("SOFTWARE\\Classes\\CLSID\\{E6D6B700-124D-11D4-86F3-DB80AFD98778}");
	Name =  _T("");
	Content = _T("AviSynth");
	reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name, Content);

	SubKey =  _T("SOFTWARE\\Classes\\CLSID\\{E6D6B700-124D-11D4-86F3-DB80AFD98778}\\InProcServer32");
	Name =  _T("");
	Content = _T("AviSynth.dll");
	reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name, Content);

	Name =  _T("ThreadingModel");
	Content = _T("Apartment");
	reg.SetValue_S_STR(HKEY_LOCAL_MACHINE,SubKey, Name, Content);

}