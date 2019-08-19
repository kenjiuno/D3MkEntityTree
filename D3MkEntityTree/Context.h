
// +--------------------------------------------------
// |
// | Context.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

#include "SizeVuff.h"
#include "RCI.h"
#include "NoMemberwiseCopy.h"
#include "Doom3ControlContext.h"
#include "Structure.h"
#include "Doom3OpenGLWnd.h"

using namespace MSHPP;

typedef enum {
	croNo = 0,
	croParseMaterial = 1,
	croParseDef = 2,
	croParseAll = (croNo |croParseMaterial |croParseDef),
	croMake_entityDef = 4 |croParseAll,
	croMake_Mapobjects = 8 |croParseMaterial,
	croMake_hhp = 16,
	croMake_Mapentity = 32 |croParseMaterial,

}	ContextRunOpts;

class CDoom3Context : public CDoom3ControlContext
{
public:
	// 
	CDoom3Context(CDoom3ContWorkpad &cwk)
		: cwk(cwk)
	{

	}
	// 
	void Run(CString strDoom3, CString strNewFolder, BOOL fExpose, BOOL fGamma, BOOL fMakeAvi, BOOL fARB_fragment_program, BOOL fARB_texture_env_dot3, ContextRunOpts cro, BOOL fLowProcessPriority, int rrt, GameSel gs)
	{
		this->strDoom3 = strDoom3;
		this->strNewFolder = strNewFolder;
		this->fMakeAvi = fMakeAvi ? true : false;
		this->cro = cro;
		this->fLowProcessPriority = fLowProcessPriority ? true : false;

		wndRenderer.rrt = (Renderer::OpenGLRenderTar)rrt;
		wndRenderer.fGamma = fGamma ? true : false;
		wndRenderer.fARB_fragment_program = fARB_fragment_program ? true : false;
		wndRenderer.fARB_texture_env_dot3 = fARB_texture_env_dot3 ? true : false;
		wndRenderer.fExpose = fExpose ? true : false;

		wk.gs = gs;

		Run();
	}

	// 
	static CString AseTex2Material(CString strTex);

protected:
	// 
	CString strDoom3, strNewFolder, strDoom3Base, strNewFolder_entityDef, strNewFolder_Mapobjs, strNewFolder_Entitymodel;
	// 
	CEntryMap m;
	// 
	CDoom3Workpad wk;
	// 
	Renderer::CDoom3OpenGLWnd wndRenderer;
	// 
	CHtmlMkThumb2 mkth2;
	// 
	bool fMakeAvi;
	// 
	ContextRunOpts cro;
	// 
	CDoom3ContWorkpad &cwk;
	// 
	CHHKTemplate hhkTempl;
	// 
	CHHCTemplate hhcTempl;
	// 
	CHHPTemplate hhpTempl;
	// 
	UINT iMapobjCur, nMapobjMax;
	// 
	bool fLowProcessPriority;
	// 
	CHwsMan hws;
	// 
	CHwsManq4 hwsq4;

	// 
	void Run();
	// 
	bool Scan();
	// 
	bool ScanDir(CString strDir, CString strPrefix);
	// 
	void Close();
	// 
	bool ReadDef();
	// 
	bool AssocDef();
	// 
	bool MkHtml();
	// 
	bool ReadSkin();
	// 
	bool ReadMaterial();
	// 
	bool ReadTest();
	// 
	bool AssocMapobjs();
	// 
	bool MkMo();
	// 
	bool MkHHP();
	// 
	bool MkEM();

