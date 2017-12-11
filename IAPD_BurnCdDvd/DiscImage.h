#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <imapi2.h>
#include <imapi2error.h>
#include <imapi2fs.h>
#include <imapi2fserror.h>
#include <atlbase.h>
#include <atlstr.h>
#include <string>
using namespace std;
class DiscImage
{
private:
	IFileSystemImage *image;
	double size;
	double maxSize;
public:
	DiscImage(double maxSize);
	bool addData(string path, double size);
	double getFreeSize();
	double getCurrentSize();
	IFileSystemImage *getImage();
	~DiscImage();
};

