#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../7zCrc.h"
#include "../../7zFile.h"
#include "../../7zVersion.h"

#include "7zAlloc.h"
#include "7zExtract.h"
#include "7zIn.h"

#define STRINGLEN 256

static void ConvertNumberToString(UInt64 value, char *s)
{
	char temp[32];
	int pos = 0;
	do 
	{
		temp[pos++] = (char)('0' + (int)(value % 10));
		value /= 10;
	}
	while (value != 0);
	do
	*s++ = temp[--pos];
	while(pos > 0);
	*s = '\0';
}

#define PERIOD_4 (4 * 365 + 1)
#define PERIOD_100 (PERIOD_4 * 25 - 1)
#define PERIOD_400 (PERIOD_100 * 4 + 1)

static void ConvertFileTimeToString(CNtfsFileTime *ft, char *s)
{
	unsigned year, mon, day, hour, min, sec;
	UInt64 v64 = ft->Low | ((UInt64)ft->High << 32);
	Byte ms[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	unsigned temp;
	UInt32 v; 
	v64 /= 10000000;
	sec = (unsigned)(v64 % 60);
	v64 /= 60;
	min = (unsigned)(v64 % 60);
	v64 /= 60;
	hour = (unsigned)(v64 % 24);
	v64 /= 24;

	v = (UInt32)v64;

	year = (unsigned)(1601 + v / PERIOD_400 * 400);
	v %= PERIOD_400;

	temp = (unsigned)(v / PERIOD_100);
	if (temp == 4)
		temp = 3;
	year += temp * 100;
	v -= temp * PERIOD_100;

	temp = v / PERIOD_4;
	if (temp == 25)
		temp = 24;
	year += temp * 4;
	v -= temp * PERIOD_4;

	temp = v / 365;
	if (temp == 4)
		temp = 3;
	year += temp;
	v -= temp * 365;

	if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
		ms[1] = 29;
	for (mon = 1; mon <= 12; mon++)
	{
		unsigned s = ms[mon - 1];
		if (v < s)
			break;
		v -= s;
	}
	day = (unsigned)v + 1;
	sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
}

void PrintError(char *sz)
{
	printf("\nERROR: %s\n", sz);
}


int Un7zFile(char *filename,char *DecPath, char *ex_filename)
{
	CFileInStream archiveStream;
	CLookToRead lookStream;
	CSzArEx db;
	SRes res;
	ISzAlloc allocImp;
	ISzAlloc allocTempImp;

	if (InFile_Open(&archiveStream.file, filename))
	{
		PrintError("can not open input file");
		return 0;
	}

	FileInStream_CreateVTable(&archiveStream);
	LookToRead_CreateVTable(&lookStream, False);

	lookStream.realStream = &archiveStream.s;
	LookToRead_Init(&lookStream);

	allocImp.Alloc = SzAlloc;
	allocImp.Free = SzFree;

	allocTempImp.Alloc = SzAllocTemp;
	allocTempImp.Free = SzFreeTemp;

	CrcGenerateTable();

	SzArEx_Init(&db);
	res = SzArEx_Open(&db, &lookStream.s, &allocImp, &allocTempImp);
	if (res == SZ_OK)
	{
		UInt32 i;

		/*
		if you need cache, use these 3 variables.
		if you use external function, you can make these variable as static.
		*/
		UInt32 blockIndex = 0xFFFFFFFF; /* it can have any value before first call (if outBuffer = 0) */
		Byte *outBuffer = 0; /* it must be 0 before first call for each new archive. */
		size_t outBufferSize = 0;  /* it can have any value before first call (if outBuffer = 0) */


		char  *dir, szCurPath[MAX_PATH + 1], szNewPath[MAX_PATH + 1], sz7zPath[MAX_PATH + 1];

		if(DecPath == NULL)
		{
			GetCurrentDirectory(MAX_PATH,szCurPath);
			strcat_s(szCurPath,MAX_PATH,"\\");
		}
		else if(ex_filename != NULL)
			SetCurrentDirectory(DecPath);
		else
			strcpy_s(szCurPath,MAX_PATH,DecPath);

		for (i = 0; i < db.db.NumFiles; i++)
		{
			size_t offset;
			size_t outSizeProcessed;
			CSzFileItem *f = db.db.Files + i;
			if (f->IsDir)
				continue;

			if(ex_filename != NULL)
			{
				if(strcmp(ex_filename,f->Name) != 0)
					continue;
			}
			else
			{
				strcpy_s(szNewPath,MAX_PATH,szCurPath);
				strcpy_s(sz7zPath , MAX_PATH,f->Name);
				dir = strrchr(sz7zPath, '/');
				if(dir)
				{
					dir[0] = 0;
					strcat_s(szNewPath,MAX_PATH,sz7zPath);
					CreateDirectory(szNewPath,NULL);
				}
				SetCurrentDirectory(szNewPath);
			}
			res = SzAr_Extract(&db, &lookStream.s, i,
				&blockIndex, &outBuffer, &outBufferSize, 
				&offset, &outSizeProcessed, 
				&allocImp, &allocTempImp);
			if (res != SZ_OK)
				break;	
			else
			{
				CSzFile outFile;
				size_t processedSize;
				char *fileName = f->Name;
				size_t nameLen = strlen(f->Name);
				for (; nameLen > 0; nameLen--)
				{
					if (f->Name[nameLen - 1] == '/')
					{
						fileName = f->Name + nameLen;
						break;
					}
				}
				if (OutFile_Open(&outFile, fileName))
				{
					PrintError("can not open output file");
					res = SZ_ERROR_FAIL;
					break;
				}
				processedSize = outSizeProcessed;
				if (File_Write(&outFile, outBuffer + offset, &processedSize) != 0 ||
					processedSize != outSizeProcessed)
				{
					PrintError("can not write output file");
					res = SZ_ERROR_FAIL;
					break;
				}
				if (File_Close(&outFile))
				{
					PrintError("can not close output file");
					res = SZ_ERROR_FAIL;
					break;
				}
			}
		}
		IAlloc_Free(&allocImp, outBuffer);
	}
	SzArEx_Free(&db, &allocImp);

	File_Close(&archiveStream.file);
	if (res == SZ_OK)
		return 1;
	return 0;
}