// MConfig.cpp: implementation of the CMConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MConfig.h"
#include "MShared.h"

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
		m_config_ex.state.Add(0);
}

void CMConfigData::Remove(CString name)
{
	for(int i = 0; i < m_config.name.GetSize(); i++)
	{
		if(m_config.name[i] == name)
		{
			m_config.state[i] = 0;
			break;
		}
	}
}

void CMConfigData::RemoveEx(CString name)
{
	for(int i = 0; i < m_config_ex.name.GetSize(); i++)
	{
		if(m_config_ex.name[i] == name)
		{
			m_config_ex.state[i] = 0;
			break;
		}
	}
}

void CMConfigData::SetValue(CString name, CString value, bool changed)
{
	bool finded = false;
	for(int i = 0; i < m_config.name.GetSize(); i++)
	{
		if(m_config.name[i] == name)
		{
			m_config.value[i] = value;
			if(changed)
				m_config.state[i] = 2;
			finded = true;
			break;
		}
	}
	if(!finded)
		AddConfig(name,value,changed);
}

void CMConfigData::SetValueEx(CString name, CString value,int type, bool changed)
{
	bool finded = false;
	for(int i = 0; i < m_config_ex.name.GetSize(); i++)
	{
		if(m_config_ex.name[i] == name)
		{
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
	for(int i = 0; i < m_config.name.GetSize(); i++)
	{
		if(m_config.name[i] == name)
		{
			value = m_config.value[i];
			m_config.state[i] = 1;
			return true;
		}
	}
	return false;
}

bool CMConfigData::GetValueEx(CString name, CString &value)
{
	for(int i = 0; i < m_config_ex.name.GetSize(); i++)
	{
		if(m_config_ex.name[i] == name)
		{
			value = m_config_ex.value[i];
			m_config_ex.state[i] = 1;
			return true;
		}
	}
	return false;
}

bool CMConfigData::IsRemoved(CString name)
{
	for(int i = 0; i < m_config.name.GetSize(); i++)
	{
		if(m_config.name[i] == name)
		{
			if(m_config.state[i] == 0)
				return true;
		}
	}
	return false;
}

bool CMConfigData::IsRemovedEx(CString name)
{
	for(int i = 0; i < m_config_ex.name.GetSize(); i++)
	{
		if(m_config_ex.name[i] == name)
		{
			if(m_config_ex.state[i] == 0)
				return true;
		}
	}
	return false;
}

CString CMConfigData::GetValue_Other()
{
	CString return_str = _T("");
	for(int i = 0 ; i < m_config.name.GetSize() ; i ++)
	{
		if(m_config.state[i] == 0)
		{
			if(m_config.name[i] == _T("###"))
				return_str += m_config.value[i] + _T("\r\n");
			else
			{
				return_str += m_config.name[i] + _T("=")
					+ m_config.value[i] + _T("\r\n");
			}
		}
	}
	return return_str;
}

CString CMConfigData::GetConfig()
{
	CString return_str = _T("[default]\r\n");
	for(int i = 0 ; i < m_config.name.GetSize() ; i ++)
	{
		if(m_config.state[i] > 0)
		{
			if(m_config.name[i] == _T("###"))
				return_str += m_config.value[i] + _T("\r\n");
			else
			{
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
			return_str +=  _T("[Status]\r\n");
			break;
		case ex_meditor:
			return_str +=  _T("[meditor]\r\n");
			break;
		}
		for(int i = 0 ; i < m_config_ex.name.GetSize() ; i ++)
		{
			if(m_config_ex.state[i] > 0 && m_config_ex.type[i] == j)
			{
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
	CStdioFile playercfg;
	if(playercfg.Open(filename,CFile::modeRead))
	{
		if(!ex)
			m_now_type = cf_default;
		CString line;
		while(playercfg.ReadString(line))
		{
			line = LocalToUnicode(line);
			AnalyseLine(line,ex);
		}
		playercfg.Close();
	}
	if(!ex)
		LoadConfigSP(filename);
}

void CMConfig::LoadConfigSP(CString filename)
{
	CStdioFile playercfg;
	m_spcfg.clear();
	if(playercfg.Open(filename,CFile::modeRead))
	{
		CString line;
		while(playercfg.ReadString(line))
		{
			line = LocalToUnicode(line);
			if(AnalyseLineSP(line))
			{
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
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++)
	{
		CString types = vi->type;
		CString name;
		CString value;
		TCHAR vl[MAX_PATH];

		name = _T("alang");
		GetPrivateProfileString(types,name,_T(""),vl,MAX_PATH,filename);
		value.Format(_T("%s"),vl);
		if(value != _T(""))
		{
			ConfigDataSP_info info;
			info.name = name;
			info.value = value;
			info.uesed = true;
			vi->info.push_back(info);
		}

		name = _T("slang");
		GetPrivateProfileString(types,name,_T(""),vl,MAX_PATH,filename);
		value.Format(_T("%s"),vl);
		if(value != _T(""))
		{
			ConfigDataSP_info info;
			info.name = name;
			info.value = value;
			info.uesed = true;
			vi->info.push_back(info);
		}

		name = _T("vf");
		GetPrivateProfileString(types,name,_T(""),vl,MAX_PATH,filename);
		value.Format(_T("%s"),vl);
		if(value != _T(""))
		{
			ConfigDataSP_info info;
			info.name = name;
			info.value = value;
			info.uesed = true;
			vi->info.push_back(info);
		}

		name = _T("flip");
		GetPrivateProfileString(types,name,_T(""),vl,MAX_PATH,filename);
		value.Format(_T("%s"),vl);
		if(value != _T(""))
		{
			ConfigDataSP_info info;
			info.name = name;
			info.value = value;
			info.uesed = true;
			vi->info.push_back(info);
		}

		name = _T("af");
		GetPrivateProfileString(types,name,_T(""),vl,MAX_PATH,filename);
		value.Format(_T("%s"),vl);
		if(value != _T(""))
		{
			ConfigDataSP_info info;
			info.name = name;
			info.value = value;
			info.uesed = true;
			vi->info.push_back(info);
		}

		name = _T("autosync");
		GetPrivateProfileString(types,name,_T(""),vl,MAX_PATH,filename);
		value.Format(_T("%s"),vl);
		if(value != _T(""))
		{
			ConfigDataSP_info info;
			info.name = name;
			info.value = value;
			info.uesed = true;
			vi->info.push_back(info);
		}

		name = _T("mc");
		GetPrivateProfileString(types,name,_T(""),vl,MAX_PATH,filename);
		value.Format(_T("%s"),vl);
		if(value != _T(""))
		{
			ConfigDataSP_info info;
			info.name = name;
			info.value = value;
			info.uesed = true;
			vi->info.push_back(info);
		}

		name = _T("af-adv");
		GetPrivateProfileString(types,name,_T(""),vl,MAX_PATH,filename);
		value.Format(_T("%s"),vl);
		if(value != _T(""))
		{
			ConfigDataSP_info info;
			info.name = name;
			info.value = value;
			info.uesed = true;
			vi->info.push_back(info);
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
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++)
	{
		return_str += _T("\r\n[") + vi->type + _T("]\r\n");
		for(fi = vi->info.begin(); fi != vi->info.end(); fi++)
		{
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
	if(ex)
	{
		if(value == _T("[option]"))
			m_now_type = ex_option;
		else if(value == _T("[status]"))
			m_now_type = ex_status;
		else if(value == _T("[meditor]"))
			m_now_type = ex_meditor;
	}
	else
	{
		int len = value.GetLength() - 1;
		if(value[0] == L'[' && value[len] == L']' && value == _T("[default]"))
			m_now_type = cf_default;
		else if(value[0] == L'[' && value[len] == L']' )
			m_now_type = cf_other;
	}

	line.TrimLeft(_T(" "));
	if(line.Find(_T("#")) == 0)
	{
		if(line.Find(_T("Mplayer Config Options")) >= 0)
				return;
		if(line.Find(_T("Write your default config options here!")) >= 0)
				return;
		if(line.Find(_T("#$$ ")) >= 0)
		{
			if(line.Find(_T(" $$#")) >= 0)
				return;
			else if(line.Find(_T("Input=")) >= 0)
				return;
			else if(line.Find(_T("CoreAVC=")) >= 0)
				return;
			else if(line.Find(_T("OSDColor=")) >= 0)
				return;
			else if(line.Find(_T("LoadType=")) >= 0)
				return;
			else if(line.Find(_T("IconsIn=")) >= 0)
				return;
			else if(line.Find(_T("SuperLink=")) >= 0)
				return;
			else if(line.Find(_T("SpecialConfig=")) >= 0)
				return;
			else if(line.Find(_T("OtherVideo=")) >= 0)
				return;
			else if(line.Find(_T("OtherAudio=")) >= 0)
				return;
			else if(line.Find(_T("OtherList=")) >= 0)
				return;
			else if(line.Find(_T("Hue=")) >= 0)
				return;
			else if(line.Find(_T("Brightness=")) >= 0)
				return;
			else if(line.Find(_T("Contrast=")) >= 0)
				return;
			else if(line.Find(_T("Saturation=")) >= 0)
				return;
			else if(line.Find(_T("VideoFilter=")) >= 0)
				return;
		}
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
		m_pconfig.SetValueEx(name,value,m_now_type,false);
	else if(m_now_type == cf_default)
		m_pconfig.SetValue(name,value,false);
}

void CMConfig::SaveConfig(CString filename, bool ex)
{
	CFile playercfg;
	if(playercfg.Open(filename , CFile::modeCreate | CFile::modeWrite))
	{
		if(ex)
		{
			int outlen = 0;
			CString cfg = m_pconfig.GetConfigEx();
			char *out = UnicodeToLocal(cfg,outlen);
			playercfg.Write(out,outlen);
			delete out;
		}
		else
		{
			int outlen = 0;
			char *out = UnicodeToLocal(_T("## Mplayer Config Options\r\n\r\n"),outlen);
			playercfg.Write(out,outlen);
			delete out;
			CString cfg = m_pconfig.GetConfig();
			if(m_other_config != _T(""))
				cfg += m_other_config;
			cfg += GetConfigSP();
			out = UnicodeToLocal(cfg,outlen);
			playercfg.Write(out,outlen);
			delete out;
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

bool CMConfig::GetValue_String(CString name, CString &value, bool ex)
{
	if(ex)
		return m_pconfig.GetValueEx(name,value);
	else
		return m_pconfig.GetValue(name,value);
}

bool CMConfig::GetValue_Integer(CString name, int &value, bool ex)
{
	CString value_str;
	if(ex)
	{
		if(!m_pconfig.GetValueEx(name,value_str))
			return false;
		value = _ttoi(value_str);
	}
	else
	{
		if(!m_pconfig.GetValue(name,value_str))
			return false;
		value = _ttoi(value_str);
	}
	return true;
}

bool CMConfig::GetValue_Double(CString name, double &value, bool ex)
{
	CString value_str;
	if(ex)
		return false;
	else
	{
		if(!m_pconfig.GetValue(name,value_str))
			return false;
		value = StringToDouble(value_str);
	}
	return true;
}

bool CMConfig::GetValue_Boolean(CString name, bool &value, bool ex)
{
	CString value_str;
	if(ex)
	{
		if(!m_pconfig.GetValueEx(name,value_str))
			return false;
		value_str.MakeLower();
		if(value_str == _T("yes"))
			value = true;
		else if(value_str == _T("no"))
			value = false;
		else if(_ttoi(value_str))
			value = true;
		else
			value = false;
	}
	else
	{
		if(!m_pconfig.GetValue(name,value_str))
			return false;
		value_str.MakeLower();
		if(value_str == _T("yes"))
			value = true;
		else if(value_str == _T("no"))
			value = false;
		else if(_ttoi(value_str))
			value = true;
		else
			value = false;
	}
	return true;
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
	if(value.Find(subname + sp) >= 0)
	{
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
	if(index == 0)
	{
		offset2 = value.Find(sp ,offset1 + 1);
		if(offset2 < 0)
			return false;
		subvalue = value.Mid(offset1 + 1 , offset2 - offset1 - 1);
		return true;
	}
	for(int i = 0 ; i < index ; i ++)
	{
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

CString CMConfig::GetValue_Other()
{
	return m_pconfig.GetValue_Other();
}

void CMConfig::SetValue_Other(CString Other)
{
	m_other_config = Other;
}

bool CMConfig::GetValueSP_String(CString type, CString name, CString &value)
{
	bool return_value = false;
	ConfigSP::iterator vi;
	ConfigSP_info::iterator fi;
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++)
	{
		if(vi->type == type)
		{
			for(fi = vi->info.begin(); fi != vi->info.end(); fi++)
			{
				if(fi->name == name)
				{
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

bool CMConfig::GetValueSP_Integer(CString type, CString name, int &value)
{
	bool return_value = false;
	ConfigSP::iterator vi;
	ConfigSP_info::iterator fi;
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++)
	{
		if(vi->type == type)
		{
			for(fi = vi->info.begin(); fi != vi->info.end(); fi++)
			{
				if(fi->name == name)
				{
					if(! fi->uesed)
						return false;
					return_value = true;
					value = _ttoi(fi->value);
					break;
				}
			}
			break;
		}

	}
	return return_value;
}

bool CMConfig::GetValueSP_Boolean(CString type, CString name, bool &value)
{
	bool return_value = false;
	ConfigSP::iterator vi;
	ConfigSP_info::iterator fi;
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++)
	{
		if(vi->type == type)
		{
			for(fi = vi->info.begin(); fi != vi->info.end(); fi++)
			{
				if(fi->name == name)
				{
					if(! fi->uesed)
						return false;
					return_value = true;
					if(fi->value == _T("yes"))
						value = true;
					else if(fi->value == _T("no"))
						value = false;
					else if(_ttoi(fi->value))
						value = true;
					else
						value = false;
					break;
				}
			}
			break;
		}

	}
	return return_value;
}

bool CMConfig::RemoveValueSP(CString type, CString name)
{
	bool return_value = false;
	ConfigSP::iterator vi;
	ConfigSP_info::iterator fi;
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++)
	{
		if(vi->type == type)
		{
			for(fi = vi->info.begin(); fi != vi->info.end(); fi++)
			{
				if(fi->name == name)
				{
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
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++)
	{
		if(vi->type == type)
		{
			have_type = true;
			for(fi = vi->info.begin(); fi != vi->info.end(); fi++)
			{
				if(fi->name == name)
				{
					have_name = true;
					fi->uesed = true;
					fi->value = value;
					break;
				}
			}
			if(!have_name)
			{
				ConfigDataSP_info new_info;
				new_info.name = name;
				new_info.value = value;
				new_info.uesed = true;
				vi->info.push_back(new_info);
			}
			break;
		}
	}
	if(!have_type)
	{
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
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++)
	{
		if(vi->type == type)
		{
			HaveType = true;
			break;
		}
	}
	if(!HaveType)
	{
		ConfigDataSP new_cfg;
		new_cfg.type = type;
		m_spcfg.push_back(new_cfg);
	}
}

void CMConfig::RemoveConfigSP(CString type)
{
	ConfigSP::iterator vi;
	for(vi = m_spcfg.begin(); vi != m_spcfg.end(); vi++)
	{
		if(vi->type == type)
		{
			m_spcfg.erase(vi);
			break;
		}
	}
}
