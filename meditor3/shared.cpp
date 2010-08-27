#include "StdAfx.h"
#include "shared.h"

void DeleteFolder(CString dir)
{
	if(FileIsDirectory(dir))
	{
		//SHFILEOPSTRUCT   Op;
		//ZeroMemory(&Op,   sizeof(Op));   //É¾³ýÎÄ¼þ¼Ð
		//Op.hwnd   =   NULL; 
		//Op.wFunc   =   FO_DELETE;
		//Op.pFrom   =   dir;
		//Op.fFlags   =   FOF_ALLOWUNDO   |   FOF_NOCONFIRMATION;     
		//SHFileOperation(&Op);

		CFindFile   tempFind;
		CString   tempFileFind;
		tempFileFind.Format(_T("%s\\*.*"),dir);
		BOOL   IsFinded   =   tempFind.FindFile(tempFileFind);
		while   (IsFinded)
		{
			IsFinded   =   tempFind.FindNextFile();
			if(!tempFind.IsDots())
			{
				if(tempFind.IsDirectory())
					DeleteFolder(tempFind.GetFilePath());
				else
					DeleteFile(tempFind.GetFilePath());
			}
		}
		tempFind.Close();
		RemoveDirectory(dir);
	}
}