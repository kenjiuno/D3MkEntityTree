
// +--------------------------------------------------
// |
// | Context.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#include "StdAfx.h"
#include "Context.h"
#include "resource.h"
#include "OSP.h"
#include <float.h>
#include <math.h>
#include "OutDeb.h"
#include "Sol.h"

#define TEST_FLAG_ALL(V,T) (((V) & (T)) == (T))
#define TEST_FLAG_ANY(V,T) (((V) & (T)) != (0))

//#define _DEBUG_1
//#define _DEBUG_PLANE

// "rotation" "m00 m01 m02 m10 m11 m12 m20 m21 m22" (DirectX/2D-Array type)

#pragma comment(lib, "vfw32.lib")

#define HTML_HEADER_1 \
	"<?xml version=\"1.0\" encoding=\"windows-1250\"?>\n" \
	"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n" \
	"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n" \
	"<head>\n" \
	"<meta name=\"GENERATOR\" content=\"D3MkEntityTree\" />\n" \
	"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1250\" />\n" \
	"</head>\n" \
	"<body>\n" \
	""

using namespace OSP;

// ->
//  ->
//   ->
//    ->
// ?
//      ->
//       ->
//        ->
//         ->

CString _DirToPlain(CString strPath)
{
	strPath.Replace('/', '_');
	return strPath;
}

CString _DirToPlain2(CString strPath)
{
	strPath.Replace('/', '_');
	strPath.Replace('.', '_');
	return strPath;
}

// ->
//  ->
//   ->
//    ->
// CDoom3Context
//      ->
//       ->
//        ->
//         ->

bool CDoom3Context::Scan()
{
	m.clear();

	typedef set<CString, CStrCaseInsensitiveLess_t> Pk4Map;
	Pk4Map pk4s;

	CFileFind ff;
	BOOL f = ff.FindFile(OSP_JoinPath(strDoom3Base, "*.pk4"));
	while (f) {
		f = ff.FindNextFile();
		if (ff.IsDirectory()) continue;
		pk4s.insert(ff.GetFilePath());
	};

	printf("Heading to pk4 files\n");

	Pk4Map::iterator
		iterPos = pk4s.begin(),
		iterEnd = pk4s.end();
	for (; iterPos != iterEnd; iterPos++) {
		TRACE1("Uz: %s\n", (LPCTSTR)*iterPos);
		CUzzPtr pUz = CUzzPtr(new Uzz::CUzz(), false);
		printf("Heading to '%s'\n", (LPCSTR)*iterPos);
		if (!pUz->Open(*iterPos))
			continue;
		if (!pUz->Mk())
			continue;
		Uzz::CUzzFInfoList::iterator
			iterEPos = pUz->z.begin(),
			iterEEnd = pUz->z.end();
		for (; iterEPos != iterEEnd; iterEPos++) {
			const Uzz::CUzzFInfo &rfi = *iterEPos;
			if (rfi.fIsDir)
				continue;
			CString strLeft = rfi.strPathName;
			strLeft.MakeLower();
			strLeft.Replace('\\', '/');

			FILETIME ft;
			SYSTEMTIME st;
			rfi.mtime.GetAsSystemTime(st);
			SystemTimeToFileTime(&st, &ft);

			CUzzExtractor ze;
			ze.pUz = pUz;
			ze.iEntry = rfi.num_of_file;
			m[strLeft] = CFileEntryPtr(new CZipFileEntity(ze, ft), false);
		}
	}

	printf("# of entries is %d\n", (int)m.size());
	printf("Heading to filesystem files\n");

	if (!ScanDir(strDoom3Base, ""))
		return false;

	printf("# of entries is %d\n", (int)m.size());
	printf("Heading ok\n\n");

	return true;
}

void CDoom3Context::Run()
{
	CWaitCursor wc;

	CProcessPriorityEnforcer low;

	if (fLowProcessPriority) low.Enforce(IDLE_PRIORITY_CLASS);

	switch (wk.gs) {
		case gsQuake4:
			strDoom3Base = OSP_JoinPath(strDoom3, "q4base"); break;
		case gsDoom3:
		default:
			strDoom3Base = OSP_JoinPath(strDoom3, "base"); break;
	};
	strNewFolder_entityDef = OSP_JoinPath(strNewFolder, "entityDef");
	strNewFolder_Mapobjs = OSP_JoinPath(strNewFolder, "Mapobjs");
	strNewFolder_Entitymodel = OSP_JoinPath(strNewFolder, "Entitymodel");

	Close();

	do {
		if (!Scan())
			return;
#ifdef _DEBUG
//		if (!ReadTest())
//			return;
#endif
		if (TEST_FLAG_ALL(cro, croParseMaterial) && !ReadMaterial())
			return;
		if (TEST_FLAG_ALL(cro, croParseMaterial) && !AssocMapobjs())
			return;
#ifdef _DEBUG
//		if (!ReadTest())
//			return;
#endif
//		if (!ReadSkin())
//			return;
		if (TEST_FLAG_ALL(cro, croParseDef) && !ReadDef())
			return;
		if (TEST_FLAG_ALL(cro, croParseDef) && !AssocDef())
			return;
#ifdef _CONTEXT_EXPORT_FEATURE
		if (TEST_FLAG_ALL(cro, croMake_entityDef) && !MkHtml())
			return;
		if (TEST_FLAG_ALL(cro, croMake_Mapobjects) && !MkMo())
			return;
		if (TEST_FLAG_ALL(cro, croMake_Mapentity) && !MkEM())
			return;
		if (TEST_FLAG_ALL(cro, croMake_hhp) && !MkHHP())
			return;
#endif
		break;

	} while(false);
}

bool CDoom3Context::LoadEntry(CString strPath, SizeBuff &sb)
{
	strPath.Replace('\\', '/');
	strPath.MakeLower();

	CEntryMap::iterator
		iterPos = m.find(strPath),
		iterEnd = m.end();
	if (iterPos != iterEnd) {
		if (iterPos->second->GetData(sb))
			return true;
	}
	return false;
}

bool CDoom3Context::LoadFileEntry(CString strPath, SizeBuff &sb)
{
	CFile fileIn;
	if (!fileIn.Open(strPath, 0 |CFile::modeRead |CFile::shareDenyWrite))
		return false;
	ULONGLONG nLen64 = fileIn.GetLength();
	int nLen = (int)nLen64;
	if (nLen != nLen64 || nLen > 1024*1024*10)
		return false;
	if (!sb.Alloc(nLen))
		return false;
	if (fileIn.Read(sb.GetData(), nLen) != nLen)
		return false;
	return true;
}

bool CDoom3Context::ScanDir(CString strDir, CString strPrefix)
{
	CFileFind ff;
	BOOL f = ff.FindFile(OSP_JoinPath(strDir, "*"));
	while (f) {
		f = ff.FindNextFile();

		CString strName = ff.GetFileName();
		strName.MakeLower();

		if (ff.IsDots()) {

		} else if (ff.IsDirectory()) {
			if (!ScanDir(ff.GetFilePath(), strPrefix + "/" + strName))
				return false;
		} else if (IsAssoc(strName)) {
			CString strPath = NormPath(strPrefix + "/" + strName);

			m[strPath] = CFileEntryPtr(new COSFileEntity(ff.GetFilePath(), strPath), false);
		}
	}

	return true;
}

bool CDoom3Context::IsAssoc(CString strName)
{
	LPCTSTR psz = _tcsrchr(strName, '.');
	if (psz == NULL) return false;
	psz++;
	if (false
		|| _tcscmp(psz, "mtr") == 0
		|| _tcscmp(psz, "skin") == 0
		|| _tcscmp(psz, "sndshd") == 0
		|| _tcscmp(psz, "def") == 0
		|| _tcscmp(psz, "fx") == 0
		|| _tcscmp(psz, "prt") == 0
		|| _tcscmp(psz, "af") == 0
		|| _tcscmp(psz, "pda") == 0

		|| _tcscmp(psz, "script") == 0
		|| _tcscmp(psz, "tga") == 0
		|| _tcscmp(psz, "md5mesh") == 0
		|| _tcscmp(psz, "md5anim") == 0
		|| _tcscmp(psz, "lwo") == 0
		|| _tcscmp(psz, "ase") == 0
		|| _tcscmp(psz, "map") == 0
	) {
		return true;
	}
	return false;
}

bool CDoom3Context::FindAll(CString strExtName, CEntryMap &m2)
{
	m2.clear();

	CEntryMap::iterator
		iterPos = m.begin(),
		iterEnd = m.end();
	for (; iterPos != iterEnd; iterPos++) {
		if (IsExtNameIs(iterPos->first, strExtName)) {
			m2.insert(*iterPos);
		}
	}

	return true;
}

bool CDoom3Context::ReadDef()
{
	printf("Parsing from def files\n");

	CEntryMap m2;
	if (!FindAll("def", m2))
		return false;
	CEntryMap::iterator
		iterPos = m2.begin(),
		iterEnd = m2.end();
	for (; iterPos != iterEnd; iterPos++) {
		TRACE1("ReadDef: %s\n", (LPCTSTR)iterPos->first);

		printf("Parsing from '%s'\n", (LPCSTR)iterPos->first);

		SizeBuff sbDef;
		if (iterPos->second->GetData(sbDef)) {
			CLL1Memr m;
			m.Init(sbDef.GetData(), sbDef.GetSize());
			CLL1Leader c(m);
			CSparseDef a(c, wk);
			if (a.Parse()) {
				printf("ok\n");
			} else {
				printf("ERR\n");
			}
		}
	}

	printf("Parsing ok\n\n");
	return true;
}

bool CDoom3Context::AssocDef()
{
	{
		CDoom3entityDefMap::iterator
			iterPos = wk.entityDefMap.begin(),
			iterEnd = wk.entityDefMap.end();
		for (; iterPos != iterEnd; iterPos++) {
			CString strName = iterPos->first;
			CDoom3entityDef &entityDef = iterPos->second;
			CString strInherit;
			if (Sol::GetValue(entityDef.m, "inherit", strInherit)) {
				wk.inheritMap[strName] = strInherit;
				wk.rinheritMap.insert(make_pair(strInherit, strName));
			} else {
				//TRACE1("No-parent: %s\n", (LPCTSTR)strName);
			}
		}
	}

	{
		CDoom3modelMap::iterator
			iterPos = wk.modelMap.begin(),
			iterEnd = wk.modelMap.end();
		for (; iterPos != iterEnd; iterPos++) {
			CString strName = iterPos->first;
			CDoom3model &model = iterPos->second;
			CString strInherit;
			if (Sol::GetValue(model.m, "inherit", strInherit)) {
				wk.inheritModelMap[strName] = strInherit;
			} else {
				//TRACE1("No-parent: %s\n", (LPCTSTR)strName);
			}
		}
	}

	return true;
}

