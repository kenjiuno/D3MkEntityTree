
// +--------------------------------------------------
// |
// | Uzz.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#include "StdAfx.h"
#include "Uzz.h"

#include "unzip.h"
#include "iowin32.h"
#include "OSP.h"

using namespace Uzz;

void CUzz::Close()
{
	CSingleLock xLock(&m_xMonitor, true);

	if (f != NULL) {
		VERIFY(unzClose(f) == UNZ_OK);
		f = NULL;
	}
}

bool CUzz::Open(LPCSTR pszJar)
{
	Close();

	if (f = unzOpen(pszJar)) {
		return true;
	}
	return false;
}

bool CUzz::Mk()
{
	if (!IsOpened()) return false;

	z.clear();

	unz_global_info g;
	if (unzGetGlobalInfo(f, &g) == UNZ_OK) {
		if (unzGoToFirstFile(f) == UNZ_OK) {

			z.resize(g.number_entry);
			CUzzFInfoList::iterator iterPos = z.begin();

			UINT iEntry;
			const UINT nEntries = g.number_entry;
			for (iEntry = 0; iEntry < nEntries; ) {
				CHAR szFileName[MAX_PATH +1];
				unz_file_info o;
				if (unzGetCurrentFileInfo(f, &o, szFileName, MAX_PATH, NULL, 0, NULL, 0) != UNZ_OK) break;

				unz_file_pos t;
				if (unzGetFilePos(f, &t) != UNZ_OK) break;

				LPCSTR pszSuffix = _tcsrchr(szFileName, '/');
				if (pszSuffix == NULL)
					pszSuffix = _tcsrchr(szFileName, '\\');
				if (pszSuffix == NULL) {
					pszSuffix = szFileName;
				} else {
					pszSuffix++;
				}

				bool fIsDir = (_tcsnextc(pszSuffix) == 0);

				CUzzFInfo &r = *iterPos;
				r.strFileName = pszSuffix;
				r.strPathName = szFileName;
				r.nUncompressedSize = o.uncompressed_size;
				r.fIsDir = fIsDir;
				r.pos_in_zip_directory = t.pos_in_zip_directory;
				r.num_of_file = t.num_of_file;
				r.mtime = CTime(HIWORD(o.dosDate), LOWORD(o.dosDate));

				iEntry++, iterPos++;

				if (unzGoToNextFile(f) != UNZ_OK) break;
			}

			if (iEntry == nEntries) {
				return true;
			}
		}
	}

	z.clear();
	return false;
}

bool CUzz::Ex(UINT iEntry, LPCTSTR pszPath, LPCTSTR pszFileName)
{
	CSingleLock xLock(&m_xMonitor, true);

	if (!IsOpened()) return false;

	CUzzFInfo *p = GetEntryAt(iEntry);
	if (p == NULL) return false;

	unz_file_pos t;
	t.pos_in_zip_directory = p->pos_in_zip_directory;
	t.num_of_file = p->num_of_file;

	CString strFilePath = pszPath;

	strFilePath += (pszFileName != NULL)
		? OSP::OSP_JoinPath(strFilePath, pszFileName)
		: p->strFileName
		;

	FILE *f2 = _tfopen(m_strEx = strFilePath, "wb");

	if (unzGoToFilePos(f, &t) == UNZ_OK) {
		if (unzOpenCurrentFile(f) == UNZ_OK) {
			int nRead;
			const UINT nVuff = 8192;
			BYTE cVuff[nVuff];
			do {
				nRead = unzReadCurrentFile(f, cVuff, nVuff);

				if (fwrite(cVuff, nRead, 1, f2) != 1) {
					nRead = UNZ_ERRNO; break;
				}
			} while (nRead > 0);

			unzCloseCurrentFile(f);

			if (nRead == UNZ_OK) {
				fclose(f2);
				return true;
			}
		}
	}

	if (f2) fclose(f2);
	return false;
}

bool CUzz::Ex(UINT iEntry, SizeBuff &sb)
{
	CSingleLock xLock(&m_xMonitor, true);

	if (!IsOpened()) return false;

	CUzzFInfo *p = GetEntryAt(iEntry);
	if (p == NULL) return false;

	unz_file_pos t;
	t.pos_in_zip_directory = p->pos_in_zip_directory;
	t.num_of_file = p->num_of_file;

	if (unzGoToFilePos(f, &t) == UNZ_OK) {
		if (unzOpenCurrentFile(f) == UNZ_OK) {
			unz_file_info o;
			if (unzGetCurrentFileInfo(f, &o, NULL, 0, NULL, 0, NULL, 0) == UNZ_OK) {
				if (sb.Alloc(o.uncompressed_size)) {
					PBYTE pData = sb.GetData();
					DWORD nDataLen = sb.GetSize();
					DWORD iDataPos = 0;

					int nRead;

					do {
						nRead = unzReadCurrentFile(f, pData +iDataPos, nDataLen -iDataPos);

						iDataPos += nRead;
					} while (nRead > 0);

					unzCloseCurrentFile(f);

					if (nRead == UNZ_OK) {
						return true;
					}
				}
			}
		}
	}

	return false;
}
