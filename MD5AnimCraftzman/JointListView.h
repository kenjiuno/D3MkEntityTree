
#pragma once

#include "ToolContext.h"
#include "10ACDoc.h"

class CJointListView : public CCtrlView, protected CToolContextSink
{
	// 
	struct Elem {
		// 
		CString strName;
		// 
		int iLv;
		// 
		int iJ;
		// 
		DWORD nBranch;
	};
	// 
	typedef vector<Elem> ElemVec;

	// 
	CToolContextSPtr pTar;
	// 
	C10ACDoc *pCurDoc;
	// 
	int m_iAccessTime;
	// 
	ElemVec evec;

public:
	// 
	CJointListView();
	// 
	virtual ~CJointListView();

	// 
	void SetToolContext(CToolContextSPtr p)
	{
		if ((pTar) != NULL)
			pTar->Remove(this);
		if ((pTar = p) != NULL)
			pTar->Add(this);
	}
	// 
	void SetDoc();
	// 
	inline CListBox &cv() { return reinterpret_cast<CListBox &>(*this); }
	// 
	void ElemMeet()
	{
		cv().SendMessage(LB_SETCOUNT, (WPARAM)evec.size());
	}
	// 
	void Walk(NfMD5::MD5AnimJointArray_t &aJ, NfMD5::MD5AnimJoint_t *pRefJ, int iLv = 0, DWORD nBranch = 0);

protected:
	// 
	virtual void OnToolContextDispatch(UINT nHint, void *pv);

	// 
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
protected:
	virtual void PostNcDestroy();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	virtual void OnInitialUpdate();
};