	// 
	bool LoadEntry(CString strPath, SizeBuff &sb);
	// 
	bool LoadFileEntry(CString strPath, SizeBuff &sb);
	// 
	bool FindAll(CString strExtName, CEntryMap &m2);
	// 
	bool MkHtml_entityDef(CDoom3entityDef &entityDef, CString strHref);
	// 
	void entityDef_Classify(CString strName, list<CString> &classes);
	// 
	bool MkHtml_entityDef_class(CString strHref);
	// 
	bool ExportModel(CString strModel, CDoom3entityDef *entityDef, CString strHref, bool fSmaller = false);
	// 
	bool GetModelMesh(CString strModel, CString &strMesh);
	// 
	bool ExportAnim(CString strModel, CDoom3entityDef *entityDef, CString strAnim, CString strHref);
	// 
	bool GetAnimNames(CString strModel, list<CString> &anims);
	// 
	bool LoadMD5mesh(CString strMeshf, CMD5MeshDeck &m10);
	// 
	bool LoadMD5anim(CString strAnimf, CMD5AnimDeck &m10);
	// 
	bool IsEntryExisted(CString strPath);
	// 
	bool IsEntityDefExportOk(CString strName);
	// 
	bool MkMo_tree(FILE *f, LPCSTR psz, vector<bool> vThread);
	// 
	bool IsMapobjExportOk(CString strName);
	// 
	bool MkMo_leaf(CString strName, CString strHref);
	// 
	bool ExportMapobj(CString strModel, CString strHref, bool fMapobj, bool fSmaller = false);
	// 
	bool LoadLWO2(CString strMesh, CLWO2MeshDeck &mesh);
	// 
	bool LoadASE(CString strMesh, CAseMeshDeck &mesh);
	// 
	CString Solve_modelName(CString strModel);
	// 
	bool MkEM_Export(CString strMap, CV2MapDeck &mesh, int &nModels);
	// 
	bool MkEM_Capture(CV2MapEntity &e0, BMEntList_t &bmel);
	// 
	bool MkASE2Tformed(CAseMeshDeck &ase, My3DMath::BBox3 &b, CTformedMeshTab &tfmt, CTformedMesh &tfm);
	// 
	bool MkLWO2ToTformed(CLWO2MeshDeck &lwo2, My3DMath::BBox3 &b, CTformedMeshTab &tfmt, CTformedMesh &tfm);
	// 
	bool MkHtml_Modelentity(CString strHref, CString strPv, CString strName, CString strParentName);

	// 
	CHwsMan &getHwsMan()
	{
		if (wk.gs == gsQuake4)
			return hwsq4;
		return hws;
	}

	// 
	static CString NormPath(CString strParh)
	{
		if (1 <= strParh.GetLength() && strParh[0] == '/') {
			return strParh.Mid(1);
		}
		return strParh;
	}
	// 
	static bool IsAssoc(CString strName);
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
	static bool IsEntityVar(CString strText, CString &strVarType, CString &strVarName)
	{
		if (strText.Left(7) != "editor_")
			return false;
		strText = strText.Mid(7);
		int t = strText.Find(' ');
		if (t < 0)
			return false;
		strVarType = strText.Mid(0, t);
		strVarName = strText.Mid(t +1);
		return true;
	}
	// 
	static CString HtmlEnc(CString str)
	{
		str.Replace("&", "&amp;");
		str.Replace("<", "&lt;");
		str.Replace(">", "&gt;");
		return str;
	}
	// 
	static CString MkHref_entityDef(CString strName)
	{
		return "" + strName + ".html";
	}
	// 
	static CString MkHref_previewModel(CString strName)
	{
		return "model_pv/" + strName + ".jpg";
	}
	// 
	static CString MkHref_thumbModel(CString strName)
	{
		return "model_thumb/" + strName + ".jpg";
	}
	// 
	static CString MkHref_animModel(CString strModel, CString strAnim)
	{
		return "model_anim/" + strModel + "_" + strAnim + ".avi";
	}
	// 
	static CString MkHref_Mapobj(CString strName)
	{
		return "" + strName + ".html";
	}
	// 
	static CString MkHref_Modelentity_map(CString strMap, CString strName)
	{
		return "maps/" + strMap + "_" + strName + ".map";
	}
	// 
	static CString MkHref_Modelentity_preview(CString strMap, CString strName, bool fSmaller)
	{
		if (fSmaller) {
			return "model_thumb/" + strMap + "_" + strName + ".jpg";
		} else {
			return "model_pv/" + strMap + "_" + strName + ".jpg";
		}
	}
	// 
	static CString MkHref_Modelentity_html(CString strMap, CString strName)
	{
		return strMap + "_" + strName + ".html";
	}

};

class CMyDoom3Context : public CDoom3Context
{
	// 
	CDoom3ContWorkpad cwk;

public:
	// 
	CMyDoom3Context()
		: CDoom3Context(cwk)
	{

	}
	// 
	CDoom3Workpad &Get_wk() { return wk; }
	// 
	void Get_EntryMap(CEntryMap &m) { m = this->m; }
	// 
	void Close()
	{
		CDoom3Context::Close();
	}
	// 
	bool Addfs(CString strDir)
	{
		strDoom3Base = strDir;

		if (!Scan())
			return false;
		return true;
	}

};
