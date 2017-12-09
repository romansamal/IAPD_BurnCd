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
	~OpticalDisc();
};

