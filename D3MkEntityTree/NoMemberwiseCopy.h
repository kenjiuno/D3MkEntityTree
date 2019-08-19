
// +--------------------------------------------------
// |
// | NoMemberwiseCopy.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

class CNoMemberwiseCopy
{
	// 
	CNoMemberwiseCopy(const CNoMemberwiseCopy &);
	// 
	void operator =(const CNoMemberwiseCopy &);

public:
	// 
	CNoMemberwiseCopy() { }

};