bool CDoom3Context::MkHtml()
{
	ff.Open_mtr();
	mkth2.Reset();

	printf("Making htmls (entityDef)\n");

#ifdef _DEBUG_1
	{
		CDoom3entityDefMap m;
		m["marscity_cinematic_player"] = wk.entityDefMap["marscity_cinematic_player"];
		m["npc_base"] = wk.entityDefMap["npc_base"];
		m["character_default"] = wk.entityDefMap["character_default"];

		wk.entityDefMap = m;
	}
#endif

	VERIFY(wndRenderer.Create());

	int nCount = 0, nDone = 0;

	nCount++, nDone += Sol::MkDir(strNewFolder_entityDef) ? 1 : 0;
	nCount++, nDone += Sol::MkDir(OSP_JoinPath(strNewFolder_entityDef, "model_pv")) ? 1 : 0;
	nCount++, nDone += Sol::MkDir(OSP_JoinPath(strNewFolder_entityDef, "model_thumb")) ? 1 : 0;
	nCount++, nDone += Sol::MkDir(OSP_JoinPath(strNewFolder_entityDef, "model_anim")) ? 1 : 0;
	nCount++, nDone += Sol::ExportResource(MAKEINTRESOURCE(IDR_DATA_NO_PREVIEW), _T("DATA"), OSP_JoinPath(strNewFolder_entityDef, "No_preview.gif")) ? 1 : 0;
	nCount++, nDone += Sol::ExportResource(MAKEINTRESOURCE(getHwsMan().getindexres()), RT_HTML, OSP_JoinPath(strNewFolder, "index.html")) ? 1 : 0;
	if (nCount == nDone) {
		FILE *f = fopen(OSP_JoinPath(strNewFolder_entityDef, "entity_list.html"), "wt");
		FILE *f2 = fopen(OSP_JoinPath(strNewFolder_entityDef, "entity_thumb.html"), "wt");
		if (f && f2) {
			mkth2.f = f2;
			mkth2.MkHead(true);
			mkth2.MkTableHead(true);

			nCount++, nDone += MkHtml_entityDef_class(MkHref_entityDef("class")) ? 1 : 0;
			//
			fprintf(f,
				HTML_HEADER_1
				"<p>\n"
				);

			size_t iPos = 0, nMax = wk.entityDefMap.size();

			CDoom3entityDefMap::iterator
				iterPos = wk.entityDefMap.begin(),
				iterEnd = wk.entityDefMap.end();
			for (; iterPos != iterEnd; iterPos++, iPos++) {
				CDoom3entityDef &entityDef = iterPos->second;

				if (!IsEntityDefExportOk(iterPos->first)) {
					fprintf(f,
						"%s<br />\n"
						, (LPCSTR)entityDef.strName
						);

					printf("Writing '%s' \n"
						, (LPCSTR)entityDef.strName
						, (UINT)iPos, (UINT)nMax
						);
				} else {
					CString strHref = MkHref_entityDef(entityDef.strName);

					fprintf(f,
						"<a href=\"%s\">%s</a><br />\n"
						, (LPCSTR)strHref
						, (LPCSTR)entityDef.strName
						);

					printf("Writing '%s' (%u/%u)\n"
						, (LPCSTR)entityDef.strName
						, (UINT)iPos, (UINT)nMax
						);

					nCount++, nDone += MkHtml_entityDef(entityDef, strHref) ? 1 : 0;
				}
			}

			fprintf(f,
				"</p>\n"
				"</body>\n"
				"</html>\n"
				);
			//
			mkth2.MkTableHead(false);
			mkth2.MkHead(false);
		}
		if (f) fclose(f);
		if (f2) fclose(f2);
	}

	if (nCount == nDone)
		printf("Making ok\n\n");
	else if (nDone != 0)
		printf("Making partially successful\n\n");
	else
		printf("Making ERR\n\n");

	wndRenderer.Close();
	return true;
}

bool CDoom3Context::MkMo()
{
	ff.Open_f(true);
	ff.Open_mtr(true, true);
	mkth2.Reset();

	printf("Making htmls (Mapobjects)\n");

	VERIFY(wndRenderer.Create());

	int nCount = 0, nDone = 0;

	nCount++, nDone += Sol::MkDir(strNewFolder_Mapobjs) ? 1 : 0;
	nCount++, nDone += Sol::MkDir(OSP_JoinPath(strNewFolder_Mapobjs, "model_pv")) ? 1 : 0;
	nCount++, nDone += Sol::MkDir(OSP_JoinPath(strNewFolder_Mapobjs, "model_thumb")) ? 1 : 0;
	nCount++, nDone += Sol::ExportResource(MAKEINTRESOURCE(IDR_DATA_NO_PREVIEW), _T("DATA"), OSP_JoinPath(strNewFolder_Mapobjs, "No_preview.gif")) ? 1 : 0;
	nCount++, nDone += Sol::ExportResource(MAKEINTRESOURCE(IDR_T0), _T("DATA"), OSP_JoinPath(strNewFolder_Mapobjs, "_0.gif")) ? 1 : 0;
	nCount++, nDone += Sol::ExportResource(MAKEINTRESOURCE(IDR_T3), _T("DATA"), OSP_JoinPath(strNewFolder_Mapobjs, "_3.gif")) ? 1 : 0;
	nCount++, nDone += Sol::ExportResource(MAKEINTRESOURCE(IDR_T5), _T("DATA"), OSP_JoinPath(strNewFolder_Mapobjs, "_5.gif")) ? 1 : 0;
	nCount++, nDone += Sol::ExportResource(MAKEINTRESOURCE(IDR_T7), _T("DATA"), OSP_JoinPath(strNewFolder_Mapobjs, "_7.gif")) ? 1 : 0;
	nCount++, nDone += Sol::ExportResource(MAKEINTRESOURCE(IDR_HTML_INDEX), RT_HTML, OSP_JoinPath(strNewFolder, "index.html")) ? 1 : 0;
	if (nCount == nDone) {
		FILE *f = fopen(OSP_JoinPath(strNewFolder_Mapobjs, "tree.html"), "wt");
		FILE *f2 = fopen(OSP_JoinPath(strNewFolder_Mapobjs, "thumb.html"), "wt");
		if (f && f2) {
			mkth2.f = f2;
			mkth2.MkHead(true);
			mkth2.MkTableHead(true);
			//
			fprintf(f,
				HTML_HEADER_1
				"<p>\n"
				);
			fprintf(f,
				" models<br />\n"
				);

			iMapobjCur = 1;
			nMapobjMax = wk.mapobjsNames.size();

			nCount++, nDone += MkMo_tree(f, "models/", vector<bool>()) ? 1 : 0;

			fprintf(f,
				"</p>\n"
				"</body>\n"
				"</html>\n"
				);
			//
			mkth2.MkTableHead(false);
			mkth2.MkHead(false);
		}
		if (f) fclose(f);
		if (f2) fclose(f2);
	}

	if (nCount == nDone)
		printf("Making ok\n\n");
	else if (nDone != 0)
		printf("Making partially successful\n\n");
	else
		printf("Making ERR\n\n");

	wndRenderer.Close();
	return true;
}

bool CDoom3Context::MkHtml_entityDef(CDoom3entityDef &entityDef, CString strHref)
{
	FILE *f = fopen(OSP_JoinPath(strNewFolder_entityDef, OSP_UnixToWin(strHref)), "wt");
	if (f) {
		//
		fprintf(f,
			HTML_HEADER_1
			);

		fprintf(f, "<h1><nobr>");
		list<CString> classes;
		entityDef_Classify(entityDef.strName, classes);
		fprintf(f, "<a href=\"%s\">/</a> ", 
			(LPCSTR)MkHref_entityDef("class")
			);
		while (!classes.empty()) {
			size_t n = classes.size();
			CString strName = classes.front();
			if (IsEntityDefExportOk(strName)) {
				fprintf(f, "<a href=\"%s\">%s</a>"
					, (LPCSTR)MkHref_entityDef(strName)
					, (LPCSTR)strName
					);
			} else {
				fprintf(f, "%s"
					, (LPCSTR)strName
					);
			}
			if (n != 1) fprintf(f, "/");
			classes.pop_front();
		}
		fprintf(f, "</nobr></h1>\n");

		// ->
		// Preview
		// ->

		{
			fprintf(ff.fmtr, "entityDef(%s)\n", (LPCSTR)entityDef.strName); fflush(ff.fmtr);

			CString strModel;
			if (Sol::GetValue(entityDef.m, "model", strModel)) {
				CString strModelNoext = OSP_RemoveExt(strModel);
				CString strModelName = OSP_GetFileName(OSP_UnixToWin(_DirToPlain(strModel)));
				CString strHrefPv = MkHref_previewModel(strModelName);
				if (ExportModel(strModel, &entityDef, strHrefPv) || ExportMapobj(strModel, strHrefPv, false)) {

				} else {
					strHrefPv = "No_preview.gif";
				}
				fprintf(f,
					"<h2>Preview</h2>\n"
					"<p>\n"
					"<img src=\"%s\" />\n"
					"</p>\n"
					, (LPCSTR)strHrefPv
					);

				if (strHrefPv != "No_preview.gif") {
					CString strHrefTh = MkHref_thumbModel(strModelName);
					if (ExportModel(strModel, &entityDef, strHrefTh, true) || ExportMapobj(strModel, strHrefTh, false, true)) {
						mkth2.AddThumb(entityDef.strName, strHrefTh, strHref);
					}

					if (fMakeAvi) {
						list<CString> anims;
						if (GetAnimNames(strModel, anims)) {
							fprintf(f, 
								"<h2>Animations</h2>\n"
								"<p>\n"
								);
							//
							while (!anims.empty()) {
								CString strAnim = anims.front(); anims.pop_front();
								CString strHrefAvi = MkHref_animModel(strModel, strAnim);
								fprintf(ff.f, " anim(%s)\n", (LPCSTR)strAnim);
								if (ExportAnim(strModel, &entityDef, strAnim, strHrefAvi)) {
									fprintf(f,
										"<a href=\"%s\">%s</a><br />\n"
										, (LPCSTR)strHrefAvi
										, (LPCSTR)strAnim
										);
								} else {

								}
							}
							//
							fprintf(f, "</p>\n");
						}
					}
				}
				wndRenderer.UnprecacheShader();
			}
		}

		// ->
		// Inheritances
		// ->
		{
			multimap<CString, CString>::iterator
				iterPos = wk.rinheritMap.lower_bound(entityDef.strName),
				iterEnd = wk.rinheritMap.upper_bound(entityDef.strName);

			if (iterPos != iterEnd) {
				fprintf(f,
					"<h2>Inheritances</h2>\n"
					);
				fprintf(f, "<p>\n");
				for (; iterPos != iterEnd; iterPos++) {
					CString strName = iterPos->second;
					if (IsEntityDefExportOk(strName)) {
						fprintf(f, "<a href=\"%s\">%s</a><br />\n"
							, (LPCSTR)MkHref_entityDef(strName)
							, (LPCSTR)strName
							);
					} else {
						fprintf(f, "%s<br />\n"
							, (LPCSTR)strName
							);
					}
				}
				fprintf(f, "</p>\n");
			}
		}

		// ->
		// Usage
		// ->
		{
			CString str;
			if (Sol::GetValue(entityDef.m, "editor_usage", str)) {
				str = HtmlEnc(str);
				str.Replace(".  ", ".<br />");
				fprintf(f,
					"<h2>Usage</h2>\n"
					);
				fprintf(f, 
					"<p>\n"
					"%s\n"
					"</p>\n"
					, (LPCSTR)str
					);
			}
		}

		// ->
		// Variables
		// ->
		{
			list<CString>::iterator
				iterPos = entityDef.o.begin(),
				iterEnd = entityDef.o.end();
			list<CString> vars;
			for (; iterPos != iterEnd; iterPos++) {
				CString strKey = *iterPos;
				CString strVType, strVName;
				if (IsEntityVar(strKey, strVType, strVName)) {
					vars.push_back(strKey);
				}
			}

			{
				list<CString>::iterator
					iterPos = vars.begin(),
					iterEnd = vars.end();
				if (iterPos != iterEnd) {
					fprintf(f,
						"<h2>Variables</h2>\n"
						);
					fprintf(f,
						"<p>\n"
						"<tt>{</tt><br />\n"
						);
					for (; iterPos != iterEnd; iterPos++) {
						CString strKey = *iterPos;

						CString strVType, strVName, strVDef;
						if (IsEntityVar(strKey, strVType, strVName)) {
							Sol::GetValue(entityDef.m, strVName, strVDef);
							if (strVDef.GetLength() != 0) {
								strVDef = " = '" + HtmlEnc(strVDef) + "'";
							}

							fprintf(f, 
								"<tt><b>%s</b> <a href=\"#%s\">%s</a>%s</tt><br />\n"
								, (LPCSTR)strVType
								, (LPCSTR)strVName
								, (LPCSTR)strVName
								, (LPCSTR)strVDef
								);

							CString strKw; strKw.Format("%s (%s)", (LPCTSTR)strVName, (LPCTSTR)entityDef.strName);
							CString strKwHref; strKwHref.Format("entityDef/%s#%s", (LPCTSTR)strHref, (LPCTSTR)strVName);

							hhkTempl.AddKw(strKw, strKwHref);
						}
					}
					fprintf(f,
						"<tt>}</tt><br />\n"
						"</p>\n"
						);

					iterPos = vars.begin();

					for (; iterPos != iterEnd; iterPos++) {
						CString strKey = *iterPos;
						CString strValue = entityDef.m[strKey];

						CString strVType, strVName, strVDef;
						if (IsEntityVar(strKey, strVType, strVName)) {
							Sol::GetValue(entityDef.m, strVName, strVDef);
							if (strVDef.GetLength() != 0) {
								strVDef = " = '" + HtmlEnc(strVDef) + "'";
							}
							strValue = HtmlEnc(strValue);
							strValue.Replace(".  ", ".<br />");
							fprintf(f,
								"<p>\n"
								"<tt><b>%s</b> <a name=\"%s\">%s</a>%s</tt><br />\n"
								"<ul>%s</ul>\n"
								"</p>\n"
								, (LPCSTR)strVType, (LPCSTR)strVName, (LPCSTR)strVName, (LPCSTR)strVDef
								, (LPCSTR)strValue
								);
						}
					}
				}
			}
		}

		fprintf(f,
			"</body>\n"
			"</html>\n"
			);
		//
		fclose(f);

		{
			CString strKw; strKw.Format("%s", (LPCTSTR)entityDef.strName);
			CString strKwHref; strKwHref.Format("entityDef/%s", (LPCTSTR)strHref);

			hhkTempl.AddKw(strKw, strKwHref);

			hhcTempl.Add_entityDef(entityDef.strName, strKwHref);

			//hhpTempl.AddHtml(strKwHref, true);
		}
	}

	return true;
}

