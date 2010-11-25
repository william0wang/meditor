// MConfig.cpp: implementation of the CMConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "shared.h"
#include "AtlStdioFile.h"
#include "MConfig.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMConfigData::CMConfigData()
{
	m_config.name.RemoveAll();
	m_config.value.RemoveAll();
	m_config.state.RemoveAll();
	m_config_ex.name.RemoveAll();
	m_config_ex.value.RemoveAll();
	m_config_ex.type.RemoveAll();
	m_config_ex.state.RemoveAll();
}

void CMConfigData::AddConfig(CString name, CString value, bool changed)
{
	m_config.name.Add(name);
	m_config.value.Add(value);
	if(changed)
		m_config.state.Add(2);
	else
		m_config.state.Add(0);
}

void CMConfigData::AddConfigEx(CString name, CString value,int type, bool changed)
{
	m_config_ex.name.Add(name);
	m_config_ex.value.Add(value);
	m_config_ex.type.Add(type);
	if(changed)
		m_config_ex.state.Add(2);
	else
		m_config_ex.state.Add(1);
}

void CMConfigData::Remove(CString name)
{
	for(int i = 0; i < m_config.name.GetSize(); i++) {
		if(m_config.name[i] == name) {
			m_config.state[i] = 0;
			break;
		}
	}
}

void CMConfigData::RemoveEx(CString name)
{
	name.MakeLower();
	for(int i = 0; i < m_config_ex.name.GetSize(); i++) {
		if(m_config_ex.name[i] == name) {
			m_config_ex.state[i] = 0;
			break;
		}
	}
}

void CMConfigData::SetValue(CString name, CString value, bool changed)
{
	bool finded = false;
	for(int i = 0; i < m_config.name.GetSize(); i++) {
		if(m_config.name[i] == name) {
			if(name == _T("vf-add") || name == _T("af-add")) {
				m_config.value[i] += _T(",") + value;
			} else {
				m_config.value[i] = value;
				if(changed)
					m_config.state[i] = 2;
				finded = true;
			}
			break;
		}
	}
	if(!finded)
		AddConfig(name,value,changed);
}

void CMConfigData::SetValueEx(CString name, CString value,int type, bool changed)
{
	name.MakeLower();
	bool finded = false;
	for(int i = 0; i < m_config_ex.name.GetSize(); i++) {
		if(m_config_ex.name[i] == name) {
			m_config_ex.value[i] = value;
			m_config_ex.type[i] = type;
			if(changed)
				m_config_ex.state[i] = 2;
			finded = true;
			break;
		}
	}
	if(!finded)
		AddConfigEx(name,value,type,changed);
}

bool CMConfigData::GetValue(CString name, CString &value)
{
	for(int i = 0; i < m_config.name.GetSize(); i++) {
		if(m_config.name[i] == name) {
			value = m_config.value[i];
			m_config.state[i] = 1;
			return true;
		}
	}
	return false;
}

bool CMConfigData::GetValueEx(CString name, CString &value)
{
	name.MakeLower();
	for(int i = 0; i < m_config_ex.name.GetSize(); i++) {
		if(m_config_ex.name[i] == name) {
			value = m_config_ex.value[i];
			m_config_ex.state[i] = 1;
			return true;
		}
	}
	return false;
}

bool CMConfigData::IsRemoved(CString name)
{
	for(int i = 0; i < m_config.name.GetSize(); i++) {
		if(m_config.name[i] == name) {
			if(m_config.state[i] == 0)
				return true;
		}
	}
	return false;
}

bool CMConfigData::IsRemovedEx(CString name)
{
	name.MakeLower();
	for(int i = 0; i < m_config_ex.name.GetSize(); i++) {
		if(m_config_ex.name[i] == name) {
			if(m_config_ex.state[i] == 0)
				return true;
		}
	}
	return false;
}

