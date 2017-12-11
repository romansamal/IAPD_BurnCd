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


bool DiscImage::addDirData(string path, double size)
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

bool DiscImage::addFileData(string path, double size)
{
	wstring wide(path.begin(), path.end());
	if (size > maxSize - this->size)
		return false;
	IFsiDirectoryItem *dirItem = NULL;
	IStream *stream = NULL;
	BSTR bstrPath = CComBSTR(path.c_str()).Detach();
	image->get_Root(&dirItem);
	HRESULT hr = SHCreateStreamOnFileEx(wide.c_str(), 0, 0, false, NULL, &stream);
	hr = dirItem->AddFile(bstrPath, stream);
	_com_error err(hr);
	LPCTSTR er = err.ErrorMessage();
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