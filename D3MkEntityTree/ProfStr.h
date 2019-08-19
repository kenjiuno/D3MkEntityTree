
// +--------------------------------------------------
// |
// | ProfStr.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

namespace ProfStr
{

extern
BOOL SaveProfileStringTo(LPCTSTR pszAppName, LPCTSTR pszKey, LPCTSTR pszValue, LPCTSTR pszFileInto);
extern
BOOL LoadProfileStringFrom(LPCTSTR pszAppName, LPCTSTR pszKey, CString &strValue, LPCTSTR pszDefault, LPCTSTR pszFileIn);
extern
BOOL LoadProfileSectionFrom(LPCTSTR pszAppName, CString &strValue, LPCTSTR pszFileIn, TCHAR tc);
extern
int GetProfileIntFrom(LPCTSTR pszAppName, LPCTSTR pszKey, int nDefault, LPCTSTR pszFileIn);
extern
BOOL SaveProfileIntTo(LPCTSTR pszAppName, LPCTSTR pszKey, int nValue, LPCTSTR pszFileIn);

};

namespace ProfStr2
{

extern
BOOL ParseProfileSectionFrom(LPCTSTR pszAppName, set<CString> &m, LPCTSTR pszFileIn);

};
