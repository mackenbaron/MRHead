#ifndef MRDIR_H
#define MRDIR_H
#include "string"
#include "iostream"
#include "vector"

// system
#ifdef WIN32
#define NOMINMAX
#include <io.h>
#include <direct.h>
#include <Windows.h>
#define EXISTS(path) (_access(path, 0)!=-1)
#define MKDIR(path) mkdir(path)
#define SLEEP(ms) Sleep(ms)
#else
#include <unistd.h>
#include <sys/stat.h>
#define EXISTS(path) (access(path, 0)!=-1)
#define MKDIR(path) mkdir(path, 0775)
#define SLEEP(ms) usleep(ms)
#endif

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

//获取当前文件夹下所有子文件夹，保存在vector<string>中
static bool getAllSubdirs(std::string strDir, std::vector<std::string> &subdirs)
{
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	std::string file2find = strDir + "/*.*";
#if _UNICODE
	hError = FindFirstFile(ANSIToUnicode(file2find.c_str()), &FindData);
#else
	hError = FindFirstFile((LPCTSTR)file2find.c_str(), &FindData);
#endif
	if (hError == INVALID_HANDLE_VALUE)
	{
		std::cout << "cannot find subdir in " << strDir<< std::endl;
		return 0;
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
	return 0;
}

static bool getAllFilesinDir(std::string strDir, std::vector<std::string> &files, std::string ext = "*.*")//获取当前文件夹下所有子文件夹，保存在vector<string>中
{
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	std::string file2find = strDir + "/" + ext;
#if _UNICODE
	hError = FindFirstFile(ANSIToUnicode(file2find.c_str()), &FindData);
#else
	hError = FindFirstFile((LPCTSTR)file2find.c_str(), &FindData);
#endif
	if (hError == INVALID_HANDLE_VALUE)
	{
		return 0;
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
	return 0;
}

#include "io.h"
static bool exist(const char*filepath)
{
	if ((_access(filepath, 0)) != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#else
#include <dirent.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <unistd.h>
#include <string.h>  
using std::string;
static bool getAllSubdirs(std::string strDir, std::vector<std::string> &subdirs)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(strDir.c_str())) == NULL)
	{    
		return 0;  
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
	return 0;
}
static bool getAllFilesinDir(std::string strDir, std::vector<std::string> &files,std::string ext="*.*")
{
	struct dirent* ent = NULL;  
	DIR *pDir;  
	pDir = opendir(strDir.c_str());  
	if (pDir == NULL) {  
		return false;  
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
	return true;
}
#endif
#endif
