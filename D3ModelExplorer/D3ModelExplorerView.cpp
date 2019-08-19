// D3ModelExplorerView.cpp : implementation of the CD3ModelExplorerView class
//

#include "stdafx.h"
#include "D3ModelExplorer.h"

#include "D3ModelExplorerDoc.h"
#include "D3ModelExplorerView.h"
#include "AfxPriv.h"
#include ".\d3modelexplorerview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CD3ModelExplorerView

IMPLEMENT_DYNCREATE(CD3ModelExplorerView, CListView)

BEGIN_MESSAGE_MAP(CD3ModelExplorerView, CListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
	ON_COMMAND_RANGE(ID_CONTEXT_TOOL1, ID_CONTEXT_TOOL3, OnContextTool1)
	ON_UPDATE_COMMAND_UI_RANGE(ID_CONTEXT_TOOL1, ID_CONTEXT_TOOL3, OnUpdateContextTool1)
	ON_WM_INITMENUPOPUP()
END_MESSAGE_MAP()

// CD3ModelExplorerView construction/destruction

CD3ModelExplorerView::CD3ModelExplorerView()
{
	// TODO: add construction code here

}

CD3ModelExplorerView::~CD3ModelExplorerView()
{
}

BOOL CD3ModelExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &=~LVS_TYPEMASK;
	cs.style |= LVS_LIST |LVS_SINGLESEL |LVS_SHOWSELALWAYS;

	return CListView::PreCreateWindow(cs);
}

void CD3ModelExplorerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}


// CD3ModelExplorerView diagnostics

#ifdef _DEBUG
void CD3ModelExplorerView::AssertValid() const
{
	CListView::AssertValid();
}

void CD3ModelExplorerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CD3ModelExplorerDoc* CD3ModelExplorerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CD3ModelExplorerDoc)));
	return (CD3ModelExplorerDoc*)m_pDocument;
}
#endif //_DEBUG


// CD3ModelExplorerView message handlers

void CD3ModelExplorerView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
	switch (lHint) {
	case 1:
		{
			if (!Mk_ctx()) break;

			lc().DeleteAllItems();

			CEntryMap m;
			m_ctx->Get_EntryMap(m);

			int n = 0;

			CEntryMap::iterator
				iterPos = m.begin(),
				iterEnd = m.end();
			for (; iterPos != iterEnd; iterPos++) {
				CString strName = iterPos->first;
				if (false
					|| IsExtNameIs(strName, "md5mesh")
				//	|| IsExtNameIs(strName, "md5anim")
					|| IsExtNameIs(strName, "ase")
				//	|| IsExtNameIs(strName, "lwo")
				) {
					lc().InsertItem(n, strName);
					n++;
				}
			}

			break;
		}
	}
}

int CD3ModelExplorerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	lc().InsertColumn(0, "Entry", LVCFMT_LEFT, 300);

	return 0;
}

#define _GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define _GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

void CD3ModelExplorerView::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	DWORD i = GetMessagePos();
	int x = _GET_X_LPARAM(i);
	int y = _GET_Y_LPARAM(i);

	int iItem = lc().GetNextItem(-1, LVNI_SELECTED);
	if (iItem < 0) {

	} else {
		m_iSel = iItem;
		CMenu m;
		m.LoadMenu(IDR_MENU_TOOLS);
		m.GetSubMenu(0)->TrackPopupMenu(0 |TPM_RIGHTBUTTON, x, y, this);
	}

	*pResult = 0;
}

void CD3ModelExplorerView::OnContextTool1(UINT nID)
{
	CD3ModelExplorerDoc *pDoc = GetDocument();

	CString str = lc().GetItemText(m_iSel, 0);

	switch (nID) {
	case ID_CONTEXT_TOOL1: pDoc->ExecTool(0, str); break;
	case ID_CONTEXT_TOOL2: pDoc->ExecTool(1, str); break;
	case ID_CONTEXT_TOOL3: pDoc->ExecTool(2, str); break;
	}
}

void CD3ModelExplorerView::OnUpdateContextTool1(CCmdUI *pCmdUI)
{
	bool fEnabled = false;

	CString strName = lc().GetItemText(m_iSel, 0);

	switch (pCmdUI->m_nID - ID_CONTEXT_TOOL1) {
	case 0:
		fEnabled = false
			|| IsExtNameIs(strName, "md5mesh")
			;
		break;
	case 1:
		fEnabled = false
			|| IsExtNameIs(strName, "md5mesh")
			|| IsExtNameIs(strName, "md5anim")
			|| IsExtNameIs(strName, "ase")
			;
		break;
	case 2:
		fEnabled = false
			|| IsExtNameIs(strName, "md5mesh")
			;
		break;
	}

	pCmdUI->Enable(fEnabled);
}

void CD3ModelExplorerView::OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu)
{
	if (bSysMenu)
		return;     // don't support system menu

	ASSERT(pMenu != NULL);
	
	// check the enabled state of various menu items

	CCmdUI state;
	state.m_pMenu = pMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// determine if menu is popup in top-level menu and set m_pOther to
	//  it if so (m_pParentMenu == NULL indicates that it is secondary popup)
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pMenu->m_hMenu)
		state.m_pParentMenu = pMenu;    // parent == child for tracking popup
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = GetTopLevelParent();
			// child windows don't have menus -- need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pMenu->m_hMenu)
				{
					// when popup is found, m_pParentMenu is containing menu
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
	  state.m_nIndex++)
	{
		state.m_nID = pMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // menu separator or invalid cmd - ignore it

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to first item of that popup
			state.m_pSubMenu = pMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // first item of popup can't be routed to
			}
			state.DoUpdate(this, FALSE);    // popups are never auto disabled
		}
		else
		{
			// normal menu item
			// Auto enable/disable if frame window has 'm_bAutoMenuEnable'
			//    set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, false && state.m_nID < 0xF000);
		}

		// adjust for menu deletions and additions
		UINT nCount = pMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}
