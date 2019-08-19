
// +--------------------------------------------------
// |
// | OutDeb.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#include "StdAfx.h"
#include "OutDeb.h"
#include "OSP.h"

using namespace OSP;

COutDeb ff;

COutDeb::COutDeb()
{
	f = NULL;
	fmtr = NULL;
	fmap = NULL;

	Open_f(true);
}

void COutDeb::Open_f(bool fOpen)
{
	if (f) fclose(f), f = NULL;

	if (!fOpen) return;

	f = NULL;

	if (f == NULL) f = fopen("NUL", "w");
	if (f == NULL) f = tmpfile();
}

void COutDeb::Open_mtr(bool fOpen, bool fDummy)
{
	if (fmtr) fclose(fmtr), fmtr = NULL;

	if (!fOpen) return;

	fmtr = NULL; // fmtr = fDummy ? NULL : fopen(OSP_JoinPath(OSP_GetDir(OSP_GetModuleFileName(NULL)), "mtr.txt"), "wt");

	if (fmtr == NULL) fmtr = fopen("NUL", "w");
	if (fmtr == NULL) fmtr = tmpfile();
}

void COutDeb::Open_fmap(bool fOpen)
{
	if (fmap) fclose(fmap), fmap = NULL;

	if (!fOpen) return;

	fmap = NULL; // fmap = fopen(OSP_JoinPath(OSP_GetDir(OSP_GetModuleFileName(NULL)), "Output.map.txt"), "wt");

	if (fmap == NULL) fmap = fopen("NUL", "w");
	if (fmap == NULL) fmap = tmpfile();
}
