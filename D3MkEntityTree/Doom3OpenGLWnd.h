
// +--------------------------------------------------
// |
// | Doom3OpenGLWnd.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

#include <gl/gl.h>
#include <gl/glu.h>
#include "My3DMath.h"
#include "Doom3ControlContext.h"
#include <xImage.h>
#include "GLExtz.h"
#include "Doom3RP.h"
#include "Structure.h"
#include "NoMemberwiseCopy.h"

#define PV_MAX CSize(400,300)
#define PV_MIN CSize(120, 90)
#define PV_SUPER CSize(800,600)

//struct CMD5MeshDeck;
//struct CMD5AnimDeck;
//class CAviMk;

//struct CDoom3Workpad;

//struct CDoom3materialIndent;

//struct CDoom3materialTexoMap;
//struct CDoom3materialTexo;

//extern CGLExtz g_gl;

namespace Renderer
{

typedef map<CString, GLuint> Tex2TexVindMap;

typedef vector<GLuint> TexVindArray;

struct DisplayEntry {
	// 
	GLuint iPrepare, iUnprepare;
};

typedef map<CString, DisplayEntry> Shader2DisplayMap;

typedef vector<DisplayEntry> DisplayArray;

struct Blendf2GLBlend {
	// 
	static bool Conv(Doom3Blendf bf1, Doom3Blendf bf2, GLenum &f1, GLenum &f2)
	{
		switch (bf1) {
			case bfBlend:
				f1 = GL_SRC_ALPHA, f2 = GL_ONE_MINUS_SRC_ALPHA; return true;
			case bfAdd:
				f1 = GL_ONE, f2 = GL_ONE; return true;
			case bfFilter:
				f1 = GL_DST_COLOR, f2 = GL_ZERO; return true;
			case bfNo:
				f1 = GL_ZERO, f2 = GL_ONE; return true;
		}
		if (Conv(bf1, f1))
			if (Conv(bf2, f2))
				return true;

		return false;
	}
	// 
	static bool Conv(Doom3Blendf bf1, GLenum &f1)
	{
		switch (bf1) {
			default:
				return false;
			case bfDstAlpha:
				f1 = GL_DST_ALPHA; break;
			case bfDstColor:
				f1 = GL_DST_COLOR; break;
			case bfOne:
				f1 = GL_ONE; break;
			case bfOneMinusDstAlpha:
				f1 = GL_ONE_MINUS_DST_ALPHA; break;
			case bfOneMinusDstColor:
				f1 = GL_ONE_MINUS_DST_COLOR; break;
			case bfOneMinusSrcAlpha:
				f1 = GL_ONE_MINUS_SRC_ALPHA; break;
			case bfOneMinusSrcColor:
				f1 = GL_ONE_MINUS_SRC_COLOR; break;
			case bfSrcColor:
				f1 = GL_SRC_COLOR; break;
			case bfZero:
				f1 = GL_ZERO; break;
		}
		return true;
	}
};

struct Materialst {
	// 
	CDoom3materialIndent a;
	// 
	int iIndent;
	// 
	float scale[2];

	// 
	float fAlphaTest;
};

typedef vector<Materialst *> MaterialstPtrArray;

typedef list<Materialst> MaterialstList;

struct Materialst2Render : CNoMemberwiseCopy {
	// 
	Materialst *pBumpmap, *pDiffusemap, *pSpecularmap;
	// 
	MaterialstPtrArray a1s;

	// 
	MaterialstList v1s;

	// 
	Materialst2Render()
		: pBumpmap(NULL), pDiffusemap(NULL), pSpecularmap(NULL)
	{

	}
	// 
	Materialst *Add1()
	{
		return &*v1s.insert(v1s.end(), Materialst());
	}
};

enum OpenGLRenderTar {
	rrtInvis,
	rrtVis,
	rrtPb,
};

class CDoom3OpenGLWnd : public CWnd, public MaterialTexo::TexoTex, public CEvalSampler
{
	// 
	enum RendererClass {
		rclsDiffuseMap,
		rclsMtex,
		rclsMtexDot3,
		rclsFragmentProgram,
	};

