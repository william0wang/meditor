// MInputList.cpp: implementation of the CInputLists class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <vector>
using namespace std;

#include "ListCtrl/ListCtrl.h"
#include "InputList.h"

CInputList::CInputList()
{
	m_key.clear();
	m_cmd.clear();
	m_val.clear();
	m_abs.clear();
}

void CInputList::Add(CString key, CString cmd, CString val, CString abs)
{
	m_key.push_back(key);
	m_cmd.push_back(cmd);
	m_val.push_back(val);
	m_abs.push_back(abs);
}

bool CInputList::GetAt(int i, CString &key, CString &cmd, CString &val, CString &abs)
{
	if(i >= m_key.size())
		return false;

	key = m_key[i];
	cmd= m_cmd[i];
	val = m_val[i];
	abs = m_abs[i];
	return true;
}

int CInputList::GetSize()
{
	return m_key.size();
}

CKeyList::CKeyList()
{
	m_keyName.clear();
	m_keyInfo.RemoveAll();
}

void CKeyList::Add(CString name, CString info)
{
	m_keyName.push_back(name);
	m_keyInfo.Add(info);
}

bool CKeyList::GetKeyName(CString info, CString &name)
{
	for(int i = 0; i < m_keyInfo.GetSize(); i++) {
		if(!m_keyInfo[i].Compare(info))	{
			name = m_keyName[i];
			return true;
		}
	}
	return false;
}

int CKeyList::GetKeyInfo(CString name)
{
	for(int i = 0; i < m_keyInfo.GetSize(); i++) {
		if(!m_keyName[i].Compare(name))
			return i;
	}
	return 0;
}
