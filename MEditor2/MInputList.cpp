// MInputList.cpp: implementation of the CMInputLists class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "meditor2.h"
#include "MInputList.h"

CMInputList::CMInputList()
{
	m_key.RemoveAll();
	m_cmd.RemoveAll();
	m_val.RemoveAll();
}

void CMInputList::Add(CString key, CString cmd, CString val, CString abs)
{
	m_key.Add(key);
	m_cmd.Add(cmd);
	m_val.Add(val);
	m_abs.Add(abs);
}

bool CMInputList::GetAt(int i, CString &key, CString &cmd, CString &val, CString &abs)
{
	if(i >= m_key.GetSize())
		return false;
	key = m_key[i];
	cmd= m_cmd[i];
	val = m_val[i];
	abs = m_abs[i];
	return true;
}

int CMInputList::GetSize()
{
	return m_key.GetSize();
}

CMKeyList::CMKeyList()
{
	m_keyName.RemoveAll();
	m_keyInfo.RemoveAll();
}

void CMKeyList::Add(CString name, CString info)
{
	m_keyName.Add(name);
	m_keyInfo.Add(info);
}

bool CMKeyList::GetKeyName(CString info, CString &name)
{
	for(int i = 0; i < m_keyInfo.GetSize(); i++)
	{
		if(m_keyInfo.GetAt(i) == info)
		{
			name = m_keyName.GetAt(i);
			return true;
		}
	}
	return false;
}

int CMKeyList::GetKeyInfo(CString name)
{
	for(int i = 0; i < m_keyInfo.GetSize(); i++)
	{
		if(m_keyName.GetAt(i) == name)
		{
			return i;
		}
	}
	return 0;
}
