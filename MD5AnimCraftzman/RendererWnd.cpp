// RendererWnd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RendererWnd.h"

#include <gl/gl.h>

#pragma comment(lib, "opengl32")

// CRendererWnd

IMPLEMENT_DYNAMIC(CRendererWnd, CView)

CRendererWnd::CRendererWnd()
{

}

CRendererWnd::~CRendererWnd()
{

}

BEGIN_MESSAGE_MAP(CRendererWnd, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CRendererWnd メッセージ ハンドラ

int CRendererWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (Restore(true)) {
		wReshape(lpCreateStruct->cx, lpCreateStruct->cy);
	}

	return 0;
}

void CRendererWnd::OnDestroy()
{
	CView::OnDestroy();

	wGL.Close();
}

void CRendererWnd::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (Restore(false)) {
		wReshape(cx, cy);
	}
}

void CRendererWnd::wReshape(int cx, int cy)
{
	glViewport(0, 0, cx, cy);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0, 0, 0.1f, 1);

	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void CRendererWnd::wDisplay()
{
	glClear(0 |GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

	
}

void CRendererWnd::OnPaint()
{
	CPaintDC dc(this);

	DoPaint();
}

void CRendererWnd::DoPaint()
{
	if (Restore(false)) {
		wDisplay();
		glFinish();
		SwapBuffers(wGL.hDC);
	}
}

UINT CRendererWnd::OnNcHitTest(CPoint point)
{
	return HTTRANSPARENT;
}

bool CRendererWnd::Restore(bool fInit)
{
	return wGL.Restore(fInit, *this);
}
