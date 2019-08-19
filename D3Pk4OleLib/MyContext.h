
#pragma once

#include "Structure.h"

class CMyContext
{
public:
	// 
	CMyContext()
	{

	}
	// 
	void Close();
	// 
	bool LoadEntry(CString strPath, SizeBuff &sb);
	// 
	bool Addfs(CString strDir)
	{
		strDoom3Base = strDir;

		if (!Scan())
			return false;
		return true;
	}
	// 
	void Get_EntryMap(CEntryMap &m) { m = this->m; }

protected:
	// 
	CEntryMap m;
	// 
	CString strDoom3Base;

	// 
	bool Scan();
	// 
	bool ScanDir(CString strDir, CString strPrefix);

	// 
	static CString NormPath(CString strParh)
	{
		if (1 <= strParh.GetLength() && strParh[0] == '/') {
			return strParh.Mid(1);
		}
		return strParh;
	}
	// 
	static bool IsAssoc(CString strName);
	// 
	static inline bool IsExtNameIs(LPCTSTR pszPath, LPCTSTR pszExtName)
	{
		LPCTSTR psz0 = _tcsrchr(pszPath, '/');
		LPCTSTR psz1 = _tcsrchr(pszPath, '.');
		if (psz0 < psz1) {
			psz1++;
			if (_tcscmp(psz1, pszExtName) == 0)
				return true;
		}
		return false;
	}

};
