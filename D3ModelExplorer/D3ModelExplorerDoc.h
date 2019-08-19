// D3ModelExplorerDoc.h : interface of the CD3ModelExplorerDoc class
//

#pragma once

#include "ToolsDlg.h"
#include "OptsDlg.h"
#include "Context.h"

typedef map<CString, CString> Material2TexNameMap;

struct MatzRiskLess {
	// 
	bool operator()(const CDoom3materialIndent &s1, const CDoom3materialIndent &s2) const
	{
		return Test(s1) < Test(s2);
	}

	// 
	static int Test(const CDoom3materialIndent &s)
	{
		if (s.soIf.IsComplexExpression())
			return 3;
		switch (s.bf1) {
		case bfDiffuseMap:
			return 0;
		case bfBumpMap:
			return 2;
		case bfZero:
			return 4;
		}
		return 1;
	}
};

typedef multimap<CDoom3materialIndent, CString, MatzRiskLess> MatzRiskMap;

class CD3ModelExplorerDoc : public CDocument
{
	// 
	CToolsDlg m_wndTools;
	// 
	COptsDlg m_wndOpts;
	// 
	CString m_strMyDir;
	// 
	CEntryMap m_vm;

	// 
	void Revert(bool fRevert);
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
	// 
	bool ReadMaterialReq(CString strModel, CStringArray &vec);
	// 
	bool ReadTexReq(CStringArray &vec, Material2TexNameMap &m);
	// 
	static CString Select1(CDoom3materialTexo &tex)
	{
		size_t x = tex.iFirst;
		while (x < tex.size()) {
			if (tex[x].tt == texFlat)
				return tex.at(x).strName;
			x = tex[x].x0;
		}
		return 0;
	}
	// 
	bool ExportTexReq(Material2TexNameMap &m);
	// 
	bool ExportModelReq(CString strName, int iTool, CString &strFileName);
	// 
	bool Commando(CString strWin, CString strCmd, CString strCurDir);
	// 
	bool Shello(CString strWin, CString strCmd, CString strCurDir);
	// 
	CString GetCurDir()
	{
		return g_strExpDir;
	}
	// 
	static bool FileExists(CString str)
	{
		DWORD x = GetFileAttributes(str);
		if (x == 0xFFFFFFFF)
			return false;
		if (x & FILE_ATTRIBUTE_DIRECTORY)
			return false;

		return true;
	}
	// 
	bool ExportTextReq2(LPCTSTR pszSrc, CString strNewTextFileName, CString strModelFileName);
	// 
	bool WriteLastWriteTime(CString strName, CString strFileName);
	// 
	static CString RemoveSuffix(CString str, TCHAR tc)
	{
		int i = str.ReverseFind(tc);
		if (i + 1 == str.GetLength() && i != -1)
			return str.Left(i);
		return str;
	}

public:
	// 
	CMyDoom3Context m_com;

	// 
	void ExecTool(int iTool, CString strModel);

protected: // create from serialization only
	CD3ModelExplorerDoc();
	DECLARE_DYNCREATE(CD3ModelExplorerDoc)

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
	virtual ~CD3ModelExplorerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAppTools();
	afx_msg void OnAppOpts();
	afx_msg void OnAppUpdate();
};