void CDoom3Context::entityDef_Classify(CString strName, list<CString> &classes)
{
	classes.push_front(strName);
	while (true) {
		map<CString, CString>::iterator
			iterPos = wk.inheritMap.find(strName),
			iterEnd = wk.inheritMap.end();
		if (iterPos == iterEnd)
			break;
		strName = iterPos->second;
		classes.push_front(strName);
	}
}

bool CDoom3Context::MkHtml_entityDef_class(CString strHref)
{
	FILE *f = fopen(OSP_JoinPath(strNewFolder_entityDef, OSP_UnixToWin(strHref)), "wt");
	if (f) {
		//
		fprintf(f,
			HTML_HEADER_1
			);

		fprintf(f, "<h1>");
		fprintf(f, "<a href=\"%s\">/</a>", 
			(LPCSTR)MkHref_entityDef("class")
			);
		fprintf(f, "</h1>\n");

		// ->
		// Inheritances
		// ->
		{
			CDoom3entityDefMap::iterator
				iterPos = wk.entityDefMap.begin(),
				iterEnd = wk.entityDefMap.end();

			{
				fprintf(f,
					"<h2>Inheritances</h2>\n"
					);
				fprintf(f, "<p>\n");
				for (; iterPos != iterEnd; iterPos++) {
					CString strName = iterPos->first;
					if (IsEntityDefExportOk(strName)) {
						if (wk.inheritMap.find(strName) != wk.inheritMap.end())
							continue;
						fprintf(f, "<a href=\"%s\">%s</a><br />\n"
							, (LPCSTR)MkHref_entityDef(strName)
							, (LPCSTR)strName
							);
					} else {
						fprintf(f, "%s<br />\n"
							, (LPCSTR)strName
							);
					}
				}
				fprintf(f, "</p>\n");
			}
		}

		fprintf(f,
			"</body>\n"
			"</html>\n"
			);
		//
		fclose(f);
	}

	return true;
}

bool CDoom3Context::ExportModel(CString strModel, CDoom3entityDef *entityDef, CString strHref, bool fSmaller)
{
	if (!wndRenderer.IsRendererAlive())
		return false;
	CDoom3modelMap::iterator
		iterPos = wk.modelMap.find(strModel),
		iterEnd = wk.modelMap.end();
	if (iterPos == iterEnd)
		return false;
	CDoom3model model = iterPos->second;
	wk.CompleteInherit(model);
	CMD5MeshDeck m10mesh;
	{
		CString strMesh;
		if (!Sol::GetValue(model.m, "mesh", strMesh))
			return false;
		if (!LoadMD5mesh(strMesh, m10mesh))
			return false;
	}
	bool fRenderHead = false;
	CMD5MeshDeck m10meshHead;
	CString strJoint;
	if (entityDef != NULL) {
		CString strMesh;
		if (Sol::GetValue(entityDef->m, "def_head", strMesh) && Sol::GetValue(entityDef->m, "head_joint", strJoint)) {
			CString strMeshf;
			if (GetModelMesh(strMesh, strMeshf)) {
				{
					UINT i;
					for (i = 0; i < m10mesh.joints.size(); i++) {
						CMD5MeshJoint &j0 = m10mesh.joints.at(i);
						if (j0.strName != strJoint) continue;
						m10meshHead.basejointAvail = true;
						m10meshHead.basejoint = j0.ok;
						break;
					}
				}
				if (!LoadMD5mesh(strMeshf, m10meshHead))
					return false;
				fRenderHead = true;
			}
		}
	}

	size_t iMesh, nMeshes;

	CTformedMesh tfm2[2];
	CTformedMeshTab tfmt2[2];

	TformedMeshTabPtrArray tfmtVec;

	My3DMath::BBox3 b; b.Empty();

	{
		wndRenderer.tfmtVec.clear();

		for (int iModel = 0; iModel < 2; iModel++) {
			CMD5MeshDeck *pMesh = (iModel == 0) ? &m10mesh : &m10meshHead;

			if (!pMesh) continue;

			CTformedMeshTab &tfmt = tfmt2[iModel];
			tfmt.p = &tfm2[iModel];
			CTformedMesh &m = *tfmt.p;

			tfmtVec.push_back(&tfmt);

			wndRenderer.pwk = &wk;
			wndRenderer.pcc = this;
			wndRenderer.tfmtVec.push_back(&tfmt);

			nMeshes = pMesh->meshes.size();

			UINT nMaxTris = 0;
			UINT nMaxVerts = 0;

			CString strShader;

			for (iMesh = 0; iMesh < nMeshes; iMesh++) {
				CMD5MeshMesh &mm = pMesh->meshes[iMesh];
				nMaxTris += mm.tris.size();
				nMaxVerts += mm.verts.size();

				Sol::GetValue(mm.m, "shader", strShader);

				tfmt.strTexs.Add(strShader);
			}

			if (!m.Malloc(nMaxTris, nMaxVerts))
				return false;

			UINT iCurTri = 0;
			UINT iCurVert = 0;

			for (iMesh = 0; iMesh < nMeshes; iMesh++) {
				CMD5MeshMesh &mm = pMesh->meshes[iMesh];
				for (size_t iTri = 0; iTri < mm.tris.size(); iTri++) {
					m.pIndex[3*iCurTri +3*iTri +0] = iCurVert +mm.tris.at(iTri).v[0];
					m.pIndex[3*iCurTri +3*iTri +1] = iCurVert +mm.tris.at(iTri).v[1];
					m.pIndex[3*iCurTri +3*iTri +2] = iCurVert +mm.tris.at(iTri).v[2];
					m.pTexIndex[iCurTri +iTri] = iMesh;
				}
				for (size_t iVert = 0; iVert < mm.verts.size(); iVert++) {
					m.pTexCoord0[2*iCurVert +2*iVert +0] =       mm.verts.at(iVert).o.x;
					m.pTexCoord0[2*iCurVert +2*iVert +1] =       mm.verts.at(iVert).o.y;
					My3DMath::Vtx3 v3;
					mm.InterpolateVertex(pMesh->joints, 0U +iVert, v3);
					memcpy(m.pVertex +3*iCurVert +3*iVert, v3.v, 4 * 3);
					b.Add(v3);
				}

				iCurTri += mm.tris.size();
				iCurVert += mm.verts.size();
			}
		}
	}
	// 
	wndRenderer.Clear();
	wndRenderer.sizeVwpt = fSmaller ? PV_MIN : PV_MAX;

	wndRenderer.tfmtVec = tfmtVec;

	{
		My3DMath::Vtx3 vmm = b.Cube();
		wndRenderer.fMaxLen = max(max(vmm.x, vmm.y), vmm.z) / 2;
		wndRenderer.center = b.Center();

		wndRenderer.pwk = &wk;
		wndRenderer.pcc = this;

		wndRenderer.PrecacheShader();
	}
	//
	if (!wndRenderer.Capture(OSP_JoinPath(strNewFolder_entityDef, OSP_UnixToWin(strHref))))
		return false;

	return true;
}

bool CDoom3Context::GetModelMesh(CString strModel, CString &strMesh)
{
	CDoom3modelMap::iterator
		iterPos = wk.modelMap.find(strModel),
		iterEnd = wk.modelMap.end();
	if (iterPos == iterEnd)
		return false;
	CDoom3model model = iterPos->second;
	wk.CompleteInherit(model);
	if (!Sol::GetValue(model.m, "mesh", strMesh))
		return false;
	return true;
}

bool CDoom3Context::ReadSkin()
{
	printf("Parsing from skin files\n");

	CEntryMap m2;
	if (!FindAll("skin", m2))
		return false;
	CEntryMap::iterator
		iterPos = m2.begin(),
		iterEnd = m2.end();
	for (; iterPos != iterEnd; iterPos++) {
		TRACE1("ReadSkin: %s\n", (LPCTSTR)iterPos->first);

		printf("Parsing from '%s'\n", (LPCSTR)iterPos->first);

		SizeBuff sbDef;
		if (iterPos->second->GetData(sbDef)) {
			CLL1Memr m;
			m.Init(sbDef.GetData(), sbDef.GetSize());
			CLL1Leader c(m);
			CSparseDef a(c, wk);
			if (a.Parse()) {
				printf("ok (possibly)\n");
			} else {
				printf("ERR\n");
			}
		}
	}

	printf("Parsing ok\n\n");
	return true;
}

