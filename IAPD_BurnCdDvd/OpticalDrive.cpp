#include "OpticalDrive.h"


OpticalDrive::OpticalDrive(string id)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CString idOfDrive(id.c_str());
	this->uniqueId = idOfDrive.AllocSysString();
}

OpticalDrive::~OpticalDrive()
{
	CoUninitialize();
	SysFreeString(uniqueId);
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
	CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	CoCreateInstance(__uuidof(MsftDiscFormat2Data), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscFormat2Data), (void **)&dataWriter);
	discRecorder->InitializeDiscRecorder(uniqueId);
	dataWriter->put_Recorder(discRecorder);
	dataWriter->get_TotalSectorsOnMedia(&totalSectors);
	return totalSectors;
}

long int OpticalDrive::getFreeMediaSectors()
{
	long int freeSectors = 0;
	CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	CoCreateInstance(__uuidof(MsftDiscFormat2Data), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscFormat2Data), (void **)&dataWriter);
	discRecorder->InitializeDiscRecorder(uniqueId);
	dataWriter->put_Recorder(discRecorder);
	dataWriter->get_FreeSectorsOnMedia(&freeSectors);
	return freeSectors;
}

IMAPI_FORMAT2_DATA_MEDIA_STATE OpticalDrive::getMediaState()
{
	IMAPI_FORMAT2_DATA_MEDIA_STATE state;
	long int freeSectors = 0;
	CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	CoCreateInstance(__uuidof(MsftDiscFormat2Data), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscFormat2Data), (void **)&dataWriter);
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
	HRESULT hr = CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	if (FAILED(hr))
		return result;
	hr = CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	if (FAILED(hr))
		return result;
	hr = CoCreateInstance(__uuidof(MsftDiscFormat2Data), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscFormat2Data), (void **)&dataWriter);
	if (FAILED(hr))
		return result;
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
	CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	CoCreateInstance(__uuidof(MsftDiscFormat2Data), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscFormat2Data), (void **)&dataWriter);
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

DWORD OpticalDrive::burn(LPVOID pArgs_)
{
	ARGS *pArgs = (ARGS*)pArgs_;
	IDiscMaster2 *discManager;
	IDiscRecorder2 *discRecorder;
	IDiscFormat2Data *dataWriter;
	HRESULT hr = CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	hr = CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	hr = CoCreateInstance(__uuidof(MsftDiscFormat2Data), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscFormat2Data), (void **)&dataWriter);
	discRecorder->InitializeDiscRecorder(*(pArgs->id));
	dataWriter->put_Recorder(discRecorder);
	hr = pArgs->image->ChooseImageDefaults(discRecorder);
	
	BurnEvent *burnEvent = new BurnEvent(pArgs->wnd);
	IConnectionPointContainer* pCPC;
	IConnectionPoint* pCP;
	hr = dataWriter->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);
	hr = pCPC->FindConnectionPoint(IID_DDiscFormat2DataEvents, &pCP);

	IUnknown* pSinkUnk;
	hr = burnEvent->QueryInterface(IID_IUnknown, (void**)&pSinkUnk);

	DWORD cookie;
	hr = pCP->Advise(pSinkUnk, &cookie);

	IFileSystemImageResult *result;
	IStream *stream;
	dataWriter->put_ClientName(CComBSTR("CD Burn App").Detach());
	hr = pArgs->image->CreateResultImage(&result);
	hr = result->get_ImageStream(&stream);
	hr = dataWriter->Write(stream);
	return 0;
}

IDiscFormat2Data *OpticalDrive::getDataWriter()
{
	return this->dataWriter;
}

void OpticalDrive::startBurn(IFileSystemImage *image, HWND wnd)
{
	LPDWORD id = 0;
	ARGS args = {image, wnd, &uniqueId};
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&burn, &args, NULL, id);
	Sleep(1500);
}

vector<string> OpticalDrive::getAvailableDrives()
{
	IDiscMaster2 *discManager;
	vector<string> result;
	BSTR item;
	DWORD index = 0;
	CoCreateInstance(__uuidof(MsftDiscMaster2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscMaster2), (void **)&discManager);
	while (SUCCEEDED(discManager->get_Item(index, &item)))
	{
		wstring currentItem = item;
		result.push_back(string(currentItem.begin(), currentItem.end()));
		index++;
	}
	return result;
}

vector<string> OpticalDrive::getDriveName(string uniqueId)
{
	vector<string> result;
	SAFEARRAY *arrayOfNames;
	long lowerBound, upperBound;
	tagVARIANT *data = NULL;
	IDiscRecorder2 *discRecorder;
	BSTR idSysString = CComBSTR(uniqueId.c_str()).Detach();
	BSTR bstrProductId;
	CoCreateInstance(__uuidof(MsftDiscRecorder2), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiscRecorder2), (void **)&discRecorder);
	discRecorder->InitializeDiscRecorder(idSysString);
	discRecorder->get_VolumePathNames(&arrayOfNames);
	discRecorder->get_ProductId(&bstrProductId);
	SafeArrayAccessData(arrayOfNames, (void **)&data);
	SafeArrayGetLBound(arrayOfNames, 1, &lowerBound);
	SafeArrayGetUBound(arrayOfNames, 1, &upperBound);
	long amountOfElement = upperBound - lowerBound + 1;
	CString productId(bstrProductId);
	for (int i = 0; i < amountOfElement; i++)
	{
		CString pathName((&data)[i]->bstrVal);
		string item = CT2A(pathName);
		item.append(" : ");
		item.append(CT2A(productId));
		result.push_back(item);
	}
	return result;
}