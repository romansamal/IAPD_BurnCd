#pragma once
#include <windows.h>
#include <imapi2.h>
#include <imapi2error.h>
#include <imapi2fs.h>
#include <imapi2fserror.h>
#include <cassert>
#include "OpticalDisc.h"


class BurnEvent : public DDiscFormat2DataEvents
{
public:
	virtual HRESULT __stdcall Update(IDispatch* object, IDispatch* progress)
	{
		int k = 0;
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


	BurnEvent()
	{
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

	// boilerplate IDispatch implementation based on Typeinfo
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