CString CMConfigData::GetValue_Other()
{
	CString return_str = _T("");
	for(int i = 0 ; i < m_config.name.GetSize() ; i ++) {
		if(m_config.state[i] == 0) {
			if(m_config.name[i] == _T("###"))
				return_str += m_config.value[i] + _T("\r\n");
			else {
				return_str += m_config.name[i] + _T("=")
					+ m_config.value[i] + _T("\r\n");
			}
		}
	}
	return return_str;
}

CString CMConfigData::GetValue_OtherEx()
{
	CString return_str = _T("");
	for(int i = 0 ; i < m_config_ex.name.GetSize() ; i ++) {
		if(m_config_ex.state[i] == 0) {
			if(m_config_ex.name[i] == _T("###"))
				return_str += m_config_ex.value[i] + _T("\r\n");
			else {
				return_str += m_config_ex.name[i] + _T("=")
					+ m_config_ex.value[i] + _T("\r\n");
			}
		}
	}
	return return_str;
}

CString CMConfigData::GetConfig()
{
	CString return_str = _T("[default]\r\n");
	for(int i = 0 ; i < m_config.name.GetSize() ; i ++) {
		if(m_config.state[i] > 0) {
			if(m_config.name[i] == _T("###"))
				return_str += m_config.value[i] + _T("\r\n");
			else {
				return_str += m_config.name[i] + _T("=")
					+ m_config.value[i] + _T("\r\n");
			}
		}
	}
	return return_str;
}

CString CMConfigData::GetConfigEx()
{
	CString return_str = _T("");
	for(unsigned int j = ex_option ; j <= ex_meditor ; j ++)
	{
		switch(j)
		{
		case ex_option:
			return_str +=  _T("[Option]\r\n");
			break;
		case ex_status:
			return_str +=  _T("\r\n[Status]\r\n");
			break;
		case ex_setting:
			return_str +=  _T("\r\n[Setting]\r\n");
			break;
		case ex_gui:
			return_str +=  _T("\r\n[Gui]\r\n");
			break;
		case ex_osd:
			return_str +=  _T("\r\n[OSD]\r\n");
			break;
		case ex_sysinfo:
			return_str +=  _T("\r\n[Sysinfo]\r\n");
			break;
		case ex_theme:
			return_str +=  _T("\r\n[Theme]\r\n");
			break;
		case ex_meditor:
			return_str +=  _T("\r\n[meditor]\r\n");
			break;
		}
		for(int i = 0 ; i < m_config_ex.name.GetSize() ; i ++) {
			if(m_config_ex.state[i] > 0 && m_config_ex.type[i] == j) {
				return_str += m_config_ex.name[i] + _T("=")
					+ m_config_ex.value[i] + _T("\r\n");
			}
		}
	}
	return return_str;
}

CMConfig::CMConfig()
{
	m_now_type = ex_option;
	m_other_config = _T("");
}

CMConfig::~CMConfig()
{

}

void CMConfig::LoadConfig(CString filename, bool ex)
{
	CAtlStdioFile playercfg;
	if(SUCCEEDED(playercfg.OpenFile(filename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING))) {
		if(!ex)
			m_now_type = cf_default;
		CStringA lineA;
		while(playercfg.ReadLineA(lineA)) {
			AnalyseLine(local2unicode(lineA.GetBuffer()).c_str(), ex);
			lineA.ReleaseBuffer();
		}
		playercfg.Close();
	}
	if(!ex)
		LoadConfigSP(filename);
}

