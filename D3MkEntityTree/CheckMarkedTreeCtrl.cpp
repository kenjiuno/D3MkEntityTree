
// +--------------------------------------------------
// |
// | CheckMarkedTreeCtrl.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#include "StdAfx.h"
#include "CheckMarkedTreeCtrl.h"

// CCheckMarkedTreeCtrl

IMPLEMENT_DYNAMIC(CCheckMarkedTreeCtrl, CTreeCtrl)

BEGIN_MESSAGE_MAP(CCheckMarkedTreeCtrl, CTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnTvnKeydown)
END_MESSAGE_MAP()

int CCheckMarkedTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!Initialize())
		return -1;

	return 0;
}

bool CCheckMarkedTreeCtrl::CreateMark(CBitmap &bm, int cx, int nState)
{
	TRY {
		do {
			CDC dcMem;
			if (!dcMem.CreateCompatibleDC(NULL))
				break;
			{
				CClientDC dc(NULL);
				if (!bm.CreateCompatibleBitmap(&dc, cx, cx)) break;
			}
			VERIFY(dcMem.SaveDC() != 0);
			dcMem.SelectObject(&bm);
			//
			dcMem.SetBkMode(OPAQUE);
			dcMem.FillSolidRect(0, 0, cx, cx, RGB(255, 255, 255));
			if (nState == 2) {
				dcMem.FillSolidRect(1, 1, cx-2, cx-2, RGB(192, 192, 192));
			}
			CBrush brBlack;
			brBlack.CreateStockObject(BLACK_BRUSH);
			CRect rc(1, 1, cx-1, cx-1);
			dcMem.FrameRect(&rc, &brBlack);
			switch (nState) {
			case 1:
			case 2:
				CFont fontMarlett;
				fontMarlett.CreatePointFont(100, "Marlett");
				dcMem.SelectObject(&fontMarlett);
				dcMem.SetBkMode(TRANSPARENT);
				dcMem.DrawText("a", CRect(1, 1, cx-1, cx-1), 0 |DT_CENTER |DT_VCENTER |DT_SINGLELINE);
				break;
			}
			//
			VERIFY(dcMem.RestoreDC(-1));
			return true;
		} while (false);
	} CATCH(CResourceException, e) {

	} END_CATCH
	return false;
}

#define _GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define _GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

void CCheckMarkedTreeCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	DWORD nPos = GetMessagePos();
	TVHITTESTINFO tvht;
	tvht.pt.x = _GET_X_LPARAM(nPos);
	tvht.pt.y = _GET_Y_LPARAM(nPos);
	GetDesktopWindow()->MapWindowPoints(CWnd::FromHandle(pNMHDR->hwndFrom), &tvht.pt, 1);
	HitTest(&tvht);
	if (tvht.flags & TVHT_ONITEMICON) {
		SelectItem(tvht.hItem);
		OnInverseCheckState(tvht.hItem);
	}

	*pResult = 0;
}

void CCheckMarkedTreeCtrl::OnInverseCheckState(HTREEITEM hItem)
{
	int iImage, iSelImage;
	if (!GetItemImage(hItem, iImage, iSelImage)) return;
	int iNewImage = (iImage == 0) ? 1 : 0;
	SetCheckState(hItem, iNewImage);
}

void CCheckMarkedTreeCtrl::SetCheckState(HTREEITEM hItem, int nState)
{
	SetAscendCheckState(hItem, nState);
	SetDescendCheckState(hItem);
}

bool CCheckMarkedTreeCtrl::SetAscendCheckState(HTREEITEM hItem, int nState, bool fNotifyAscend)
{
	if (SetItemImage(hItem, nState, nState)) {
		if (fNotifyAscend)
			OnChangeCheckState(hItem);
		OnDecideNewCheckState(hItem, nState == 1);
		return true;
	}
	return false;
}

void CCheckMarkedTreeCtrl::OnChangeCheckState(HTREEITEM hItem)
{
	HTREEITEM hParent = GetParentItem(hItem);
	if (!hParent) return;
	hItem = GetChildItem(hParent);
	int nCnt = 0, nYes = 0, nGrayed = 0;
	while (hItem) {
		nCnt++;
		switch (GetCheckState(hItem)) {
		case 1: nYes++; break;
		case 2: nGrayed++; break;
		}
		hItem = GetNextSiblingItem(hItem);
	}
	int nState;
	if (nGrayed != 0) {
		nState = 2;
	} else if (nCnt == nYes) {
		nState = 1;
	} else if (nYes != 0) {
		nState = 2;
	} else {
		nState = 0;
	}
	SetAscendCheckState(hParent, nState);
}

int CCheckMarkedTreeCtrl::GetCheckState(HTREEITEM hItem)
{
	int x, y;
	if (GetItemImage(hItem, x, y)) {
		return x;
	}
	return -1;
}

void CCheckMarkedTreeCtrl::OnDecideNewCheckState(HTREEITEM hItem, bool fCheck)
{

}

void CCheckMarkedTreeCtrl::SetDescendCheckState(HTREEITEM hItem)
{
	int nState = GetCheckState(hItem);
	switch (nState) {
	case 0:
	case 1:
		{
			InheritCheckState(hItem);
			break;
		}
	}
}

void CCheckMarkedTreeCtrl::InheritCheckState(HTREEITEM hItem)
{
	if (hItem == NULL) return;

	int nState = GetCheckState(hItem);
	hItem = GetChildItem(hItem);
	while (hItem) {
		int nHisState = GetCheckState(hItem);
		if (nHisState != nState) {
			SetAscendCheckState(hItem, nState, false);
			InheritCheckState(hItem);
		}
		hItem = GetNextSiblingItem(hItem);
	}
}

bool CCheckMarkedTreeCtrl::Initialize()
{
	int cy = GetItemHeight();
	m_checkMarks.DeleteImageList();
	if (!m_checkMarks.Create(cy, cy, 0 |ILC_COLOR |ILC_MASK, 0, 3))
		return false;

	UINT i;
	for (i = 0; i < 3; i++) {
		CBitmap bm;
		VERIFY(CreateMark(bm, cy, i));
		m_checkMarks.Add(&bm, RGB(255,255,255));
		bm.Detach();
	}

	SetImageList(&m_checkMarks, TVSIL_NORMAL);

	return true;
}

void CCheckMarkedTreeCtrl::PreSubclassWindow()
{
	VERIFY(Initialize());

	CTreeCtrl::PreSubclassWindow();
}

void CCheckMarkedTreeCtrl::OnTvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	switch (pTVKeyDown->wVKey) {
	case ' ':
		if (!(GetKeyState(VK_MENU) & 0x8000)) {
			OnInverseCheckState(GetSelectedItem());
			*pResult = 1;
			return;
		}
		break;
	}
	*pResult = 0;
}
