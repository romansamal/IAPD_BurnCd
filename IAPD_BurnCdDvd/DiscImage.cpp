#include "DiscImage.h"


DiscImage::DiscImage(double max)
{
	CoCreateInstance(__uuidof(MsftFileSystemImage), NULL, CLSCTX_INPROC_SERVER, __uuidof(IFileSystemImage), (void **)&image);
	size = 0;
	maxSize = max;
}


DiscImage::~DiscImage()
{
}


bool DiscImage::addDirData(wstring path, double size)
{
	if (size > maxSize - this->size)
		return false;
	IFsiDirectoryItem *dirItem;
	BSTR bstrPath = CComBSTR(path.c_str()).Detach();
	image->get_Root(&dirItem);
	HRESULT hr = dirItem->AddTree(bstrPath, VARIANT_TRUE);
	this->size += size;
	return true;
}

bool DiscImage::addFileData(wstring path, double size)
{
	CString str(path.c_str());
	wstring wide(path.begin(), path.end());
	if (size > maxSize - this->size)
		return false;
	IFsiDirectoryItem *dirItem = NULL;
	IStream *stream = NULL;
	BSTR bstrPath = CComBSTR(path.c_str()).Detach();
	image->get_Root(&dirItem);
	HRESULT hr = SHCreateStreamOnFileEx(str, 0, 0,	FALSE,
		NULL, &stream);
	_com_error err(hr);
	hr = dirItem->AddFile(str.Mid(str.ReverseFind(L'\\')).AllocSysString(), stream);
	this->size += size;
	return true;
}

double DiscImage::getCurrentSize()
{
	return size;
}

double DiscImage::getFreeSize()
{
	return maxSize - size;
}

IFileSystemImage *DiscImage::getImage()
{
	return this->image;
}

bool DiscImage::changeMaxSize(double maxSize)
{
	if (this->size > maxSize)
		return false;
	else
	{
		this->maxSize = maxSize;
		return true;
	}
}