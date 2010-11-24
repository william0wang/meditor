// MConfig.h: interface for the CMConfig class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include "ListCtrl/ListTypes.h"
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
	CListArray<CString> name;
	CListArray<CString> value;
	CListArray<UINT> type;
	CListArray<UINT> state;
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
	CString GetValue_OtherEx();
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
	bool GetString(CString name, CString &value, bool ex = false);
	int GetInteger(CString name, int def_value = 0, bool ex = false);
	bool GetBoolean(CString name, bool def_value = false, bool ex = false);
	double GetDouble(CString name, double def_value = 0, bool ex = false);
	bool HaveSubValue(CString value, CString subname, CString sp = _T(","));
	bool GetSubValue(CString value, CString subname, CString &subvalue,int index = 0, CString sp = _T(","));
	void SetValue(CString name, CString value, bool ex = false, int type = ex_option);
	CString GetOtherValue(bool ex = false);
	void SetValue_Other(CString Other, bool ex = false);
	bool GetStringSP(CString type, CString name, CString &value);
	int GetIntegerSP(CString type, CString name, int def_value = 0);
	bool GetBooleanSP(CString type, CString name, bool def_value = false);
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
