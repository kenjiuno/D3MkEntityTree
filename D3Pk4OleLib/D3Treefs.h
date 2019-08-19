// D3Treefs.h : CD3Treefs ÇÃêÈåæ

#pragma once
#include "resource.h"       // ÉÅÉCÉì ÉVÉìÉ{Éã

#include "D3Pk4OleLib.h"

#include "MyContext.h"

// CD3Treefs

class ATL_NO_VTABLE CD3Treefs : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CD3Treefs, &CLSID_D3Treefs>,
	public IDispatchImpl<ID3Treefs, &IID_ID3Treefs, &LIBID_D3Pk4OleLibLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
	// 
	CMyContext m_com;
	// 
	CString m_strFileSel;
	// 
	CEntryMap m_em;

public:
	// 
	CD3Treefs()
	{

	}
	// 
	CFileEntryPtr FindEntry(CString str)
	{
		CEntryMap::iterator
			iterPos = m_em.find(str),
			iterEnd = m_em.end();
		if (iterPos == iterEnd) {
			return NULL;
		}
		return iterPos->second;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_D3TREEFS)

BEGIN_COM_MAP(CD3Treefs)
	COM_INTERFACE_ENTRY(ID3Treefs)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{

	}

public:
	STDMETHOD(Close)(void);
	STDMETHOD(Addfs)(BSTR PathName, VARIANT_BOOL *ReturnVal);
	STDMETHOD(GetListfs)(VARIANT* ReturnVal);
	STDMETHOD(SelectFile)(BSTR PathName, VARIANT_BOOL* ReturnVal);
	STDMETHOD(get_File)(BSTR* pVal);
	STDMETHOD(get_FileSize)(LONG* pVal);
	STDMETHOD(get_FileData)(VARIANT* pVal);
	STDMETHOD(get_FileTime)(DATE* pVal);
	STDMETHOD(get_FileDataAsText)(BSTR* pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(D3Treefs), CD3Treefs)
