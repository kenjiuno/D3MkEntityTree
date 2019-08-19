// MD5AnimCraftzmanDoc.cpp : implementation of the CMD5AnimCraftzmanDoc class
//

#include "stdafx.h"
#include "MD5AnimCraftzman.h"

#include "MD5AnimCraftzmanDoc.h"
#include "FM.h"
#include "ToolContext.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMD5AnimCraftzmanDoc

IMPLEMENT_DYNCREATE(CMD5AnimCraftzmanDoc, CDocument)

BEGIN_MESSAGE_MAP(CMD5AnimCraftzmanDoc, CDocument)
	ON_COMMAND(ID_EDIT_ASSOC_MD5MESH, OnEditAssocMd5mesh)
	ON_COMMAND(ID_EDIT_EXPORT_MD5ANIM, OnEditExportMd5anim)
	ON_COMMAND(ID_EDIT_IMPORT_MD5ANIM, OnEditImportMd5anim)
END_MESSAGE_MAP()


// CMD5AnimCraftzmanDoc construction/destruction

CMD5AnimCraftzmanDoc::CMD5AnimCraftzmanDoc()
{
	// TODO: add one-time construction code here

}

CMD5AnimCraftzmanDoc::~CMD5AnimCraftzmanDoc()
{
}

BOOL CMD5AnimCraftzmanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMD5AnimCraftzmanDoc serialization

void CMD5AnimCraftzmanDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CMD5AnimCraftzmanDoc diagnostics

#ifdef _DEBUG
void CMD5AnimCraftzmanDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMD5AnimCraftzmanDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMD5AnimCraftzmanDoc commands

static const DWORD nDefOpenFlags = 0
	|OFN_ENABLESIZING
	|OFN_EXPLORER
	|OFN_FILEMUSTEXIST
	|OFN_HIDEREADONLY
	|OFN_LONGNAMES
	|OFN_PATHMUSTEXIST
	;

BOOL CMD5AnimCraftzmanDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	return false;
}

BOOL CMD5AnimCraftzmanDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	AfxThrowNotSupportedException();
	return false;
}

void CMD5AnimCraftzmanDoc::DeleteContents()
{
	m_iMeet = 0;

	CDocument::DeleteContents();
}

void CMD5AnimCraftzmanDoc::OnEditExportMd5anim()
{
	AfxThrowNotSupportedException();
}

void CMD5AnimCraftzmanDoc::OnEditImportMd5anim()
{
	CFileDialog wndDlg(
		true,
		"md5anim",
		NULL,
		nDefOpenFlags,
		"md5anim (*.md5anim)|*.md5anim||",
		AfxGetMainWnd()
		);

	if (wndDlg.DoModal() != IDOK) return;

	CString strPathName = wndDlg.GetPathName();

	CWaitCursor wc;
	m_iAccessTime++;

	HRESULT hr = 0;
	try {
		m_iMeet &=~mcAnim;

		fm f;
		if (!f.Open(strPathName)) throw hr = E_FAIL;
		LPVOID pData = f.MapView();
		DWORD nData = f.GetFileSize();

		CLL1Memr m;
		m.Init((PBYTE)pData, nData);
		CLL1Leader r(m);
		NfMD5::ParseMD5Anim_t a(r, m_a10);
		if (!a.Parse()) throw hr = E_FAIL;

		m_iMeet |= mcAnim;
		f.Close();
		m_strA10 = strPathName;
		return;
	} catch (HRESULT) {
		AfxThrowArchiveException(CArchiveException::badIndex, strPathName);
	}
}

void CMD5AnimCraftzmanDoc::OnEditAssocMd5mesh()
{
	CFileDialog wndDlg(
		true,
		"md5mesh",
		NULL,
		nDefOpenFlags,
		"md5mesh (*.md5mesh)|*.md5mesh||",
		AfxGetMainWnd()
		);

	if (wndDlg.DoModal() != IDOK) return;

	CString strPathName = wndDlg.GetPathName();

	CWaitCursor wc;
	m_iAccessTime++;

	HRESULT hr = 0;
	try {
		m_iMeet &=~mcMesh;

		fm f;
		if (!f.Open(strPathName)) throw hr = E_FAIL;
		LPVOID pData = f.MapView();
		DWORD nData = f.GetFileSize();

		CLL1Memr m;
		m.Init((PBYTE)pData, nData);
		CLL1Leader r(m);
		NfMD5::ParseMD5Mesh_t a(r, m_m10);
		if (!a.Parse()) throw hr = E_FAIL;

		m_iMeet |= mcMesh;
		f.Close();
		m_strM10 = strPathName;
		return;
	} catch (HRESULT) {
		AfxThrowArchiveException(CArchiveException::badIndex, strPathName);
	}
}

void CMD5AnimCraftzmanDoc::OnCloseDocument()
{
	POSITION pos = GetFirstViewPosition();
	while (pos) {
		CView *pVw = GetNextView(pos);
		CFrameWnd *p = pVw->GetTopLevelFrame();
		CToolContextSPtr pToolCom = CMainFrame::GetToolCom(p);
		if (pToolCom != NULL) {
			pToolCom->SetDoc(this, false);
		}
	}

	__super::OnCloseDocument();
}
