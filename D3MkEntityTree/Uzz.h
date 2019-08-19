
// +--------------------------------------------------
// |
// | Uzz.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

#include "SizeVuff.h"
#include <afxmt.h>
#include <vector>
#include "RCI.h"

namespace Uzz
{

struct CUzzFInfo {
	// 
	CString strFileName;
	// 
	CString strPathName;
	// 
	DWORD nUncompressedSize;
	// 
	bool fIsDir;
	// 
	CTime mtime;

	// 
	DWORD pos_in_zip_directory;
	// 
	DWORD num_of_file;
};

typedef std::vector<CUzzFInfo> CUzzFInfoList;

class CUzz : public CRCI
{
	// 
	typedef void *unzFile;

	// 
	unzFile f;

	// 
	CUzz(const CUzz &);
	// 
	void operator =(const CUzz &);

	// 
	CUzzFInfo *GetEntryAt(UINT iEntry)
	{
		if (z.size() <= iEntry) return NULL;
		return &z[iEntry];
	}

public:
	// 
	CUzzFInfoList z;
	// 
	CString m_strEx;
	// 
	CMutex m_xMonitor;

	// 
	CUzz()
		: f(NULL)
	{

	}
	// 
	~CUzz()
	{
		Close();
	}
	// 
	void Close();

	// 
	bool Open(LPCSTR pszJar);
	// 
	bool Mk();
	// 
	bool IsOpened() const { return f != NULL; }
	// 
	bool Ex(UINT iEntry, LPCTSTR pszPath, LPCTSTR pszFileName = NULL);
	// 
	bool Ex(UINT iEntry, SizeBuff &sb);
};

};
