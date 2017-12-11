#pragma once
#include <Windows.h>
#include <imapi2.h>
#include <imapi2error.h>
#include <imapi2fs.h>
#include <imapi2fserror.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlcom.h>
#include <string>
#include "BurnEvent.h"
#define SECTOR_SIZE 2048
#define MB_SIZE 1048576

using namespace std;
class OpticalDrive
{
private:
	IDiscMaster2 *discManager;
	IDiscRecorder2 *discRecorder;
	IDiscFormat2Data *dataWriter;
	long int getTotalMediaSectors();
	long int getFreeMediaSectors();
	static DWORD WINAPI burn(IFileSystemImage *image, HWND wnd);
public:
	OpticalDrive();
	long int getDeviceCount();
	double getTotalMediaSize();
	double getFreeMediaSize();
	IMAPI_FORMAT2_DATA_MEDIA_STATE getMediaState();
	string getMediaType();
	bool isMediaSupported();
	void startBurn(IFileSystemImage *image, HWND wnd);
	IDiscFormat2Data *getDataWriter();
	~OpticalDrive();
};