bool CDoom3Context::ReadMaterial()
{
	printf("Parsing from mtr files\n");

	CEntryMap m2;
	if (!FindAll("mtr", m2))
		return false;
	CEntryMap::iterator
		iterPos = m2.begin(),
		iterEnd = m2.end();
	for (; iterPos != iterEnd; iterPos++) {
		TRACE1("ReadMtr: %s\n", (LPCTSTR)iterPos->first);

		printf("Parsing from '%s'\n", (LPCSTR)iterPos->first);

		SizeBuff sbDef;
		if (iterPos->second->GetData(sbDef)) {
			CLL1Memr m;
			m.Init(sbDef.GetData(), sbDef.GetSize());
			CLL1Leader c(m);
			CSparseDef a(c, wk);
			if (a.Parse()) {
				printf("ok (possibly)\n");
			} else {
				printf("ERR\n");
			}
		}
	}

	printf("Parsing ok\n\n");
	return true;
}

bool CDoom3Context::ReadTest()
{
	printf("Parsing test running\n");

#if 0
	CEntryMap m2;
	if (!FindAll("md5mesh", m2))
		return false;
	CEntryMap::iterator
		iterPos = m2.begin(),
		iterEnd = m2.end();
	for (; iterPos != iterEnd; iterPos++) {
		TRACE1("ReadMD5Mesh: %s\n", (LPCTSTR)iterPos->first);

		printf("Parsing from '%s'\n", (LPCSTR)iterPos->first);

		CString strName = OSP_GetFileName(OSP_UnixToWin(iterPos->first));

		SizeBuff sbDef;
		if (iterPos->second->GetData(sbDef)) {
			CLL1Memr m;
			m.Init(sbDef.GetData(), sbDef.GetSize());
			CMD5MeshDeck mesh;
			CLL1Leader c(m);
			CSparseMD5Mesh a(c, mesh);
			if (a.Parse()) {
				CEaseWriter ew(mesh);
				if (ew.Open("\\" + strName + ".ew") && ew.Write()) {
					printf("ok (possibly)\n");
				}
			} else {
				printf("ERR\n");
			}
		}
	}
#endif
#if 0
	CEntryMap m2;
	if (!FindAll("tga", m2))
		return false;
	CEntryMap::iterator
		iterPos = m2.begin(),
		iterEnd = m2.end();
	for (; iterPos != iterEnd; iterPos++) {
		TRACE1("ReadTGA: %s\n", (LPCTSTR)iterPos->first);

		printf("Parsing from '%s'\n", (LPCSTR)iterPos->first);

		SizeBuff sbDef;
		if (iterPos->second->GetData(sbDef)) {
			CDoom3FlatIma fi;
			CSparseTarga m(fi);
			m.pData = sbDef.GetData();
			m.iData = 0, m.nData = sbDef.GetSize(), m.fLE = true;
			if (m.Parse()) {
				printf("ok (possibly)\n");
			} else {
				printf("ERR\n");
			}
		}
	}
#endif
#if 0
	CEntryMap m2;
	if (!FindAll("md5anim", m2))
		return false;
	CEntryMap::iterator
		iterPos = m2.begin(),
		iterEnd = m2.end();
	for (; iterPos != iterEnd; iterPos++) {
		TRACE1("ReadMD5: %s\n", (LPCTSTR)iterPos->first);

		printf("Parsing from '%s'\n", (LPCSTR)iterPos->first);

		SizeBuff sbDef;
		if (iterPos->second->GetData(sbDef)) {
			CLL1Memr m;
			m.Init(sbDef.GetData(), sbDef.GetSize());
			NfMD5::MD5AnimDeck_t mesh;
			CLL1Leader c(m);
			NfMD5::ParseMD5Anim_t a(c, mesh);
			if (a.Parse()) {
				printf("ok (possibly)\n");
			} else {
				printf("ERR\n");
			}
		}
	}
#endif
#if 0
	CEntryMap m2;
	if (!FindAll("md5mesh", m2))
		return false;
	CEntryMap::iterator
		iterPos = m2.begin(),
		iterEnd = m2.end();
	for (; iterPos != iterEnd; iterPos++) {
		TRACE1("ReadMD5: %s\n", (LPCTSTR)iterPos->first);

		printf("Parsing from '%s'\n", (LPCSTR)iterPos->first);

		SizeBuff sbDef;
		if (iterPos->second->GetData(sbDef)) {
			CLL1Memr m;
			m.Init(sbDef.GetData(), sbDef.GetSize());
			NfMD5::MD5MeshDeck_t mesh;
			CLL1Leader c(m);
			NfMD5::ParseMD5Mesh_t a(c, mesh);
			if (a.Parse()) {
				printf("ok (possibly)\n");
			} else {
				printf("ERR\n");
			}
		}
	}
#endif
#if 0
	MaterialDistributionMMap mm;

	CEntryMap m2;
	if (!FindAll("map", m2))
		return false;
	CEntryMap::iterator
		iterPos = m2.begin(),
		iterEnd = m2.end();
	for (; iterPos != iterEnd; iterPos++) {
		TRACE1("ReadMap: %s\n", (LPCTSTR)iterPos->first);

		printf("Parsing from '%s'\n", (LPCSTR)iterPos->first);

		SizeBuff sbDef;
		if (iterPos->second->GetData(sbDef)) {
			CLL1Memr m;
			m.Init(sbDef.GetData(), sbDef.GetSize());
			CLL1Leader c(m);
			CSparseV2Map a(c, mm);
			if (a.Parse()) {
				printf("ok (possibly)\n");
			} else {
				printf("ERR\n");
			}
		}
	}

	CMaterialStats t(wk, mm);
	t.Run();
#endif
#if 0
	CEntryMap m2;
	if (!FindAll("ase", m2))
		return false;
	CEntryMap::iterator
		iterPos = m2.begin(),
		iterEnd = m2.end();
	for (; iterPos != iterEnd; iterPos++) {
		TRACE1("ReadASE: %s\n", (LPCTSTR)iterPos->first);

		printf("Parsing from '%s'\n", (LPCSTR)iterPos->first);

		SizeBuff sbDef;
		if (iterPos->second->GetData(sbDef)) {
			CLL1Memr m;
			m.Init(sbDef.GetData(), sbDef.GetSize());
			CAseMeshDeck mesh;
			CLL1Leader c(m);
			CSparseASE a(c, mesh);
			if (a.Parse()) {
				printf("ok (possibly)\n");
			} else {
				printf("ERR\n");
			}
		}
	}
#endif
#if 0
	CEntryMap m2;
	if (!FindAll("md5anim", m2))
		return false;
	CEntryMap::iterator
		iterPos = m2.begin(),
		iterEnd = m2.end();
	for (; iterPos != iterEnd; iterPos++) {
		TRACE1("ReadMD5Anim: %s\n", (LPCTSTR)iterPos->first);

		printf("Parsing from '%s'\n", (LPCSTR)iterPos->first);

		SizeBuff sbDef;
		if (iterPos->second->GetData(sbDef)) {
			CLL1Memr m;
			m.Init(sbDef.GetData(), sbDef.GetSize());
			CMD5AnimDeck anim;
			CLL1Leader c(m);
			CSparseMD5Anim a(c, anim);
			if (a.Parse()) {
				printf("ok (possibly)\n");
			} else {
				printf("ERR\n");
			}
		}
	}
#endif

	printf("Parsing test ok\n\n");
	return true;
}

bool CDoom3Context::AssocMapobjs()
{
	CEntryMap::iterator
		iterPos = m.begin(),
		iterEnd = m.end();
	for (; iterPos != iterEnd; iterPos++) {
		if (false
			|| IsExtNameIs(iterPos->first, "lwo")
			|| IsExtNameIs(iterPos->first, "ase")
		) {
			if (iterPos->first.Find("models/") == 0)
				wk.mapobjsNames.insert(iterPos->first);
		}
	}

	return true;
}

