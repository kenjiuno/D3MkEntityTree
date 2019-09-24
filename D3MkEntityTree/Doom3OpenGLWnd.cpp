
// +--------------------------------------------------
// |
// | Doom3OpenGLWnd.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#include "StdAfx.h"
#include "Doom3OpenGLWnd.h"
#include <math.h>
#include "SizeVuff.h"
#include "Context.h"
#include "OSP.h"
#include "Sol.h"
#include "OutDeb.h"
#include <algorithm>
#include "WGLExtAhh.h"
#include ".\doom3openglwnd.h"

//#define _DEBUG_1

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

using namespace GLExtz_c_ARB_multitexture;
using namespace GLExtz_c_ARB_texture_env_combine;
using namespace GLExtz_c_ARB_texture_env_dot3;

using namespace Renderer;

using namespace GLExtz_c_WGL_ARB_pbuffer;
using namespace GLExtz_c_WGL_ARB_pixel_format;

IMPLEMENT_DYNAMIC(CDoom3OpenGLWnd, CWnd)

BEGIN_MESSAGE_MAP(CDoom3OpenGLWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()

bool CDoom3OpenGLWnd::Mk_OpenGLWin()
{
	for (; ; ) {
		int nColorBits = GetDeviceCaps(hDC_Win, BITSPIXEL);
		if (nColorBits < 15)
			break;
		ZeroMemory(&pfd, sizeof(pfd));
		pfd.nVersion = 1;
		pfd.nSize = sizeof(pfd);
		pfd.dwFlags = 0
			| PFD_DRAW_TO_WINDOW
			| PFD_SUPPORT_OPENGL
			| PFD_DOUBLEBUFFER
			;
		pfd.cColorBits = nColorBits;
		pfd.cAlphaBits = 0;
		pfd.cDepthBits = 16;
		pfd.iPixelType = PFD_TYPE_RGBA;

		int iSel = ChoosePixelFormat(hDC_Win, &pfd);
		if (iSel == 0)
			break;
		DescribePixelFormat(hDC_Win, iSel, sizeof(pfd), &pfd);
		if (!SetPixelFormat(hDC_Win, iSel, &pfd))
			break;

		hRC_Win = wglCreateContext(hDC_Win);
		if (hRC_Win == NULL)
			break;
		if (!wglMakeCurrent(hDC_Win, hRC_Win))
			break;

		nDevColorBits = pfd.cColorBits;
		nDevAlphaBits = pfd.cAlphaBits;
		nDevDepthBits = pfd.cDepthBits;
		nDevClass = 0;
		fDevSW = ((pfd.dwFlags & PFD_GENERIC_FORMAT) != 0);
		fDevHW = !fDevSW;
		return true;
	}

	Close_OpenGL();
	return false;
}

bool CDoom3OpenGLWnd::Mk_OpenGL_Pb()
{
	if (!gl.fWGL_ARB_pixel_format || !gl.fWGL_ARB_pbuffer)
		return false;

	for (; ; ) {
		WGLExtAhh::MyWGLExtz w(gl);

		map<int, int> iAtts;
		vector<int> iFmts;

		iAtts[WGL_DRAW_TO_PBUFFER_ARB] = 1;
		iAtts[WGL_SUPPORT_OPENGL_ARB] = 1;
		iAtts[WGL_COLOR_BITS_ARB] = 32;
		iAtts[WGL_ALPHA_BITS_ARB] = 8;
		iAtts[WGL_DEPTH_BITS_ARB] = 16;

		if (!w.My_wglChoosePixelFormatARB(hDC_Win, iAtts, iFmts))
			break;
		iAtts.clear();
		hPB_Pb = w.My_wglCreatePbufferARB(hDC_Win, iFmts[0], sizeVwpt.cx, sizeVwpt.cy, iAtts);
		if (hPB_Pb == NULL)
			break;
		hDC_Pb = gl.wglGetPbufferDCARB(hPB_Pb);
		if (hDC_Pb == NULL)
			break;
		hRC_Pb = wglCreateContext(hDC_Pb);
		if (hRC_Pb == NULL)
			break;
		if (!wglMakeCurrent(hDC_Pb, hRC_Pb))
			break;

		iAtts.clear();
		iAtts[WGL_COLOR_BITS_ARB];
		iAtts[WGL_ALPHA_BITS_ARB];
		iAtts[WGL_DEPTH_BITS_ARB];
		iAtts[WGL_ACCELERATION_ARB];
		if (w.My_wglGetPixelFormatAttribivARB(hDC_Pb, iFmts[0], 0, iAtts)) {
			nDevAlphaBits = iAtts[WGL_ALPHA_BITS_ARB];
			nDevColorBits = iAtts[WGL_COLOR_BITS_ARB];
			nDevDepthBits = iAtts[WGL_DEPTH_BITS_ARB];

			int x;
			x = iAtts[WGL_ACCELERATION_ARB];
			if (false);
			else if (x == WGL_NO_ACCELERATION_ARB) nDevClass = '0';
			else if (x == WGL_GENERIC_ACCELERATION_ARB) nDevClass = 'M';
			else if (x == WGL_FULL_ACCELERATION_ARB) nDevClass = 'I';
			else nDevClass = 1;
		}
		return true;
	}

	Close_OpenGL();
	return false;
}

void CDoom3OpenGLWnd::Close_OpenGL()
{
	if (hRC_Pb != NULL && wglGetCurrentContext() == hRC_Pb) {
		wglMakeCurrent(NULL, NULL);
	}

	if (hDC_Pb != NULL) gl.wglReleasePbufferDCARB(hPB_Pb, hDC_Pb), hDC_Pb = NULL;
	if (hRC_Pb != NULL) wglDeleteContext(hRC_Pb), hRC_Pb = NULL;
	if (hPB_Pb != NULL) gl.wglDestroyPbufferARB(hPB_Pb), hPB_Pb = NULL;

	if (hRC_Win != NULL && wglGetCurrentContext() == hRC_Win) {
		wglMakeCurrent(NULL, NULL);
	}

	if (hRC_Win != NULL) wglDeleteContext(hRC_Win), hRC_Win = NULL;
}

void CDoom3OpenGLWnd::Close_DC()
{
	if (hDC_Win != NULL) ::ReleaseDC(*this, hDC_Win), hDC_Win = NULL;
}

int CDoom3OpenGLWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	gl.Close();

	ResetCaps();

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	hDC_Win = ::GetDC(*this);
	if (hDC_Win == NULL)
		return -1;

	hRC_Win = NULL;

	hDC_Pb = NULL;
	hRC_Pb = NULL;
	hPB_Pb = NULL;

	if (!Mk_OpenGLWin()) {
		Close_DC(); return -1;
	}

	gl.Load(hDC_Win);

	switch (rrt) {
		case rrtVis:
		case rrtInvis:
			{
				break;
			}
		case rrtPb:
			{
				if (!Mk_OpenGL_Pb()) {
					Close_DC(); return -1;
				}
				break;
			}
		default:
			{
				return -1;
			}
	}

	DecideRendererClass();

//	fnReshape();

	iDisplayDoNothing = glGenLists(1);
	glNewList(iDisplayDoNothing, GL_COMPILE);
	glEndList();

	return 0;
}

