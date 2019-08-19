
// +--------------------------------------------------
// |
// | Doom3ControlContext.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

#include "SizeVuff.h"

class CDoom3ControlContext
{
public:
	// 
	virtual bool LoadEntry(CString strPath, SizeBuff &sb) = NULL;

};
