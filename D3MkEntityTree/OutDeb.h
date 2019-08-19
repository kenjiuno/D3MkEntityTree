
// +--------------------------------------------------
// |
// | OutDeb.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

class COutDeb
{
public:
	// 
	FILE *f, *fmtr, *fmap;

	// 
	COutDeb();
	// 
	~COutDeb()
	{
		Close();
	}
	// 
	void Close()
	{
		Open_f(false);
		Open_mtr(false);
		Open_fmap(false);
	}
	// 
	void Open_f(bool fOpen = true);
	// 
	void Open_mtr(bool fOpen = true, bool fDummy = false);
	// 
	void Open_fmap(bool fOpen = true);

};

extern COutDeb ff;