void CDoom3OpenGLWnd::OnDestroy()
{
	CWnd::OnDestroy();

	Close_OpenGL();

	if (hDC_Win != NULL) ::ReleaseDC(*this, hDC_Win), hDC_Win = NULL;
}

void CDoom3OpenGLWnd::fnReshape()
{
	CRect rc;
	GetClientRect(&rc);

	int cx = sizeVwpt.cx;
	int cy = sizeVwpt.cy;

	glViewport(0, 0, cx, cy);

	float fx = 1;
	float fy = 1;
	if (cy < cx) {
		fy = (float)cy / cx;
	} else {
		fx = (float)cx / cy;
	}

	if (fExpose) {
		glDrawBuffer(GL_FRONT_AND_BACK);
	}

	glClearDepth(1.0);
	glClearColor(0, 0, 0, 1);
	glClear(0 |GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

	const GLfloat sp = 0.8f;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-sp,+sp,-sp,+sp, 1.0, 20 *fMaxLen);

	const GLfloat v = fMaxLen *1.3f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		v, v, v,
		0, 0, 0, 
		0, 0, 1.0
		);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CCW);

//	glDisable(GL_CULL_FACE);

//	glEnable(GL_LIGHTING);
//	float f00[] = {0.0f, 0.0f, 0.0f, 1.0f};
//	float f02[] = {0.2f, 0.2f, 0.2f, 1.0f};
//	float f08[] = {0.8f, 0.8f, 0.8f, 1.0f};
//	float f10[] = {1.0f, 1.0f, 1.0f, 1.0f};
//	float fc1[] = {0.0f, 0.0f, 0.8f, 1.0f};
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, fc1);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, f08);
//
//	{
//		glEnable(GL_LIGHT0);
//		float flp[] = {0, 5, 0};
//		glLightfv(GL_LIGHT0, GL_AMBIENT, f10);
//		glLightfv(GL_LIGHT0, GL_DIFFUSE, f10);
//		glLightfv(GL_LIGHT0, GL_POSITION, flp);
//	}

	glTexParameterf(
		GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_NEAREST
		);
	glTexParameterf(
		GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_NEAREST
		);
	glTexEnvf(
		GL_TEXTURE_ENV,
		GL_TEXTURE_ENV_MODE,
		GL_REPLACE
		);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
}

void CDoom3OpenGLWnd::fnDraw()
{
	glTranslatef(-center.x, -center.y, -center.z);

	if (pam != NULL) fnDraw_Axis();

	fnDraw_Mesh(false);
//	fnDraw_Vone();

//	glClear(GL_DEPTH_BUFFER_BIT);
//	fnDraw_Mesh(true);

	glFlush();
	glFinish();
}

void CDoom3OpenGLWnd::OnPaint()
{
	CPaintDC dc(this);

//	fnDraw();
//	SwapBuffers(hDCPerm);
}

bool CDoom3OpenGLWnd::Capture(LPCTSTR psz)
{
//	if (veca.empty()) return false;

	fnReshape();
	fnDraw();

	UINT nPitch = 3 * sizeVwpt.cx;
	UINT nLen = nPitch * sizeVwpt.cy;

	SizeBuff sb;
	if (sb.Alloc(nLen)) {
		glReadPixels(0, 0, sizeVwpt.cx, sizeVwpt.cy, GL_BGR_EXT, GL_UNSIGNED_BYTE, sb.GetData());
		CxImage xIma;
		if (xIma.CreateFromArray(sb.GetData(), sizeVwpt.cx, sizeVwpt.cy, 24, nPitch, false)) {
			if (fGamma)
				xIma.Gamma(3.0);
			if (xIma.Save(psz, CXIMAGE_FORMAT_JPG)) {
				return true;
			}
		}
	}
	return false;
}

void CDoom3OpenGLWnd::fnDraw_Axis(bool fGrid)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

	glEnable(GL_LINE_WIDTH);
	glLineWidth(.8f);

	glEnable(GL_LINE_SMOOTH);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	const GLdouble v = fMaxLen;

	if (fGrid) {
		float cx = 5;
		float s = 100;

		glColor4d(.0, .7, .7, 1);
		glPushMatrix();
		glTranslatef(s*cx/-2, s*cx/-2, -10);
		glScalef(s, s, 1);
		glBegin(GL_LINES);
		for (float x = 0; x <= cx; x++) {
			glVertex2f( 0, x);
			glVertex2f(cx, x);

			glVertex2f(x , 0);
			glVertex2f(x ,cx);
		}
		glEnd();
		glPopMatrix();
	} else {
		glColor4f(1.0, 0.5, 0.5, 1);
		glBegin(GL_LINES);
		glVertex3d(-v, 0, 0);
		glVertex3d(+v, 0, 0);
		glEnd();

		glColor4f(0.5, 1.0, 0.5, 1);
		glBegin(GL_LINES);
		glVertex3d( 0,-v, 0);
		glVertex3d( 0,+v, 0);
		glEnd();

		glColor4f(0.5, 0.5, 1.0, 1);
		glBegin(GL_LINES);
		glVertex3d( 0, 0,-v);
		glVertex3d( 0, 0,+v);
		glEnd();
	}
}

