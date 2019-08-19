
#pragma once

#include "GLExtz.h"
#include <map>
#include <vector>

namespace WGLExtAhh
{
	// 
	using namespace std;

	struct MyWGLExtz
	{
		// 
		CGLExtz &gl;

		// 
		MyWGLExtz(CGLExtz &gl): gl(gl)
		{

		}

		// 
		bool My_wglChoosePixelFormatARB(HDC hDC, const map<int, int>& iAtts, vector<int> &iFmts);
		// 
		HPBUFFERARB My_wglCreatePbufferARB(HDC hDC, int iPixFmt, int cx, int cy, const map<int, int> &iAtts);
		// 
		bool My_wglGetPixelFormatAttribivARB(HDC hDC, int iPixFmt, int iLayerPlane, map<int, int> &iAtts);
	};

};
