// MD5AnimCraftzmanView.h : interface of the CMD5AnimCraftzmanView class
//


#pragma once

#include "RendererWnd.h"
#include "ToolContext.h"

class CMD5AnimCraftzmanView : public CRendererWnd, public CToolContextSink
{
public:
	// 
	CMD5AnimCraftzmanView();
	// 
	virtual void OnToolContextDispatch(UINT nHint, void *pv);

	// 
	void SetToolContext(CToolContextSPtr p)
	{
		if ((pToolCom) != NULL)
			pToolCom->Remove(this);
		if ((pToolCom = p) != NULL)
			pToolCom->Add(this);
	}

	// 
	DECLARE_DYNCREATE(CMD5AnimCraftzmanView)

// Attributes
public:
	CMD5AnimCraftzmanDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMD5AnimCraftzmanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// 
	CToolContextSPtr pToolCom;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	virtual void PostNcDestroy();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
};

#ifndef _DEBUG  // debug version in MD5AnimCraftzmanView.cpp
inline CMD5AnimCraftzmanDoc* CMD5AnimCraftzmanView::GetDocument() const
   { return reinterpret_cast<CMD5AnimCraftzmanDoc*>(m_pDocument); }
#endif