void CDoom3OpenGLWnd::fnDraw_Mesh(bool fWireframe)
{
	glColor4f(1, 1, 1, 1);

	UINT iMesh;
	for (iMesh = 0; iMesh < tfmtVec.size(); iMesh++) {
		CTformedMeshTab &tfmt = *tfmtVec[iMesh];
		CTformedMesh &tfm = *tfmt.p;

		glVertexPointer(3, GL_FLOAT, tfm.nVertexStride, tfm.pVertex);
		glTexCoordPointer(2, GL_FLOAT, tfm.nTexCoord0Stride, tfm.pTexCoord0);

		if (fWireframe) {
			glPushAttrib(GL_ALL_ATTRIB_BITS);

			glDisable(GL_TEXTURE_2D);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

			glEnable(GL_LINE_WIDTH);
			glLineWidth(.8f);

			glEnable(GL_LINE_SMOOTH);

			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

			static const float f4c[][4] = {
				{.5f, .5f, .5f, 1.f},
				{.5f, .5f, .8f, 1.f},
				{.5f, .8f, .5f, 1.f},
				{.5f, .8f, .8f, 1.f},
				{.8f, .5f, .5f, 1.f},
				{.8f, .5f, .8f, 1.f},
				{.8f, .8f, .5f, 1.f},
				{.8f, .8f, .8f, 1.f},
			};

			glColor4fv(f4c[iMesh % 8]);

			UINT iTri;
			for (iTri = 0; iTri < 0U +tfm.nVerts; iTri++) {
				//
				UINT iTex = tfm.pTexIndex[iTri];

				glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_SHORT, &tfm.pIndex[3 *iTri]);
				//
			}

			glPopAttrib();
			continue;
		}

		UINT iTri;
		for (iTri = 0; iTri < 0U +tfm.nVerts; iTri++) {
			//
			UINT iTex = tfm.pTexIndex[iTri];

			DisplayEntry f;

			SelectPShader(tfmt.strTexs[iTex], f);

			glCallList(f.iPrepare);

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &tfm.pIndex[3 *iTri]);

			glCallList(f.iUnprepare);
			//
		}
	}

	glFinish();
}

//		MD5MeshMeshArray::iterator
//			iter1Pos = p->meshes.begin(),
//			iter1End = p->meshes.end();
//		for (; iter1Pos != iter1End; iter1Pos++, iMesh++) {
//			CMD5MeshMesh &mesh = *iter1Pos;
//			TRACE1("Mesh#(%d)\n", iMesh);
//
//			CxImage xIma;
//
//			bool fTex0 = false;
//			GLuint iTex0 = -1;
//
//			CString strShader;
//			if (Sol::GetValue(mesh.m, "shader", strShader)) {
//				Shader2DisplayNameMap::iterator
//					iterPos = s2m.find(strShader),
//					iterEnd = s2m.end();
//				if (iterPos != iterEnd) {
//					fTex0 = true;
//					iTex0 = iterPos->second;
//				}
//			}
//
//			if (iTex0) {
//				glCallList(iTex0);
//
//				if (!fWireframe) glColor4f(1, 1, 1, 1);
//				if (!fWireframe) glEnable(GL_TEXTURE_2D);
//			} else {
//				if (!fWireframe) glColor4f(0, 0, 1, 1);
//				if (!fWireframe) glDisable(GL_TEXTURE_2D);
//			}
//
//			if (!fTex0) {
//				static const BYTE texture[] = {
//					-1,-1,-1,-1,
//					-1,-1,-1,-1,
//					-1,-1,-1,-1,
//					-1,-1,-1,-1,
//				};
//				glTexImage2D(
//					GL_TEXTURE_2D,
//					0,
//					1,
//					4,
//					4,
//					0,
//					GL_BLUE,
//					GL_UNSIGNED_BYTE,
//					texture
//					);
//			}
//
//			if (!fWireframe) glBegin(GL_TRIANGLES);
//
//			MD5MeshMeshTriArray::iterator
//				iter2Pos = mesh.tris.begin(),
//				iter2End = mesh.tris.end();
//			for (; iter2Pos != iter2End; iter2Pos++) {
//				CMD5MeshMeshTri &tri = *iter2Pos;
//
//				if (fWireframe) glBegin(GL_LINE_LOOP);
//
//				for (char ti = 0; ti < 3; ti++) {
//					int vi = tri.v[ti];
//
//					CMD5MeshMeshVert &vert = mesh.verts[vi];
//
//					My3DMath::Vtx3 v;
//					mesh.InterpolateVertex(p->joints, vi, v);
//
//					float tx, ty;
//
//					if (fTex0) {
//						tx = vert.o.x;
//						ty = 1 - vert.o.y;
//					} else {
//						tx = 0;
//						ty = 0;
//					}
//
//					glTexCoord2f(tx, ty);
//					gl.MultiTexCoord2fARB(GL_TEXTURE0_ARB, tx, ty);
//					gl.MultiTexCoord2fARB(GL_TEXTURE1_ARB, tx, ty);
//
//					glVertex3fv(v.v);
//				}
//
//				if (fWireframe) glEnd();
//			}
//
//			if (!fWireframe) glEnd();
//		}

