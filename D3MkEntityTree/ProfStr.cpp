
// +--------------------------------------------------
// |
// | ProfStr.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#include "StdAfx.h"
#include "ProfStr.h"

namespace ProfStr
{

BOOL SaveProfileStringTo(LPCTSTR pszAppName, LPCTSTR pszKey, LPCTSTR pszValue, LPCTSTR pszFileInto)
{
	if (!WritePrivateProfileString(pszAppName, pszKey, pszValue, pszFileInto))
		return FALSE;
	return TRUE;
}

BOOL LoadProfileStringFrom(LPCTSTR pszAppName, LPCTSTR pszKey, CString &strValue, LPCTSTR pszDefault, LPCTSTR pszFileIn)
{
	TCHAR tc[MAX_PATH] = {""};
	if (GetPrivateProfileString(pszAppName, pszKey, pszDefault, tc, MAX_PATH, pszFileIn) > 0) {
		strValue = tc;
		return TRUE;
	}
	return FALSE;
}

BOOL LoadProfileSectionFrom(LPCTSTR pszAppName, CString &strValue, LPCTSTR pszFileIn, TCHAR tc)
{
	const UINT cbVuff = 256 *1024;
	LPTSTR pszVuff = strValue.GetBuffer(cbVuff);
	DWORD x = GetPrivateProfileSection(pszAppName, pszVuff, cbVuff, pszFileIn);
	if (x != 0) {
		LPTSTR pszPos = pszVuff;
		LPTSTR pszEnd = pszVuff + x;
		for (; pszPos < pszEnd; pszPos++)
			if (*pszPos == 0)
				*pszPos = tc;

		strValue.ReleaseBuffer(x);
		return TRUE;
	}
	strValue.ReleaseBuffer(0);
	return FALSE;
}

int GetProfileIntFrom(LPCTSTR pszAppName, LPCTSTR pszKey, int nDefault, LPCTSTR pszFileIn)
{
	return GetPrivateProfileInt(pszAppName, pszKey, nDefault, pszFileIn);
}

BOOL SaveProfileIntTo(LPCTSTR pszAppName, LPCTSTR pszKey, int nValue, LPCTSTR pszFileIn)
{
	CString strVal; strVal.Format("%d", nValue);

	return WritePrivateProfileString(pszAppName, pszKey, strVal, pszFileIn);
}

};

namespace ProfStr2
{

BOOL ParseProfileSectionFrom(LPCTSTR pszAppName, set<CString> &m, LPCTSTR pszFileIn)
{
	const UINT cbVuff = 256 *1024;
	CString strValue;
	LPTSTR pszVuff = strValue.GetBuffer(cbVuff);
	DWORD x = GetPrivateProfileSection(pszAppName, pszVuff, cbVuff, pszFileIn);
	if (x != 0) {
		LPTSTR pszPos = pszVuff;
		LPTSTR pszEnd = pszVuff + x;
		LPTSTR pszOrg = pszPos;
		for (; pszPos < pszEnd; pszPos++) {
			if (pszPos[0] == 0) {
				m.insert(pszOrg);
				pszOrg = pszPos + 1;
			}
		}

		strValue.ReleaseBuffer(x);
		return TRUE;
	}
	strValue.ReleaseBuffer(0);
	return FALSE;
}

};