void CMConfig::LoadConfigSP(CString filename)
{
	CAtlStdioFile playercfg;
	m_spcfg.clear();
	if(SUCCEEDED(playercfg.OpenFile(filename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING))) {
		CStringA lineA;
		while(playercfg.ReadLineA(lineA)){
			CString line = local2unicode(lineA.GetBuffer()).c_str();
			lineA.ReleaseBuffer();
			if(AnalyseLineSP(line)) {
				line.TrimLeft(_T(" "));
				line.TrimRight(_T(" "));
				line.TrimLeft(_T("["));
				line.TrimRight(_T("]"));
				ConfigDataSP sp;
				sp.type = line;
				m_spcfg.push_back(sp);
			}
		}
		playercfg.Close();
	}
	ConfigSP::iterator vi;

	CString name[] = {_T("alang"), _T("slang"), _T("delay"), _T("vf"), _T("flip"), _T("af")
		, _T("autosync"), _T("mc"), _T("af-adv"), _T("nocache"), _T("dshow-demux"), _T("")}; 

	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++) {
		CString types = vi->type;
		CString value;
		TCHAR vl[MAX_PATH];
	
		for(int i = 0; name[i].GetLength() > 1 ;i++) {
			GetPrivateProfileString(types, name[i], _T(""), vl, MAX_PATH, filename);
			value.Format(_T("%s"),vl);
			if(value != _T("")) {
				ConfigDataSP_info info;
				info.name = name[i];
				info.value = value;
				info.uesed = true;
				vi->info.push_back(info);
			}
		}
	}
}

bool CMConfig::AnalyseLineSP(CString line)
{
	line.TrimLeft(_T(" "));
	line.TrimRight(_T(" "));
	line.MakeLower();
	int len = line.GetLength() - 1;
	if(line[0] == L'[' && line[len] == L']' && line != _T("[default]"))
		return true;
	return false;
}

CString CMConfig::GetConfigSP()
{
	CString return_str = _T("");

	ConfigSP::iterator vi;
	ConfigSP_info::iterator fi;
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++) {
		return_str += _T("\r\n[") + vi->type + _T("]\r\n");
		for(fi = vi->info.begin(); fi != vi->info.end(); fi++) {
			if(fi->uesed)
				return_str += fi->name + _T("=") + fi->value + _T("\r\n");
		}

	}
	return return_str;
}

void CMConfig::AnalyseLine(CString line, bool ex)
{
	if(line.GetLength() < 1)
		return;
	CString name, value;
	value = line;
	value.TrimLeft(_T(" "));
	value.TrimRight(_T(" "));
	value.MakeLower();
	if(ex) {
		line.MakeLower();
		if(value == _T("[option]"))
			m_now_type = ex_option;
		else if(value == _T("[status]"))
			m_now_type = ex_status;
		else if(value == _T("[setting]"))
			m_now_type = ex_setting;
		else if(value == _T("[gui]"))
			m_now_type = ex_gui;
		else if(value == _T("[osd]"))
			m_now_type = ex_osd;
		else if(value == _T("[sysinfo]"))
			m_now_type = ex_sysinfo;
		else if(value == _T("[theme]"))
			m_now_type = ex_theme;
		else if(value == _T("[meditor]"))
			m_now_type = ex_meditor;
	} else {
		int len = value.GetLength() - 1;
		if(value[0] == L'[' && value[len] == L']' && value == _T("[default]"))
			m_now_type = cf_default;
		else if(value[0] == L'[' && value[len] == L']' )
			m_now_type = cf_other;
	}

	line.TrimLeft(_T(" "));
	if(line.Find(_T("#")) == 0) {
		if(line.Find(_T("Mplayer Config Options")) >= 0)
			return;
		if(line.Find(_T("Write your default config options here!")) >= 0)
			return;
		line.TrimRight(_T(" "));
		m_pconfig.AddConfig(_T("###"),line);
		return;
	}
	line += _T("#");
	line.TrimLeft(_T(" "));
	int sp = -1;
	sp = line.Find(_T("="));
	if(sp <= 0)
		return;
	name = line.Left(sp);
	line.Delete(0,sp + 1);

	line.TrimLeft(_T(" "));
	sp = line.Find(_T("#"));
	if(sp <= 0)
		return;
	value = line.Left(sp);
	name.TrimRight(_T(" "));
	value.TrimRight(_T(" "));
	if(ex)
		m_pconfig.SetValueEx(name, value, m_now_type, false);
	else if(m_now_type == cf_default)
		m_pconfig.SetValue(name, value, false);
}

