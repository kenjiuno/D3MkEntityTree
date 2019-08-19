// MD5AnimCraftzmanView.cpp : implementation of the CMD5AnimCraftzmanView class
//

#include "stdafx.h"
#include "MD5AnimCraftzman.h"

#include "MD5AnimCraftzmanDoc.h"
#include "MD5AnimCraftzmanView.h"
#include "PM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMD5AnimCraftzmanView

IMPLEMENT_DYNCREATE(CMD5AnimCraftzmanView, CRendererWnd)

BEGIN_MESSAGE_MAP(CMD5AnimCraftzmanView, CRendererWnd)
	ON_WM_SIZE()
	ON_WM_COPYDATA()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMD5AnimCraftzmanView construction/destruction

CMD5AnimCraftzmanView::CMD5AnimCraftzmanView()
{
	// TODO: add construction code here

}

CMD5AnimCraftzmanView::~CMD5AnimCraftzmanView()
{
}

BOOL CMD5AnimCraftzmanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRendererWnd::PreCreateWindow(cs);
}

// CMD5AnimCraftzmanView drawing

void CMD5AnimCraftzmanView::OnDraw(CDC* /*pDC*/)
{

}


// CMD5AnimCraftzmanView diagnostics

#ifdef _DEBUG
void CMD5AnimCraftzmanView::AssertValid() const
{
	CRendererWnd::AssertValid();
}

void CMD5AnimCraftzmanView::Dump(CDumpContext& dc) const
{
	CRendererWnd::Dump(dc);
}

CMD5AnimCraftzmanDoc* CMD5AnimCraftzmanView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMD5AnimCraftzmanDoc)));
	return (CMD5AnimCraftzmanDoc*)m_pDocument;
}
#endif //_DEBUG


// CMD5AnimCraftzmanView message handlers

void CMD5AnimCraftzmanView::OnSize(UINT nType, int cx, int cy)
{
	CRendererWnd::OnSize(nType, cx, cy);

	RepositionBars(0, 0xffff, 0x0100);
}

BOOL CMD5AnimCraftzmanView::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	return CRendererWnd::OnCopyData(pWnd, pCopyDataStruct);
}

int CMD5AnimCraftzmanView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRendererWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CMD5AnimCraftzmanView::OnEraseBkgnd(CDC* pDC)
{
	return true;
}

void CMD5AnimCraftzmanView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRendererWnd::OnLButtonDown(nFlags, point);
}

void CMD5AnimCraftzmanView::PostNcDestroy()
{
	SetToolContext(NULL);
}

void CMD5AnimCraftzmanView::OnToolContextDispatch(UINT nHint, void *pv)
{

}

void CMD5AnimCraftzmanView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	//TRACE3("CJointListView::OnActivateView(%d,%p,%p)\n", bActivate, pActivateView, pDeactiveView);

	if (pToolCom && bActivate) {
		pToolCom->SetDoc(GetDocument(), true);
	}

	CRendererWnd::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
