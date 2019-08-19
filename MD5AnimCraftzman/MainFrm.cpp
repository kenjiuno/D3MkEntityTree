// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MD5AnimCraftzman.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_VIEW_HIERARCHYBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_HIERARCHYBAR, OnBarCheck)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	pToolCom.Attach(new CToolContext());

	if (false
		|| !m_wndToolBar.CreateEx(
			this,
			TBSTYLE_FLAT, 
			0 |WS_CHILD |WS_VISIBLE |CBRS_TOP |CBRS_GRIPPER |CBRS_TOOLTIPS |CBRS_FLYBY |CBRS_SIZE_DYNAMIC
			)
		|| !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)
	) {
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetHeight(GetSystemMetrics(SM_CYSMICON) + 12);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	if (false
		|| !m_wndStatusBar.Create(this)
		|| !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT))
	) {
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (false
		|| !m_wndJointListBar.Create("Hierarchy?", this, AFX_IDW_DIALOGBAR)
	) {
		return -1;
	}

	m_wndJointListBar.SetBarStyle(m_wndJointListBar.GetBarStyle() |CBRS_SIZE_DYNAMIC);
	m_wndJointListBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndJointListBar.m_frm.m_vw.SetToolContext(pToolCom);

	EnableDocking(CBRS_ALIGN_ANY);

#ifdef _SCB_REPLACE_MINIFRAME
    m_pFloatingFrameClass = RUNTIME_CLASS(CSCBMiniDockFrameWnd);
#endif //_SCB_REPLACE_MINIFRAME

	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndJointListBar, AFX_IDW_DOCKBAR_RIGHT);

	GetApp()->AddIdle(this);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers


void CMainFrame::OnDestroy()
{
	__super::OnDestroy();

	GetApp()->RemoveIdle(this);
}