void CMConfig::SaveConfig(CString filename, bool ex)
{
	CAtlStdioFile playercfg;
	if(SUCCEEDED(playercfg.OpenFile(filename, GENERIC_WRITE, 0, CREATE_ALWAYS))) {
		if(ex) {
			CString cfg = m_pconfig.GetConfigEx();
			cfg.MakeLower();
			string out = unicode2local(cfg.GetBuffer());
			cfg.ReleaseBuffer();
			playercfg.Write(out.c_str(), out.length());
		} else {
			string out = "## Mplayer Config Options\r\n\r\n";
			playercfg.Write(out.c_str(), out.length());

			CString cfg = m_pconfig.GetConfig();
			if(m_other_config != _T(""))
				cfg += m_other_config;
			cfg += GetConfigSP();
			out = unicode2local(cfg.GetBuffer());
			cfg.ReleaseBuffer();
			playercfg.Write(out.c_str(), out.length());
		}
		playercfg.Close();
	}
}

bool CMConfig::IsRemoved(CString name, bool ex)
{
	if(ex)
		return m_pconfig.IsRemovedEx(name);
	else
		return m_pconfig.IsRemoved(name);
}

bool CMConfig::GetString(CString name, CString &value, bool ex)
{
	if(ex)
		return m_pconfig.GetValueEx(name, value);
	else
		return m_pconfig.GetValue(name, value);
}

int CMConfig::GetInteger(CString name, int def_value, bool ex)
{
	CString value_str;
	if(GetString(name, value_str, ex))
		return _ttoi(value_str);

	return def_value;
}

double CMConfig::GetDouble(CString name, double def_value, bool ex)
{
	CString value_str;
	if(GetString(name, value_str, ex))
		return _ttof(value_str);
	return def_value;
}

bool CMConfig::GetBoolean(CString name, bool def_value, bool ex)
{
	CString value_str;
	if(GetString(name, value_str, ex)) {
		value_str.MakeLower();
		if(value_str == _T("yes"))
			return true;
		else if(value_str == _T("no"))
			return false;
		else if(_ttoi(value_str))
			return true;
		else
			return false;
	}
	return def_value;
}
	
void CMConfig::SetValue(CString name, CString value, bool ex, int type)
{
	if(ex)
		m_pconfig.SetValueEx(name,value,type);
	else
		m_pconfig.SetValue(name,value);
}

bool CMConfig::HaveSubValue(CString value, CString subname, CString sp)
{
	value += sp;
	int offset = value.Find(subname + sp);
	if(offset <0)
		offset = value.Find(subname + _T("="));
	if(offset > 0)
		offset = value.Find(sp + subname);
	if(offset < 0)
		return false;
	return true;
}

bool CMConfig::GetSubValue(CString value, CString subname, CString &subvalue,int index, CString sp)
{
	if(value.Find(subname + sp) >= 0) {
		subvalue = _T("");
		return true;
	}
	value += sp;
	int offset = value.Find(subname);
	if(offset < 0)
		return false;
	int offset1 = value.Find(_T("="),offset);
	if(offset1 < 0)
		return false;
	int offset2 = -1;
	if(index == 0) {
		offset2 = value.Find(sp ,offset1 + 1);
		if(offset2 < 0)
			return false;
		subvalue = value.Mid(offset1 + 1 , offset2 - offset1 - 1);
		return true;
	}
	for(int i = 0 ; i < index ; i ++) {
		offset2 = value.Find(_T(":"),offset1 + 1);
		if(offset2 < 0)
			offset2 = value.Find(sp,offset1 + 1);
		if(offset2 < 0)
			return false;
		subvalue = value.Mid(offset1 + 1 , offset2 - offset1 - 1);
		offset1 = offset2;
	}
	return true;
}

