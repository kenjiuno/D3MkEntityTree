
// +--------------------------------------------------
// |
// | Sol.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#include "StdAfx.h"
#include "Sol.h"

namespace Sol
{

bool GetValue(const map<CString, CString> &m, CString strKey, CString &strValue)
{
	map<CString, CString>::const_iterator
		iterPos = m.find(strKey),
		iterEnd = m.end();
	if (iterPos != iterEnd) {
		strValue = iterPos->second;
		return true;
	}
	return false;
}

CString MkText(set<CString> &m)
{
	CString str;
	set<CString>::iterator
		iterPos = m.begin(),
		iterEnd = m.end();
	for (; iterPos != iterEnd; iterPos++) {
		str += *iterPos + "\r\n";
	}
	str += '\0';
	return str;
}

bool ExportResource(LPCTSTR pszName, LPCTSTR pszType, LPCTSTR pszFile)
{
	HRSRC hRc = FindResource(NULL, pszName, pszType);
	if (hRc) {
		HGLOBAL hMem = LoadResource(NULL, hRc);
		if (hMem) {
			DWORD nMem = SizeofResource(NULL, hRc);

			LPVOID pvMem = LockResource(hMem);
			if (pvMem) {

				FILE *f = fopen(pszFile, "wb");
				if (f) {
					fwrite(pvMem, nMem, 1, f);
					fclose(f);
					return true;
				}
			}
		}
	}
	return false;
}

bool MkDir(LPCTSTR psz)
{
	DWORD i = GetFileAttributes(psz);
	if (i == 0xFFFFFFFF) {
		if (CreateDirectory(psz, NULL))
			return true;
		return false;
	}
	if (i & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	return false;
}

bool ExtractResource(LPCTSTR pszName, LPCTSTR pszType, SizeBuff &sb)
{
	HRSRC hRc = FindResource(NULL, pszName, pszType);
	if (hRc) {
		HGLOBAL hMem = LoadResource(NULL, hRc);
		if (hMem) {
			DWORD nMem = SizeofResource(NULL, hRc);

			LPVOID pvMem = LockResource(hMem);
			if (pvMem) {
				if (sb.Alloc(nMem +1)) {
					memcpy(sb.GetData(), pvMem, nMem);
					sb.GetData()[nMem] = 0;
					return true;
				}
			}
		}
	}
	return false;
}


CString f2s(float x)
{
	CString str; str.Format("%f", x);
	int z = str.GetLength();
	while (z > 1) {
		char a = str[z -1];
		if (a != '0') {
			if (a == '.') {
				z--;
			}
			break;
		}
		z--;
	}
	ASSERT(str.GetLength() > 0);
	return str.Left(z);
}

};
