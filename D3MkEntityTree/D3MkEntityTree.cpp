
// +--------------------------------------------------
// |
// | D3MkEntityTree.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#include "stdafx.h"
#include "D3MkEntityTree.h"
#include "ExpDlg.h"
#include "OutDeb.h"
#include "GLExtz.h"

#include "Structure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 唯一のアプリケーション オブジェクトです。

CWinApp theApp;

//CGLExtz g_gl;

using namespace std;

int _Out_Of_Memory()
{
	MessageBox(
		NULL,
		"OUT OF MEMORY",
		"D3MkEntityTree",
		0 |MB_SYSTEMMODAL
		);
	return 1;
}

int _D3MkEntityTree()
{
	ff.Open_f();

	if (0) {
		CAviMk mk;
		mk.Create("\\A.avi", 24, CSize(64, 64));
		mk.Close(true);
	}
	if (0) {
		CLL1Memr m;
		BYTE *psz = (BYTE *)"(a+b+c/d+e*f)/1";
		m.Init(psz, _mbslen(psz));
		CLL1Leader t(m);
		CSparseExpr f(t);
		t.Next();
		SymList syms;
		VERIFY(f.Parse(syms));
		printf("");
		CExprBinder e(syms);
		VERIFY(e.Parse());
		e.Printo();
		printf("");
		getchar();
		return 0;
	}
	if (0) {
		FILE *f1 = fopen("C:\\imp\\imp.md5mesh", "rb");
		static BYTE buff1[1024*1024];
		size_t r1 = fread(buff1, 1, 1024*1024, f1);
		fclose(f1);

		CLL1Memr m1;
		m1.Init(buff1, r1);
		CLL1Leader c1(m1);
		CMD5MeshDeck ff;
		CSparseMD5Mesh p1(c1, ff);
		VERIFY(p1.Parse());

		FILE *f2 = fopen("C:\\imp\\idle1.md5anim", "rb");
		static BYTE buff2[1024*1024];
		size_t r2 = fread(buff2, 1, 1024*1024, f2);
		fclose(f2);

		CLL1Memr m2;
		m2.Init(buff2, r2);
		CLL1Leader c2(m2);
		CMD5AnimDeck fa;
		CSparseMD5Anim p2(c2, fa);
		VERIFY(p2.Parse());

		printf("");
	}

	try {
		TRY {
			CExpDlg wndDlg;
			if (wndDlg.DoModal() != IDOK) return 1;
		} CATCH(CMemoryException, e) {
			return _Out_Of_Memory();
		} AND_CATCH(CResourceException, e) {
			return _Out_Of_Memory();
		} END_CATCH
	} catch (bad_alloc) {
		return _Out_Of_Memory();
	}

	return 0;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
		return 1;
	if (!AfxInitRichEdit())
		return 1;
	HRESULT hr;
	if (FAILED(hr = CoInitialize(NULL)))
		return 1;
	return _D3MkEntityTree();
}
