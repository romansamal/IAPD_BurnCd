#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <imapi2.h>
#include <imapi2error.h>
#include <imapi2fs.h>
#include <imapi2fserror.h>
#include <atlbase.h>
#include <atlstr.h>
#include <cassert>
#include <shlwapi.h>
#include <string>
#include <comdef.h>
#pragma comment (lib, "shlwapi.lib")
using namespace std;
class DiscImage
{
private:
	IFileSystemImage *image;
	double size;
	double maxSize;
public:
	DiscImage(double maxSize);
	bool addDirData(wstring path, double size);
	bool addFileData(wstring path, double size);
	bool changeMaxSize(double maxSize);
	double getFreeSize();
	double getCurrentSize();
	IFileSystemImage *getImage();
	~DiscImage();
};