void CDoom3OpenGLWnd::fnDraw_Vone()
{
//	glClear(0 |GL_DEPTH_BUFFER_BIT);
//
//	glDisable(GL_LIGHTING);
//	glDisable(GL_TEXTURE_2D);
//
//	glLineWidth(2.5);
//
//	glColor4f(1, 1, 1, 1);
//
//	MD5MeshJointArray::iterator
//		iterPos = p->joints.begin(),
//		iterEnd = p->joints.end();
//	for (; iterPos != iterEnd; iterPos++) {
//		CMD5MeshJoint &j0 = *iterPos;
//		if (j0.iJointRef < 0) continue;
//		CMD5MeshJoint &j1 = p->joints[j0.iJointRef];
//
//		glBegin(GL_LINES);
//		glVertex3fv(j0.ok.o.v);
//		glVertex3fv(j1.ok.o.v);
//		glEnd();
//	}
}

bool CDoom3OpenGLWnd::CaptureAnim()
{
//	if (veca.empty()) return false;

	fnReshape();
	fnDraw();

#ifdef _DEBUG_1
	return true;
#else
	UINT nPitch = 3 * sizeVwpt.cx;
	UINT nLen = nPitch * sizeVwpt.cy;

	SizeBuff sb;
	if (sb.Alloc(nLen)) {
		glReadPixels(0, 0, sizeVwpt.cx, sizeVwpt.cy, GL_BGR_EXT, GL_UNSIGNED_BYTE, sb.GetData());
		CxImage xIma;
		if (xIma.CreateFromArray(sb.GetData(), sizeVwpt.cx, sizeVwpt.cy, 24, nPitch, false)) {
			if (fGamma)
				xIma.Gamma(3.0);
			BYTE *p = xIma.GetBits();
			if (pam->AddFrame(p)) {
				return true;
			}
		}
	}
	return false;
#endif
}

bool CDoom3OpenGLWnd::LoadIma(CString strName, CxImage &xIma)
{
	strName = OSP::OSP_RemoveExt(strName) + ".tga";
	SizeBuff sb;
	if (!pcc->LoadEntry(strName, sb)) {
		return false;
	}
	if (!xIma.Decode(sb.GetData(), sb.GetSize(), CXIMAGE_FORMAT_TGA)) {
		return false;
	}
	return true;
}

void CDoom3OpenGLWnd::PrecacheShader(CString strShader)
{
	strShader.MakeLower();
	strShader = OSP::OSP_RemoveExt(strShader);

	ASSERT(strShader != "models/mapobjects/hell/delta5/doorb/hornskin");

	CString strValue;
	if (s2psMap.find(strShader) != s2psMap.end())
		return;
//	TRACE1("Precache: %s\n", (LPCTSTR)strShader);
	fprintf(ff.fmtr, " Precache ->     %s\n", (LPCTSTR)strShader); fflush(ff.fmtr);
	CDoom3materialMap::iterator
		iterPos = pwk->materialMap.find(strShader),
		iterEnd = pwk->materialMap.end();
	if (iterPos == iterEnd) {
		iterPos = pwk->materialMap.find(strShader + ".tga");
	}

	if (iterPos != iterEnd) {
		PrecacheTexsh(iterPos->first);
	}

	DisplayEntry t;
	t.iPrepare = glGenLists(1);
	t.iUnprepare = glGenLists(1);

	glNewList(t.iPrepare, GL_COMPILE);

	int iTexIdx = 0, iIndent = 0;
	//
	GLenum sfactor = -1, dfactor = -1;

	Materialst2Render m2r;

	if (iterPos != iterEnd)
	{
		CDoom3material &mat = iterPos->second;

		CDoom3materialIndentArray::iterator
			iterPos = mat.v.begin(),
			iterEnd = mat.v.end();
		for (; iterPos != iterEnd; iterPos++, iIndent++) {
			CDoom3materialIndent &ma = *iterPos;
			bool fIf = true;
			{
				CEvalExpr ee(*this);
				if (ee.Eval(ma.soIf, 0) && ee.vals.size() == 1) {
					double v = ee.vals[0];
					fIf = ((int)v) ? 1 : 0;
				}
			}
			float nAlphaTest = 1.0f;
			{
				CEvalExpr ee(*this);
				if (ee.Eval(ma.soAlphaTest, 0) && ee.vals.size() == 1) {
					nAlphaTest = (float)ee.vals[0];
				}
			}
			float sx = 1.f, sy = 1.f;
			{
				CEvalExpr ee(*this);
				if (ee.Eval(ma.so_scale, 0) && ee.Eval(ma.so_scale, 1) && ee.vals.size() == 2) {
					sx = (float)ee.vals[0];
					sy = (float)ee.vals[1];
				}
			}
			if (fIf) {
				switch (ma.bf1) {
					case bfBumpMap:
					case bfDiffuseMap:
					case bfSpecularMap:
						{
							Materialst *v = m2r.Add1();
							v->a = ma;
							v->fAlphaTest = nAlphaTest;
							v->iIndent = iIndent;
							v->scale[0] = sx, v->scale[1] = sy;

							switch (ma.bf1) {
							case bfBumpMap: m2r.pBumpmap = v; break;
							case bfDiffuseMap: m2r.pDiffusemap = v; break;
							case bfSpecularMap: m2r.pSpecularmap = v; break;
							}
							break;
						}
					default:
						{
							Materialst *v = m2r.Add1();
							v->a = ma;
							v->fAlphaTest = nAlphaTest;
							v->iIndent = iIndent;
							v->scale[0] = sx, v->scale[1] = sy;

							m2r.a1s.push_back(v);
							break;
						}
				}
			}
		}

		int iTexIdx = 0;

		GLuint iTexSel;

		GLfloat fAlphaMin = 1.f;

		GLint iMatrixMode = GL_MODELVIEW;
		glMatrixMode(GL_TEXTURE); // BE CAREFUL!
		glLoadIdentity();

		if (m2r.pDiffusemap) {
			Materialst *pMatst = m2r.pDiffusemap;
			if (pMatst && FindTexsh(strShader, pMatst->iIndent, iTexSel)) {
				fAlphaMin = __min(fAlphaMin, pMatst->fAlphaTest);

				TexstInstall(iTexIdx, -1, iTexSel, bfDiffuseMap, bfNo, sfactor, dfactor);

				iTexIdx++;

				glScalef(pMatst->scale[0], pMatst->scale[1], 1.f);
			}
		}

		for (size_t iTexx = 0; iTexx < m2r.a1s.size(); iTexx++) {
			Materialst *pMatst = m2r.a1s[iTexx];
			if (pMatst && FindTexsh(strShader, pMatst->iIndent, iTexSel)) {
				fAlphaMin = __min(fAlphaMin, pMatst->fAlphaTest);

				TexstInstall(iTexIdx, iTexx, iTexSel, pMatst->a.bf1, pMatst->a.bf2, sfactor, dfactor);

				iTexIdx++;

				glScalef(pMatst->scale[0], pMatst->scale[1], 1.f);
			}
		}

		glMatrixMode(iMatrixMode);

		glEnable(GL_ALPHA_TEST);

		if (fAlphaMin != 1.f) {
			glAlphaFunc(GL_GEQUAL, fAlphaMin);
		} else {
			glAlphaFunc(GL_ALWAYS, fAlphaMin);
		}

		if (mat.m.find("translucent") != mat.m.end()) {
			dfactor = GL_ONE;
		}

		if (gl.fARB_multitexture) gl.ActiveTextureARB(GL_TEXTURE0_ARB);
	}

	if (sfactor == -1) sfactor = GL_ZERO;
	if (dfactor == -1) dfactor = GL_ONE;

	glEnable(GL_BLEND);
	glBlendFunc(sfactor, dfactor);

	glEndList();

	glNewList(t.iUnprepare, GL_COMPILE);
	//
	{
		{
			for (int i = 0; i < nTexs; i++) {
				if (gl.fARB_multitexture) gl.ActiveTextureARB(GL_TEXTURE0_ARB +i);

				if (gl.fARB_texture_env_combine) {
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,	GL_COMBINE_ARB);

					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB,	GL_REPLACE);
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB,	GL_REPLACE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB,	GL_PREVIOUS_ARB);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB,	GL_PREVIOUS_ARB);
					glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB,	GL_SRC_COLOR);
					glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB,GL_SRC_ALPHA);
				}

				EnableTexs(false);
			}
		}
		if (gl.fARB_multitexture) gl.ActiveTextureARB(GL_TEXTURE0_ARB);
	}
	//
	glEndList();

	psVec.push_back(t);
	s2psMap[strShader] = t;

	fprintf(ff.fmtr, " Precache <-     %s\n", (LPCTSTR)strShader); fflush(ff.fmtr);
}

