// ChildFrm.h : interface of the CChildFrame class
//

#pragma once

#include "MD5AnimCraftzmanView.h"

class CChildFrame : public CMDIChildWnd
{
public:
	// 
	CMD5AnimCraftzmanView m_vw;
	// 
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 
	DECLARE_DYNCREATE(CChildFrame);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnDestroy();
};
