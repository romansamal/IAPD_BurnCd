#include "OpticalDisc.h"


OpticalDisc::OpticalDisc()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
}


OpticalDisc::~OpticalDisc()
{
}


long int OpticalDisc::getDeviceCount()
{
	IDiscMaster2 *discManager;
	LONG k;
	CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	discManager->get_Count(&k);
	VARIANT_BOOL isSupported;
	HRESULT hr = discManager->get_IsSupportedEnvironment(&isSupported);
	return (SUCCEEDED(hr) && isSupported == VARIANT_TRUE) ? k : 0;
}