bool CDoom3Context::ExportAnim(CString strModel, CDoom3entityDef *entityDef, CString strAnim, CString strHref)
{
	if (!wndRenderer.IsRendererAlive())
		return false;
	CDoom3modelMap::iterator
		iterPos = wk.modelMap.find(strModel),
		iterEnd = wk.modelMap.end();
	if (iterPos == iterEnd)
		return false;
	CDoom3model model = iterPos->second;
	wk.CompleteInherit(model);
	CMD5MeshDeck m10mesh;
	{
		CString strMeshf;
		if (!Sol::GetValue(model.m, "mesh", strMeshf))
			return false;
		fprintf(ff.f, "  meshf(%s)\n", (LPCSTR)strMeshf);
		SizeBuff sb;
		if (!LoadEntry(strMeshf, sb))
			return false;
		CLL1Memr m;
		m.Init(sb.GetData(), sb.GetSize());
		TRACE1("Mesh: %s\n", (LPCTSTR)strMeshf);
		CLL1Leader c(m);
		CSparseMD5Mesh a(c, m10mesh);
		if (!a.Parse())
			return false;
	}
	CMD5AnimDeck m10anim;
	{
		CString strAnimf;
		if (!Sol::GetValue(model.anims, strAnim, strAnimf))
			return false;
		fprintf(ff.f, "  animf(%s)\n", (LPCSTR)strAnimf);
		SizeBuff sb;
		if (!LoadEntry(strAnimf, sb))
			return false;
		CLL1Memr m;
		m.Init(sb.GetData(), sb.GetSize());
		TRACE1("Anim: %s\n", (LPCTSTR)strAnimf);
		CLL1Leader c(m);
		CSparseMD5Anim a(c, m10anim);
		if (!a.Parse())
			return false;
	}
	bool fRenderHead = false;
	CMD5MeshDeck m10meshHead;
	CMD5AnimDeck m10animHead;
	if (entityDef != NULL) {
		CString strJoint;
		CString strMesh;
		if (Sol::GetValue(entityDef->m, "def_head", strMesh) && Sol::GetValue(entityDef->m, "head_joint", strJoint)) {
			CDoom3modelMap::iterator
				iterPos = wk.modelMap.find(strMesh),
				iterEnd = wk.modelMap.end();
			if (iterPos != iterEnd) {
				CDoom3model modelHead = iterPos->second;
				wk.CompleteInherit(modelHead);
				CString strMD5anim, strMD5mesh;
				if (Sol::GetValue(modelHead.m, "mesh", strMD5mesh)) {
					if (Sol::GetValue(modelHead.anims, strAnim, strMD5anim)) {
						if (LoadMD5mesh(strMD5mesh, m10meshHead)) {
							{
								UINT i;
								for (i = 0; i < m10anim.joints.size(); i++) {
									CMD5AnimJoint &j0 = m10anim.joints.at(i);
									if (j0.strName != strJoint) continue;
									m10animHead.basejointAvail = true;
									m10animHead.basejoint.resize(m10anim.frames.size());
									UINT x;
									for (x = 0; x < m10anim.frames.size(); x++) {
										m10animHead.basejoint.at(x) = m10anim.frames.at(x).at(i);
									}
									break;
								}
							}
							if (LoadMD5anim(strMD5anim, m10animHead)) {
								fRenderHead = true;
							}
						}
					}
				}
			}
		}
	}
	//
	CSize sizeVw = PV_MAX;

	wndRenderer.Clear();
	wndRenderer.sizeVwpt = sizeVw;

	CAviMk aviMk;
	if (!aviMk.Create(OSP_JoinPath(strNewFolder_entityDef, strHref), m10anim.nFrameRate, sizeVw))
		return false;

	size_t iFrame, iMesh, nFrames = m10anim.frames.size(), nMeshes;

	TformedMeshTabPtrArray tfmtVec;

	vector<My3DMath::BBox3> bv; bv.resize(nFrames);

	for (iFrame = 0; iFrame < nFrames; iFrame++) {
		bv[iFrame].Empty();
		CTformedMesh tfm2[2];
		CTformedMeshTab tfmt2[2];
		tfmtVec.clear();
		for (int iModel = 0; iModel < 2; iModel++) {
			CMD5MeshDeck *pMesh = (iModel == 0) ? &m10mesh : &m10meshHead;
			CMD5AnimDeck *pAnim = (iModel == 0) ? &m10anim : &m10animHead;

			if (!pMesh || !pAnim) continue;

			CTformedMeshTab &tfmt = tfmt2[iModel];
			tfmt.p = &tfm2[iModel];
			CTformedMesh &m = *tfmt.p;

			if (pAnim->frames.size() <= iFrame) continue;

			tfmtVec.push_back(&tfmt);

			MD5AnimFramePatchArray &frm = pAnim->frames.at(iFrame);
			MD5MeshJointArray &js = pMesh->joints;
			for (UINT i = 0; i < js.size(); i++) {
				js[i].ok = frm.at(i);
			}

			nMeshes = pMesh->meshes.size();

			UINT nMaxTris = 0;
			UINT nMaxVerts = 0;

			CString strShader;

			for (iMesh = 0; iMesh < nMeshes; iMesh++) {
				CMD5MeshMesh &mm = pMesh->meshes[iMesh];
				nMaxTris += mm.tris.size();
				nMaxVerts += mm.verts.size();

				Sol::GetValue(mm.m, "shader", strShader);

				tfmt.strTexs.Add(strShader);
			}

			if (!m.Malloc(nMaxTris, nMaxVerts))
				return false;

			UINT iCurTri = 0;
			UINT iCurVert = 0;

			for (iMesh = 0; iMesh < nMeshes; iMesh++) {
				CMD5MeshMesh &mm = pMesh->meshes[iMesh];
				for (size_t iTri = 0; iTri < mm.tris.size(); iTri++) {
					m.pIndex[3*iCurTri +3*iTri +0] = iCurVert +mm.tris.at(iTri).v[0];
					m.pIndex[3*iCurTri +3*iTri +1] = iCurVert +mm.tris.at(iTri).v[1];
					m.pIndex[3*iCurTri +3*iTri +2] = iCurVert +mm.tris.at(iTri).v[2];
					m.pTexIndex[iCurTri +iTri] = iMesh;
				}
				for (size_t iVert = 0; iVert < mm.verts.size(); iVert++) {
					m.pTexCoord0[2*iCurVert +2*iVert +0] =       mm.verts.at(iVert).o.x;
					m.pTexCoord0[2*iCurVert +2*iVert +1] =       mm.verts.at(iVert).o.y;
					My3DMath::Vtx3 v3;
					mm.InterpolateVertex(pMesh->joints, 0U +iVert, v3);
					memcpy(m.pVertex +3*iCurVert +3*iVert, v3.v, 4 * 3);
					bv[iFrame].Add(v3);
				}

				iCurTri += mm.tris.size();
				iCurVert += mm.verts.size();
			}
		}

		{
			My3DMath::Vtx3 vmm = bv[iFrame].Cube();
			wndRenderer.fMaxLen = max(max(vmm.x, vmm.y), vmm.z) / 2;
			wndRenderer.center = bv[iFrame].Center();

			wndRenderer.tfmtVec = tfmtVec;

			wndRenderer.pwk = &wk;
			wndRenderer.pcc = this;
			wndRenderer.pam = &aviMk;

			wndRenderer.PrecacheShader();
		}
		//
		if (!wndRenderer.CaptureAnim()) {
			puts("");
			return false;
		}
	}
	// 
	aviMk.Close(true);
	return true;
}

bool CDoom3Context::GetAnimNames(CString strModel, list<CString> &anims)
{
	if (!wndRenderer.IsRendererAlive())
		return false;
	CDoom3modelMap::iterator
		iterPos = wk.modelMap.find(strModel),
		iterEnd = wk.modelMap.end();
	if (iterPos == iterEnd)
		return false;
	CDoom3model &model = iterPos->second;
	{
		VarMap::iterator
			iterPos = model.anims.begin(),
			iterEnd = model.anims.end();
		for (; iterPos != iterEnd; iterPos++)
			anims.push_back(iterPos->first);
	}
	return true;
}

bool CDoom3Context::LoadMD5anim(CString strAnimf, CMD5AnimDeck &m10)
{
	SizeBuff sb;
	if (!LoadEntry(strAnimf, sb))
		return false;
	CLL1Memr m;
	m.Init(sb.GetData(), sb.GetSize());
	TRACE1("Anim: %s\n", (LPCTSTR)strAnimf);
	CLL1Leader c(m);
	CSparseMD5Anim a(c, m10);
	if (!a.Parse())
		return false;
	return true;
}

bool CDoom3Context::LoadMD5mesh(CString strMeshf, CMD5MeshDeck &m10)
{
	SizeBuff sb;
	if (!LoadEntry(strMeshf, sb))
		return false;
	CLL1Memr m;
	m.Init(sb.GetData(), sb.GetSize());
	TRACE1("Mesh: %s\n", (LPCTSTR)strMeshf);
	CLL1Leader c(m);
	CSparseMD5Mesh a(c, m10);
	if (!a.Parse())
		return false;
	return true;
}

bool CDoom3Context::IsEntryExisted(CString strPath)
{
	strPath.Replace('\\', '/');
	strPath.MakeLower();

	CEntryMap::iterator
		iterPos = m.find(strPath),
		iterEnd = m.end();
	if (iterPos != iterEnd) {
		return true;
	}
	return false;
}

bool CDoom3Context::IsEntityDefExportOk(CString strName)
{
	if (!cwk.fOnly_entityDef)
		return true;
	if (cwk.entityDefExport.find(strName) != cwk.entityDefExport.end())
		return true;
	return false;
}

bool CDoom3Context::MkMo_tree(FILE *f, LPCSTR psz, vector<bool> vThread)
{
	CString strLower = psz;
	CString strUpper = psz;

	int n = strUpper.GetLength();
	if (n != 0) {
		strUpper.SetAt(n -1, strUpper.GetAt(n -1) +1);
	}

	int i = strlen(psz);
	NameSet names, leaves;
	NameSet::iterator
		iterPos = wk.mapobjsNames.lower_bound(strLower),
		iterEnd = wk.mapobjsNames.upper_bound(strUpper);
	for (; iterPos != iterEnd; iterPos++) {
		int t = iterPos->Find('/', i +1);
		int n = t;
		if (t < 0) {
			n = iterPos->GetLength();
		}

		CString str = iterPos->Mid(i, n -i);

		if (t < 0) leaves.insert(str);

		names.insert(str);
	}

	{
		UINT nDescend = vThread.size() +1;

		UINT y = 0, cy = names.size();

		NameSet::iterator
			iterPos = names.begin(),
			iterEnd = names.end();
		for (; iterPos != iterEnd; iterPos++, y++) {
			vThread.push_back((y +1) != cy);

			CString strName = *iterPos;
			UINT i;
			for (i = 0; i < nDescend; i++) {
				LPCSTR pszSrc = "";
				bool fMore = vThread[i];
				if (i +1 == nDescend) {
					if (fMore) {
						pszSrc = "_7.gif";
					} else {
						pszSrc = "_3.gif";
					}
				} else {
					if (fMore) {
						pszSrc = "_5.gif";
					} else {
						pszSrc = "_0.gif";
					}
				}
				fprintf(f,
					"<img src=\"%s\" align=\"absmiddle\" />"
					, pszSrc
					);
			}

			CString strPath = psz + strName;

			bool fBranch = leaves.find(strName) == leaves.end();

			bool fExportOk = IsMapobjExportOk(strPath);

			if (!fBranch) {
				printf(fExportOk
						? "Writing '%s' (%u/%u)\n"
						: "Writing '%s' \n"
					, (LPCSTR)strPath
					, iMapobjCur
					, nMapobjMax
					);
			}

			if (!fBranch && fExportOk) {
				CString strHref = MkHref_Mapobj(_DirToPlain(strPath));
				fprintf(f,
					" <a href=\"%s\">%s</a><br />\n"
					, (LPCSTR)strHref
					, (LPCSTR)strName
					);

				MkMo_leaf(strPath, strHref);
			} else {
				fprintf(f,
					" %s<br />\n"
					, (LPCSTR)strName
					);
			}

			if (!fBranch) {
				iMapobjCur++;
			}

			MkMo_tree(f, strPath + '/', vThread);

			vThread.pop_back();
		}
	}

	return true;
}

bool CDoom3Context::IsMapobjExportOk(CString strName)
{
	if (!cwk.fOnly_mapobjs)
		return true;
	if (cwk.mapobjsExport.find(strName) != cwk.mapobjsExport.end())
		return true;
	return false;
}

bool CDoom3Context::MkMo_leaf(CString strName, CString strHref)
{
	FILE *f = fopen(OSP_JoinPath(strNewFolder_Mapobjs, strHref), "wt");
	if (f) {
		//
		fprintf(f,
			HTML_HEADER_1
			);
		fprintf(f,
			"<h1><nobr>%s</nobr></h1>\n"
			, (LPCSTR)Solve_modelName(strName)
			);

		CString strBaseName = OSP_GetFileName(OSP_UnixToWin(strName));

		// ->
		// Preview
		// ->
		{
			CString strFlatName = _DirToPlain(strName);

			CString strHrefPv = MkHref_previewModel(strFlatName);
			if (ExportMapobj(strName, strHrefPv, true)) {

			} else {
				strHrefPv = "No_preview.gif";
			}

			fprintf(f,
				"<h2>Preview</h2>\n"
				"<p>\n"
				"<img src=\"%s\" />\n"
				"</p>\n"
				, (LPCSTR)strHrefPv
				);

			if (strHrefPv != "No_preview.gif") {
				CString strHrefTh = MkHref_thumbModel(strFlatName);
				if (ExportMapobj(strName, strHrefTh, true, true)) {
					mkth2.AddThumb(strBaseName, strHrefTh, strHref);
				}
			}

			wndRenderer.UnprecacheShader();
		}

		fprintf(f,
			"</body>\n"
			"</html>\n"
			);
		//
		fclose(f);

		{
			CString strKwHref = "Mapobjs/" + strHref;

			hhcTempl.Add_Mapobject(strName, strKwHref);
			hhkTempl.AddKw(strBaseName, strKwHref);
			//hhpTempl.AddHtml(strKwHref, false);
		}
		return true;
	}
	return false;
}

