// MConfig.h: interface for the CMConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCONFIG_H__F3AA9003_59DB_485F_AE6C_121952846E3E__INCLUDED_)
#define AFX_MCONFIG_H__F3AA9003_59DB_485F_AE6C_121952846E3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum ConfigTypeEx
{
	ex_option,
	ex_status,
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

private:
	CMConfigData m_pconfig;
	CString m_other_config;
	int m_now_type;
	void AnalyseLine(CString line, bool ex = false);
};

#endif // !defined(AFX_MCONFIG_H__F3AA9003_59DB_485F_AE6C_121952846E3E__INCLUDED_)
