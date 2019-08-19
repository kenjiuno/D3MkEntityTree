// D3ModelExplorerDoc.cpp : implementation of the CD3ModelExplorerDoc class
//

#include "stdafx.h"
#include "D3ModelExplorer.h"

#include "D3ModelExplorerDoc.h"
#include "ProfStr.h"
#include "OSP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace OSP;

bool ReadContents(CString str, SizeBuff &sb)
{
	CFile fileIn;
	if (fileIn.Open(str, 0 |CFile::modeRead |CFile::shareDenyWrite)) {
		DWORD nLen = (DWORD)fileIn.GetLength();
		if (sb.Alloc(nLen +1)) {
			sb.GetData()[nLen] = 0;
			if (fileIn.Read(sb.GetData(), nLen) == nLen) {
				fileIn.Close();
				return true;
			}
		}
		fileIn.Abort();
	}
	return false;
}

bool WriteContents(CString str, LPCSTR psz)
{
	FILE *f = fopen(str, "wb");
	if (f) {
		if (fwrite(psz, strlen(psz), 1, f) == 1) {
			fclose(f);
			return true;
		}
		fclose(f);
	}
	return false;
}

bool WriteContents(CString str, SizeBuff &sb)
{
	FILE *f = fopen(str, "wb");
	if (f) {
		if (fwrite(sb.GetData(), sb.GetSize(), 1, f) == 1) {
			fclose(f);
			return true;
		}
		fclose(f);
	}
	return false;
}

// CD3ModelExplorerDoc

IMPLEMENT_DYNCREATE(CD3ModelExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CD3ModelExplorerDoc, CDocument)
	ON_COMMAND(ID_APP_TOOLS, OnAppTools)
	ON_COMMAND(ID_APP_OPTS, OnAppOpts)
	ON_COMMAND(ID_APP_UPDATE, OnAppUpdate)
END_MESSAGE_MAP()


// CD3ModelExplorerDoc construction/destruction

CD3ModelExplorerDoc::CD3ModelExplorerDoc()
{
	// TODO: add one-time construction code here

}

CD3ModelExplorerDoc::~CD3ModelExplorerDoc()
{
	Revert(false);
}

BOOL CD3ModelExplorerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_strMyDir = OSP_GetDir(OSP_GetModuleFileName(NULL));

	Revert(true);

	return TRUE;
}

void CD3ModelExplorerDoc::Revert(bool fRevert)
{
	LPCSTR pszApp = "D3ModelExplorer";
	if (fRevert) {
		ProfStr::LoadProfileStringFrom(pszApp, "Doom3", m_wndOpts.m_strDoom3, "", g_strCnf);
		//ProfStr::LoadProfileStringFrom(pszApp, "NewFolder", m_wndOpts.m_strNewFolder, "", g_strCnf);

		ProfStr::LoadProfileStringFrom(pszApp, "Tool1_exe", m_wndTools.w1.m_strExe, "", g_strCnf);

		ProfStr::LoadProfileStringFrom(pszApp, "Tool2_exe", m_wndTools.w2.m_strExe, "", g_strCnf);
		//ProfStr::LoadProfileStringFrom(pszApp, "Tool2_txt", m_wndTools.w2.m_strTxt, "", g_strCnf);

		ProfStr::LoadProfileStringFrom(pszApp, "Tool3_exe", m_wndTools.w3.m_strExe, "", g_strCnf);
		//ProfStr::LoadProfileStringFrom(pszApp, "Tool3_txt", m_wndTools.w3.m_strTxt, "", g_strCnf);
	} else {
		ProfStr::SaveProfileStringTo(pszApp, "Doom3", m_wndOpts.m_strDoom3, g_strCnf);
		//ProfStr::SaveProfileStringTo(pszApp, "NewFolder", m_wndOpts.m_strNewFolder, g_strCnf);

		ProfStr::SaveProfileStringTo(pszApp, "Tool1_exe", m_wndTools.w1.m_strExe, g_strCnf);

		ProfStr::SaveProfileStringTo(pszApp, "Tool2_exe", m_wndTools.w2.m_strExe, g_strCnf);
		//ProfStr::SaveProfileStringTo(pszApp, "Tool2_txt", m_wndTools.w2.m_strTxt, g_strCnf);

		ProfStr::SaveProfileStringTo(pszApp, "Tool3_exe", m_wndTools.w3.m_strExe, g_strCnf);
		//ProfStr::SaveProfileStringTo(pszApp, "Tool3_txt", m_wndTools.w3.m_strTxt, g_strCnf);
	}
}


// CD3ModelExplorerDoc serialization

void CD3ModelExplorerDoc::Serialize(CArchive& ar)
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


// CD3ModelExplorerDoc diagnostics

#ifdef _DEBUG
void CD3ModelExplorerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CD3ModelExplorerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CD3ModelExplorerDoc commands

void CD3ModelExplorerDoc::OnAppTools()
{
	if (m_wndTools.DoModal() != IDOK) return;

	return;
}

