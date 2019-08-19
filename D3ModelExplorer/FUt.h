
#ifndef _FUT_H_
#define _FUT_H_

inline bool FileExists(CString str)
{
	DWORD x = GetFileAttributes(str);
	if (x == 0xFFFFFFFF)
		return false;
	if (x & FILE_ATTRIBUTE_DIRECTORY)
		return false;

	return true;
}

#endif
