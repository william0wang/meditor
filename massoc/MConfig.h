// MConfig.h: interface for the CMConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCONFIG_H__F3AA9003_59DB_485F_AE6C_121952846E3E__INCLUDED_)
#define AFX_MCONFIG_H__F3AA9003_59DB_485F_AE6C_121952846E3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
using namespace std;

struct ConfigDataSP_info
{
	bool uesed;
	CString name;
	CString value;
};
typedef vector<ConfigDataSP_info> ConfigSP_info;

struct ConfigDataSP
{
	CString type;
	ConfigSP_info info;
};
typedef vector<ConfigDataSP> ConfigSP;

enum ConfigType
{
	cf_default,
	cf_other
};

enum ConfigTypeEx
{
	ex_option,
	ex_status,
	ex_setting,
	ex_gui,
	ex_osd,
	ex_sysinfo,
	ex_theme,
	ex_meditor
};

struct ConfigData
{
	CStringArray name;
	CStringArray value;
	CUIntArray type;
	CUIntArray state;
};

class CMConfigData
{
private:
	ConfigData m_config;
	ConfigData m_config_ex;
public:
	CMConfigData();
	void AddConfig(CString name, CString value, bool changed = false);
	void AddConfigEx(CString name, CString value,int type, bool changed = false);
	void Remove(CString name);
	void RemoveEx(CString name);
	CString GetValue_Other();
	bool GetValue(CString name, CString &value);
	bool GetValueEx(CString name, CString &value);
	void SetValue(CString name, CString value, bool changed = true);
	void SetValueEx(CString name, CString value,int type, bool changed = true);
	bool IsRemoved(CString name);
	bool IsRemovedEx(CString name);
	CString GetConfig();
	CString GetConfigEx();
};

class CMConfig
{
public:
	CMConfig();
	virtual ~CMConfig();
	void LoadConfig(CString filename, bool ex = false);
	void SaveConfig(CString filename, bool ex = false);
	void RemoveValue(CString name, bool ex = false);
	bool IsRemoved(CString name, bool ex = false);
	bool GetValue_String(CString name, CString &value, bool ex = false);
	bool GetValue_Integer(CString name, int &value, bool ex = false);
	bool GetValue_Boolean(CString name, bool &value, bool ex = false);
	bool GetValue_Double(CString name, double &value, bool ex = false);
	bool HaveSubValue(CString value, CString subname, CString sp = _T(","));
	bool GetSubValue(CString value, CString subname, CString &subvalue,int index = 0, CString sp = _T(","));
	void SetValue(CString name, CString value, bool ex = false, int type = ex_option);
	CString GetValue_Other();
	void SetValue_Other(CString Other);
	bool GetValueSP_String(CString type, CString name, CString &value);
	bool GetValueSP_Integer(CString type, CString name, int &value);
	bool GetValueSP_Boolean(CString type, CString name, bool &value);
	bool RemoveValueSP(CString type, CString name);
	void AddConfigSP(CString type);
	void RemoveConfigSP(CString type);
	void SetValueSP(CString type, CString name, CString value);
	bool GetTypeSp(size_t index, CString &value);
	size_t GetSizeSp();

private:
	ConfigSP m_spcfg;
	CMConfigData m_pconfig;
	CString m_other_config;
	int m_now_type;
	void AnalyseLine(CString line, bool ex = false);
	void LoadConfigSP(CString filename);
	CString GetConfigSP();
	bool AnalyseLineSP(CString line);
};

#endif // !defined(AFX_MCONFIG_H__F3AA9003_59DB_485F_AE6C_121952846E3E__INCLUDED_)