	// 
	HDC hDC_Win, hDC_Pb;
	// 
	HGLRC hRC_Win, hRC_Pb;
	// 
	HPBUFFERARB hPB_Pb;
	// 
	PIXELFORMATDESCRIPTOR pfd;
	// 
	CGLExtz gl;
	// renderer class
	int rcls;
	// # of texture stages avail.
	int nTexs;
	// 
	Tex2TexVindMap t2tvMap;
	// 
	TexVindArray tvVec;
	// 
	Shader2DisplayMap s2psMap;
	// 
	DisplayArray psVec;
	// 
	GLuint iDisplayDoNothing;

	// 
	void fnReshape();
	// 
	void fnDraw();
	// 
	void fnDraw_Axis(bool fGrid = true);
	// 
	void fnDraw_Mesh(bool fWireframe = false);
	// 
	void fnDraw_Vone();

	// 
	bool LoadIma(CString strName, CxImage &xIma);
	// 
	void SelectPShader(CString strName, DisplayEntry &f);
	// 
	bool PrecacheTex(CString strName, GLuint &iTex, bool fPrecache = true);
	// 
	void EnableTexs(bool fEnable);
	// 
	void PrecacheTexsh(CString strName);
	// 
	bool FindTexsh(CString strName, int i, GLuint &iTex);
	// 
	void TexstInstall(int iTexst, int iOptTexst, UINT iTex, Doom3Blendf bf1, Doom3Blendf bf2, GLenum &sfactor, GLenum &dfactor);

	// 
	void DecideRendererClass();

	// 
	static CString MkTexst(CString strName, int i)
	{
		CString str; str.Format("%s!%d"
			, (LPCTSTR)strName, i
			);
		return str;
	}

	// 
	virtual float EvalTable(CString strName, float x);
	// 
	virtual float EvalNameo(CString strName);

	// 
	bool Mk_OpenGLWin();
	// 
	bool Mk_OpenGL_Pb();
	// 
	void Close_OpenGL();
	// 
	void Close_DC();

	// 
	GLsizei computeNextPow2(GLsizei cx)
	{
		for (int x = 0; x < 32; x++) {
			GLsizei n = 1 << x;
			if (cx <= n)
				return n;
		}
		return 0;
	}

	// 
	DECLARE_DYNAMIC(CDoom3OpenGLWnd)
	// 
	DECLARE_MESSAGE_MAP()

public:
	// 
	CSize sizeVwpt;
	// 
	float fMaxLen;
	// 
	My3DMath::Vtx3 center;
	// 
	bool fGamma;
	// 
	bool fARB_texture_env_dot3, fARB_fragment_program;
	// 
	OpenGLRenderTar rrt;
	// 
	bool fExpose;
	// 
	int nDevColorBits, nDevAlphaBits, nDevDepthBits;
	// 
	bool fDevHW, fDevSW;
	// 
	int nDevClass;

	// 
	CDoom3Workpad *pwk;
	// 
	CDoom3ControlContext *pcc;
	// 
	CAviMk *pam;
	// 
	TformedMeshTabPtrArray tfmtVec;

	// 
	CDoom3OpenGLWnd()
	{
		sizeVwpt = PV_MAX;
		fMaxLen = 5.0f;
	}
	// 
	bool Create()
	{
		return CreateSized(PV_MAX);
	}
	// 
	bool CreateSized(CSize size)
	{
		if (IsWindow(GetSafeHwnd()))
			return true;
		if (CreateDef(NULL, size))
			return true;
		return false;
	}
	// 
	BOOL CreateDef(CWnd *pParentWnd, CSize sizeMax = PV_MAX);
	// 
	bool IsRendererAlive()
	{
		if (hRC_Pb != NULL && hRC_Pb == wglGetCurrentContext())
			return true;
		if (hRC_Win != NULL && hRC_Win == wglGetCurrentContext())
			return true;

		return false;
	}
	// 
	void Close()
	{
		DestroyWindow();
	}
	// 
	void ResetCaps()
	{
		nDevAlphaBits = 0;
		nDevColorBits = 0;
		nDevDepthBits = 0;
		nDevClass = 0;
	}

	// 
	void Clear()
	{
		pwk = NULL;
		pcc = NULL;
		pam = NULL;
	}
	// 
	bool Capture(LPCTSTR psz);
	// 
	bool CaptureAnim();
	// 
	void PrecacheShader();
	// 
	void PrecacheShader(CString strShader);
	// 
	void UnprecacheShader();
	// 
	HBITMAP CaptureTestScene();

	// 
	virtual bool LoadIma(CString strName, CDoom3FlatIma &rIma);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
};

};
