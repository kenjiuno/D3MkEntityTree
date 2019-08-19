
#pragma once

#include "Context.h"

struct C10ACDoc {
	// 
	NfMD5::MD5AnimDeck_t m_a10;
	// 
	NfMD5::MD5MeshDeck_t m_m10;
	// 
	CString m_strA10, m_strM10;

	// 
	int m_iMeet;
	// 
	int m_iAccessTime;

	// 
	C10ACDoc()
	{
		m_iMeet = m_iAccessTime = 0;
	}
};
