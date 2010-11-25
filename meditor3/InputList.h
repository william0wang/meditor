// MInputList.h: interface for the CInputList class.
//////////////////////////////////////////////////////////////////////

#pragma once

class CInputList
{
public:
	vector<CString> m_key;
	vector<CString> m_cmd;
	vector<CString> m_val;
	vector<CString> m_abs;
	CInputList();
	void Add(CString key, CString cmd, CString val, CString abs = _T(""));
	bool GetAt(int i, CString &key, CString &cmd, CString &val, CString &abs);
	int GetSize();
};

class CKeyList
{
public:
	vector<CString> m_keyName;
	CListArray< CString > m_keyInfo;
	CKeyList();
	void Add(CString name, CString info);
	int GetKeyInfo(CString name);
	bool GetKeyName(CString info, CString &name);
};