void CDoom3OpenGLWnd::UnprecacheShader()
{
	UINT i;

	for (i = 0; i < tvVec.size(); i++) {
		GLuint r = tvVec[i];
		glDeleteTextures(1, &r);
	}
	tvVec.clear();

	t2tvMap.clear();

	for (i = 0; i < psVec.size(); i++) {
		DisplayEntry &r = psVec[i];
		glDeleteLists(r.iPrepare, 1);
		glDeleteLists(r.iUnprepare, 1);
	}
	psVec.clear();

	s2psMap.clear();
}

void CDoom3OpenGLWnd::DecideRendererClass()
{
	rcls = rclsDiffuseMap;

	if (gl.fARB_fragment_program && fARB_fragment_program) {
		rcls = rclsFragmentProgram;
	} else if (gl.fARB_multitexture && gl.fARB_texture_env_combine) {
		rcls = rclsMtex;
		if (gl.fARB_texture_env_dot3 && fARB_texture_env_dot3) {
			rcls = rclsMtexDot3;
		}
	}

	nTexs = 1;
	if (gl.fARB_multitexture) {
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &nTexs);
	}
}

bool CDoom3OpenGLWnd::PrecacheTex(CString strTex, GLuint &iTex, bool fPrecache)
{
	Tex2TexVindMap::iterator
		iterPos = t2tvMap.find(strTex),
		iterEnd = t2tvMap.end();
	if (iterPos != iterEnd) {
		iTex = iterPos->second;
		return true;
	}
	if (!fPrecache)
		return false;

	CxImage xIma;
	if (!LoadIma(strTex, xIma))
		return false;

	SizeBuff sb;

	UINT cx = xIma.GetWidth();
	UINT cy = xIma.GetHeight();

	BYTE *pvIma = xIma.GetBits();

	GLenum fmt = 0, intfmt = 0;

	switch (xIma.GetBpp()) {
	case 8:
		{
			fmt = GL_INTENSITY8; intfmt = GL_INTENSITY8;
			break;
		}
	case 24:
		{
			fmt = GL_BGR_EXT; intfmt = GL_RGB;

			if (xIma.AlphaIsValid()) {
				if (!sb.Alloc(cy * cx * 4))
					return false;
				pvIma = sb.GetData();
				for (UINT y = 0; y < cy; y++) {
					BYTE *pSrc0 = xIma.GetBits(y);
					BYTE *pSrc1 = xIma.AlphaGetBits() +y*cx;
					BYTE *pNew0 = pvIma +cx*4*y;
					for (UINT x = 0; x < cx; x++) {
						pNew0[0] = pSrc0[0];
						pNew0[1] = pSrc0[1];
						pNew0[2] = pSrc0[2];
						pNew0[3] = pSrc1[0];

						pSrc0 += 3;
						pSrc1++;
						pNew0 += 4;
					}
				}

				fmt = GL_BGRA_EXT; intfmt = GL_RGBA;
			}
			break;
		}
	default:
		ASSERT(false);
		return false;
	}

	GLboolean fIs = 0;

	glGenTextures(1, &iTex);

	glBindTexture(GL_TEXTURE_2D, iTex);

	glTexImage2D(GL_TEXTURE_2D, 0, intfmt, cx, cy, 0, fmt, GL_UNSIGNED_BYTE, pvIma);

	tvVec.push_back(iTex);

	t2tvMap[strTex] = iTex;

	return true;
}

