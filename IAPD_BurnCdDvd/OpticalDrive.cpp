#include "OpticalDrive.h"


OpticalDrive::OpticalDrive()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
}

OpticalDrive::~OpticalDrive()
{
	CoUninitialize();
}

long int OpticalDrive::getDeviceCount()
{
	LONG k;
	CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	discManager->get_Count(&k);
	VARIANT_BOOL isSupported;
	HRESULT hr = discManager->get_IsSupportedEnvironment(&isSupported);
	discManager->Release();
	return (SUCCEEDED(hr) && isSupported == VARIANT_TRUE) ? k : 0;
}

long int OpticalDrive::getTotalMediaSectors()
{
	long int totalSectors = 0;
	BSTR uniqueId;
	CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	CoCreateInstance(__uuidof(MsftDiscFormat2Data), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscFormat2Data), (void **)&dataWriter);
	discManager->get_Item(0, &uniqueId);
	discRecorder->InitializeDiscRecorder(uniqueId);
	dataWriter->put_Recorder(discRecorder);
	dataWriter->get_TotalSectorsOnMedia(&totalSectors);
	return totalSectors;
}

long int OpticalDrive::getFreeMediaSectors()
{
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

IMAPI_FORMAT2_DATA_MEDIA_STATE OpticalDrive::getMediaState()
{
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

string OpticalDrive::getMediaType()
{
	string result;
	IMAPI_MEDIA_PHYSICAL_TYPE type;
	long int freeSectors = 0;
	BSTR uniqueId;
	HRESULT hr = CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	if (FAILED(hr))
		return result;
	hr = CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	if (FAILED(hr))
		return result;
	hr = CoCreateInstance(__uuidof(MsftDiscFormat2Data), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscFormat2Data), (void **)&dataWriter);
	if (FAILED(hr))
		return result;
	discManager->get_Item(0, &uniqueId);
	discRecorder->InitializeDiscRecorder(uniqueId);
	dataWriter->put_Recorder(discRecorder);
	dataWriter->get_CurrentPhysicalMediaType(&type);
	switch (type)
	{
		case IMAPI_MEDIA_TYPE_UNKNOWN:
		{
			result = "Unknown";
			break;
		}
		case IMAPI_MEDIA_TYPE_CDR:
		{
			result = "CD-R";
			break;
		}
		case IMAPI_MEDIA_TYPE_CDROM:
		{
			result = "CD-ROM";
			break;
		}
		case IMAPI_MEDIA_TYPE_CDRW:
		{
			result = "CD-RW";
			break;
		}
		case IMAPI_MEDIA_TYPE_DVDROM:
		{
			result = "DVD-ROM";
			break;
		}
		case IMAPI_MEDIA_TYPE_DVDRAM:
		{
			result = "DVD-RAM";
			break;
		}
		case IMAPI_MEDIA_TYPE_DVDPLUSR:
		{
			result = "DVD+R";
			break;
		}
		case IMAPI_MEDIA_TYPE_DVDDASHR:
		{
			result = "DVD-R";
			break;
		}
		case IMAPI_MEDIA_TYPE_DVDDASHRW:
		{
			result = "DVD-RW";
			break;
		}
	}
	return result;
}

bool OpticalDrive::isMediaSupported()
{
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

double OpticalDrive::getTotalMediaSize()
{
	long long size = getTotalMediaSectors() * SECTOR_SIZE;
	return (double) size / MB_SIZE;
}

double OpticalDrive::getFreeMediaSize()
{
	long long size = getFreeMediaSectors() * SECTOR_SIZE;
	return (double) size / MB_SIZE;
}

void OpticalDrive::burn(IFileSystemImage *image, HWND wnd)
{
	BSTR uniqueId;
	HRESULT hr = CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	hr = CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	hr = CoCreateInstance(__uuidof(MsftDiscFormat2Data), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscFormat2Data), (void **)&dataWriter);
	discManager->get_Item(0, &uniqueId);
	discRecorder->InitializeDiscRecorder(uniqueId);
	dataWriter->put_Recorder(discRecorder);
	hr = image->ChooseImageDefaults(discRecorder);

	BurnEvent *burnEvent = new BurnEvent(wnd);
	IConnectionPointContainer* pCPC;
	IConnectionPoint* pCP;
	hr = dataWriter->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);

	hr = pCPC->FindConnectionPoint(IID_DDiscFormat2DataEvents, &pCP);

	IUnknown* pSinkUnk;
	hr = burnEvent->QueryInterface(IID_IUnknown, (void**)&pSinkUnk);

	// Set up advice link
	DWORD cookie;
	hr = pCP->Advise(pSinkUnk, &cookie);

	IFileSystemImageResult *result;
	IStream *stream;
	dataWriter->put_ClientName(CComBSTR("TEST CLIENT").Detach());
	hr = image->CreateResultImage(&result);
	hr = result->get_ImageStream(&stream);
	hr = dataWriter->Write(stream);
	return;
}

IDiscFormat2Data *OpticalDrive::getDataWriter()
{
	return this->dataWriter;
}