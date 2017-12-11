#pragma once
#include <windows.h>
#include <imapi2.h>
#include <imapi2error.h>
#include <imapi2fs.h>
#include <imapi2fserror.h>
#include <cassert>
#include "OpticalDrive.h"
#define IMAPI_MESSAGE_VALIDATION 2315
#define IMAPI_MESSAGE_FORMATTING 2316
#define IMAPI_MESSAGE_INITIALIZING_HARDWARE 2317
#define IMAPI_MESSAGE_CALIBRATING_POWER 2318
#define IMAPI_MESSAGE_WRITING_DATA 2319
#define IMAPI_MESSAGE_ACTION_FINALIZATION 2320
#define IMAPI_MESSAGE_ACTION_COMPLETED 2321


class BurnEvent : public DDiscFormat2DataEvents
{
public:
	HWND windowHandle;
	virtual HRESULT __stdcall Update(IDispatch* object, IDispatch* progress)
	{
		IDiscFormat2DataEventArgs* progressArg = NULL;
		HRESULT hr = progress->QueryInterface(IID_PPV_ARGS(&progressArg));
		LONG percent = 0;
		IDiscFormat2Data* discFormatData = NULL;
		hr = object->QueryInterface(IID_PPV_ARGS(&discFormatData));
		IMAPI_FORMAT2_DATA_WRITE_ACTION currentAction = IMAPI_FORMAT2_DATA_WRITE_ACTION_VALIDATING_MEDIA;
		progressArg->get_CurrentAction(&currentAction);
		switch (currentAction)
		{
			case IMAPI_FORMAT2_DATA_WRITE_ACTION_WRITING_DATA:
			{
				LONG lastLBA, startLBA, sectorCount, writtenSectors;
				progressArg->get_SectorCount(&sectorCount);
				progressArg->get_LastWrittenLba(&lastLBA);
				progressArg->get_StartLba(&startLBA);
				writtenSectors = lastLBA - startLBA;
				percent = writtenSectors * 100 / sectorCount;
				SendMessage(windowHandle, IMAPI_MESSAGE_WRITING_DATA, NULL, percent);
				break;
			}

			case IMAPI_FORMAT2_DATA_WRITE_ACTION_VALIDATING_MEDIA:
			{
				SendMessage(windowHandle, IMAPI_MESSAGE_VALIDATION, NULL, NULL);
				break;
			}

			case IMAPI_FORMAT2_DATA_WRITE_ACTION_FORMATTING_MEDIA:
			{
				SendMessage(windowHandle, IMAPI_MESSAGE_FORMATTING, NULL, NULL);
				break;
			}

			case IMAPI_FORMAT2_DATA_WRITE_ACTION_INITIALIZING_HARDWARE:
			{
				SendMessage(windowHandle, IMAPI_MESSAGE_INITIALIZING_HARDWARE, NULL, NULL);
				break;
			}

			case IMAPI_FORMAT2_DATA_WRITE_ACTION_CALIBRATING_POWER:
			{
				SendMessage(windowHandle, IMAPI_MESSAGE_CALIBRATING_POWER, NULL, NULL);
				break;
			}

			case IMAPI_FORMAT2_DATA_WRITE_ACTION_FINALIZATION:
			{
				SendMessage(windowHandle, IMAPI_MESSAGE_ACTION_FINALIZATION, NULL, NULL);
				break;
			}

			case IMAPI_FORMAT2_DATA_WRITE_ACTION_COMPLETED:
			{
				SendMessage(windowHandle, IMAPI_MESSAGE_ACTION_COMPLETED, NULL, NULL);
				break;
			}
		}
		return S_OK;
	}

	ITypeInfo *m_ptinfo;
	ULONG m_refCount;

	STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
	{
		*ppv = NULL;

		if (riid == IID_IUnknown)
			*ppv = static_cast<void*>(static_cast<IUnknown*>(this));
		else if (riid == IID_IDispatch)
			*ppv = static_cast<void*>(static_cast<IDispatch*>(this));
		else if (riid == __uuidof(DDiscFormat2DataEvents))
			*ppv = static_cast<void*>(static_cast<DDiscFormat2DataEvents*>(this));

		if (*ppv) {
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	STDMETHOD_(ULONG, AddRef)()
	{
		return InterlockedIncrement(&m_refCount);
	}

	STDMETHOD_(ULONG, Release)()
	{
		ULONG result = InterlockedDecrement(&m_refCount);
		if (result == 0)
			delete this;
		return result;
	}


	BurnEvent(HWND h)
	{
		windowHandle = h;
		m_ptinfo = NULL;

		HRESULT hr = 0;
		LPTYPELIB ptlib = NULL;

		hr = LoadRegTypeLib(LIBID_IMAPILib2, IMAPILib2_MajorVersion, IMAPILib2_MinorVersion, LOCALE_SYSTEM_DEFAULT, &ptlib);

		if (!FAILED(hr))
		{
			hr = ptlib->GetTypeInfoOfGuid(IID_DDiscFormat2DataEvents, &m_ptinfo);
			ptlib->Release();
		}
	}


	STDMETHODIMP GetTypeInfoCount(UINT FAR *pctinfo)
	{
		*pctinfo = 1;

		return NOERROR;
	}

	STDMETHODIMP GetTypeInfo(UINT itinfo, LCID lcid, ITypeInfo FAR * FAR *pptinfo)
	{
		*pptinfo = NULL;

		if (itinfo != 0) return ResultFromScode(DISP_E_BADINDEX);

		m_ptinfo->AddRef();

		*pptinfo = m_ptinfo;

		return NOERROR;
	}

	STDMETHODIMP GetIDsOfNames(REFIID riid, OLECHAR FAR * FAR *rgszNames, UINT cNames, LCID lcid, DISPID FAR *rgdispid)
	{
		return DispGetIDsOfNames(m_ptinfo, rgszNames, cNames, rgdispid);
	}

	STDMETHODIMP Invoke(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR *pdispparams, VARIANT FAR *pvarResult, EXCEPINFO FAR *pexcepinfo, UINT FAR *puArgErr)
	{
		return DispInvoke(this, m_ptinfo, dispidMember, wFlags, pdispparams, pvarResult, pexcepinfo, puArgErr);
	}
};