bool CDoom3Context::MkASE2Tformed(CAseMeshDeck &ase, My3DMath::BBox3 &b, CTformedMeshTab &tfmt, CTformedMesh &tfm)
{
	UINT z;
	UINT nMaxTris = 0;

	for (z = 0; z < ase.mma.size(); z++) {
		CString strName = ase.mma[z].strTex;
		tfmt.strTexs.Add(AseTex2Material(strName));
	}
	for (z = 0; z < ase.mza.size(); z++) {
		CAseMeshGEOM &mz = ase.mza[z];
		nMaxTris += mz.mfa.size();
	}
	if (!tfm.Malloc(nMaxTris, 3 *nMaxTris))
		return false;
	UINT c = 0;
	UINT t;
	UINT iCurTri = 0;
	for (z = 0; z < ase.mza.size(); z++) {
		CAseMeshGEOM &mz = ase.mza[z];
		UINT nTris = mz.mfa.size();
		for (t = 0; t < nTris; t++) {
			tfm.pIndex[3*iCurTri +3*t +0] = 3*iCurTri +3*t +0;
			tfm.pIndex[3*iCurTri +3*t +1] = 3*iCurTri +3*t +1;
			tfm.pIndex[3*iCurTri +3*t +2] = 3*iCurTri +3*t +2;
			tfm.pTexIndex[iCurTri +t] = mz.mfa[t].tex0;

			UINT v;
			for (v = 0; v < 3; v++, c++) {
				UINT iv = mz.mfa[t].v[v];
				UINT itv = mz.mfa[t].tf[v];
				if (mz.mva.size() <= iv)
					return false;
				if (mz.mtva.size() <= itv)
					return false;

				My3DMath::Vtx3 v3 = mz.mva[iv].v;
				My3DMath::Vtx2 vt0 = mz.mtva[itv].vt0;
				b.Add(v3);

				tfm.pVertex[+3*c +0] = v3.x;
				tfm.pVertex[+3*c +1] = v3.y;
				tfm.pVertex[+3*c +2] = v3.z;
				tfm.pTexCoord0[+2*c +0] =    vt0.x;
				tfm.pTexCoord0[+2*c +1] = 1 -vt0.y;
			}
		}
		iCurTri += nTris;
	}
	return true;
}

bool CDoom3Context::MkLWO2ToTformed(CLWO2MeshDeck &lwo2, My3DMath::BBox3 &b, CTformedMeshTab &tfmt, CTformedMesh &tfm)
{
	UINT nTris = 0;
	UINT i;
	for (i = 0; i < lwo2.at0.size(); i++) {
		CString strName = lwo2.at0[i];
		tfmt.strTexs.Add(strName);
	}
	for (i = 0; i < lwo2.ap.size(); i++) {
		CLWO2MeshPoly &mp = lwo2.ap[i];
		UINT m = mp.verts.size();
		if (3 <= m) {
			nTris += m - 2;
		}
	}
	My3DMath::Mtx m3;
	m3.RotateX(m3, 90.0f /180.0f *3.14f);
	UINT nVerts = lwo2.av.size();
	if (!tfm.Malloc(nTris, nVerts))
		return false;
	for (i = 0; i < lwo2.av.size(); i++) {
		CLWO2MeshVertex &mv = lwo2.av[i];
		My3DMath::Vtx3 v3 = mv.v * m3;
		v3.x = -v3.x;
		tfm.pVertex[3*i +0] = v3.x;
		tfm.pVertex[3*i +1] = v3.y;
		tfm.pVertex[3*i +2] = v3.z;
		tfm.pTexCoord0[2*i +0] =       mv.vt0.x;
		tfm.pTexCoord0[2*i +1] =    1 -mv.vt0.y;
		b.Add(v3);
	}
	UINT v = 0;
	for (i = 0; i < lwo2.ap.size(); i++) {
		CLWO2MeshPoly &mp = lwo2.ap[i];
		UINT m = mp.verts.size();
		UINT t;
		for (t = 0; t < m - 2; t++, v++) {
			tfm.pIndex[3*v +0] = (GLshort)mp.verts[0].vt0;
			tfm.pIndex[3*v +1] = (GLshort)mp.verts[t +1].vt0;
			tfm.pIndex[3*v +2] = (GLshort)mp.verts[t +2].vt0;
			tfm.pTexIndex[v] = mp.texture;

			UINT iTbl[] = {
				0,
				t +1,
				t +2,
			};
			UINT a;
			for (a = 0; a < 3; a++) {
				UINT w0 = mp.verts[iTbl[a]].v;
				UINT w1 = mp.verts[iTbl[a]].vt0;
				if (w0 != w1) {
					tfm.pVertex[3*w1 +0] = tfm.pVertex[3*w0 +0];
					tfm.pVertex[3*w1 +1] = tfm.pVertex[3*w0 +1];
					tfm.pVertex[3*w1 +2] = tfm.pVertex[3*w0 +2];
				}
			}
		}
	}
	return true;
}

bool CDoom3Context::ExportMapobj(CString strModel, CString strHref, bool fMapobj, bool fSmaller)
{
	if (!wndRenderer.IsRendererAlive())
		return false;
	CTformedMeshTab tfmt;
	CTformedMesh tfm;
	tfmt.p = &tfm;
	My3DMath::BBox3 b; b.Empty();
	CLWO2MeshDeck lwo2;
	CAseMeshDeck ase;
	if (false);
	else if (IsExtNameIs(strModel, "ase") && LoadASE(strModel, ase)) {
		if (!MkASE2Tformed(ase, b, tfmt, tfm))
			return false;
		printf("");
	}
	else if (IsExtNameIs(strModel, "lwo") && LoadLWO2(strModel, lwo2)) {
		if (!MkLWO2ToTformed(lwo2, b, tfmt, tfm))
			return false;
		printf("");
	}
	else {
		return false;
	}
	// 
	wndRenderer.Clear();
	wndRenderer.sizeVwpt = fSmaller ? PV_MIN : PV_MAX;

	wndRenderer.tfmtVec.clear();
	wndRenderer.tfmtVec.push_back(&tfmt);

	{
		My3DMath::Vtx3 vmm = b.Cube();
		wndRenderer.fMaxLen = max(max(vmm.x, vmm.y), vmm.z) / 2;
		wndRenderer.center = b.Center();

		wndRenderer.pwk = &wk;
		wndRenderer.pcc = this;

		wndRenderer.PrecacheShader();
	}
	//
	if (!wndRenderer.Capture(OSP_JoinPath(fMapobj ? strNewFolder_Mapobjs : strNewFolder_entityDef, OSP_UnixToWin(strHref))))
		return false;

	return true;
}

bool CDoom3Context::LoadLWO2(CString strMeshf, CLWO2MeshDeck &mesh)
{
	SizeBuff sb;
	if (!LoadEntry(strMeshf, sb))
		return false;
	CSparseLWO2 a(mesh);
	a.pData = sb.GetData();
	a.iData = 0;
	a.nData = sb.GetSize();
	a.fLE = false;
	if (!a.Parse())
		return false;
	return true;
}

bool CDoom3Context::LoadASE(CString strMeshf, CAseMeshDeck &mesh)
{
	SizeBuff sb;
	if (!LoadEntry(strMeshf, sb))
		return false;
	CLL1Memr m;
	m.Init(sb.GetData(), sb.GetSize());
	CLL1Leader c(m);
	CSparseASE a(c, mesh);
	if (!a.Parse())
		return false;
	return true;
}

// table builduplight { clamp { 0, .2, .4, .6, .8, 1, 1, 1 } }

// alphaTest 0.25 + 0.5 * (time - parm7)
// alphaTest	0.5 + 0.5 * sintable [ time * .2  ]
// alphaTest .5 + 0.5 * sintable [ time * .2  ] -.02
// rgb 	(.3 * sintable [.2 + time ]) +.2
// map addnormals (textures/base_wall/gotendo1_local.tga, heightmap (textures/base_wall/gotendo1_bmp.tga, 4))	
// bumpmap		addnormals(models/monsters/wraith/wraith_local.tga, heightmap(models/monsters/wraith/wraith_h.tga, 3 ) )
// bumpmap 	heightmap(textures/decals/p_poster_b.tga, 1 ) 
// scroll	time * 0 , time * -4
// translate	time * 0 , time * 0.6
// renderbump  -size 256 256 -trace 0.1 -colorMap -aa 2  models/weapons/shell1/sshell_local.tga models/weapons/shell1/sshell_hi.lwo
// rgb emptytable[ time * 2 ]
// red	flickertable [ time * 0.3 ]
// rotate	time * -8
// red 	muzzleflashrandom[ time * 3.33 ] * muzzleflashtable[ (time + parm4) * 4 * Parm3 ] * Parm0
// if ( ( time * 9 ) % 9 == 0 )
// if ( ((time + parm4) * 0.2) < 1.0 )
// red 	( Parm0 * acceleratorflashtable[ ( time - global1 ) / 7 ] )
// red 		( ( blasttable [Parm4 + (time / 6 * (Parm3)) ]) * Parm0 ) * ( acceleratorflashtable[ ( time - global1 ) / 7 ] )
// if ( parm7 == 0 )
// rgb ( (.3 * sintable [.15 + time * .5 ]) +.7 ) * ( acceleratortextable [ ( time - global1 ) / 7 ] )

CString CDoom3Context::Solve_modelName(CString strModel)
{
	CString str;
	if (IsEntryExisted(str = strModel + ".lwo"))
		return str;
	if (IsEntryExisted(str = strModel + ".ase"))
		return str;
	return strModel;
}

bool CDoom3Context::MkHHP()
{
	printf("Making HHP\n");

	int nCount = 0, nDone = 0;

	{
		nCount++;
		CHHPMk mk(getHwsMan());
		if (mk.Open(OSP_JoinPath(strNewFolder, getHwsMan().gethhp()))) {
			if (hhpTempl.Commit(mk)) {
				nDone++;
			}
			mk.Close();
		}
	}
	{
		nCount++;
		CHHCMk mk;
		if (mk.Open(OSP_JoinPath(strNewFolder, getHwsMan().gethhc()))) {
			mk.MkHeader(true);
			if (hhcTempl.Commit(mk)) {
				mk.MkHeader(false);
				nDone++;
			}
			mk.Close();
		}
	}
	{
		nCount++;
		CHHKMk mk;
		if (mk.Open(OSP_JoinPath(strNewFolder, getHwsMan().gethhk()))) {
			mk.MkHeader(true);
			if (hhkTempl.Commit(mk)) {
				mk.MkHeader(false);
				nDone++;
			}
			mk.Close();
		}
	}

	if (nCount == nDone)
		printf("Making ok\n\n");
	else if (nDone != 0)
		printf("Making partially successful\n\n");
	else
		printf("Making ERR\n\n");

	return true;
}

CString CDoom3Context::AseTex2Material(CString strTex)
{
	strTex.MakeLower();
	strTex.Replace('\\', '/');
	int t = strTex.Find("/doom/base/");
	if (t < 0)
		return "";
	strTex.Delete(0, t +11);
	if (strTex.Right(4) == ".tga") {
		strTex = strTex.Mid(0, strTex.GetLength() -4);
		return strTex;
	}
	return "";
}

void CDoom3Context::Close()
{
	m.clear();
	wk.Close();
	wndRenderer.Close();
	mkth2.Reset();
}

