#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <imapi2.h>
#include <imapi2error.h>
#include <imapi2fs.h>
#include <imapi2fserror.h>
#include <string>
using namespace std;
class DiscImage
{
private:
	IFileSystemImage *image;
	long long size;
public:
	DiscImage();
	bool addData(string path);
	~DiscImage();
};

