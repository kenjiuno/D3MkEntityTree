// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "JointListBar.h"
#include "ToolContext.h"
#include "OnIdle.h"
#include "PM.h"

class CMainFrame : public CMDIFrameWnd, public COnIdle
{
	// 
	DECLARE_DYNAMIC(CMainFrame)

	// 
	CToolContextSPtr pToolCom;

public:
	// 
	CMainFrame();
	// 
	CToolContextSPtr GetToolCom() { return pToolCom; }
	// 
	virtual bool OnIdle(LONG i)
	{
		DWORD x = 0;
		pToolCom->Dispatch(PM_ON_IDLE, &x);
		return x != 0;
	}

	// 
	static CToolContextSPtr GetToolCom(CFrameWnd *p)
	{
		CMainFrame *pMe = (CMainFrame *)DYNAMIC_DOWNCAST(CMainFrame, p);
		if (pMe == NULL) return NULL;
		return pMe->GetToolCom();
	}

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// 
	CStatusBar  m_wndStatusBar;
	// 
	CToolBar    m_wndToolBar;
	// 
	CJointListBar m_wndJointListBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};


