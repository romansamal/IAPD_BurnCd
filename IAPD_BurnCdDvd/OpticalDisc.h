#pragma once
#include <Windows.h>
#include <imapi2.h>
#include <imapi2error.h>
#include <imapi2fs.h>
#include <imapi2fserror.h>

class OpticalDisc
{
private:

public:
	OpticalDisc();
	long int getDeviceCount();
	void checkMedia();
	long int getMediaSectors();
	IMAPI_FORMAT2_DATA_MEDIA_STATE getMediaState();
	IMAPI_MEDIA_PHYSICAL_TYPE getMediaType();
	bool isMediaSupported();
	~OpticalDisc();
};

