#ifndef MRDIR_H
#define MRDIR_H
#include "string"
#include "iostream"
#include "vector"

//////////////////////////////////////////////////////////////////////////
///	用于实现跨平台文件操作
///	EXISTS 判断文件是否存在
///	MKDIR  新建文件夹(必须逐级创建)
///	SLEEP  休眠毫秒
//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <io.h>
#include <direct.h>
#include <windows.h>
#define EXISTS(path) (_access(path, 0)!=-1)
#define MKDIR(path) _mkdir(path)
#define SLEEP(ms) Sleep(ms)
static bool exist(const char*filepath){return (_access(filepath, 0)) != -1;}
#else
#include <unistd.h>
#include <sys/stat.h>
#define EXISTS(path) (access(path, 0)!=-1)
#define MKDIR(path) mkdir(path, 0775)
#define SLEEP(ms) usleep(ms)
#endif

//////////////////////////////////////////////////////////////////////////
///	 @brief用于实现Windows下字符集转换的相关操作
///	ANSIToUnicode 
///	UnicodeToANSI
///	UTF8ToUnicode
///	UnicodeToUTF8
///	ANSIToUTF8
///	UTF8ToANSI
///	@param str 待转换字符
///	@retval 转换后的字符
//////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#include <tchar.h>
#include "windows.h"
#if _UNICODE
static wchar_t * ANSIToUnicode(const char* str)
{
	int textlen;
	wchar_t * result;
	textlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	result = (wchar_t *)malloc((textlen + 1)*sizeof(wchar_t));
	memset(result, 0, (textlen + 1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)result, textlen);
	return result;
}
static char * UnicodeToANSI(const wchar_t* str)
{
	char* result;
	int textlen;
	textlen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	result = (char *)malloc((textlen + 1)*sizeof(char));
	memset(result, 0, sizeof(char)* (textlen + 1));
	WideCharToMultiByte(CP_ACP, 0, str, -1, result, textlen, NULL, NULL);
	return result;
}
static wchar_t * UTF8ToUnicode(const char* str)
{
	int textlen;
	wchar_t * result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t *)malloc((textlen + 1)*sizeof(wchar_t));
	memset(result, 0, (textlen + 1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
	return result;
}
static char * UnicodeToUTF8(const wchar_t* str)
{
	char* result;
	int textlen;
	textlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	result = (char *)malloc((textlen + 1)*sizeof(char));
	memset(result, 0, sizeof(char)* (textlen + 1));
	WideCharToMultiByte(CP_UTF8, 0, str, -1, result, textlen, NULL, NULL);
	return result;
}
static char* ANSIToUTF8(const char* str)
{
	return UnicodeToUTF8(ANSIToUnicode(str));
}
static char* UTF8ToANSI(const char* str)
{
	return UnicodeToANSI(UTF8ToUnicode(str));
}
#endif

//////////////////////////////////////////////////////////////////////////
///  @brief 获取当前文件夹下所有子文件夹
///  @param strDir 要获取的文件夹路径
///  @param subdirs 获取到的所有的子目录名称
//////////////////////////////////////////////////////////////////////////
static std::vector<std::string> getAllSubdirs(std::string strDir)
{
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	std::string file2find = strDir + "/*.*";
	std::vector<std::string> subdirs;
#if _UNICODE
	hError = FindFirstFile(ANSIToUnicode(file2find.c_str()), &FindData);
#else
	hError = FindFirstFile((LPCTSTR)file2find.c_str(), &FindData);
#endif
	if (hError == INVALID_HANDLE_VALUE)
	{
		std::cout << "cannot find subdir in " << strDir<< std::endl;
		return subdirs;
	}
	else
	{
		do
		{
			if (lstrcmp(FindData.cFileName, TEXT(".")) == 0 || lstrcmp(FindData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}
			if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				continue;
			}
			if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
#if _UNICODE
				subdirs.push_back(UnicodeToANSI(FindData.cFileName));
#else
				subdirs.push_back(FindData.cFileName);
#endif
			}
		} while (::FindNextFile(hError, &FindData));
	}
	FindClose(hError);
	return subdirs;
}
//////////////////////////////////////////////////////////////////////////
///  @brief 获取当前文件夹下所有文件
///  @param strDir 要获取的文件夹路径
///  @param subdirs 获取到的所有的文件名称
///  @param ext 后缀名
//////////////////////////////////////////////////////////////////////////
static std::vector<std::string> getAllFilesinDir(std::string strDir, std::string ext = "*.*")
{
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	std::string file2find = strDir + "/" + ext;
	std::vector<std::string> files;
#if _UNICODE
	hError = FindFirstFile(ANSIToUnicode(file2find.c_str()), &FindData);
#else
	hError = FindFirstFile((LPCTSTR)file2find.c_str(), &FindData);
#endif
	if (hError == INVALID_HANDLE_VALUE)
	{
		return files;
	}
	else
	{
		do
		{
			if (lstrcmp(FindData.cFileName, TEXT(".")) == 0 || lstrcmp(FindData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}
			else if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				continue;
			}
			else if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
#if _UNICODE
				files.push_back(UnicodeToANSI(FindData.cFileName));
#else
				files.push_back(FindData.cFileName);
#endif
			}
		} while (::FindNextFile(hError, &FindData));
	}
	FindClose(hError);
	return files;
}

#else
#include <dirent.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <unistd.h>
#include <string.h>  
using std::string;
static std::vector<std::string> getAllSubdirs(std::string strDir)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	std::vector<std::string> subdirs;
	if((dp = opendir(strDir.c_str())) == NULL)
	{    
		return subdirs;
	}
	while((entry = readdir(dp)) != NULL)
	{  
		lstat(entry->d_name, &statbuf);
		if(S_IFDIR &statbuf.st_mode)
		{
			if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)  
				continue;  
			subdirs.push_back(entry->d_name); 
		} 
	} 
	closedir(dp);
	return subdirs;
}
static std::vector<std::string>getAllFilesinDir(std::string strDir, std::string ext="*.*")
{
	struct dirent* ent = NULL;  
	DIR *pDir;
	std::vector<std::string> files;
	pDir = opendir(strDir.c_str());  
	if (pDir == NULL) {  
		return files;
	}
	while (NULL != (ent = readdir(pDir))) {  
		if (ent->d_type == 8) {  
			files.push_back(ent->d_name);
		}
		else {
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
				continue;
			}
		}
	}
	closedir(pDir);  
	return files;
}
#endif
#endif
