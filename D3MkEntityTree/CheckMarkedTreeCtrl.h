
// +--------------------------------------------------
// |
// | CheckMarkedTreeCtrl.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

// CCheckMarkedTreeCtrl

class CCheckMarkedTreeCtrl : public CTreeCtrl
{
	// 
	void OnInverseCheckState(HTREEITEM hItem);
	// 
	bool SetAscendCheckState(HTREEITEM hItem, int nState, bool fNotifyAscend = true);
	// 
	void OnChangeCheckState(HTREEITEM hItem);
	// 
	void SetDescendCheckState(HTREEITEM hItem);
	// 
	void InheritCheckState(HTREEITEM hItem);

	// 
	DECLARE_DYNAMIC(CCheckMarkedTreeCtrl)

public:
	// 
	bool Initialize();
	// 
	int GetCheckState(HTREEITEM hItem);
	// 
	void SetCheckState(HTREEITEM hItem, int nState);

protected:
	// 
	CImageList m_checkMarks;

	// 
	virtual void OnDecideNewCheckState(HTREEITEM hItem, bool fCheck);

	// 
	static bool CreateMark(CBitmap &bm, int cx, int nState);

	// 
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnTvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
};


