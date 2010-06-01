// Reg.h: interface for the CReg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REG_H__22726F6A_489C_48B2_BBAE_CBCD58F73289__INCLUDED_)
#define AFX_REG_H__22726F6A_489C_48B2_BBAE_CBCD58F73289__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CReg  
{
public:
	CReg();
	virtual ~CReg();

	//注册表操作
	HKEY hKey;
	TCHAR content[256];			//所查询注册表键值的内容
	DWORD dwType;		//定义读取数据类型
	DWORD dwLength;
	struct HKEY__*RootKey;		//注册表主键名称
	TCHAR *SubKey;				//欲打开注册表项的地址
	TCHAR *KeyName;				//欲设置项的名字
	TCHAR *ValueName;			//欲设置值的名称
	LPBYTE SetContent_S;		//字符串类型
	int SetContent_D[256];		//DWORD类型
	BYTE SetContent_B[256];		//二进制类型
	
	bool ShowContent_STR (struct HKEY__*ReRootKey,CString ReSubKey,CString ValueName);
	bool SetValue_S_STR (struct HKEY__*ReRootKey,CString ReSubKey,CString ValueName, CString Content);
	bool SetValue_N_STR (struct HKEY__*ReRootKey,CString ReSubKey,CString ValueName);
	bool DeleteValue_STR (struct HKEY__*ReRootKey,CString ReSubKey,CString ValueName);
	bool DeleteKey_STR (struct HKEY__*ReRootKey,CString ReSubKey,CString KeyName);
	bool SetValue_N (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName);
	
	bool ShowContent (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName);
	bool SetValue_S (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName,unsigned char *ReSetContent_S,unsigned long length);
	int SetValue_D (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName,int ReSetContent_D[256]);
	bool SetValue_B (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName,BYTE ReSetContent_B[256]);
	int DeleteKey (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReKeyName);
	int DeleteValue (struct HKEY__*ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName);

};


#endif // !defined(AFX_REG_H__22726F6A_489C_48B2_BBAE_CBCD58F73289__INCLUDED_)
