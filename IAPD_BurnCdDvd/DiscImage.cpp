#include "DiscImage.h"


DiscImage::DiscImage()
{
	CoCreateInstance(__uuidof(MsftFileSystemImage), NULL, CLSCTX_INPROC_SERVER, __uuidof(IFileSystemImage), (void **)&image);
}


DiscImage::~DiscImage()
{
}


bool DiscImage::addData(string path)
{
	IFsiDirectoryItem *dirItem;
	CoCreateInstance(__uuidof(MsftFileSystemImage), NULL, CLSCTX_INPROC_SERVER, __uuidof(IFileSystemImage), (void **)&image);
	image->get_Root(&dirItem);

}