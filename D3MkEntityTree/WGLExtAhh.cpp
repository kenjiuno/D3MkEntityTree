
#include "StdAfx.h"
#include "WGLExtAhh.h"

using namespace std;
using namespace WGLExtAhh;

#define Pb_MAX_ATTRIBS 32
#define Pb_MAX_FORMATS 32

bool MyWGLExtz::My_wglChoosePixelFormatARB(HDC hDC, const map<int, int>& iAtts, vector<int> &iFmts)
{
	int i;
	int iAtts0[2 * (Pb_MAX_ATTRIBS+1)];
	int iFmts0[2 * (Pb_MAX_FORMATS+1)];
	float fAtts0[2] = {0, 0};
	UINT nFmts0 = Pb_MAX_FORMATS;

	{
		map<int, int>::const_iterator
			iterPos = iAtts.begin(),
			iterEnd = iAtts.end();
		for (i = 0; i < Pb_MAX_ATTRIBS && iterPos != iterEnd; i++, iterPos++) {
			iAtts0[2*i + 0] = iterPos->first;
			iAtts0[2*i + 1] = iterPos->second;
		}
		iAtts0[2*i + 0] = 0;
		iAtts0[2*i + 1] = 0;
	}

	BOOL r = gl.wglChoosePixelFormatARB(
		hDC,
		iAtts0,
		fAtts0,
		nFmts0,
		iFmts0,
		&nFmts0
		);
	if (!r)
		return false;

	iFmts.assign(iFmts0, iFmts0 + nFmts0);
	return true;
}

HPBUFFERARB MyWGLExtz::My_wglCreatePbufferARB(HDC hDC, int iPixFmt, int cx, int cy, const map<int, int> &iAtts)
{
	int i;
	int iAtts0[2 * (Pb_MAX_ATTRIBS+1)];
	
	{
		map<int, int>::const_iterator
			iterPos = iAtts.begin(),
			iterEnd = iAtts.end();
		for (i = 0; i < Pb_MAX_ATTRIBS && iterPos != iterEnd; i++, iterPos++) {
			iAtts0[2*i + 0] = iterPos->first;
			iAtts0[2*i + 1] = iterPos->second;
		}
		iAtts0[2*i + 0] = 0;
		iAtts0[2*i + 1] = 0;
	}

	return gl.wglCreatePbufferARB(
		hDC,
		iPixFmt,
		cx,
		cy,
		iAtts0
		);
}

bool MyWGLExtz::My_wglGetPixelFormatAttribivARB(HDC hDC, int iPixFmt, int iLayerPlane, map<int, int> &iAtts)
{
	int i;
	int iAtts0[(Pb_MAX_ATTRIBS+1)];
	int iAtts1[(Pb_MAX_ATTRIBS+1)];

	{
		map<int, int>::const_iterator
			iterPos = iAtts.begin(),
			iterEnd = iAtts.end();
		for (i = 0; i < Pb_MAX_ATTRIBS && iterPos != iterEnd; i++, iterPos++) {
			iAtts0[i] = iterPos->first;
		}
		iAtts0[i] = 0;
	}

	BOOL r = gl.wglGetPixelFormatAttribivARB(
		hDC,
		iPixFmt,
		iLayerPlane,
		i,
		iAtts0,
		iAtts1
		);
	if (!r) {
		return false;
	}

	{
		iAtts.clear();

		for (int k = 0; k < i; k++) {
			iAtts[iAtts0[k]] = iAtts1[k];
		}
	}

	return true;
}
