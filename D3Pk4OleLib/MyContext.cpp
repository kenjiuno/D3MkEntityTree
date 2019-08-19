
#include "StdAfx.h"
#include "MyContext.h"
#include "OSP.h"

using namespace OSP;

// ->
//  ->
//   ->
//    ->
// CMyContext
//      ->
//       ->
//        ->
//         ->

bool CMyContext::Scan()
{
	m.clear();

	typedef set<CString> Pk4Map;
	Pk4Map pk4s;

	CFileFind ff;
	BOOL f = ff.FindFile(OSP_JoinPath(strDoom3Base, "*.pk4"));
	while (f) {
		f = ff.FindNextFile();
		if (ff.IsDirectory()) continue;
		pk4s.insert(ff.GetFilePath());
	};

	//printf("Heading to pk4 files\n");

	Pk4Map::iterator
		iterPos = pk4s.begin(),
		iterEnd = pk4s.end();
	for (; iterPos != iterEnd; iterPos++) {
		TRACE1("Uz: %s\n", (LPCTSTR)*iterPos);
		CUzzPtr pUz = CUzzPtr(new Uzz::CUzz(), false);
		//printf("Heading to '%s'\n", (LPCSTR)*iterPos);
		if (!pUz->Open(*iterPos))
			continue;
		if (!pUz->Mk())
			continue;
		Uzz::CUzzFInfoList::iterator
			iterEPos = pUz->z.begin(),
			iterEEnd = pUz->z.end();
		for (; iterEPos != iterEEnd; iterEPos++) {
			const Uzz::CUzzFInfo &rfi = *iterEPos;
			if (rfi.fIsDir)
				continue;
			CString strLeft = rfi.strPathName;
			strLeft.MakeLower();
			strLeft.Replace('\\', '/');

			FILETIME ft;
			SYSTEMTIME st;
			rfi.mtime.GetAsSystemTime(st);
			SystemTimeToFileTime(&st, &ft);

			CUzzExtractor ze;
			ze.pUz = pUz;
			ze.iEntry = rfi.num_of_file;
			m[strLeft] = CFileEntryPtr(new CZipFileEntity(ze, ft), false);
		}
	}

	//printf("# of entries is %d\n", (int)m.size());
	//printf("Heading to filesystem files\n");

	if (!ScanDir(strDoom3Base, ""))
		return false;

	//printf("# of entries is %d\n", (int)m.size());
	//printf("Heading ok\n\n");

	return true;
}

bool CMyContext::ScanDir(CString strDir, CString strPrefix)
{
	CFileFind ff;
	BOOL f = ff.FindFile(OSP_JoinPath(strDir, "*"));
	while (f) {
		f = ff.FindNextFile();

		CString strName = ff.GetFileName();
		strName.MakeLower();

		if (ff.IsDots()) {

		} else if (ff.IsDirectory()) {
			if (!ScanDir(ff.GetFilePath(), strPrefix + "/" + strName))
				return false;
		} else if (IsAssoc(strName)) {
			CString strPath = NormPath(strPrefix + "/" + strName);

			m[strPath] = CFileEntryPtr(new COSFileEntity(ff.GetFilePath(), strPath), false);
		}
	}

	return true;
}

bool CMyContext::IsAssoc(CString strName)
{
	LPCTSTR psz = _tcsrchr(strName, '.');
	if (psz == NULL) return false;
	psz++;
	if (false
		|| _tcscmp(psz, "mtr") == 0
		|| _tcscmp(psz, "skin") == 0
		|| _tcscmp(psz, "sndshd") == 0
		|| _tcscmp(psz, "def") == 0
		|| _tcscmp(psz, "fx") == 0
		|| _tcscmp(psz, "prt") == 0
		|| _tcscmp(psz, "af") == 0
		|| _tcscmp(psz, "pda") == 0

		|| _tcscmp(psz, "script") == 0
		|| _tcscmp(psz, "tga") == 0
		|| _tcscmp(psz, "md5mesh") == 0
		|| _tcscmp(psz, "md5anim") == 0
		|| _tcscmp(psz, "lwo") == 0
		|| _tcscmp(psz, "ase") == 0
	) {
		return true;
	}
	return false;
}

void CMyContext::Close()
{
	m.clear();
}
