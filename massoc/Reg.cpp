// Reg.cpp: implementation of the CReg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Reg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReg::CReg()
{
	dwType=REG_SZ;		//定义读取数据类型
	dwLength=256;
}

CReg::~CReg()
{

}
//查看函数
bool CReg::ShowContent (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName)
{
	bool result = false;
	if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKey,ReValueName,NULL,&dwType,(unsigned char *)content,&dwLength) == ERROR_SUCCESS)
			result = true;
		RegCloseKey(hKey);
	}
	return result;
}

//设置字符串值函数
bool CReg::SetValue_S (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName,unsigned char *ReSetContent_S,unsigned long length)
{
	bool result = false;
	if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
	{
		if(RegSetValueEx(hKey,ReValueName,NULL,REG_SZ,ReSetContent_S,length)==ERROR_SUCCESS)
			result = true;
		RegCloseKey(hKey);
	}
	else if(RegCreateKey(ReRootKey,ReSubKey,&hKey)==ERROR_SUCCESS)
	{
		if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
		{
			if(RegSetValueEx(hKey,ReValueName,NULL,REG_SZ,ReSetContent_S,length)==ERROR_SUCCESS)
				result = true;
			RegCloseKey(hKey);
		}
	}
	return result;
}

//设置DWORD值函数
int CReg::SetValue_D (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName,int ReSetContent_D[256])
{
	int i=0;	//操作结果：0==succeed
	if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
	{
		if(RegSetValueEx(hKey,ReValueName,NULL,REG_DWORD,(const unsigned char *)ReSetContent_D,4)!=ERROR_SUCCESS)
		{
			i=1;
		}
		RegCloseKey(hKey);
	}
	else
	{
		i=1;
	}
	return i;
}

//设置二进制值函数
bool CReg::SetValue_B (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName,BYTE *ReSetContent_B)
{
	bool result = false;
	if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
	{
		if(RegSetValueEx(hKey,ReValueName,NULL,REG_BINARY,(const unsigned char *)ReSetContent_B,4)==ERROR_SUCCESS)
			result = true;
		RegCloseKey(hKey);
	}
	else if(RegCreateKey(ReRootKey,ReSubKey,&hKey)==ERROR_SUCCESS)
	{
		if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
		{
			if(RegSetValueEx(hKey,ReValueName,NULL,REG_BINARY,(const unsigned char *)ReSetContent_B,4)==ERROR_SUCCESS)
				result = true;
			RegCloseKey(hKey);
		}
	}
	return result;
}

//删除子项函数
int CReg::DeleteKey (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReKeyName)
{
	int i=0;	//操作结果：0==succeed
	if((RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey))==ERROR_SUCCESS)
	{
		if((RegDeleteKey(hKey,ReKeyName))!=ERROR_SUCCESS)
		{
			i=1;
		}
		RegCloseKey(hKey);
	}
	else
	{
		i=1;
	}
	return i;
}

//删除键值函数
int CReg::DeleteValue (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName)
{
	int i=0;	//操作结果：0==succeed
	if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
	{
		if(RegDeleteValue(hKey,ReValueName)!=ERROR_SUCCESS)
		{
			i=1;
		}
		RegCloseKey(hKey);
	}
	else
	{
		i=1;
	}
	return i;
}

//注册表操作结束

bool CReg::ShowContent_STR (struct HKEY__*ReRootKey,CString SubKeyStr,CString ValueNameStr)
{
	bool result = false;
	TCHAR *SubKey = SubKeyStr.GetBuffer(SubKeyStr.GetLength());
	TCHAR *ValueName = ValueNameStr.GetBuffer(ValueNameStr.GetLength());
	if(ShowContent(ReRootKey,SubKey, ValueName))
		result = true;
	SubKeyStr.ReleaseBuffer();
	ValueNameStr.ReleaseBuffer();
	return result;
}

bool CReg::SetValue_S_STR (struct HKEY__*ReRootKey,CString SubKeyStr,CString ValueNameStr, CString ContentStr)
{
	bool result = false;
	TCHAR *SubKey = SubKeyStr.GetBuffer(SubKeyStr.GetLength());
	TCHAR *ValueName = ValueNameStr.GetBuffer(ValueNameStr.GetLength());
	unsigned char *Content = (unsigned char *)ContentStr.GetBuffer(ContentStr.GetLength());
#ifdef UNICODE
	int ConLength = ContentStr.GetLength() *2;
#else
	int ConLength = ContentStr.GetLength();
#endif
	if(SetValue_S(ReRootKey,SubKey, ValueName,Content,ConLength))
		result = true;
	SubKeyStr.ReleaseBuffer();
	ValueNameStr.ReleaseBuffer();
	ContentStr.ReleaseBuffer();
	return result;
}

bool CReg::SetValue_N_STR (struct HKEY__*ReRootKey,CString SubKeyStr,CString ValueNameStr)
{
	bool result = false;
	TCHAR *SubKey = SubKeyStr.GetBuffer(SubKeyStr.GetLength());
	TCHAR *ValueName = ValueNameStr.GetBuffer(ValueNameStr.GetLength());
	if(SetValue_N(ReRootKey,SubKey, ValueName))
		result = true;
	SubKeyStr.ReleaseBuffer();
	ValueNameStr.ReleaseBuffer();
	return result;
}

bool CReg::DeleteValue_STR (struct HKEY__*ReRootKey,CString SubKeyStr,CString ValueNameStr)
{
	bool result = false;
	TCHAR *SubKey = SubKeyStr.GetBuffer(SubKeyStr.GetLength());
	TCHAR *ValueName = ValueNameStr.GetBuffer(ValueNameStr.GetLength());
	if(DeleteValue(ReRootKey,SubKey, ValueName) == 0)
		result = true;
	SubKeyStr.ReleaseBuffer();
	ValueNameStr.ReleaseBuffer();
	return result;
}


bool CReg::DeleteKey_STR (struct HKEY__*ReRootKey,CString SubKeyStr,CString KeyNameStr)
{
	bool result = false;
	TCHAR *SubKey = SubKeyStr.GetBuffer(SubKeyStr.GetLength());
	TCHAR *KeyName = KeyNameStr.GetBuffer(KeyNameStr.GetLength());
	if(DeleteKey(ReRootKey,SubKey, KeyName) == 0)
		result = true;
	SubKeyStr.ReleaseBuffer();
	KeyNameStr.ReleaseBuffer();
	return result;
}

bool CReg::SetValue_N (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName)
{
	bool result = false;
	if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
	{
		if(RegSetValueEx(hKey,ReValueName,NULL,REG_NONE,NULL,0)==ERROR_SUCCESS)
			result = true;
		RegCloseKey(hKey);
	}
	else if(RegCreateKey(ReRootKey,ReSubKey,&hKey)==ERROR_SUCCESS)
	{
		if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
		{
			if(RegSetValueEx(hKey,ReValueName,NULL,REG_NONE,NULL,0)==ERROR_SUCCESS)
				result = true;
			RegCloseKey(hKey);
		}
	}
	return result;
}
