// D3Treefs.cpp : CD3Treefs ‚ÌŽÀ‘•

#include "stdafx.h"
#include "D3Treefs.h"


// CD3Treefs


STDMETHODIMP CD3Treefs::Close(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_em.clear();
	m_com.Close();

	return S_OK;
}

STDMETHODIMP CD3Treefs::Addfs(BSTR PathName, VARIANT_BOOL *ReturnVal)
{
	if (ReturnVal == NULL) return (E_POINTER);

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TRY {
		try {
			m_com.Addfs(CString(PathName));

			m_em.clear();
			m_com.Get_EntryMap(m_em);

			*ReturnVal = VARIANT_TRUE;

			return S_OK;
		} catch (bad_alloc) {
			return E_OUTOFMEMORY;
		}
	} CATCH_ALL(e) {
		return E_FAIL;
	} END_CATCH_ALL
}

SAFEARRAY *CreateVarArray(CEntryMap &m)
{
	SAFEARRAYBOUND sab[1];
	sab[0].lLbound = 0;
	sab[0].cElements = (ULONG)m.size();
	SAFEARRAY *psa = SafeArrayCreate(VT_VARIANT, 1, sab);
	HRESULT hr;
	if (psa != NULL) {
		CEntryMap::iterator iterPos = m.begin();
		size_t i = 0, n = m.size();
		VARIANT *pvt;
		if (SUCCEEDED(hr = SafeArrayAccessData(psa, (void **)&pvt))) {
			for (; i < n; i++, iterPos++) {
				CComVariant vt((LPCTSTR)iterPos->first);
				vt.Detach(&pvt[i]);
			}
			SafeArrayUnaccessData(psa);
			return psa;
		}
		SafeArrayDestroy(psa);
	}
	return NULL;
}

bool CreateVarArray(CEntryMap &m, VARIANT &vt)
{
	vt.vt = VT_EMPTY;
	vt.parray = CreateVarArray(m);
	if (vt.parray == NULL) return false;
	vt.vt = 0 |VT_ARRAY |VT_VARIANT;
	return true;
}

STDMETHODIMP CD3Treefs::GetListfs(VARIANT* ReturnVal)
{
	if (ReturnVal == NULL) return (E_POINTER);

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TRY {
		try {
			if (!CreateVarArray(m_em, *ReturnVal)) return E_FAIL;

			return S_OK;
		} catch (bad_alloc) {
			return E_OUTOFMEMORY;
		}
	} CATCH_ALL(e) {
		return E_FAIL;
	} END_CATCH_ALL
}

STDMETHODIMP CD3Treefs::SelectFile(BSTR PathName, VARIANT_BOOL* ReturnVal)
{
	if (ReturnVal == NULL) return (E_POINTER);

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TRY {
		try {
			CString strFile = CW2CT(PathName);

			strFile.Replace('\\', '/');
			strFile.MakeLower();

			if (FindEntry(strFile) == NULL) {
				*ReturnVal = VARIANT_FALSE;
				return S_FALSE;
			}

			m_strFileSel = strFile;
			*ReturnVal = VARIANT_TRUE;
		} catch (bad_alloc) {
			return E_OUTOFMEMORY;
		}
	} CATCH_ALL(e) {
		return E_FAIL;
	} END_CATCH_ALL

	return S_OK;
}

STDMETHODIMP CD3Treefs::get_File(BSTR* pVal)
{
	if (pVal == NULL) return (E_POINTER);

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*pVal = m_strFileSel.AllocSysString();

	return S_OK;
}

STDMETHODIMP CD3Treefs::get_FileSize(LONG* pVal)
{
	if (pVal == NULL) return (E_POINTER);

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CFileEntryPtr p = FindEntry(m_strFileSel);
	if (p == NULL) return *pVal = 0, S_FALSE;

	SizeBuff fv;
	if (!p->GetData(fv)) return *pVal = 0, E_OUTOFMEMORY;

	*pVal = fv.GetSize();

	return S_OK;
}

SAFEARRAY *CreateVarByteArray(const void *pData, UINT nDataLen)
{
	SAFEARRAYBOUND sab[1];
	sab[0].lLbound = 0;
	sab[0].cElements = nDataLen;
	SAFEARRAY *psa = SafeArrayCreate(VT_UI1, 1, sab);
	HRESULT hr;
	if (psa != NULL) {
		void *pv;
		if (SUCCEEDED(hr = SafeArrayAccessData(psa, &pv))) {
			memcpy(pv, pData, nDataLen);
			SafeArrayUnaccessData(psa);
			return psa;
		}
		SafeArrayDestroy(psa);
	}
	return NULL;
}

bool CreateVarByteArray(const void *pData, UINT nDataLen, VARIANT &vt)
{
	vt.vt = VT_EMPTY;
	vt.parray = CreateVarByteArray(pData, nDataLen);
	if (vt.parray == NULL) return false;
	vt.vt = 0 |VT_ARRAY |VT_UI1;
	return true;
}

STDMETHODIMP CD3Treefs::get_FileData(VARIANT* pVal)
{
	if (pVal == NULL) return (E_POINTER);

	VariantInit(pVal);

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CFileEntryPtr p = FindEntry(m_strFileSel);
	if (p == NULL) return S_FALSE;

	SizeBuff fv;
	if (!p->GetData(fv)) return E_OUTOFMEMORY;

	if (!CreateVarByteArray(fv.GetData(), fv.GetSize(), *pVal)) return E_OUTOFMEMORY;

	return S_OK;
}

STDMETHODIMP CD3Treefs::get_FileTime(DATE* pVal)
{
	if (pVal == NULL) return (E_POINTER);

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CFileEntryPtr p = FindEntry(m_strFileSel);
	if (p == NULL) return *pVal = 0, S_FALSE;

	FILETIME ft;
	if (!p->GetModifiedTime(ft)) return E_FAIL;

	SYSTEMTIME st;
	if (!FileTimeToSystemTime(&ft, &st)) return E_FAIL;

	if (SystemTimeToVariantTime(&st, pVal) == 0) return E_FAIL;

	return S_OK;
}

STDMETHODIMP CD3Treefs::get_FileDataAsText(BSTR* pVal)
{
	if (pVal == NULL) return (E_POINTER);

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CFileEntryPtr p = FindEntry(m_strFileSel);
	if (p == NULL) return S_FALSE;

	SizeBuff fv;
	if (!p->GetData(fv)) return E_OUTOFMEMORY;

	CComBSTR bstr(fv.GetSize(), (LPCSTR)fv.GetData());

	*pVal = bstr.Detach();
	return S_OK;
}