void CD3ModelExplorerDoc::OnAppOpts()
{
	if (m_wndOpts.DoModal() != IDOK) return;

	return;
}

void CD3ModelExplorerDoc::OnAppUpdate()
{
	CWaitCursor wc;

	m_com.Run(m_wndOpts.m_strDoom3, "", false, false, false, false, false, croParseMaterial, false, Renderer::rrtInvis, gsDoom3);

	UpdateAllViews(NULL, 1);
}

void CD3ModelExplorerDoc::ExecTool(int iTool, CString strModel)
{
	m_com.Get_EntryMap(m_vm);

	CStringArray vec;
	Material2TexNameMap m;
	CString strModelFileName;

	if (CWaitCursor(), !ReadMaterialReq(strModel, vec))
		return;
	if (CWaitCursor(), !ReadTexReq(vec, m))
		return;
	if (CWaitCursor(), !ExportTexReq(m))
		return;
	if (CWaitCursor(), !ExportModelReq(strModel, iTool, strModelFileName))
		return;

	switch (iTool) {
	case 1:
		{
			CString strScenetxt = OSP_JoinPath(OSP_GetDir(m_wndTools.w2.m_strExe, false), "scene.txt");

			if (CWaitCursor(), !ExportTextReq2("scene.txt", strScenetxt, strModelFileName))
				return;

			break;
		}
	case 2:
		{
			CString strScenetxt = OSP_JoinPath(OSP_GetDir(m_wndTools.w3.m_strExe, false), "xkscene.txt");

			if (CWaitCursor(), !ExportTextReq2("xkscene.txt", strScenetxt, strModelFileName))
				return;

			break;
		}
	}

	switch (iTool) {
	case 0:
		{
			if (!FileExists(m_wndTools.w1.m_strExe))
				break;

			CString strWin; strWin.Format(
				"%s"
				, (LPCTSTR)m_wndTools.w1.m_strExe
				);
			CString strCmd; strCmd.Format(
				" \"%s\" \"%s\""
				, (LPCTSTR)RemoveSuffix(g_strExpDir, '\\')
				, (LPCTSTR)OSP_GetBaseName(OSP_UnixToWin(strModel))
				);
			Shello(strWin, strCmd, OSP_GetDir(m_wndTools.w1.m_strExe));
			break;
		}
	case 1:
		{
			if (!FileExists(m_wndTools.w2.m_strExe))
				break;

			CString strWin; strWin.Format(
				"%s"
				, (LPCTSTR)m_wndTools.w2.m_strExe
				);
			CString strCmd; strCmd.Format(
				""
				);
			Commando(strWin, strCmd, OSP_GetDir(m_wndTools.w2.m_strExe));
			break;
		}
	case 2:
		{
			if (!FileExists(m_wndTools.w3.m_strExe))
				break;

			CString strWin; strWin.Format(
				"%s"
				, (LPCTSTR)m_wndTools.w3.m_strExe
				);
			CString strCmd; strCmd.Format(
				""
				);
			Commando(strWin, strCmd, OSP_GetDir(m_wndTools.w3.m_strExe));
			break;
		}
	}
}

bool CD3ModelExplorerDoc::ExportTextReq2(LPCTSTR pszSrc, CString strNewTextFileName, CString strModelFileName)
{
	SizeBuff sb;

	if (!ReadContents(OSP_JoinPath(m_strMyDir, pszSrc), sb))
		return false;

	SetEnvironmentVariable("D3_BASEPATH", g_strExpDir);
	SetEnvironmentVariable("D3_MD5MESH", strModelFileName);

	CHAR c[65536];
	DWORD n = ExpandEnvironmentStrings((LPCSTR)sb.GetData(), c, sizeof(c));
	if (n == 0)
		return false;

	if (!WriteContents(strNewTextFileName, c))
		return false;

	return true;
}

bool CD3ModelExplorerDoc::Shello(CString strWin, CString strCmd, CString strCurDir)
{
	HINSTANCE h = ShellExecute(GetActiveWindow(), NULL, strWin, strCmd, strCurDir, SW_SHOW);
	if ((INT_PTR)h <= 32)
		return false;
	return true;
}

bool CD3ModelExplorerDoc::Commando(CString strWin, CString strCmd, CString strCurDir)
{
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	BOOL f = CreateProcess(
		strWin,
		const_cast<LPTSTR>((LPCTSTR)strCmd),
		NULL,
		NULL,
		false,
		0,
		NULL,
		strCurDir,
		&si,
		&pi
		);
	if (!f)
		return false;

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return true;
}

bool MkDirs(CString strDir)
{
	int i = 0;
	for (; ; ) {
		int p = strDir.Find('\\', i);
		if (p < 0)
			break;
		if (p > 2) {
			CString strName(strDir, p);

			if (!CreateDirectory(strName, NULL)) {
				DWORD nErr = GetLastError();
				if (nErr != ERROR_ALREADY_EXISTS) // 183
					return false;
				DWORD x = GetFileAttributes(strName);
				if (!(x & FILE_ATTRIBUTE_DIRECTORY))
					return false;
				printf("");
			}
		}

		i = p + 1;
	}

	return true;
}

