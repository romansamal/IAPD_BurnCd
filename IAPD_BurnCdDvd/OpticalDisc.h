#pragma once
#include <Windows.h>
#include <imapi2.h>
#include <imapi2error.h>
#include <imapi2fs.h>
#include <imapi2fserror.h>
#include <string>
using namespace std;
class OpticalDisc
{
private:
	IDiscMaster2 *discManager;
	IDiscRecorder2 *discRecorder;
	IDiscFormat2Data *dataWriter;
public:
	OpticalDisc();
	long int getDeviceCount();
	void checkMedia();
	long int getMediaSectors();
	IMAPI_FORMAT2_DATA_MEDIA_STATE getMediaState();
	string getMediaType();
	bool isMediaSupported();
	~OpticalDisc();
};

