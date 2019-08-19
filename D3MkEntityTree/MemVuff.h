
// +--------------------------------------------------
// |
// | MemVuff.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

struct CMemVuff {
	// 
	BYTE *pData;
	// 
	UINT iData, nData;

	// 
	bool fLE;

	// 
	int ReadByte()
	{
		int x = -1;
		if (iData < nData) {
			x = pData[iData]; iData++;
		}
		return x;
	}
	// 
	bool ReadByte(BYTE &x)
	{
		if (iData < nData) {
			x = pData[iData]; iData++;
			return true;
		}
		return false;
	}
	// 
	int ReadWord()
	{
		int x0 = ReadByte();
		int x1 = ReadByte();
		if (x0 < 0 || x1 < 0) return -1;
		if (fLE)
			return x0 + 256 * x1;
		return 256 * x0 + x1;
	}
	// 
	bool ReadWord(WORD &x)
	{
		int x0 = ReadByte();
		int x1 = ReadByte();
		if (x0 < 0 || x1 < 0) return false;
		if (fLE) {
			x = x0 + 256 * x1;
		} else {
			x = 256 * x0 + x1;
		}
		return true;
	}
	// 
	int ReadDWord()
	{
		int x0 = ReadWord();
		int x1 = ReadWord();
		if (x0 < 0 || x1 < 0) return -1;
		if (fLE)
			return x0 + 65536 * x1;
		return 65536 * x0 + x1;
	}
	// 
	bool ReadDWord(DWORD &x)
	{
		int x0 = ReadWord();
		int x1 = ReadWord();
		if (x0 < 0 || x1 < 0) return false;
		if (fLE) {
			x = x0 + 65536 * x1;
		} else {
			x = 65536 * x0 + x1;
		}
		return true;
	}
	// 
	bool SurelyReadBytes(void *pv, UINT cb)
	{
		if (iData + cb <= nData) {
			memcpy(pv, pData +iData, cb);
			iData += cb;
			return true;
		}
		return false;
	}
	// 
	bool SurelySkipBytes(UINT cb)
	{
		if (iData + cb <= nData) {
			iData += cb;
			return true;
		}
		return false;
	}

	// 
	bool WriteByte(BYTE x)
	{
		if (iData < nData) {
			pData[iData] = x; iData++;
			return true;
		}
		return false;
	}
	// 
	bool WriteWord(WORD x)
	{
		union {
			WORD x;
			struct { BYTE a, b; } y;
		}	z;

		if (iData + 1 < nData) {
			z.x = x;
			if (fLE)
				pData[iData] = z.y.a, iData++, 
				pData[iData] = z.y.b, iData++;
			else
				pData[iData] = z.y.b, iData++, 
				pData[iData] = z.y.a, iData++;
			return true;
		}
		return false;
	}
	// 
	bool WriteDWord(DWORD x)
	{
		union {
			DWORD x;
			struct { BYTE a, b, c, d; } y;
		}	z;

		if (iData + 3 < nData) {
			z.x = x;
			if (fLE)
				pData[iData] = z.y.a, iData++, 
				pData[iData] = z.y.b, iData++,
				pData[iData] = z.y.c, iData++,
				pData[iData] = z.y.d, iData++;
			else
				pData[iData] = z.y.d, iData++, 
				pData[iData] = z.y.c, iData++,
				pData[iData] = z.y.b, iData++,
				pData[iData] = z.y.a, iData++;
			return true;
		}
		return false;
	}
	// 
	bool WriteBytes(const void *pv, UINT cb)
	{
		if (iData + cb <= nData) {
			memcpy(pData +iData, pv, cb);
			iData += cb;
			return true;
		}
		return false;
	}
};