bool CDoom3Context::MkEM()
{
	ff.Open_mtr(true, true);
	ff.Open_fmap(true);

	mkth2.Reset();

	printf("Making htmls (Entity model)\n");

	VERIFY(wndRenderer.CreateSized(wndRenderer.fExpose ? PV_SUPER : PV_MAX));

	int nCount = 0, nDone = 0;

	nCount++, nDone += Sol::MkDir(strNewFolder_Entitymodel) ? 1 : 0;
	nCount++, nDone += Sol::MkDir(OSP_JoinPath(strNewFolder_Entitymodel, "model_pv")) ? 1 : 0;
	nCount++, nDone += Sol::MkDir(OSP_JoinPath(strNewFolder_Entitymodel, "model_thumb")) ? 1 : 0;
	nCount++, nDone += Sol::MkDir(OSP_JoinPath(strNewFolder_Entitymodel, "maps")) ? 1 : 0;
	nCount++, nDone += Sol::ExportResource(MAKEINTRESOURCE(getHwsMan().getindexres()), RT_HTML, OSP_JoinPath(strNewFolder, "index.html")) ? 1 : 0;
	if (nCount == nDone) {
		FILE *f = fopen(OSP_JoinPath(strNewFolder_Entitymodel, "tree.html"), "wt");
		FILE *f2 = fopen(OSP_JoinPath(strNewFolder_Entitymodel, "thumb.html"), "wt");

		nCount++, nDone += (f != NULL) ? 1 : 0;
		nCount++, nDone += (f2 != NULL) ? 1 : 0;

		bool fPass = (nCount == nDone);

		nCount = nDone = 0;

		if (f && f2 && fPass) {
			mkth2.f = f2;
			mkth2.MkHead(true);
			//
			fprintf(f,
				HTML_HEADER_1
				"<p>\n"
				);
			fprintf(f,
				" maps<br />\n"
				);

			CEntryMap m2;
			FindAll("map", m2);

			CEntryMap::iterator
				iterPos = m2.begin(),
				iterEnd = m2.end();
			for (; iterPos != iterEnd; iterPos++) {
				printf("\n" "Parsing from '%s'\n", (LPCSTR)iterPos->first);

				SizeBuff sbDef;
				if (iterPos->second->GetData(sbDef)) {
					CLL1Memr m;
					m.Init(sbDef.GetData(), sbDef.GetSize());
					CLL1Leader c(m);
					CV2MapDeck mesh;
					CSparseV2Map a(c, mesh, wk.Q4());
					bool r = a.Parse();
					if (!r) {
						printf("ERR\n");
					} else {
						printf("ok (possibly)\n");

						printf("Export inprogress.\n");

						int nModels;

						bool r = MkEM_Export(iterPos->first, mesh, nModels);

						nCount++, nDone += r ? 1 : 0;

						if (r) {
							printf("Export ok. %d Entity models exported.\n", nModels);
						} else {
							printf("Export ERR.\n");
						}
					}
				}
			}

			fprintf(f,
				"</p>\n"
				"</body>\n"
				"</html>\n"
				);
			//
			mkth2.MkHead(false);
		}
		if (f) fclose(f);
		if (f2) fclose(f2);
	}

	if (nDone != 0 && nCount == nDone)
		printf("Making ok\n\n");
	else if (nDone != 0)
		printf("Making partially successful\n\n");
	else
		printf("Making ERR\n\n");

	wndRenderer.Close();
	return true;
}

bool CDoom3Context::MkEM_Export(CString strMap, CV2MapDeck &mesh, int &nModels)
{
	nModels = 0;

	CV2MapEntity ws;
	ws.m["classname"] = "worldspawn";

	mkth2.MkTableHead(true, HtmlEnc(strMap));

	V2MapEntityList_t::iterator
		iterPos = mesh.ea.begin(),
		iterEnd = mesh.ea.end();
	size_t i;
	for (i = 0; iterPos != iterEnd; i++, iterPos++) {
		if (i == 0) {
			continue;
		}
		CV2MapEntity &e0 = *iterPos;
		if (e0.strName.IsEmpty() || e0.fHasBind || mesh.vs.find(e0.strName) == mesh.vs.end()) {
			continue;
		}
		StrSet_t ss;
		mesh.SearchDep(e0.strName, ss);
		if (ss.size() < 2) {
			continue;
		}
		CString strMapName = MkHref_Modelentity_map(_DirToPlain2(strMap), e0.strName);
		CString strMapf = OSP_JoinPath(strNewFolder_Entitymodel, OSP_UnixToWin(strMapName));
		CString strPv0 = MkHref_Modelentity_preview(_DirToPlain2(strMap), e0.strName, true);
		CString strPv1 = MkHref_Modelentity_preview(_DirToPlain2(strMap), e0.strName, false);
		CString strHrefHtml = MkHref_Modelentity_html(_DirToPlain2(strMap), e0.strName);
		CV2MapWriter w;
		if (w.Open(strMapf)) {
			w.MkHeader();

			BMEntList_t bmel;

			vector<CV2MapEntity *> aem;

			w.MkEntityHeader();
			w.MkEntity(ws);
			w.MkEntityFooter();
			StrSet_t::iterator
				iterPos = ss.begin(),
				iterEnd = ss.end();
			for (; iterPos != iterEnd; iterPos++) {
				CV2MapEntity *e1 = mesh.FindEntity(*iterPos);
				if (e1 == NULL) continue;

				MkEM_Capture(*e1, bmel);

				My3DMath::Vtx3 o = e1->o - e0.o;
				CString strOrigin; strOrigin.Format("%s %s %s"
					, (LPCTSTR)Sol::f2s(o.x)
					, (LPCTSTR)Sol::f2s(o.y)
					, (LPCTSTR)Sol::f2s(o.z)
					);
				e1->m["origin"] = strOrigin;

				CString strModel = e1->m["model"];
				if (strModel.GetLength() != 0 && strModel != e1->strName) {
					aem.push_back(e1);
				}

				{
					V2MapBrushArray_t &va = e1->va;
					size_t iv;
					for (iv = 0; iv < va.size(); iv++) {
						V2MapPlaneArray_t &pa = va[iv].pa;
						size_t ip;
						for (ip = 0; ip < pa.size(); ip++) {
							CV2MapPlane &p0 = pa[ip];
							My3DMath::Vtx3 vo = e0.o;

							float tx = (vo).DotProduct(p0.v3R);
							float ty = (vo).DotProduct(p0.v3Up);
							float x1 = (p0.m32[0][0] * tx + p0.m32[0][1] * ty) + p0.m32[0][2];
							float y1 = (p0.m32[1][0] * tx + p0.m32[1][1] * ty) + p0.m32[1][2];
							p0.m32[0][2] = x1;
							p0.m32[1][2] = y1;
						}
					}
				}

				w.patchDef_forceo = -e0.o;

				w.MkEntityHeader();
				w.MkEntity(*e1);
				w.MkEntityFooter();
			}
			w.Close();

			try {
				map<CString, int> mi;

				TformedMeshTabPtrArray tfmtpa;
				TformedMeshTabArray tfmta;
				TformedMeshArray tfma;

				int nEnts = aem.size() +1;

				tfmtpa.resize(nEnts);
				tfmta.resize(nEnts);
				tfma.resize(nEnts);

				CTformedMeshTab &tfmt = tfmta[0];
				CTformedMesh &tfm = tfma[0];

				for (int iEnt = 0; iEnt < nEnts; iEnt++) {
					tfmtpa[iEnt] = &tfmta[iEnt];
					tfmtpa[iEnt]->p = &tfma[iEnt];
				}

				int nVerts = 0;
				int nTris = 0;

				{
					BMEntList_t::iterator
						iter1Pos = bmel.begin(),
						iter1End = bmel.end();
					for (; iter1Pos != iter1End; iter1Pos++) {
						CBMEnt &bme = *iter1Pos;
						BMapPtArrayList_t::iterator
							iter2Pos = bme.alm.begin(),
							iter2End = bme.alm.end();
						for (; iter2Pos != iter2End; iter2Pos++) {
							BMapPtArray_t &bmpa = *iter2Pos;
							if (mi.find(bmpa.strTex) == mi.end()) {
								mi[bmpa.strTex] = tfmt.strTexs.GetSize();
								tfmt.strTexs.Add(bmpa.strTex);
							}

							nTris += bmpa.size() - 2;
							nVerts += bmpa.size();
						}
					}
				}

				if (!tfm.Malloc(nTris, nVerts)) {
					throw int();
				}

				GLshort *pIndex = tfm.pIndex;
				GLfloat *pVertex = tfm.pVertex;
				GLfloat *pTexCoord0 = tfm.pTexCoord0;
				GLshort *pTexIndex = tfm.pTexIndex;

				My3DMath::BBox3 b; b.Empty();

				{
					int iVerto = 0;
					int iTrio = 0;

					BMEntList_t::iterator
						iter1Pos = bmel.begin(),
						iter1End = bmel.end();
					for (; iter1Pos != iter1End; iter1Pos++) {
						CBMEnt &bme = *iter1Pos;
						BMapPtArrayList_t::iterator
							iter2Pos = bme.alm.begin(),
							iter2End = bme.alm.end();
						for (; iter2Pos != iter2End; iter2Pos++) {
							BMapPtArray_t &bmpa = *iter2Pos;
							int iTex = mi[bmpa.strTex];
							int x, cx = bmpa.size();

							for (x = 0; x < cx - 2; x++) {
								pIndex[3*iTrio +3*x +0] = iVerto;
								pIndex[3*iTrio +3*x +1] = iVerto +x+1;
								pIndex[3*iTrio +3*x +2] = iVerto +x+2;
								pTexIndex[iTrio +x] = iTex;
							}

							for (x = 0; x < cx; x++) {
								CBMapPt &bmp = bmpa[x];
								My3DMath::Vtx3 v = (bmp.v * bme.rotation) + bme.o;
								b.Add(v);
								memcpy(&pVertex[3*iVerto +3*x], v.v, sizeof(float) * 3);
								pTexCoord0[2*iVerto +2*x +0] = bmp.tv.x;
								pTexCoord0[2*iVerto +2*x +1] = bmp.tv.y;
							}

							iTrio += cx - 2;
							iVerto += cx;
						}
					}
				}

//				fprintf(ff.fmap, "\n\n->#<->#<->#<->#<->#<->#<->#<->#<->#<->#<- %s %s\n\n\n"
//					, (LPCSTR)strMap
//					, (LPCSTR)e0.strName
//					);

//				fprintf(ff.fmap,
//					"#--          BBox((%6d,%6d,%6d),(%6d,%6d,%6d))\n"
//						, (int)b.vmin.x
//						, (int)b.vmin.y
//						, (int)b.vmin.z
//						, (int)b.vmax.x
//						, (int)b.vmax.y
//						, (int)b.vmax.z
//					);

				for (int iMe = 1; iMe < nEnts; iMe++) {
					CV2MapEntity *p1 = aem[iMe -1];

					CString strModel = p1->m["model"];

					strModel.Replace('\\', '/');
					strModel.MakeLower();

					CTformedMeshTab &tfmt = tfmta[iMe];
					CTformedMesh &tfm = tfma[iMe];

					tfm.nVerts = 0;

					CLWO2MeshDeck lwo2;
					CAseMeshDeck ase;
					My3DMath::BBox3 b0; b0.Empty();
					if (false);
					else if (IsExtNameIs(strModel, "ase") && LoadASE(strModel, ase)) {
						if (!MkASE2Tformed(ase, b0, tfmt, tfm)) {
							ASSERT(false);
						}
					}
					else if (IsExtNameIs(strModel, "lwo") && LoadLWO2(strModel, lwo2)) {
						if (!MkLWO2ToTformed(lwo2, b0, tfmt, tfm)) {
							ASSERT(false);
						}
					}
					else {
						//ASSERT(false);
					}

					b0.Empty();

					size_t i, nv = tfm.fvVertex.GetSize() / (4*3);
					for (i = 0; i < (size_t)nv; i++) {
						GLfloat *pv = &tfm.pVertex[3*i];
						My3DMath::Vtx3 v;
						memcpy(v.v, pv, 4*3);
						v = (v * p1->rotation) + p1->o;
						memcpy(pv, v.v, 4*3);
						b.Add(v);
						b0.Add(v);

					//	fprintf(ff.fmap, "%5d,%5d,%5d\n"
					//		, (int)v.x
					//		, (int)v.y
					//		, (int)v.z
					//		);
					}

//					fprintf(ff.fmap, 
//						"#%02u V(%5d),BBox((%6d,%6d,%6d),(%6d,%6d,%6d)),Model('%s'), Rotation((%f,%f,%f),(%f,%f,%f),(%f,%f,%f))\n"
//						, iMe
//						, (int)tfm.nVerts
//						, (int)b0.vmin.x
//						, (int)b0.vmin.y
//						, (int)b0.vmin.z
//						, (int)b0.vmax.x
//						, (int)b0.vmax.y
//						, (int)b0.vmax.z
//						, (LPCSTR)strModel
//						, p1->rotation.v[ 0]
//						, p1->rotation.v[ 1]
//						, p1->rotation.v[ 2]
//						, p1->rotation.v[ 4]
//						, p1->rotation.v[ 5]
//						, p1->rotation.v[ 6]
//						, p1->rotation.v[ 8]
//						, p1->rotation.v[ 9]
//						, p1->rotation.v[10]
//						);
				}

//				fprintf(ff.fmap,
//					"#--          BBox((%6d,%6d,%6d),(%6d,%6d,%6d))\n"
//						, (int)b.vmin.x
//						, (int)b.vmin.y
//						, (int)b.vmin.z
//						, (int)b.vmax.x
//						, (int)b.vmax.y
//						, (int)b.vmax.z
//					);

				//
				wndRenderer.Clear();

				wndRenderer.pwk = &wk;
				wndRenderer.pcc = this;
				wndRenderer.tfmtVec.swap(tfmtpa);

				My3DMath::Vtx3 vmm = b.Cube();
				wndRenderer.fMaxLen = max(max(vmm.x, vmm.y), vmm.z) / 2;
				wndRenderer.center = b.Center();

				wndRenderer.PrecacheShader();
				//
				for (int ri = 0; ri < 2; ri++) {
					bool fSmaller = ri == 0;
					//
					wndRenderer.sizeVwpt = fSmaller ? PV_MIN : (wndRenderer.fExpose ? PV_SUPER : PV_MAX);
					//
					if (!wndRenderer.Capture(OSP_JoinPath(strNewFolder_Entitymodel, OSP_UnixToWin(fSmaller ? strPv0 : strPv1))))
						return false;
				}

				if (MkHtml_Modelentity(strHrefHtml, strPv1, e0.strName, strMap)) {
					mkth2.AddThumb(e0.strName, strPv0, strHrefHtml);
				}

				printf("");
			} catch (int) {

			}
			nModels++;

		}
	}

	mkth2.MkTableHead(false);

	return true;
}

