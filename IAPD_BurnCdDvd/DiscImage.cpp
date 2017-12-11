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


bool DiscImage::addData(string path, double size)
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