void CDoom3OpenGLWnd::EnableTexs(bool fEnable)
{
	if (fEnable) {
		glEnable(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	} else {
		glDisable(GL_TEXTURE_2D);
	}
}

void CDoom3OpenGLWnd::PrecacheShader()
{
	UINT i;
	for (i = 0; i < tfmtVec.size(); i++) {
		CStringArray &vec = tfmtVec[i]->strTexs;
		UINT v;
		for (v = 0; v < 0U +vec.GetSize(); v++) {
			PrecacheShader(vec[v]);
		}
	}
}

void CDoom3OpenGLWnd::SelectPShader(CString strName, DisplayEntry &f)
{
	strName.MakeLower();
	strName = OSP::OSP_RemoveExt(strName);

	Shader2DisplayMap::iterator
		iterPos = s2psMap.find(strName),
		iterEnd = s2psMap.end();
	if (iterPos == iterEnd) {
		f.iPrepare = 
		f.iUnprepare = iDisplayDoNothing;
	} else {
		f = iterPos->second;
	}
	return;
}

void CDoom3OpenGLWnd::PrecacheTexsh(CString strName)
{
	CDoom3materialMap::iterator
		iterPos = pwk->materialMap.find(strName),
		iterEnd = pwk->materialMap.end();
	if (iterPos == iterEnd) {
		return;
	}

	CDoom3material &r = iterPos->second;

	size_t i;
	for (i = 0; i < r.v.size(); i++) {
		CDoom3materialIndent &ri = r.v[i];
		MaterialTexo::TexoEval ev(*this, ri.tex);
		CDoom3FlatIma aIma, aIma1;
		if (!ev.Eval(aIma)) {
			continue;
		}

		GLenum fmt = 0, intfmt = 0;

		switch (aIma.nBitCount) {
		case 8:
			{
				fmt = GL_ALPHA; intfmt = GL_ALPHA8;
				break;
			}
		case 24:
			{
				fmt = GL_BGR_EXT; intfmt = GL_RGB;
				break;
			}
		case 32:
			{
				fmt = GL_BGRA_EXT; intfmt = GL_RGBA;
				break;
			}
		default:
			{
				ASSERT(false);
				continue;
			}
		}
		GLint cxMaxi;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &cxMaxi);
		GLsizei cxMax = cxMaxi;
		//
		GLsizei cx = aIma.cx;
		GLsizei cy = aIma.cy;
		GLsizei cxNew = (std::min)(256, (std::min)(cxMax, computeNextPow2(cx)));
		GLsizei cyNew = (std::min)(256, (std::min)(cxMax, computeNextPow2(cy)));

		GLvoid *pvIma = aIma.pData;

		if (cx != cxNew || cy != cyNew) {
			if (!aIma1.Create(cxNew, cyNew, aIma.nBitCount)) {
				continue;
			}

			GLvoid *pvNew = aIma1.GetVert(0);

			gluScaleImage(
				fmt,
				cx,
				cy,
				GL_UNSIGNED_BYTE,
				pvIma,
				cxNew,
				cyNew,
				GL_UNSIGNED_BYTE,
				pvNew
				);

			pvIma = pvNew;
			cx = cxNew;
			cy = cyNew;
		}

		size_t iTex = 0;

		glGenTextures(1, &iTex);

		glBindTexture(GL_TEXTURE_2D, iTex);

		while (0 != glGetError());

		glTexImage2D(GL_TEXTURE_2D, 0, intfmt, cx, cy, 0, fmt, GL_UNSIGNED_BYTE, pvIma);

		GLenum err = glGetError(); ASSERT(err == 0);

		glFlush();

		tvVec.push_back(iTex);

		t2tvMap[MkTexst(strName, i)] = iTex;
		//
	}
}

bool CDoom3OpenGLWnd::LoadIma(CString strName, CDoom3FlatIma &rIma)
{
	strName = OSP::OSP_RemoveExt(strName) + ".tga";
	SizeBuff sb;
	if (!pcc->LoadEntry(strName, sb)) {
		return false;
	}

	CSparseTarga t(rIma);
	t.pData = sb.GetData();
	t.iData = 0;
	t.nData = sb.GetSize();
	t.fLE = true;
	if (!t.Parse())
		return false;
	return true;
}

bool CDoom3OpenGLWnd::FindTexsh(CString strName, int i, GLuint &iTex)
{
	Tex2TexVindMap::iterator
		iterPos = t2tvMap.find(MkTexst(strName, i)),
		iterEnd = t2tvMap.end();
	if (iterPos != iterEnd) {
		iTex = iterPos->second;
		return true;
	}
	return false;
}

float CDoom3OpenGLWnd::EvalTable(CString strName, float x)
{
	CDoom3table *p =pwk->FindTable(strName);
	if (p != NULL) {
		float r = p->Eval(x);
		return r;
	}
	return 0;
}

float CDoom3OpenGLWnd::EvalNameo(CString strName)
{
	return 0;
}

