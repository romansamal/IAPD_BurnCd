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
	discManager->Release();
	return (SUCCEEDED(hr) && isSupported == VARIANT_TRUE) ? k : 0;
}

long int OpticalDisc::getMediaSectors()
{
	IDiscMaster2 *discManager;
	IDiscRecorder2 *discRecorder;
	IDiscFormat2Data *dataWriter;
	long int freeSectors = 0;
	BSTR uniqueId;
	CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	CoCreateInstance(__uuidof(MsftDiscFormat2Data), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscFormat2Data), (void **)&dataWriter);
	discManager->get_Item(0, &uniqueId);
	discRecorder->InitializeDiscRecorder(uniqueId);
	dataWriter->put_Recorder(discRecorder);
	dataWriter->get_FreeSectorsOnMedia(&freeSectors);
	return freeSectors;
}

IMAPI_FORMAT2_DATA_MEDIA_STATE OpticalDisc::getMediaState()
{
	IDiscMaster2 *discManager;
	IDiscRecorder2 *discRecorder;
	IDiscFormat2Data *dataWriter;
	IMAPI_FORMAT2_DATA_MEDIA_STATE state;
	long int freeSectors = 0;
	BSTR uniqueId;
	CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	CoCreateInstance(__uuidof(MsftDiscFormat2Data), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscFormat2Data), (void **)&dataWriter);
	discManager->get_Item(0, &uniqueId);
	discRecorder->InitializeDiscRecorder(uniqueId);
	dataWriter->put_Recorder(discRecorder);
	dataWriter->get_CurrentMediaStatus(&state);
	return state;
}

IMAPI_MEDIA_PHYSICAL_TYPE OpticalDisc::getMediaType()
{
	IDiscMaster2 *discManager;
	IDiscRecorder2 *discRecorder;
	IDiscFormat2Data *dataWriter;
	IMAPI_MEDIA_PHYSICAL_TYPE type;
	long int freeSectors = 0;
	BSTR uniqueId;
	CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	CoCreateInstance(__uuidof(MsftDiscFormat2Data), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscFormat2Data), (void **)&dataWriter);
	discManager->get_Item(0, &uniqueId);
	discRecorder->InitializeDiscRecorder(uniqueId);
	dataWriter->put_Recorder(discRecorder);
	dataWriter->get_CurrentPhysicalMediaType(&type);
	return type;
}

bool OpticalDisc::isMediaSupported()
{
	IDiscMaster2 *discManager;
	IDiscRecorder2 *discRecorder;
	IDiscFormat2Data *dataWriter;
	VARIANT_BOOL isSupported;
	long int freeSectors = 0;
	BSTR uniqueId;
	CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	CoCreateInstance(__uuidof(MsftDiscFormat2Data), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscFormat2Data), (void **)&dataWriter);
	discManager->get_Item(0, &uniqueId);
	discRecorder->InitializeDiscRecorder(uniqueId);
	dataWriter->IsCurrentMediaSupported(discRecorder, &isSupported);
	return isSupported == VARIANT_TRUE;
}