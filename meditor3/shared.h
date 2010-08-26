#pragma once

#define FileExist(lpszFileName) (::GetFileAttributes((lpszFileName))   !=   INVALID_FILE_ATTRIBUTES)

#define FileIsDirectory(lpszFileName) (::GetFileAttributes((lpszFileName)) & FILE_ATTRIBUTE_DIRECTORY)

void DeleteFolder(CString dir);