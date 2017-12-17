#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <imapi2.h>
#include <imapi2error.h>
#include <imapi2fs.h>
#include <imapi2fserror.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlcom.h>
#include <string>
#include <vector>
#include <OleAuto.h>
#include "BurnEvent.h"
#define SECTOR_SIZE 2048
#define MB_SIZE 1048576
using namespace std;
#pragma comment (lib, "OleAut32.lib")

struct ARGS
{
	IFileSystemImage *image;
	HWND wnd;
	BSTR *id;
};

class OpticalDrive
{
private:
	BSTR uniqueId;
	IDiscMaster2 *discManager;
	IDiscRecorder2 *discRecorder;
	IDiscFormat2Data *dataWriter;
	long int getTotalMediaSectors();
	long int getFreeMediaSectors();
	static DWORD WINAPI burn(LPVOID pArgs_);
public:
	OpticalDrive(string id);
	long int getDeviceCount();
	double getTotalMediaSize();
	double getFreeMediaSize();
	IMAPI_FORMAT2_DATA_MEDIA_STATE getMediaState();
	string getMediaType();
	bool isMediaSupported();
	void startBurn(IFileSystemImage *image, HWND wnd);
	IDiscFormat2Data *getDataWriter();
	static vector<string> getAvailableDrives();
	static vector<string> getDriveName(string uniqueId);
	~OpticalDrive();
};