void CMConfig::RemoveValue(CString name, bool ex)
{
	if(ex)
		m_pconfig.RemoveEx(name);
	else
		m_pconfig.Remove(name);
}

CString CMConfig::GetOtherValue(bool ex)
{
	if(ex)
		return m_pconfig.GetValue_OtherEx();
	
	return m_pconfig.GetValue_Other();
}

void CMConfig::SetValue_Other(CString Other, bool ex)
{
	if(!ex)
		m_other_config = Other;
}

bool CMConfig::GetStringSP(CString type, CString name, CString &value)
{
	bool return_value = false;
	ConfigSP::iterator vi;
	ConfigSP_info::iterator fi;
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++) {
		if(vi->type == type) {
			for(fi = vi->info.begin(); fi != vi->info.end(); fi++) {
				if(fi->name == name) {
					if(! fi->uesed)
						return false;
					return_value = true;
					value = fi->value;
					break;
				}
			}
			break;
		}
	}
	return return_value;
}

int CMConfig::GetIntegerSP(CString type, CString name, int def_value)
{
	CString str_value;
	if(GetStringSP(type, name, str_value))
		return _ttoi(str_value);

	return def_value;
}

bool CMConfig::GetBooleanSP(CString type, CString name, bool def_value)
{
	CString str_value;
	if(GetStringSP(type, name, str_value)) {
		if(str_value == _T("yes"))
			return true;
		else if(str_value == _T("no"))
			return false;
		else if(_ttoi(str_value))
			return true;
		else
			return false;
	}

	return def_value;
}

bool CMConfig::RemoveValueSP(CString type, CString name)
{
	bool return_value = false;
	ConfigSP::iterator vi;
	ConfigSP_info::iterator fi;
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++) {
		if(vi->type == type) {
			for(fi = vi->info.begin(); fi != vi->info.end(); fi++) {
				if(fi->name == name) {
					return_value = true;
					fi->uesed = false;
					break;
				}
			}
			break;
		}
	}
	return return_value;

}

void CMConfig::SetValueSP(CString type, CString name, CString value)
{
	bool have_name = false;
	bool have_type = false;
	ConfigSP::iterator vi;
	ConfigSP_info::iterator fi;
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++) {
		if(vi->type == type) {
			have_type = true;
			for(fi = vi->info.begin(); fi != vi->info.end(); fi++) {
				if(fi->name == name) {
					have_name = true;
					fi->uesed = true;
					fi->value = value;
					break;
				}
			}
			if(!have_name) {
				ConfigDataSP_info new_info;
				new_info.name = name;
				new_info.value = value;
				new_info.uesed = true;
				vi->info.push_back(new_info);
			}
			break;
		}
	}
	if(!have_type) {
		ConfigDataSP_info new_info;
		new_info.name = name;
		new_info.value = value;
		new_info.uesed = true;
		ConfigDataSP new_cfg;
		new_cfg.type = type;
		new_cfg.info.push_back(new_info);
		m_spcfg.push_back(new_cfg);
	}
}

size_t CMConfig::GetSizeSp()
{
	return m_spcfg.size();
}

bool CMConfig::GetTypeSp(size_t index, CString &value)
{
	if(index < 0 || m_spcfg.empty() || index >= m_spcfg.size())
		return false;
	value = m_spcfg[index].type;
	return true;
}

void CMConfig::AddConfigSP(CString type)
{
	bool HaveType = false;
	ConfigSP::iterator vi;
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++) {
		if(vi->type == type) {
			HaveType = true;
			break;
		}
	}
	if(!HaveType) {
		ConfigDataSP new_cfg;
		new_cfg.type = type;
		m_spcfg.push_back(new_cfg);
	}
}

void CMConfig::RemoveConfigSP(CString type)
{
	ConfigSP::iterator vi;
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++) {
		if(vi->type == type) {
			m_spcfg.erase(vi);
			break;
		}
	}
}
