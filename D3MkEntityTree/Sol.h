
// +--------------------------------------------------
// |
// | Sol.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

#include <map>
#include "SizeVuff.h"

namespace Sol
{
	// 
	bool GetValue(const map<CString, CString> &m, CString strKey, CString &strValue);
	// 
	CString MkText(set<CString> &m);
	// 
	bool ExportResource(LPCTSTR pszName, LPCTSTR pszType, LPCTSTR pszFile);
	// 
	bool MkDir(LPCTSTR psz);
	// 
	bool ExtractResource(LPCTSTR pszName, LPCTSTR pszType, SizeBuff &sb);
	// 
	CString f2s(float x);

};