bool CD3ModelExplorerDoc::ExportModelReq(CString strName, int iTool, CString &strFileName)
{
	CDoom3ControlContext &cc = m_com;

	strName = OSP_UnixToWin(strName);

	SizeBuff sb;
	if (!cc.LoadEntry(strName, sb))
		return false;
	switch (iTool) {
	case 0:
		{
			strFileName = OSP_JoinPath(g_strExpDir, "models\\md5\\monsters\\" + OSP_GetFileName(strName));
			break;
		}
	case 1:
	case 2:
		{
			strFileName = OSP_JoinPath(g_strExpDir, strName);
			break;
		}
	default:
		{
			return false;
		}
	}
	if (!MkDirs(OSP_GetDir(strFileName)))
		return false;
	if (!WriteContents(strFileName, sb))
		return false;

	WriteLastWriteTime(strName, strFileName);

	return true;
}

bool CD3ModelExplorerDoc::WriteLastWriteTime(CString strName, CString strFileName)
{
	strName.Replace('\\', '/');

	CEntryMap::iterator
		iterPos = m_vm.find(strName),
		iterEnd = m_vm.end();
	if (iterPos == iterEnd)
		return false;
	FILETIME ft;
	if (!iterPos->second->GetModifiedTime(ft))
		return false;
	HANDLE f = CreateFile(strFileName, 0 |GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (f != INVALID_HANDLE_VALUE) {
		if (SetFileTime(f, NULL, NULL, &ft)) {
			CloseHandle(f);
			return true;
		}
		CloseHandle(f);
	}
	return false;
}

// C:\\ABC\\models\\

bool CD3ModelExplorerDoc::ExportTexReq(Material2TexNameMap &m)
{
	CDoom3ControlContext &cc = m_com;

	Material2TexNameMap::iterator
		iterPos = m.begin(),
		iterEnd = m.end();
	for (; iterPos != iterEnd; iterPos++) {
		CString strName = OSP::OSP_RemoveExt(iterPos->second) + ".tga";
		SizeBuff sb;
		if (!cc.LoadEntry(strName, sb))
			continue;
		CString strTex = iterPos->first;
		CString strNewTex = OSP::OSP_JoinPath(g_strExpDir, OSP::OSP_UnixToWin(strTex)) + ".tga";
		if (!MkDirs(OSP::OSP_GetDir(strNewTex)))
			continue;
		if (!WriteContents(strNewTex, sb))
			continue;
		WriteLastWriteTime(strName, strNewTex);
		printf("");
	}

	return true;
}

bool CD3ModelExplorerDoc::ReadTexReq(CStringArray &vec, Material2TexNameMap &m)
{
	m.clear();

	INT_PTR i;
	for (i = 0; i < vec.GetSize(); i++) {
		CString strName = vec[i];
		if (strName.IsEmpty())
			continue;
		CDoom3materialMap::iterator
			iterPos = m_com.Get_wk().materialMap.find(strName),
			iterEnd = m_com.Get_wk().materialMap.end();
		if (iterPos == iterEnd)
			continue;
		CDoom3materialIndentArray &a = iterPos->second.v;
		size_t x;
		MatzRiskMap rm;
		for (x = 0; x < a.size(); x++) {
			rm.insert(make_pair(a[x], Select1(a[x].tex)));
		}
		if (rm.empty())
			continue;
		CString strTex = rm.begin()->second;

		m.insert(make_pair(strName, strTex));
	}

	return true;
}

namespace
{

};

bool CD3ModelExplorerDoc::ReadMaterialReq(CString strModel, CStringArray &vec)
{
	SizeBuff sb;
	CDoom3ControlContext &cc = m_com;
	if (!cc.LoadEntry(strModel, sb))
		return false;
	CLL1Memr m;
	m.Init(sb.GetData(), sb.GetSize());
	CLL1Leader s(m);

	if (false);
	else if (IsExtNameIs(strModel, "md5mesh")) {
		CMD5MeshDeck o;
		CSparseMD5Mesh a(s, o);
		if (!a.Parse())
			return false;
		size_t i;
		for (i = 0; i < o.meshes.size(); i++) {
			CString str = o.meshes[i].m["shader"];
			vec.Add(str);
		}
	}
	else if (IsExtNameIs(strModel, "ase")) {
		CAseMeshDeck o;
		CSparseASE a(s, o);
		if (!a.Parse())
			return false;
		size_t i;
		for (i = 0; i < o.mma.size(); i++) {
			CString str = CDoom3Context::AseTex2Material(o.mma[i].strTex);
			vec.Add(str);
		}
	}
	else {
		return false;
	}

	return true;
}
