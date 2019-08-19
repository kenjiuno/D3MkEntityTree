// JointListView.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "MD5AnimCraftzman.h"
#include "JointListView.h"
#include "MD5AnimCraftzmanDoc.h"
#include "PM.h"
#include ".\jointlistview.h"

// CJointListView

CJointListView::CJointListView()
: CCtrlView("ListBox", 0 |WS_CHILD |WS_VISIBLE |LBS_NOTIFY |LBS_OWNERDRAWFIXED |LBS_NODATA |LBS_NOINTEGRALHEIGHT |WS_VSCROLL)
{

}

CJointListView::~CJointListView()
{

}

BEGIN_MESSAGE_MAP(CJointListView, CCtrlView)
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()

void CJointListView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	DRAWITEMSTRUCT &t = *lpDrawItemStruct;
	size_t i = t.itemID;
	if (!(i < evec.size())) return;
	Elem &e = evec[i];

	CDC *pDC = CDC::FromHandle(t.hDC);

	COLORREF clrBack = (t.itemState & ODS_SELECTED)
		? GetSysColor(COLOR_HIGHLIGHT)
		: GetSysColor(COLOR_WINDOW)
		;
	COLORREF clrFore = (t.itemState & ODS_SELECTED)
		? GetSysColor(COLOR_HIGHLIGHTTEXT)
		: GetSysColor(COLOR_WINDOWTEXT)
		;

	CRect rc = t.rcItem;

	pDC->SetTextColor(clrFore);
	pDC->FillSolidRect(rc, clrBack);

	if (t.itemState & ODS_FOCUS) pDC->DrawFocusRect(rc);

	rc.DeflateRect(0, 0, 0, 0);

	CRect rcText = rc;

	rcText.DeflateRect(4 +e.iLv*8, 0, 0, 0);

	pDC->DrawText(e.strName, rcText, 0 |DT_LEFT |DT_SINGLELINE |DT_VCENTER);

	int nROP2 = pDC->SetROP2(R2_NOT);

	int my = (rc.top + rc.bottom) / 2;

	int cx = e.iLv, x;

	for (x = 0; x < cx; x++) {
		int mx = rc.left + 8*x +4;
		if (x+1 == cx) {
			if (e.nBranch & (1 << x)) {
				pDC->MoveTo(mx, rc.top);
				pDC->LineTo(mx, rc.bottom);
			} else {
				pDC->MoveTo(mx, rc.top);
				pDC->LineTo(mx, my);
			}
			pDC->MoveTo(mx   , my);
			pDC->LineTo(mx +8, my);
		} else {
			if (e.nBranch & (1 << x)) {
				pDC->MoveTo(mx, rc.top);
				pDC->LineTo(mx, rc.bottom);
			} else {

			}
		}
	}

	pDC->SetROP2(nROP2);
}

void CJointListView::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 32;
}

void CJointListView::PostNcDestroy()
{
	SetToolContext(NULL);
}

void CJointListView::OnToolContextDispatch(UINT nHint, void *pv)
{
	switch (nHint) {
	case PM_SET_DOC:
		{
			CMD5AnimCraftzmanDoc *pDoc = CMD5AnimCraftzmanDoc::GetDoc((CObject *)pv);
			pCurDoc = pDoc;
			SetDoc();
			break;
		}
	case PM_ON_IDLE:
		{
			if (pCurDoc != NULL && pCurDoc->m_iAccessTime != m_iAccessTime) {
				SetDoc();
				m_iAccessTime = pCurDoc->m_iAccessTime;
			}
			break;
		}
	}
}

void CJointListView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	//TRACE3("CJointListView::OnActivateView(%d,%p,%p)\n", bActivate, pActivateView, pDeactiveView);

	CCtrlView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CJointListView::SetDoc()
{
	evec.clear();
	evec.reserve(100);

	if (pCurDoc == NULL || !(pCurDoc->m_iMeet & mcAnim) || pCurDoc->m_a10.framea.size() == 0) {
		ElemMeet();
		return;
	}

	NfMD5::MD5AnimJointArray_t &aJ = pCurDoc->m_a10.jointa;

	Walk(aJ, NULL);

	ElemMeet();
}

void CJointListView::OnInitialUpdate()
{
	__super::OnInitialUpdate();

	CFont font;
	font.CreateStockObject(DEFAULT_GUI_FONT);
	SetFont(&font);

	pCurDoc = NULL;
	m_iAccessTime = -1;
}

void CJointListView::Walk(NfMD5::MD5AnimJointArray_t &aJ, NfMD5::MD5AnimJoint_t *pRefJ, int iLv, DWORD nBranch)
{
	vector<int> v;

	size_t i;
	for (i = 0; i < aJ.size(); i++) {
		NfMD5::MD5AnimJoint_t *pJ = &aJ[i];
		if (pJ->pRefJ != pRefJ) continue;

		v.push_back(i);
	}

	for (i = 0; i < v.size(); i++) {
		NfMD5::MD5AnimJoint_t *pJ = &aJ[v[i]];

		DWORD nBranchNew = nBranch;
		if (iLv != 0 && iLv < 32 && i+1 != v.size()) nBranchNew |= (1 << (iLv-1));

		Elem e;
		e.iLv = iLv;
		e.strName = pJ->strName;
		e.iJ = v[i];
		e.nBranch = nBranchNew;

		evec.push_back(e);

		Walk(aJ, pJ, iLv +1, nBranchNew);
	}
}
