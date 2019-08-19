// MD5AnimCraftzmanDoc.h : interface of the CMD5AnimCraftzmanDoc class
//

#pragma once

#include "10ACDoc.h"

enum MeetConditionType {
	mcMesh = 1<<( 0),
	mcAnim = 1<<( 1),
};

class CMD5AnimCraftzmanDoc : public CDocument, public C10ACDoc
{
public:

	// 
	static CMD5AnimCraftzmanDoc *GetDoc(CObject *p)
	{
		CMD5AnimCraftzmanDoc *pDoc = (CMD5AnimCraftzmanDoc *)DYNAMIC_DOWNCAST(CMD5AnimCraftzmanDoc, p);
		return pDoc;
	}

protected:
	// 
	CMD5AnimCraftzmanDoc();
	// 
	DECLARE_DYNCREATE(CMD5AnimCraftzmanDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CMD5AnimCraftzmanDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditAssocMd5mesh();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	afx_msg void OnEditExportMd5anim();
	afx_msg void OnEditImportMd5anim();
	virtual void OnCloseDocument();
};