HBITMAP CDoom3OpenGLWnd::CaptureTestScene()
{
	int cx = sizeVwpt.cx;
	int cy = sizeVwpt.cy;

	ASSERT(false
		|| wglGetCurrentContext() == hRC_Pb
		|| wglGetCurrentContext() == hRC_Win
		);

	glViewport(0, 0, cx, cy);

//	glDrawBuffer(GL_FRONT);

	glClearDepth(1.0);
	glClearColor(.2f, .2f, .3f, 1);
	glClear(0 |GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glTranslatef(0, 0, -.5f);

	for (int i = 0; i < 100; i++) {
		float x = (100.f - i) / 100.f;
		glColor4f(x, x, x, 1);

		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(1, 0);
		glVertex2f(1, 1);
		glVertex2f(0, 1);
		glEnd();

		glTranslatef(-.01f, -.01f, +0.01f);
	}

	UINT nPitch = 3 * sizeVwpt.cx;
	UINT nLen = nPitch * sizeVwpt.cy;

	SizeBuff sb;
	if (sb.Alloc(nLen)) {
		glReadPixels(0, 0, sizeVwpt.cx, sizeVwpt.cy, GL_BGR_EXT, GL_UNSIGNED_BYTE, sb.GetData());
		CxImage xIma;
		if (xIma.CreateFromArray(sb.GetData(), sizeVwpt.cx, sizeVwpt.cy, 24, nPitch, false)) {
			return xIma.MakeBitmap(hDC_Win);
		}
	}
	return NULL;
}

BOOL CDoom3OpenGLWnd::CreateDef(CWnd *pParentWnd, CSize sizeMax)
{
	const DWORD nStyle = (rrt == rrtVis)
		? (0 |WS_POPUP)
		: (0 |WS_POPUP)
		;
	const DWORD nExStyle = (rrt == rrtVis)
		? (fExpose ? WS_EX_TOPMOST : 0)
		: (fExpose ? 0 : 0)
		;
	sizeVwpt = sizeMax;
	CRect rc(0, 0, sizeVwpt.cx, sizeVwpt.cy);
	AdjustWindowRectEx(&rc, nStyle, false, nExStyle);
	rc.OffsetRect(-rc.TopLeft());
	BOOL f = CWnd::CreateEx(
		nExStyle,
		AfxRegisterWndClass(
			0 |CS_HREDRAW |CS_VREDRAW,
			LoadCursor(NULL, IDC_ARROW),
			(rrt == rrtVis) ? CreateSolidBrush(0) : NULL,
			NULL
			),
		_T("MD5mesh"),
		nStyle,
		rc,
		pParentWnd,
		0
		);
	if (!f)
		return f;
	if (rrt == rrtVis)
		SetWindowPos(NULL, 0, 0, 0, 0, 0 |SWP_NOACTIVATE |SWP_NOMOVE |SWP_NOOWNERZORDER |SWP_NOSIZE |SWP_NOZORDER |SWP_SHOWWINDOW);
	return f;
}

void CDoom3OpenGLWnd::TexstInstall(int iTexst, int iOptTexst, UINT iTex, Doom3Blendf bf1, Doom3Blendf bf2, GLenum &sfactor, GLenum &dfactor)
{
	if (!gl.fARB_multitexture && iTexst != 0) return;

	switch (bf1) {
	case bfBlend:
		{
			bf1 = bfSrcAlpha;
			bf2 = bfOneMinusSrcAlpha;
			break;
		}
	case bfAdd:
		{
			bf1 = bfOne;
			bf2 = bfOne;
			break;
		}
	case bfFilter:
		{
			bf1 = bfDstColor;
			bf2 = bfZero;
			break;
		}
	case bfDiffuseMap:
		{
			bf1 = bfOne;
			bf2 = bfZero;
			break;
		}
	case bfNo:
		{
			bf1 = bfOne;
			bf2 = bfNo;
			break;
		}
	}

//	if (iTexst == 0 && bf1 != bfZero) {
//		sfactor = GL_ONE; dfactor = GL_ZERO;
//	}

	if (sfactor == -1) {
		switch (bf1) {
		case bfZero:
		case bfOne:
		case bfDstColor:
		case bfOneMinusDstColor:
		case bfSrcAlpha:
		case bfOneMinusSrcAlpha:
		case bfDstAlpha:
		case bfOneMinusDstAlpha:
		// GL_SRC_ALPHA_SATURATE
			{
				switch (bf1) {
				case bfZero:			sfactor = GL_ZERO; break;
				case bfOne:				sfactor = GL_ONE; break;
				case bfDstColor:		sfactor = GL_SRC_COLOR; break;
				case bfOneMinusDstColor:sfactor = GL_ONE_MINUS_SRC_COLOR; break;
				case bfSrcAlpha:		sfactor = GL_SRC_ALPHA; break;
				case bfOneMinusSrcAlpha:sfactor = GL_ONE_MINUS_SRC_ALPHA; break;
				case bfDstAlpha:		sfactor = GL_SRC_ALPHA; break;
				case bfOneMinusDstAlpha:sfactor = GL_ONE_MINUS_SRC_ALPHA; break;
				}
				break;
			}
		}
	}

	if (dfactor == -1) {
		switch (bf2) {
		case bfZero:
		case bfOne:
		case bfSrcColor:
		case bfOneMinusSrcColor:
		case bfSrcAlpha: 
		case bfOneMinusSrcAlpha:
		case bfDstAlpha:
		case bfOneMinusDstAlpha:
			{
				switch (bf2) {
				case bfZero:			dfactor = GL_ZERO; break;
				case bfOne:				dfactor = GL_ONE; break;
				case bfSrcColor:		dfactor = GL_SRC_COLOR; break;
				case bfOneMinusSrcColor:dfactor = GL_ONE_MINUS_SRC_COLOR; break;
				case bfSrcAlpha:		dfactor = GL_SRC_ALPHA; break;
				case bfOneMinusSrcAlpha:dfactor = GL_ONE_MINUS_SRC_ALPHA; break;
				case bfDstAlpha:		dfactor = GL_DST_ALPHA; break;
				case bfOneMinusDstAlpha:dfactor = GL_ONE_MINUS_DST_ALPHA; break;
				}
				break;
			}
		}
	}

	if (false);
	else if (gl.fARB_multitexture && gl.fARB_texture_env_combine) {

		float 
			fCombineRGB		= GL_REPLACE,
			fCombineAlpha	= GL_REPLACE,
			fSrc0RGB		= GL_TEXTURE,
			fSrc1RGB		= GL_PREVIOUS_ARB,
			fSrc2RGB		= GL_CONSTANT_ARB,
			fSrc0Alpha		= GL_TEXTURE,
			fSrc1Alpha		= GL_PREVIOUS_ARB,
			fSrc2Alpha		= GL_CONSTANT_ARB,
			fOp0RGB			= GL_SRC_COLOR,
			fOp1RGB			= GL_SRC_COLOR,
			fOp2RGB			= GL_SRC_ALPHA,
			fOp0Alpha		= GL_SRC_ALPHA,
			fOp1Alpha		= GL_SRC_ALPHA,
			fOp2Alpha		= GL_SRC_ALPHA;

		// SRC = GL_TEXTURE
		// DST = GL_PREVIOUS_ARB

		fCombineRGB =	GL_ADD;
		fCombineAlpha =	GL_ADD;

		bool fCombineDone = false;

		if (bf1 == bfDstAlpha) {
			fCombineDone = true;

			fCombineRGB		= GL_MODULATE,
			fSrc0RGB		= GL_TEXTURE,
			fSrc1RGB		= GL_PREVIOUS_ARB,
			fOp0RGB			= GL_SRC_COLOR,
			fOp1RGB			= GL_SRC_ALPHA,
			fCombineAlpha	= GL_MODULATE,
			fSrc0Alpha		= GL_TEXTURE,
			fSrc1Alpha		= GL_PREVIOUS_ARB,
			fOp0Alpha		= GL_SRC_ALPHA,
			fOp1Alpha		= GL_SRC_ALPHA;
		}
		if (bf1 == bfOne && bf2 == bfZero) {
			fCombineDone = true;

			fCombineRGB		= GL_REPLACE,
			fSrc0RGB		= GL_TEXTURE,
			fOp0RGB			= GL_SRC_COLOR,
			fCombineAlpha	= GL_REPLACE,
			fSrc0Alpha		= GL_TEXTURE,
			fOp0Alpha		= GL_SRC_ALPHA;
		}

		if (!fCombineDone) {
			for (char s = 0; s < 2; s++) {
				Doom3Blendf bf = (s == 0) ? bf1 : bf2;
				float
					fSrcRGB,
					fOpRGB,
					fSrcAlpha,
					fOpAlpha;

				float fPrior = (iTexst == 0) ? (float)GL_CONSTANT_ARB : (float)GL_PREVIOUS_ARB;

				float fSrcKey = (s == 0) ? GL_TEXTURE : fPrior;
				float fDstKey = (s == 0) ? fPrior : GL_TEXTURE;

				switch (bf) {
				default:
				case bfNo:
				case bfZero:
					{
						fSrcRGB =	GL_CONSTANT_ARB;
						fOpRGB =	GL_SRC_ALPHA;
						fSrcAlpha =	GL_CONSTANT_ARB;
						fOpAlpha =	GL_SRC_ALPHA;
						break;
					}
				case bfOne:
					{
						fSrcRGB =	fSrcKey;
						fOpRGB =	GL_SRC_COLOR;
						fSrcAlpha =	fSrcKey;
						fOpAlpha =	GL_SRC_ALPHA;
						break;
					}
				case bfDstColor:
					{
						fSrcRGB =	fSrcKey; // GL_PREVIOUS_ARB;
						fOpRGB =	GL_SRC_COLOR;
						fSrcAlpha =	fSrcKey; // GL_PREVIOUS_ARB;
						fOpAlpha =	GL_SRC_ALPHA;
						break;
					}
				case bfOneMinusDstColor:
					{
						fSrcRGB =	fSrcKey; // GL_PREVIOUS_ARB;
						fOpRGB =	GL_ONE_MINUS_SRC_COLOR;
						fSrcAlpha =	fSrcKey; // GL_PREVIOUS_ARB;
						fOpAlpha =	GL_ONE_MINUS_SRC_ALPHA;
						break;
					}
				case bfSrcAlpha:
					{
						fSrcRGB =	fSrcKey; // GL_TEXTURE;
						fOpRGB =	GL_SRC_ALPHA;
						fSrcAlpha =	fSrcKey; // GL_TEXTURE;
						fOpAlpha =	GL_SRC_ALPHA;
						break;
					}
				case bfOneMinusSrcAlpha:
					{
						fSrcRGB =	fSrcKey; // GL_TEXTURE;
						fOpRGB =	GL_ONE_MINUS_SRC_ALPHA;
						fSrcAlpha =	fSrcKey; // GL_TEXTURE;
						fOpAlpha =	GL_ONE_MINUS_SRC_ALPHA;
						break;
					}
				case bfDstAlpha:
					{
						fSrcRGB =	fDstKey;
						fOpRGB =	GL_SRC_ALPHA;
						fSrcAlpha =	fDstKey;
						fOpAlpha =	GL_SRC_ALPHA;
						break;
					}
				case bfOneMinusDstAlpha:
					{
						fSrcRGB =	fSrcKey; // GL_PREVIOUS_ARB;
						fOpRGB =	GL_ONE_MINUS_SRC_ALPHA;
						fSrcAlpha =	fSrcKey; // GL_PREVIOUS_ARB;
						fOpAlpha =	GL_ONE_MINUS_SRC_ALPHA;
						break;
					}
				}

				switch (s) {
				case 0:
					{
						fSrc0RGB		= fSrcRGB,
						fSrc0Alpha		= fSrcAlpha,
						fOp0RGB			= fOpRGB,
						fOp0Alpha		= fOpAlpha;
						break;
					}
				case 1:
					{
						fSrc1RGB		= fSrcRGB,
						fSrc1Alpha		= fSrcAlpha,
						fOp1RGB			= fOpRGB,
						fOp1Alpha		= fOpAlpha;
						break;
					}
				}
			}
		}

		gl.ActiveTextureARB(GL_TEXTURE0_ARB +iTexst);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,	GL_COMBINE_ARB);

		glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB,	fCombineRGB);
		glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB,	fCombineAlpha);
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB,	fSrc0RGB);
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB,	fSrc1RGB);
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB,	fSrc2RGB);
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB,	fSrc0Alpha);
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB,	fSrc1Alpha);
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA_ARB,	fSrc2Alpha);
		glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB,	fOp0RGB);
		glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB,	fOp1RGB);
		glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB,	fOp2RGB);
		glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB,fOp0Alpha);
		glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_ARB,fOp1Alpha);
		glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA_ARB,fOp2Alpha);
	}
	else {
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,	GL_REPLACE);
	}

	if (!gl.fARB_multitexture && iTexst != 0) return;

	static const float f0[] = {0,0,0,0};

	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, f0);

	glBindTexture(GL_TEXTURE_2D, iTex);

	EnableTexs(true);
}