bool CDoom3Context::MkHtml_Modelentity(CString strHref, CString strHrefPv, CString strName, CString strParentName)
{
	FILE *f = fopen(OSP_JoinPath(strNewFolder_Entitymodel, OSP_UnixToWin(strHref)), "wt");
	if (f) {
		//
		fprintf(f,
			HTML_HEADER_1
			);
		fprintf(f,
			"<h1><nobr>%s (%s)</nobr></h1>\n"

			"<h2>Preview</h2>\n"
			"<p>\n"
			"<img src=\"%s\" />\n"
			"</p>\n"
			, (LPCSTR)HtmlEnc(strName)
			, (LPCSTR)HtmlEnc(strParentName)
			, (LPCSTR)strHrefPv
			);
		fprintf(f,
			"</body>\n"
			"</html>\n"
			);
		//
		fclose(f);
		return true;
	}
	return false;
}

#define UTMOST (18000)
#define ERR_EPS (1e-2)

bool CDoom3Context::MkEM_Capture(CV2MapEntity &e0, BMEntList_t &bmel)
{
#ifdef _DEBUG_PLANE
	TRACE1("E(%s)\n", (LPCTSTR)e0.strName);
#endif // _DEBUG_PLANE

	CBMEnt &bme = *(bmel.insert(bmel.end(), CBMEnt()));
	bme.o = e0.o;
	bme.rotation = e0.rotation;

	fprintf(ff.fmap, "\n\n\n");

	V2MapBrushArray_t &va = e0.va;
	size_t iv;
	for (iv = 0; iv < va.size(); iv++) {
		CV2MapBrush &v0 = va[iv];
		size_t ip;
		try {
			for (ip = 0; ip < v0.pa.size(); ip++) {
				CV2MapPlane &p0 = v0.pa[ip];
				My3DMath::Vtx3 p0v = -p0.v;
				float p0r = p0.fr;
				// Loot Code from Q3Radiant ->
				float maxx = -UTMOST;

				int i;
				int axis = -1;
				for (i = 0; i < 3; i++) {
					float v = fabs(p0v.v[i]);
					if (v > maxx) {
						axis = i;
						maxx = v;
					}
				}
				if (axis < 0) {
					continue;
				}
				My3DMath::Vtx3 v3Up;
				v3Up.Empty();
				switch (axis) {
				case 0:
				case 1:
					v3Up.z = 1.f;
					break;
				case 2:
					v3Up.x = 1.f;
					break;
				}

				float v = v3Up.DotProduct(p0v);
				v3Up += p0v.Mult(-v);
				v3Up.Normalize();

				My3DMath::Vtx3 o = p0v.Mult(p0r);

				My3DMath::Vtx3 v3R = v3Up.CrossProduct(p0v);

				p0.v3Up = v3Up;
				p0.v3R = v3R;

				v3R = v3R.Mult(UTMOST);
				v3Up = v3Up.Mult(UTMOST);

				BMapPtArray_t &am = *(bme.alm.insert(bme.alm.end(), BMapPtArray_t()));

				am.resize(4);

				am[0].v = o - v3R + v3Up;
				am[1].v = o + v3R + v3Up;
				am[2].v = o + v3R - v3Up;
				am[3].v = o - v3R - v3Up;
				//
				size_t ip2;
				for (ip2 = 0; ip2 < v0.pa.size(); ip2++) {
					CV2MapPlane &p1 = v0.pa[ip2];
					My3DMath::Vtx3 p1v = -p1.v;
					float p1r = p1.fr;

					if (ip == ip2) {
						continue;
					}
					float f0 = p0v.DotProduct(p1v);
					float f1 = fabs(p0r - p1r);
					if (0.999 < f0 && f1 < 0.01) {
						if (ip < ip2)
							throw char();
						continue;
					}
					My3DMath::Vtx3 p2v = -p1v;
					float p2r = -p1r;
					//
					{
						BMapPtArray_t am2;

						float vl[128];
						BYTE vs[128];
						int vp[3] = {0, 0, 0};
						size_t ia;
						for (ia = 0; ia < am.size(); ia++) {
							float f0 = am[ia].v.DotProduct(p1v);
							f0 -= p1r;
							vl[ia] = f0;
							if (false);
							else if (+ERR_EPS < f0) {
								vs[ia] = 0;
							}
							else if (f0 < -ERR_EPS) {
								vs[ia] = 1;
							}
							else {
								vs[ia] = 2;
							}
							vp[vs[ia]]++;
						}
						vl[ia] = vl[0];
						vs[ia] = vs[0];

						if (vp[0] == 0) {
							throw char();
						}
						if (vp[1] == 0) {

						} else {
							for (ia = 0; ia < am.size(); ia++) {
								My3DMath::Vtx3 av0 = am[ia].v;
								if (false);
								else if (vs[ia] == 2) {
									CBMapPt pt; pt.v = av0;
									am2.push_back(pt);
									continue;
								}
								else if (vs[ia] == 0) {
									CBMapPt pt; pt.v = av0;
									am2.push_back(pt);
								}

								if (vs[ia+1] == 2 || vs[ia+1] == vs[ia]) {
									continue;
								}

								My3DMath::Vtx3 av1 = am[(ia+1) % am.size()].v;

								float f0 = vl[ia] / (vl[ia] - vl[ia+1]);
								My3DMath::Vtx3 avm;
								size_t t;
								for (t = 0; t < 3; t++) {
									if (false);
									else if (p1v.v[t] == +1) {
										avm.v[t] = +p1r;
									}
									else if (p1v.v[t] == -1) {
										avm.v[t] = -p1r;
									}
									else {
										avm.v[t] = av0.v[t] + f0 * (av1.v[t] - av0.v[t]);
									}
								}

								CBMapPt pt; pt.v = avm;
								am2.push_back(pt);
							}
							am.swap(am2);
						}
					}
					//
				}
				// <- Loot Code from Q3Radiant

#ifdef _DEBUG_PLANE
				TRACE1(" P#(%5d)\n", 0+ip);
#endif // _DEBUG_PLANE

				size_t ia;
				for (ia = 0; ia < am.size(); ia++) {
					CBMapPt &t0 = am[ia];
					My3DMath::Vtx3 vo = t0.v + e0.o - p0v.Mult(p0r);
				//	float rv = vo.DotProduct(p0v);
					float tx = (vo).DotProduct(p0.v3R);
					float ty = (vo).DotProduct(p0.v3Up);
					float x1 = (p0.m32[0][0] * tx + p0.m32[0][1] * ty) + p0.m32[0][2];
					float y1 = (p0.m32[1][0] * tx + p0.m32[1][1] * ty) + p0.m32[1][2];

					t0.tv.x = x1;
					t0.tv.y = y1;

					fprintf(ff.fmap,
						"E(%s)V(%2d)P(%2d)A(%2d)T(%-10s) (%5d %5d %5d)(%8.3f %8.3f)\n"
						, (LPCSTR)e0.strName
						, 0+iv
						, 0+ip
						, 0+ia
						, (LPCSTR)p0.strTex.Right(10)
						, (int)t0.v.x
						, (int)t0.v.y
						, (int)t0.v.z
						, t0.tv.x
						, t0.tv.y
						);

#if 0
					static const float a = -90.f / 180.f * 3.14f;

					t0.tv.x = x1 * cos(a) + y1 * sin(a);
					t0.tv.y = x1 *-sin(a) + y1 * cos(a);
#endif

#ifdef _DEBUG_PLANE
					AfxTrace("(%5d,%5d,%5d)(%8.3f,%8.3f)\n"
						, (int)t0.v.x
						, (int)t0.v.y
						, (int)t0.v.z
						, t0.tv.x
						, t0.tv.y
						);
#endif // _DEBUG_PLANE
				}
				am.strTex = p0.strTex;
				bme.alm.push_back(am);
			}
		} catch (char) {

		}
	}

	return false;
}
