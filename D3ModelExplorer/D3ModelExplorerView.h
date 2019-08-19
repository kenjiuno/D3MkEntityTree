// D3ModelExplorerView.h : interface of the CD3ModelExplorerView class
//


#pragma once


class CD3ModelExplorerView : public CListView
{
	// 
	CMyDoom3Context *m_ctx;
	// 
	int m_iSel;

	// 
	inline CListCtrl &lc() { return GetListCtrl(); }
	// 
	bool Mk_ctx()
	{
		m_ctx = &GetDocument()->m_com;

		return m_ctx != NULL;
	}
	// 
	static inline bool IsExtNameIs(LPCTSTR pszPath, LPCTSTR pszExtName)
	{
		LPCTSTR psz0 = _tcsrchr(pszPath, '/');
		LPCTSTR psz1 = _tcsrchr(pszPath, '.');
		if (psz0 < psz1) {
			psz1++;
			if (_tcscmp(psz1, pszExtName) == 0)
				return true;
		}
		return false;
	}

protected:
	// 
	CD3ModelExplorerView();

	// 
	void OnUpdateContextTool1(CCmdUI *pCmdUI);

	// 
	DECLARE_DYNCREATE(CD3ModelExplorerView)

// Attributes
public:
	CD3ModelExplorerDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CD3ModelExplorerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextTool1(UINT nID);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
};

#ifndef _DEBUG  // debug version in D3ModelExplorerView.cpp
inline CD3ModelExplorerDoc* CD3ModelExplorerView::GetDocument() const
   { return reinterpret_cast<CD3ModelExplorerDoc*>(m_pDocument); }
#endif

