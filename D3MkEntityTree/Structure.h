
// +--------------------------------------------------
// |
// | Structure.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

#include <list>
#include <map>
#include <set>
#include <vector>

using namespace std;

#include <vfw.h>
#include <atlbase.h>

#include "My3DMath.h"
#include "DM2RPtr.h"
#include "Uzz.h"
#include "Doom3RP.h"
#include "MemVuff.h"

class CLL1R
{
public:
	// 
	virtual int ReadByte() = NULL;
};

class CLL1Memr : public CLL1R
{
public:
	// 
	BYTE *pData;
	// 
	UINT iPos, nMax;

	// 
	void Init(BYTE *pData, UINT nLen)
	{
		this->pData = pData;
		this->iPos = 0, nMax = nLen;
	}
	// 
	virtual int ReadByte()
	{
		int r = -1;
		if (iPos < nMax) {
			r = pData[iPos];
			iPos++;
		}
		return r;
	}

};

class CLL1Leader
{
	// 
	CLL1R &fIn;
	// 
	int iCur;

public:
	// 
	CLL1Leader(CLL1R &f)
		: fIn(f)
		, iCur(-1)
	{

	}
	// 
	bool Next()
	{
		iCur = fIn.ReadByte();

		if (iCur < 0)
			return false;
		return true;
	}
	// 
	int Cur()
	{
		return iCur;
	}
	// 
	bool IsEnd()
	{
		if (iCur < 0)
			return true;
		return false;
	}

};

class CLL1LeaderProxy
{
	// 
	CLL1Leader &f;

public:
	// 
	CLL1LeaderProxy(CLL1Leader &f)
		: f(f)
	{

	}
	// 
	bool Next() { return f.Next(); }
	// 
	int Cur() { return f.Cur(); }
	// 
	bool IsEnd() { return f.IsEnd(); }

protected:
	// 
	CLL1Leader &GetLeader() { return f; }

};

struct CDoom3entityDef {
	// 
	CString strName;
	// 
	map<CString, CString> m;
	// 
	list<CString> o;

};

struct CDoom3model {
	// 
	CString strName, strOffset;
	// 
	map<CString, CString> channels;
	// 
	map<CString, CString> anims;
	// 
	map<CString, CString> m;

};

typedef map<CString, CDoom3entityDef> CDoom3entityDefMap;

typedef map<CString, CDoom3model> CDoom3modelMap;

typedef map<CString, CString> VarMap;

typedef set<CString> StrSet_t;

typedef vector<pair<CString, CString> > SkinMaterialArray;

struct CDoom3skin {
	// 
    CString strName;
	// 
	VarMap m;

	// 
	SkinMaterialArray materialArray;

	// 
	CDoom3skin()
	{
		materialArray.reserve(10);
	}
};

typedef map<CString, CDoom3skin> CDoom3skinMap;

struct CSymo;

typedef vector<CSymo *> SymoArray;

struct CSymo {
	// 
	enum Symo {
		symoNo,
		symoDecimal,
		symoName,
		symoIndexo,
		symoAdd2,
		symoMul2,
		symoSub2,
		symoDiv2,
		symoMod2,
		symoAdd1,
		symoSub1,
		symoOpEqual,
		symoOpLess,
		symoOpLEqual,
		symoOpGreater,
		symoOpGEqual,
		symoOpNotEqual,
		symoOpLAnd,
		symoOpLOr,
	};

	// 
	Symo symo;
	// 
	CString strName;
	// 
	SymoArray var, ops;

	// 
	CSymo()
	{

	}
	// 
	CSymo(Symo symo)
		: symo(symo)
	{

	}
	// 
	CSymo(Symo symo, CString str)
		: symo(symo)
		, strName(str)
	{

	}
	// 
	CSymo(const CSymo &s)
	{
		*this = s;
	}
	// 
	const CSymo &operator =(const CSymo &s)
	{
		symo = s.symo;
		strName = s.strName;

		UINT i;

		for (i = 0; i < var.size(); i++) {
			delete var[i];
		}
		var.resize(s.var.size());
		for (i = 0; i < var.size(); i++) {
			var[i] = new CSymo(*s.var[i]);
		}

		for (i = 0; i < ops.size(); i++) {
			delete ops[i];
		}
		ops.resize(s.ops.size());
		for (i = 0; i < ops.size(); i++) {
			ops[i] = new CSymo(*s.ops[i]);
		}

		return s;
	}
	// 
	~CSymo()
	{
		Close();
	}
	// 
	void Close()
	{
		UINT i;
		for (i = 0; i < var.size(); i++) delete var[i]; var.clear();
		for (i = 0; i < ops.size(); i++) delete ops[i]; ops.clear();
	}
	// 
	int Prio()
	{
		return Prio(symo);
	}
	// 
	static bool IsOp(Symo symo)
	{
		switch (symo) {
		case symoOpEqual:
		case symoOpLess:
		case symoOpLEqual:
		case symoOpGreater:
		case symoOpGEqual:
		case symoOpNotEqual:
		case symoOpLAnd:
		case symoOpLOr:
		case symoAdd2:
		case symoSub2:
		case symoMul2:
		case symoDiv2:
		case symoMod2:
		case symoAdd1:
		case symoSub1:
			return true;
		}
		return false;
	}
	// 
	static int Prio(Symo symo)
	{
		switch (symo) {
		case symoOpEqual:
		case symoOpNotEqual:
			return 7;
		case symoOpLAnd:
		case symoOpLOr:
			return 8;
		case symoOpLess:
		case symoOpLEqual:
		case symoOpGreater:
		case symoOpGEqual:
			return 9;
		case symoAdd2:
		case symoSub2:
			return 10;
		case symoMul2:
		case symoDiv2:
		case symoMod2:
			return 11;
		case symoAdd1:
		case symoSub1:
			return 12;
		}
		return -1;
	}
	// 
	LPCSTR GetOpName() const
	{
		if (symo == symoNo) return "symoNo";
		if (symo == symoDecimal) return "symoDecimal";
		if (symo == symoName) return "symoName";
		if (symo == symoIndexo) return "symoIndexo";
		if (symo == symoAdd2) return "symoAdd2";
		if (symo == symoMul2) return "symoMul2";
		if (symo == symoSub2) return "symoSub2";
		if (symo == symoDiv2) return "symoDiv2";
		if (symo == symoMod2) return "symoMod2";
		if (symo == symoAdd1) return "symoAdd1";
		if (symo == symoSub1) return "symoSub1";
		if (symo == symoOpEqual) return "symoOpEqual";
		if (symo == symoOpLess) return "symoOpLess";
		if (symo == symoOpLEqual) return "symoOpLEqual";
		if (symo == symoOpGreater) return "symoOpGreater";
		if (symo == symoOpGEqual) return "symoOpGEqual";
		if (symo == symoOpNotEqual) return "symoOpNotEqual";
		if (symo == symoOpLAnd) return "symoOpLAnd";
		if (symo == symoOpLOr) return "symoOpLOr";
		return "?";
	}
	// 
	bool IsComplexExpression() const
	{
		UINT i;
		for (i = 0; i < var.size(); i++) {
			CSymo *so = var[i];
			switch (so->symo) {
			case symoName:
			case symoIndexo:
				return true;
			}
		}
		return false;
	}

};

typedef list<CSymo> SymoList;

typedef enum {
	texFlat,
	texAn, // addnormals
	texHm, // heightmap
	texMa, // makeAlpha
	texMi, // makeIntensity
	texSn, // smoothnormals
	texAx, // add
	texSx, // scale
	texIa, // invertAlpha
	texDs, // downsize // q4

}	Doom3TexMapType;

inline LPCSTR GetTexMapTypeName(Doom3TexMapType x)
{
	switch (x) {
	case texFlat: return "map";
	case texAn: return "addnormals";
	case texHm: return "heightmap";
	case texMa: return "makeAlpha";
	case texMi: return "makeIntensity";
	case texSn: return "smoothnormals";
	case texAx: return "add";
	case texSx: return "scale";
	case texIa: return "invertAlpha";
	}
	return "?";
}

struct CDoom3materialTex {
	// 
	Doom3TexMapType tt;
	// 
	CString strFlat, strAn, strHm, strMa, strMi;
	// 
	float fHmV;
};

typedef enum {
	bfNo,
	bfDstAlpha,
	bfDstColor,
	bfOne,
	bfOneMinusDstAlpha,
	bfOneMinusDstColor,
	bfOneMinusSrcAlpha,
	bfOneMinusSrcColor,
	bfSrcColor,
	bfZero,
	bfSrcAlpha,
	bfNone, // q4

	bfBlend,
	bfAdd,
	bfBumpMap,
	bfDiffuseMap,
	bfSpecularMap,
	bfFilter,
	bfDownSize, // q4

}	Doom3Blendf;

inline LPCSTR GetBlendfName(Doom3Blendf x)
{
	switch (x) {
	case bfNo: return "bfNo";
	case bfDstAlpha: return "bfDstAlpha";
	case bfDstColor: return "bfDstColor";
	case bfOne: return "bfOne";
	case bfOneMinusDstAlpha: return "bfOneMinusDstAlpha";
	case bfOneMinusDstColor: return "bfOneMinusDstColor";
	case bfOneMinusSrcAlpha: return "bfOneMinusSrcAlpha";
	case bfOneMinusSrcColor: return "bfOneMinusSrcColor";
	case bfSrcColor: return "bfSrcColor";
	case bfZero: return "bfZero";
	case bfSrcAlpha: return "bfSrcAlpha";
	case bfBlend: return "bfBlend";
	case bfAdd: return "bfAdd";
	case bfBumpMap: return "bfBumpMap";
	case bfDiffuseMap: return "bfDiffuseMap";
	case bfSpecularMap: return "bfSpecularMap";
	case bfFilter: return "bfFilter";
	}
	return "?";
}

struct CDoom3FlatIma {
	// 
	SizeBuff fv;

	// 
	UINT cx, cy;
	// 
	WORD nBitCount, nPitch;
	// 
	DWORD nDataLen;
	// 
	BYTE *pData;

	// 
	CDoom3FlatIma()
	{
		Close();
	}
	// 
	bool Create(const CDoom3FlatIma &s);
	// 
	bool Create(UINT cx, UINT cy, WORD nBitCount);
	// 
	void Close();
	// 
	BYTE *GetVert(UINT y);

	// 
	CDoom3FlatIma(const CDoom3FlatIma &s)
	{
		*this = s;
	}
	// 
	const CDoom3FlatIma &operator =(const CDoom3FlatIma &s);
};

struct CDoom3materialTexoMap {
	// 
	Doom3TexMapType tt;
	// 
	CString strName;
	// 
	size_t x0, x1;
	// 
	float v0, v1, v2, v3;

};

struct CDoom3materialTexo : vector<CDoom3materialTexoMap> {
	// 
	size_t iFirst;
};

namespace MaterialTexo
{

struct TexoTex {
	// 
	virtual bool LoadIma(CString strName, CDoom3FlatIma &rIma) = NULL;
};

class TexoEval
{
	// 
	struct Error { };

	// 
	TexoTex &rTex;
	// 
	CDoom3materialTexo &mt;

	// 
	bool eval(size_t i, CDoom3FlatIma &rIma);

public:
	// 
	TexoEval(TexoTex &rTex, CDoom3materialTexo &mt)
		: rTex(rTex), mt(mt)
	{

	}
	// 
	bool Eval(CDoom3FlatIma &rIma);

};

};

struct CDoom3materialIndent {
	// 
	VarMap m;
	// 
	Doom3Blendf bf1, bf2;
	// 
	CSymo soAlphaTest, soIf, so_scale, so_translate, so_rotate;
	// 
	CDoom3materialTexo tex;

	// 
	CDoom3materialIndent()
		: bf1(bfNo), bf2(bfNo)
	{

	}
	// 
	static bool Parse_blendf1(CString strValue, Doom3Blendf &bf)
	{
		if (false);
		else if (strValue == "none") bf = bfNone;
		else if (strValue == "blend") bf = bfBlend;
		else if (strValue == "add") bf = bfAdd;
		else if (strValue == "bumpmap") bf = bfBumpMap;
		else if (strValue == "diffusemap") bf = bfDiffuseMap;
		else if (strValue == "specularmap") bf = bfSpecularMap;
		else if (strValue == "filter") bf = bfFilter;
		else return false;
		return true;
	}
	// 
	static bool Parse_blendf2(CString strValue, Doom3Blendf &bf)
	{
		if (false);
		else if (strValue == "gl_dst_alpha") bf = bfDstAlpha;
		else if (strValue == "gl_dst_color") bf = bfDstColor;
		else if (strValue == "gl_one") bf = bfOne;
		else if (strValue == "gl_one_minus_dst_alpha") bf = bfOneMinusDstAlpha;
		else if (strValue == "gl_one_minus_dst_color") bf = bfOneMinusDstColor;
		else if (strValue == "gl_one_minus_src_alpha") bf = bfOneMinusSrcAlpha;
		else if (strValue == "gl_one_minus_src_color") bf = bfOneMinusSrcColor;
		else if (strValue == "gl_src_color") bf = bfSrcColor;
		else if (strValue == "gl_zero") bf = bfZero;
		else if (strValue == "gl_src_alpha") bf = bfSrcAlpha;
		else return false;
		return true;
	}
};

typedef vector<CDoom3materialIndent> CDoom3materialIndentArray;

struct CDoom3material {
	// 
	CString strName;
	// 
	VarMap m;

	// 
	CDoom3materialIndentArray v;
};

typedef map<CString, CDoom3material> CDoom3materialMap;

typedef map<CString, CString> InheritMap;

typedef multimap<CString, CString> ReverseInheritMap;

typedef set<CString> NameSet;

typedef vector<float> DecimalArray;

struct CDoom3table {
	// 
	int m;
	// 
	bool fSnap, fClamp;
	// 
	DecimalArray vec;

	// 
	CDoom3table()
	{
		m = 0;
		fSnap = fClamp = false;
	}
	// 
	float Eval(float x);
};

typedef map<CString, CDoom3table> CDoom3tableMap;

typedef enum {
	gsDoom3,
	gsQuake4,
}	GameSel;

struct CDoom3Workpad {
	// 
	CDoom3entityDefMap entityDefMap;
	// 
	CDoom3modelMap modelMap;
	// 
	InheritMap inheritMap;
	// 
	InheritMap inheritModelMap;
	// 
	ReverseInheritMap rinheritMap;
	// 
	CDoom3skinMap skinMap;
	// 
	CDoom3materialMap materialMap;
	// 
	NameSet mapobjsNames;
	// 
	CDoom3tableMap tableMap;
	// 
	GameSel gs;

	// 
	void CompleteInherit(CDoom3entityDef &entityDef);
	// 
	void CompleteInherit(CDoom3model &model);
	// 
	void Close()
	{
		entityDefMap.clear();
		modelMap.clear();
		inheritMap.clear();
		inheritModelMap.clear();
		rinheritMap.clear();
		skinMap.clear();
		materialMap.clear();
		mapobjsNames.clear();
		tableMap.clear();
	}
	// 
	bool Q4()
	{
		return (gs == gsQuake4);
	}

	// 
	CDoom3table *FindTable(CString strName)
	{
		CDoom3tableMap::iterator
			iterPos = tableMap.find(strName),
			iterEnd = tableMap.end();
		if (iterPos != iterEnd)
			return &iterPos->second;
		return NULL;
	}
};

struct CDoom3ContWorkpad {
	// 
	NameSet entityDefExport, mapobjsExport;
	// 
	bool fOnly_entityDef, fOnly_mapobjs;
};

class CSparseBase : public CLL1LeaderProxy
{
public:
	// 
	CSparseBase(CLL1Leader &f)
		: CLL1LeaderProxy(f)
	{

	}

protected:
	// 
	class Error { };

	// 
	void KillMe()
	{
		throw Error(); // CSparseBase
	}
	// 
	bool SkipWs();
	// 
	bool IsWs();
	// 
	bool IsLineBReak();
	// 
	bool IsSPC();
	// 
	bool IsInlineComment();
	// 
	void ReadTextQuoted(CString &strText);
	// 
	void ForceMatch(char c, bool fSkipAlWs = true);
	// 
	bool IsMatch(char c, bool fSkipAlWs = true);
	// 
	bool IsMatchFinally(char c, bool fSkipAlWs = true);
	// 
	bool SkipAlWsComment();
	// 
	bool ReadTexto(CString &strText);
	// 
	bool ReadDecimalo(CString &strText);
	// 
	bool SkipMidBracketContext();
	// 
	bool SkipTokensUntilMidBracket();
	// 
	void SkipTokensUntilLf();
	// 
	void ReadInlineTexto(CString &strText);
	// 
	void ForceReadTexto(CString &strText);
	// 
	bool ReadTextTokens(CString &strText);
	// 
	void Force1(bool fIf)
	{
		if (!fIf) KillMe();
	}

	// 
	static bool ParseInt(CString &strValue, int &nValue)
	{
		char *psz = NULL;
		nValue = strtol(strValue, &psz, 10);
		return psz && (*psz == 0);
	}
	// 
	static bool ParseFloat(CString &strValue, float &fValue)
	{
		char *psz = NULL;
		fValue = (float)strtod(strValue, &psz);
		return psz && (*psz == 0);
	}

};

class CSparseDef : public CSparseBase
{
	// 
	CDoom3Workpad &wk;

	// 
	void KillMe()
	{
		CSparseBase::KillMe(); // CSparseDef
	}
	// 
	bool ReadText1(CString &strText, bool fQuotedCondOk = false);
	// 
	void Parse_entityDef();
	// 
	void Parse_model();
	// 
	bool ReadText2(CString &strText, bool fQuotedCondOk = false);
	// 
	void ReadTextBracketInside(CString &strText);
	// 
	bool ReadText3(CString &strText);
	// 
	void Parse_sound();
	// 
	void Parse_export();
	// 
	bool ReadText4(CString &strText);
	// 
	void Parse_mapDef();
	// 
	void Parse_any();

	// 
	void Parse_skin();

	// 
	void Parse_material(CString strName);
	// 
	void Parse_indent(CDoom3materialIndentArray &v);
	// 
	void Parse_table();

	// 
	void Parse_guide();
	// 
	void Parse_camera();

	// 
	void Parse_map(CDoom3materialTexo &tex);

	// 
	bool Q4() { return wk.Q4(); }

public:
	// 
	CSparseDef(CLL1Leader &fIn, CDoom3Workpad &wk)
		: CSparseBase(fIn)
		, wk(wk)
	{

	}
	// 
	bool Parse();
};

struct CMD5AnimFrameBasis {
	// 
	My3DMath::Vtx3 o, q;

	// 
	My3DMath::Mtx m;
};

struct CMD5AnimJoint {
	// 
	CString strName;
	// 
	int iJointRef, nFlags, iVarIndex;

};

struct CMD5AnimFramePatch : CMD5AnimFrameBasis {

};

typedef vector<CMD5AnimFramePatch> MD5AnimFramePatchArray;

typedef vector<CMD5AnimJoint> MD5AnimJointArray;

typedef vector<MD5AnimFramePatchArray> MD5AnimFrameArray;

struct CMD5AnimDeck {
	// 
	MD5AnimJointArray joints;
	// 
	MD5AnimFrameArray frames;
	// 
	MD5AnimFramePatchArray baseframe;
	// 
	int nFrameRate;
	// 
	int nAnimatedComp;

	// 
	MD5AnimFramePatchArray basejoint;
	// 
	bool basejointAvail;

	// 
	CMD5AnimDeck()
	{
		basejointAvail = false;
	}
};

class CSparseMD5Anim : public CSparseBase
{
	// 
	CMD5AnimDeck &anim;

	// 
	void Parse_hierarchy();
	// 
	void Parse_bounds();
	// 
	void Parse_baseframe();
	// 
	void Parse_frame();
	// 
	void ReadVtx3(My3DMath::Vtx3 &v);

public:
	// 
	CSparseMD5Anim(CLL1Leader &fIn, CMD5AnimDeck &anim)
		: CSparseBase(fIn)
		, anim(anim)
	{

	}
	// 
	bool Parse();

};

namespace NfMD5
{

struct MD5MeshJoint_t {
	// 
	MD5MeshJoint_t *pRefJ;
	// 
	CString strName;
	// 
	My3DMath::Vtx3 o;
	// 
	My3DMath::Quatern q;

	// 
	int iJ;
};

struct MD5MeshWeight_t {
	// 
	MD5MeshJoint_t *pJ;
	// 
	My3DMath::Vtx3 o;
	// 
	float nMass;

	// 
	int iJ;
};

typedef vector<MD5MeshWeight_t *> MD5MeshWeightPtrArray_t;

struct MD5MeshVert_t {
	// 
	MD5MeshWeightPtrArray_t ppW;
	// 
	My3DMath::Vtx2 o;

	// 
	int iW, nW;
};

struct MD5MeshTri_t {
	// 
	MD5MeshVert_t *pV[3];

	// 
	int iV[3];
};

typedef vector<MD5MeshJoint_t> MD5MeshJointArray_t;

typedef vector<MD5MeshWeight_t> MD5MeshWeightArray_t;

typedef vector<MD5MeshVert_t> MD5MeshVertArray_t;

typedef vector<MD5MeshTri_t> MD5MeshTriArray_t;

struct MD5MeshModel_t {
	// 
	MD5MeshWeightArray_t aW;
	// 
	MD5MeshVertArray_t aV;
	// 
	MD5MeshTriArray_t aT;
	// 
	CString strShader;

};

typedef vector<MD5MeshModel_t> MD5MeshModelArray_t;

struct MD5MeshDeck_t {
	// 
	MD5MeshJointArray_t aJ;
	// 
	MD5MeshModelArray_t aM;
};

struct MD5AnimBound_t {
	// 
	My3DMath::Vtx3 vmin, vmax;

};

struct MD5AnimPatch_t {
	// 
	My3DMath::Vtx3 o, p;
	// 
	My3DMath::Quatern q;
};

typedef vector<MD5AnimPatch_t> MD5AnimFrame_t;

struct MD5AnimJoint_t {
	// 
	CString strName;
	// 
	MD5AnimJoint_t *pRefJ;
	// 
	int f6;

	// 
	int iRefJ;
};

typedef vector<MD5AnimBound_t> MD5AnimBoundArray_t;

typedef vector<MD5AnimFrame_t> MD5AnimFrameArray_t;

typedef vector<MD5AnimJoint_t> MD5AnimJointArray_t;

struct MD5AnimDeck_t {
	// 
	MD5AnimBoundArray_t bounda;
	// 
	MD5AnimFrameArray_t framea;
	// 
	MD5AnimJointArray_t jointa;
	// 
	MD5AnimFrame_t baseframe;

	// 
	int nFrameRate, nAnimatedComp;

};

class ParseMD5Mesh_t : public CSparseBase
{
	// 
	MD5MeshDeck_t &mesh;
	// 
	UINT iCurMesh;

	// 
	void Parse_joints();
	// 
	void ReadVtx3(My3DMath::Vtx3 &v);
	// 
	void ReadVtx2(My3DMath::Vtx2 &v);
	// 
	void Parse_mesh();

public:
	// 
	ParseMD5Mesh_t(CLL1Leader &fIn, MD5MeshDeck_t &mesh)
		: CSparseBase(fIn)
		, mesh(mesh)
	{

	}
	// 
	bool Parse();

};

class ParseMD5Anim_t : public CSparseBase
{
	// 
	MD5AnimDeck_t &anim;

	// 
	void Parse_hierarchy();
	// 
	void Parse_bounds();
	// 
	void Parse_baseframe();
	// 
	void Parse_frame();
	// 
	void ReadVtx3(My3DMath::Vtx3 &v);

public:
	// 
	ParseMD5Anim_t(CLL1Leader &fIn, MD5AnimDeck_t &anim)
		: CSparseBase(fIn)
		, anim(anim)
	{

	}
	// 
	bool Parse();

};

};

struct CMD5MeshJoint {
	// 
	CString strName;
	// 
	int iJointRef;

	// 
	CMD5AnimFrameBasis ok;
};

struct CMD5MeshMeshVert {
	// 
	My3DMath::Vtx2 o;
	// 
	int iWeight, nWeights;
};

struct CMD5MeshMeshTri {
	// 
	int v[3];
};

struct CMD5MeshMeshWeight {
	// 
	int iJoint;
	// 
	float nMass;
	// 
	My3DMath::Vtx3 o;
};

typedef vector<CMD5MeshMeshVert> MD5MeshMeshVertArray;

typedef vector<CMD5MeshMeshTri> MD5MeshMeshTriArray;

typedef vector<CMD5MeshMeshWeight> MD5MeshMeshWeightArray;

typedef vector<CMD5MeshJoint> MD5MeshJointArray;

struct CMD5MeshMesh {
	// 
	VarMap m;

	// 
	MD5MeshMeshVertArray verts;
	// 
	MD5MeshMeshTriArray tris;
	// 
	MD5MeshMeshWeightArray weights;

	// 
	CMD5MeshMesh()
	{

	}
	// 
	void InterpolateVertex(MD5MeshJointArray &j, UINT iVert, My3DMath::Vtx3 &v);

};

typedef vector<CMD5MeshMesh> MD5MeshMeshArray;

struct CMD5MeshDeck {
	// 
	MD5MeshJointArray joints;
	// 
	MD5MeshMeshArray meshes;

	// 
	CMD5AnimFrameBasis basejoint;
	// 
	bool basejointAvail;

	// 
	CMD5MeshDeck()
	{
		basejointAvail = false;
	}
};

class CSparseMD5Mesh : public CSparseBase
{
	// 
	CMD5MeshDeck &mesh;
	// 
	UINT iCurMesh;

	// 
	void Parse_joints();
	// 
	void ReadVtx3(My3DMath::Vtx3 &v);
	// 
	void ReadVtx2(My3DMath::Vtx2 &v);
	// 
	void Parse_mesh();

public:
	// 
	CSparseMD5Mesh(CLL1Leader &fIn, CMD5MeshDeck &mesh)
		: CSparseBase(fIn)
		, mesh(mesh)
	{

	}
	// 
	bool Parse();

};

class CSparseSkin : public CSparseBase
{
	// 
	CDoom3Workpad &wk;

	// 
	void Parse_skin();

public:
	// 
	CSparseSkin(CLL1Leader &fIn, CDoom3Workpad &wk)
		: CSparseBase(fIn)
		, wk(wk)
	{

	}
	// 
	bool Parse();

};

typedef CDM2SPtr<Uzz::CUzz> CUzzPtr;

class CUzzExtractor
{
public:
	// 
	CUzzPtr pUz;
	// 
	UINT iEntry;

	// 
	virtual bool GetData(SizeBuff &sb)
	{
		return pUz->Ex(iEntry, sb);
	}
};

class IFileEntity : public IRCI
{
public:
	// 
	virtual CString GetPath() = NULL;
	// 
	virtual bool GetData(SizeBuff &fv) = NULL;
	// 
	virtual bool GetModifiedTime(FILETIME &ft) = NULL;
};

typedef CDM2SPtr<IFileEntity> CFileEntryPtr;

class COSFileEntity : public CRCI, public IFileEntity
{
	// 
	CString strFile, strPath;

public:
	// 
	COSFileEntity(LPCTSTR psz, LPCTSTR pszPath)
	{
		strFile = psz;
		strPath = pszPath;
	}
	// 
	virtual CString GetPath()
	{
		return strPath;
	}
	// 
	virtual bool GetData(SizeBuff &fv)
	{
		CFile fileIn;
		ULONGLONG nLen64;
		int nLen;
		if (true
			&& fileIn.Open(strFile, 0 |CFile::modeRead |CFile::shareDenyWrite)
			&& (nLen64 = fileIn.GetLength(), nLen = (int)nLen64, nLen64 < 1024*1024*10)
			&& fv.Alloc(nLen)
			&& fileIn.Read(fv.GetData(), nLen) == nLen
		) {
			return true;
		}
		return false;
	}
	// 
	virtual bool GetModifiedTime(FILETIME &ft)
	{
		CFile fileIn;
		if (true
			&& fileIn.Open(strFile, 0 |CFile::modeRead)
			&& GetFileTime(fileIn.m_hFile, NULL, NULL, &ft)
		) {
			return true;
		}
		return false;
	}

	// 
	virtual LONG AddRef() { return CRCI::AddRef(); }
	// 
	virtual LONG Release() { return CRCI::Release(); }

};

class CZipFileEntity : public CRCI, public IFileEntity
{
	// 
	CString strPath;
	// 
	CUzzExtractor ze;
	// 
	FILETIME ft;

public:
	// 
	CZipFileEntity(const CUzzExtractor &ze, const FILETIME &ft)
		: ze(ze), ft(ft)
	{

	}

	// 
	virtual CString GetPath()
	{
		return strPath;
	}
	// 
	virtual bool GetData(SizeBuff &sb)
	{
		return ze.GetData(sb);
	}
	// 
	virtual bool GetModifiedTime(FILETIME &ft)
	{
		ft = this->ft;
		return true;
	}

	// 
	virtual LONG AddRef() { return CRCI::AddRef(); }
	// 
	virtual LONG Release() { return CRCI::Release(); }

};

typedef map<CString, CFileEntryPtr> CEntryMap;

class CHtmlMkThumb2
{
public:
	// 
	FILE *f;
	// 
	int iCol;
	// 
	int iTable;

	// 
	CHtmlMkThumb2()
	{
		Reset();
	}
	// 
	void Reset()
	{
		f = NULL;
		iCol = -1;
	}
	// 
	void MkHead(bool fHead);
	// 
	void MkTableHead(bool fHead, LPCTSTR pszName = NULL);
	// 
	void AddThumb(CString strName, CString strHref, CString strHrefThere);
};

class CAviMk
{
	// 
	CComPtr<IAVIFile> pAvif;
	// 
	CComPtr<IAVIStream> pAvisVid;
	// 
	bool fUseComp, fCont;
	// 
	UINT iCurFrame;
	// 
	COMPVARS cv;
	// 
	BITMAPINFO bi;
	// 
	UINT nSizeImage;
	// 
	CString strAvif;

public:
	// 
	CAviMk()
	{
		ZeroMemory(&cv, sizeof(cv));
	}
	// 
	~CAviMk()
	{
		Close();
	}
	// 
	bool Create(CString strAvi, int nFrameRate, CSize size);
	// 
	void Close(bool fOk = false);
	// 
	bool AddFrame(void *pvData);

	// 
	UINT GetSizeImage() const { return nSizeImage; }

};

struct CSym {
	// 
	enum Sym {
		symNo,
		symDecimal,	// 0.0 .0 1.
		symIndexo,	// [
		symIndexc,	// ]
		symBracketo,// (
		symBracketc,// )
		symAdd,		// +
		symMul,		// *
		symSub,		// -
		symDiv,		// /
		symMod,		// %
		symOpLess,		// <
		symOpLEqual,	// <=
		symOpGreater,	// >
		symOpGEqual,	// >=
		symComma,	// ,
		symName,	// parm7
		symOpEqual,	// ==
		symOpNotEqual,	// !=
		symOpLAnd,	// &&
		symOpLOr,	// ||
	};
	// 
	Sym sym;
	// 
	CString strName;

	// 
	CSym() { }
	// 
	CSym(Sym sym)
		: sym(sym)
	{

	}
	// 
	bool IsValue() const
	{
		switch (sym) {
		case symDecimal:
		case symName:
			return true;
		}
		return false;
	}
	// 
	bool IsOp1() const
	{
		switch (sym) {
		case symAdd:
		case symSub:
			return true;
		}
		return false;
	}
	// 
	bool IsOp2() const
	{
		switch (sym) {
		case symAdd:
		case symMul:
		case symSub:
		case symDiv:
		case symMod:
		case symOpLess:
		case symOpLEqual:
		case symOpGreater:
		case symOpGEqual:
		case symOpEqual:
		case symOpNotEqual:
		case symOpLAnd:
		case symOpLOr:
			return true;
		}
		return false;
	}
	// 
	LPCSTR GetOpName() const
	{
		if (sym == symNo) return "symNo";
		if (sym == symDecimal) return "symDecimal";
		if (sym == symIndexo) return "symIndexo";
		if (sym == symIndexc) return "symIndexc";
		if (sym == symBracketo) return "symBracketo";
		if (sym == symBracketc) return "symBracketc";
		if (sym == symAdd) return "symAdd";
		if (sym == symMul) return "symMul";
		if (sym == symSub) return "symSub";
		if (sym == symDiv) return "symDiv";
		if (sym == symMod) return "symMod";
		if (sym == symOpLess) return "symOpLess";
		if (sym == symOpLEqual) return "symOpLEqual";
		if (sym == symOpGreater) return "symOpGreater";
		if (sym == symOpGEqual) return "symOpGEqual";
		if (sym == symComma) return "symComma";
		if (sym == symName) return "symName";
		if (sym == symOpEqual) return "symOpEqual";
		if (sym == symOpNotEqual) return "symOpNotEqual";
		if (sym == symOpLAnd) return "symOpLAnd";
		if (sym == symOpLOr) return "symOpLOr";
		return "?";
	}

};

typedef list<CSym> SymList;

class CSparseExpr : public CSparseBase
{
	// 
	CSym sym;

	// 
	bool ParseToken();
	// 
	void Parse_decimal();
	// 
	void Parse_name();
	// 
	bool Next(bool fAccum = true);

	// 
	static inline bool Is_Name(int a)
	{
		if (a == '_' || a == '.' || ('0' <= a && a <= '9') || ('a' <= a && a <= 'z') || ('A' <= a && a <= 'Z'))
			return true;
		return false;
	}

public:
	// 
	CSparseExpr(CLL1Leader &f)
		: CSparseBase(f)
	{

	}
	// 
	bool Parse(SymList &syms);

};

class CExprBinder
{
	// 
	struct Error { };

	// 
	SymList &syms;
	// 
	SymList::iterator iterPos, iterEnd;
	// 
	SymoArray *sa;

	// 
	bool IsEnd() { return iterPos == iterEnd; }
	// 
	bool Next()
	{
		if (IsEnd()) return false;
		iterPos++;
		return true;
	}
	// 
	const CSym &Cur() { return *iterPos; }

	// 
	void KillMe()
	{
		throw Error(); // CExprBinder
	}

	// 
	void Parse_1(CSym::Sym symEnd, bool fRoot = false);
	// 
	void Parse_op1();
	// 
	void Parse_op2();
	// 
	void Parse_value();

	// 
	void AddSymo_ops(CSymo::Symo symo)
	{
		CSymo *p1 = new CSymo(symo);
		CSymo *p0 = sa->back();
		while (p0->ops.size() != 0) {
			int v1 = p0->ops.back()->Prio();
			int v2 = p1->Prio();
			if (v1 < v2)
				break;
			p0->var.push_back(p0->ops.back()); p0->ops.pop_back();
		}
		sa->back()->ops.push_back(p1);
	}
	// 
	void AddSymo_var(CSymo::Symo symo)
	{
		CSymo *p = new CSymo(symo, Cur().strName);
		sa->back()->var.push_back(p);
	}
	// 
	void AddSymo_var(CSymo::Symo symo, CSym sym)
	{
		CSymo *p = new CSymo(symo, sym.strName);
		sa->back()->var.push_back(p);
	}
	// 
	void AddSymo_indexo(CSym sym)
	{
		CSymo *p = sa->back(); sa->pop_back();
		p->symo = CSymo::symoIndexo;
		p->strName = sym.strName;
		sa->back()->var.push_back(p);
	}
	// 
	void AddSymo_bracketo()
	{
		CSymo *p = sa->back(); sa->pop_back();
		sa->back()->var.insert(sa->back()->var.end(), p->var.begin(), p->var.end());
		p->var.clear();
		delete p;
	}
	// 
	void EndSymo()
	{
		CSymo *p = sa->back();

		for (; p->ops.size() != 0; p->ops.pop_back()) p->var.push_back(p->ops.back());
	}

public:
	// 
	CSymo so;

	// 
	CExprBinder(SymList &syms)
		: syms(syms)
	{

	}
	// 
	~CExprBinder()
	{
		Close();
	}
	// 
	void Close();
	// 
	bool Parse();
	// 
	void Printo();
	// 
	void Printo(SymoArray &sa, int f);

};

namespace EvalOp
{
#define EPS 1E-5
	inline double Add1(double v1)	{ return +v1; }
	inline double Sub1(double v1)	{ return -v1; }

	inline double Add2(double v1, double v2)	{ return v1 + v2; }
	inline double Mul2(double v1, double v2)	{ return v1 * v2; }
	inline double Sub2(double v1, double v2)	{ return v1 - v2; }
	inline double Div2(double v1, double v2)	{ return v1 / v2; }
	inline double Mod2(double v1, double v2)	{ return fmod(v1, v2); }
	inline double OpEqual(double v1, double v2)	{ return fabs(v1 - v2) < EPS; }
	inline double OpLess(double v1, double v2)		{ return v1 < v2; }
	inline double OpLEqual(double v1, double v2)	{ return v1 <= v2; }
	inline double OpGreater(double v1, double v2)	{ return v1 > v2; }
	inline double OpGEqual(double v1, double v2)	{ return v1 >= v2; }
	inline double OpNotEqual(double v1, double v2)	{ return fabs(v1 - v2) > EPS; }
	inline double OpLAnd(double v1, double v2)		{ return (int)v1 && (int)v2; }
	inline double OpLOr(double v1, double v2)		{ return (int)v1 || (int)v2; }
};

class CEvalSampler
{
public:
	// 
	virtual float EvalTable(CString strName, float x) { return 0; };
	// 
	virtual float EvalNameo(CString strName) { return 0; }
};

class CEvalExpr
{
	// 
	struct Error { };

	// 
	CEvalSampler &evalTable;
	// 
	CEvalSampler &evalNameo;

	// 
	void KillMe()
	{
		throw Error(); // CEvalExpr
	}

	// 
	void eval(SymoArray &sa);

public:
	// 
	vector<double> vals;
	// 
	CEvalExpr(CEvalSampler &eval)
		: evalTable(eval)
		, evalNameo(eval)
	{

	}

	// 
	bool Eval(SymoArray &sa);
	// 
	bool Eval(CSymo &so, UINT iIdx);
	// 
	bool EvalIndexo(CSymo &so);

};

class CSparseMapExpr : public CSparseBase
{
	// 
	CDoom3materialTexo &mt;

	// 
	size_t Parse_map();

public:
	// 
	CSparseMapExpr(CLL1Leader &f, CDoom3materialTexo &mt)
		: CSparseBase(f)
		, mt(mt)
	{

	}
	// 
	bool Parse();

};

struct CLWO2MeshVertex {
	// 
	My3DMath::Vtx3 v;
	// 
	My3DMath::Vtx2 vt0;

	// 
	CLWO2MeshVertex()
	{
		v.Empty();
		vt0.Empty();
	}
};

typedef vector<CLWO2MeshVertex> LWO2MeshVertexArray;

struct CLWO2MeshPolyMap {
	// 
	DWORD v, vt0;
};

struct CLWO2MeshPoly {
	// 
	vector<CLWO2MeshPolyMap> verts;
	// 
	WORD texture;
};

typedef vector<CLWO2MeshPoly> LWO2MeshPolyArray;

struct CLWO2MeshVertexPatchKey {
	// 
	DWORD vert, poly;

	// 
	bool operator <(const CLWO2MeshVertexPatchKey &s) const
	{
		if (vert < s.vert) return true;
		if (vert > s.vert) return false;
		if (poly < s.poly) return true;
		if (poly > s.poly) return false;

		return false;
	};
};

struct CLWO2MeshVertexPatchVal {
	// 
	My3DMath::Vtx2 vt0;
};

typedef map<CLWO2MeshVertexPatchKey, CLWO2MeshVertexPatchVal> LWO2MeshVertexPatchMap;

typedef vector<CString> LWO2MeshTexArray;

struct CLWO2MeshDeck {
	// 
	LWO2MeshVertexArray av;
	// 
	LWO2MeshPolyArray ap;
	// 
	LWO2MeshTexArray at0;

};

class CSparseLWO2 : public CMemVuff
{
	// 
	struct Error { };

	// 
	CLWO2MeshDeck &mesh;

	// 
	void KillMe()
	{
		throw Error();
	}
	// 
	void Read4(DWORD &x4)
	{
		if (!ReadDWord(x4))
			KillMe();
	}
	// 
	void Read4(float &x4)
	{
		if (!ReadDWord((DWORD &)x4))
			KillMe();
	}
	// 
	void Read4r(DWORD &x4)
	{
		if (!SurelyReadBytes(&x4, 4))
			KillMe();
	}
	// 
	void Sure4r(DWORD x4)
	{
		DWORD x;
		Read4r(x);
		if (x != x4)
			KillMe();
	}
	// 
	void Skip1()
	{
		BYTE x;
		if (!ReadByte(x))
			KillMe();
	}
	// 
	void Skip(UINT v)
	{
		if (!SurelySkipBytes(v))
			KillMe();
	}
	// 
	void Parse_1();
	// 
	bool IsEnd()
	{
		return (iData == nData);
	}
	// 
	void ReadVtx3(My3DMath::Vtx3 &v3)
	{
		Read4(v3.x);
		Read4(v3.y);
		Read4(v3.z);
	}
	// 
	void ReadVtx2(My3DMath::Vtx2 &v2)
	{
		Read4(v2.x);
		Read4(v2.y);
	}
	// 
	void ReadX(DWORD &x)
	{
		WORD x0;
		Read2(x0);
		if (0xFF00 < x0) {
			WORD x1;
			Read2(x1);
			x = MAKELONG(x1, x0);
		} else {
			x = x0;
		}
	}
	// 
	void Read2(WORD &x2)
	{
		if (!ReadWord(x2))
			KillMe();
	}
	// 
	void Sure2(WORD x2)
	{
		WORD x;
		Read2(x);
		if (x != x2)
			KillMe();
	}
	// 
	void ReadName()
	{
		CString str; ReadName(str);
	}
	// 
	void ReadName(CString &strName)
	{
		strName.Empty();

		UINT i = 0;
		for (; ; ) {
			BYTE x;
			Read1(x);
			i++;
			if (x == 0) break;
			strName += (char)x;
		}
		if (i & 1) {
			Skip1();
		}
	}
	// 
	void Read1(BYTE &x)
	{
		if (!ReadByte(x))
			KillMe();
	}

public:
	// 
	CSparseLWO2(CLWO2MeshDeck &mesh)
		: mesh(mesh)
	{

	}
	// 
	bool Parse();

};

struct CAseMeshVertex {
	// 
	My3DMath::Vtx3 v;
};

typedef vector<CAseMeshVertex> AseMeshVertexArray;

struct CAseMeshFace {
	// 
	DWORD v[3], tf[3];
	// 
	WORD tex0;

	// 
	CAseMeshFace()
	{
		tex0 = 0;
	}
};

typedef vector<CAseMeshFace> AseMeshFaceArray;

struct CAseMeshTVert {
	// 
	My3DMath::Vtx2 vt0;
};

typedef vector<CAseMeshTVert> AseMeshTVertArray;

struct CAseMeshMaterial {
	// 
	CString strTex;
};

typedef vector<CAseMeshMaterial> AseMeshMaterialArray;

struct CAseMeshGEOM {
	// 
	AseMeshVertexArray mva;
	// 
	AseMeshFaceArray mfa;
	// 
	AseMeshTVertArray mtva;
};

inline void swap(CAseMeshGEOM &r0, CAseMeshGEOM &r1)
{
	r0.mva.swap(r1.mva);
	r0.mfa.swap(r1.mfa);
	r0.mtva.swap(r1.mtva);
}

typedef vector<CAseMeshGEOM> AseMeshGEOMArray;

struct CAseMeshDeck {
	// 
	AseMeshMaterialArray mma;
	// 
	AseMeshGEOMArray mza;

};

class CSparseASE : public CSparseBase
{
	// 
	CAseMeshDeck &mesh;

	// 
	void KillMe()
	{
		CSparseBase::KillMe(); // CSparseASE
	}

	// 
	void Parse_1();
	// 
	void ReadLeadTexto(CString &str)
	{
		ForceMatch('*', false);
		if (!ReadTexto(str))
			KillMe();
	}
	// 
	void Parse_GEOMOBJECT();
	// 
	void Parse_MESH();
	// 
	void Parse_MATERIAL_LIST();
	// 
	bool ReadNameo(CString &strText);
	// 
	void SkipAny(int nLv = 0);

public:
	// 
	CSparseASE(CLL1Leader &fIn, CAseMeshDeck &mesh)
		: CSparseBase(fIn)
		, mesh(mesh)
	{

	}
	// 
	bool Parse();

};

class CTextPlainMk
{
public:
	// 
	bool Open(LPCSTR psz)
	{
		Close();
		if (f = fopen(psz, "wt")) {
			return true;
		}
		return false;
	}
	// 
	void Close()
	{
		if (f) fclose(f), f = NULL;
	}

protected:
	// 
	FILE *f;

	// 
	CTextPlainMk()
	{
		f = NULL;
	}
	// 
	~CTextPlainMk()
	{
		Close();
	}

};

class CHwsMan
{
public:
	// 
	virtual CString gethhp() { return "D3hhp.hhp"; }
	// 
	virtual CString gethhc() { return "D3hhp.hhc"; }
	// 
	virtual CString gethhk() { return "D3hhp.hhk"; }
	// 
	virtual UINT getindexres();
	// 
	virtual UINT gethhpres();
};

class CHwsManq4 : public CHwsMan
{
	// 
	virtual CString gethhp() { return "Q4hhp.hhp"; }
	// 
	virtual CString gethhc() { return "Q4hhp.hhc"; }
	// 
	virtual CString gethhk() { return "Q4hhp.hhk"; }
	// 
	virtual UINT getindexres();
	// 
	virtual UINT gethhpres();
};

namespace MSHPP
{

class CHHKMk : public CTextPlainMk
{
public:
	// 
	void MkHeader(bool fHeader = true);
	// 
	void StartKw(LPCSTR pszDispName);
	// 
	void WriteKwHref(LPCSTR pszDispAlt, LPCSTR pszHref);
	// 
	void EndKw();
};

class CHHPMk : public CTextPlainMk
{
	// 
	CHwsMan &rhw;

public:
	// 
	CHHPMk(CHwsMan &rhw): rhw(rhw) { }

	// 
	bool Mk(LPCSTR pszFiles);

};

class CHHCMk : public CTextPlainMk
{
public:
	// 
	void MkHeader(bool fHeader = true);
	// 
	void StartDir(LPCSTR pszDispName, LPCSTR pszHref);
	// 
	void WriteItem(LPCSTR pszDispName, LPCSTR pszHref);
	// 
	void EndDir();
};

class CHHKTemplate
{
	// 
	typedef multimap<CString, CString> KwMMap;

	// 
	KwMMap kmm;

public:
	// 
	void AddKw(CString pszKw, CString pszHref)
	{
		kmm.insert(make_pair(pszKw, pszHref));
	}

	// 
	bool Commit(CHHKMk &mk);
};

class CHHCTemplate
{
	// 
	typedef multimap<CString, CString> KwMMap;

	// 
	KwMMap km_entityDef, km_Mapobject;

public:
	// 
	void Add_entityDef(CString strName, CString strHref)
	{
		km_entityDef.insert(make_pair(strName, strHref));
	}
	// 
	void Add_Mapobject(CString strName, CString strHref)
	{
		km_Mapobject.insert(make_pair(strName, strHref));
	}

	// 
	bool Commit(CHHCMk &mk);
};

class CHHPTemplate
{
	// 
	typedef list<CString> FileList;

	// 
	FileList files;

public:
	// 
	void AddHtml(CString strHref, bool fIs_entityDef = true)
	{
		files.push_back(strHref);
	}

	// 
	bool Commit(CHHPMk &mk);
};

};

typedef map<CString, UINT> MaterialDistributionMMap;

typedef multimap<UINT, CString, greater<UINT> > MaterialDistributionRMMap;

class CMaterialStats
{
	// 
	struct Data {
		// 
		Doom3Blendf bf1, bf2;

		// 
		bool operator <(const Data &s) const
		{
			return memcmp(this, &s, sizeof(Data)) < 0;
		}
	};
	// 
	typedef vector<Data> DataArray;
	// 
	struct DataArrayLess : less<DataArray> {
		// 
		bool operator()(const DataArray &s1, const DataArray &s2) const
		{
			UINT i;
			for (i = 0; ; i++) {
				if (false);
				else if (s1.size() <= i) {
					if (s2.size() <= i)
						return false;
					return true;
				}
				else if (s2.size() <= i) {
					return false;
				}
				if (s1[i] < s2[i])
					return true;
				if (s2[i] < s1[i])
					return false;
			}
		}
	};
	// 
	struct DataValue {
		// 
		UINT cx;
		// 
		list<CString> names;

		// 
		DataValue()
			: cx(0)
		{

		}
		// 
		bool operator <(const DataValue &s) const
		{
			return (cx < s.cx);
		}
		// 
		bool operator >(const DataValue &s) const
		{
			return (cx > s.cx);
		}
	};
	// 
	typedef map<DataArray, DataValue, DataArrayLess> DataMap;
	// 
	typedef multimap<DataValue, DataArray, greater<DataValue> > DataRMMap;

	// 
	const CDoom3Workpad &wk;
	// 
	const MaterialDistributionMMap &mm;

	// 
	static LPCSTR GetBlendfName(Doom3Blendf x)
	{
		switch (x) {
		case bfNo: return "No";
		case bfDstAlpha: return "GL_DST_ALPHA";
		case bfDstColor: return "GL_DST_COLOR";
		case bfOne: return "GL_ONE";
		case bfOneMinusDstAlpha: return "GL_ONE_MINUS_DST_ALPHA";
		case bfOneMinusDstColor: return "GL_ONE_MINUS_DST_COLOR";
		case bfOneMinusSrcAlpha: return "GL_ONE_MINUS_SRC_ALPHA";
		case bfOneMinusSrcColor: return "GL_ONE_MINUS_SRC_COLOR";
		case bfSrcColor: return "GL_SRC_COLOR";
		case bfZero: return "GL_ZERO";
		case bfSrcAlpha: return "GL_SRC_ALPHA";
		case bfBlend: return "Blend";
		case bfAdd: return "Add";
		case bfBumpMap: return "BumpMap";
		case bfDiffuseMap: return "DiffuseMap";
		case bfSpecularMap: return "SpecularMap";
		case bfFilter: return "Filter";
		}
		return "?";
	}

public:
	// 
	CMaterialStats(const CDoom3Workpad &wk, const MaterialDistributionMMap &mm)
		: wk(wk), mm(mm)
	{

	}
	// 
	void Run();
};

struct CV2MapPlane {
	// plane
	My3DMath::Vtx3 v;
	// plane minimal radius
	float fr;
	// for texture coord. (x,y,1) * m32 -> (s,t)
	float m32[2][3];
	// 
	CString strTex;

	// unit vector to move 'origin'
	My3DMath::Vtx3 v3Up, v3R;
};

typedef vector<CV2MapPlane> V2MapPlaneArray_t;

struct CV2MapBrush {
	// 
	V2MapPlaneArray_t pa;
};

typedef vector<CV2MapBrush> V2MapBrushArray_t;

struct CV2MapPatchDef3Coord {
	// 
	My3DMath::Vtx3 v;
	// 
	My3DMath::Vtx2 tv;
};

typedef vector<CV2MapPatchDef3Coord> V2MapPatchDef3CoordArray_t;

struct CV2MapPatchDef3 {
	// 
	int cx, cy;
	// 
	int t[5];
	// 
	bool fIs3;
	// 
	CString strTex;

	// 
	V2MapPatchDef3CoordArray_t ca;
};

typedef vector<CV2MapPatchDef3> V2MapPatchDef3Array_t;

struct CV2MapEntity {
	// 
	VarMap m;
	// 
	CString strName;
	// 
	My3DMath::Vtx3 o;
	// 
	bool fHasBind;
	// 
	My3DMath::Mtx rotation;

	// 
	V2MapBrushArray_t va;
	// 
	V2MapPatchDef3Array_t p3a;
};

typedef list<CV2MapEntity> V2MapEntityList_t;

typedef map<CString, CV2MapEntity *> V2MapEntityPtrMap_t;

typedef multimap<CString, CString> V2MapBindRMMap_t;

struct CV2MapDeck {
	// 
	V2MapEntityList_t ea;
	// 
	V2MapEntityPtrMap_t em;
	// 
	V2MapBindRMMap_t vrmm;
	// 
	StrSet_t vs;

	// 
	void SearchDep(CString strName, StrSet_t &ss) const;
	// 
	CV2MapEntity *FindEntity(CString strName) const;
};

class CSparseV2Map : public CSparseBase
{
	// 
	MaterialDistributionMMap &mm;
	// 
	MaterialDistributionMMap mm0;
	// 
	CV2MapDeck &mesh;
	// 
	CV2MapDeck mesh0;
	// 
	bool fStat;
	// 
	bool fq4; // q4
	// 
	char iVer;

	// 
	void Parse_1();

public:
	// 
	CSparseV2Map(CLL1Leader &fIn, MaterialDistributionMMap &mm)
		: CSparseBase(fIn), mm(mm), mesh(mesh0), fStat(true), fq4(false)
	{

	}
	// 
	CSparseV2Map(CLL1Leader &fIn, CV2MapDeck &mesh, bool q4)
		: CSparseBase(fIn), mm(mm0), mesh(mesh), fStat(false), fq4(q4)
	{

	}
	// 
	bool Parse();

};

class CProcessPriorityEnforcer
{
	// 
	DWORD nSaved;

public:
	// 
	CProcessPriorityEnforcer()
	{
		nSaved = 0;
	}
	// 
	~CProcessPriorityEnforcer()
	{
		Close();
	}
	// 
	void Close()
	{
		if (nSaved != 0) {
			VERIFY(SetPriorityClass(GetCurrentProcess(), nSaved));
			nSaved = 0;
		}
	}
	// 
	void Enforce(DWORD nNew)
	{
		DWORD n = GetPriorityClass(GetCurrentProcess());
		if (n == 0) return;
		Close();
		nSaved = n;

		VERIFY(SetPriorityClass(GetCurrentProcess(), nNew));
	}

};

struct CDoom3ImaPix {
	// 
	CDoom3FlatIma &rIma;

	// 
	UINT x, y, x1;
	// 
	bool fUpdown;

	// 
	CDoom3ImaPix(CDoom3FlatIma &rIma, bool fUpdown)
		: rIma(rIma), fUpdown(fUpdown)
	{
		x = y = 0;
		x1 = rIma.nBitCount / 8;
		ASSERT(x1 != 0);
	}
	// 
	bool WritePix(const void *pv, UINT nRepeat)
	{
		if (y == rIma.cy) return false;

		BYTE *pVert = rIma.GetVert(fUpdown ? y : (rIma.cy -y -1)) + x1 * x;

		const BYTE *p = (const BYTE *)pv;

		for (UINT iRepeat = 0; iRepeat < nRepeat; iRepeat++, pVert += x1, x++) {
			if (x == rIma.cx) {
				x = 0;
				y++;

				pVert = rIma.GetVert(fUpdown ? y : (rIma.cy -y -1));
			}
			if (y == rIma.cy) return false;

			memcpy(pVert, p, x1);
		}
		return true;
	}
};

class CSparseTarga : public CMemVuff
{
	// 
	CDoom3FlatIma &rIma;

public:
	// 
	CSparseTarga(CDoom3FlatIma &rIma)
		: rIma(rIma)
	{

	}
	// 
	bool Parse();

};

class CEaseWriter : public CTextPlainMk
{
	// 
	CMD5MeshDeck &mesh;

public:
	// 
	CEaseWriter(CMD5MeshDeck &mesh)
		: mesh(mesh)
	{

	}
	// 
	bool Write();

};

class CV2MapWriter : public CTextPlainMk
{
public:
	// 
	int iEntity, iPrimitive;
	// 
	My3DMath::Vtx3 patchDef_forceo;

	// 
	bool MkHeader();
	// 
	bool MkEntityHeader();
	// 
	bool MkEntity(CV2MapEntity &entity);
	// 
	bool MkEntityFooter();
};

struct CBMapPt {
	// 
	My3DMath::Vtx3 v;
	// 
	My3DMath::Vtx2 tv;
};

struct BMapPtArray_t : vector<CBMapPt> {
	// 
	CString strTex;

	// 
	BMapPtArray_t() { }
	// 
	BMapPtArray_t(size_t i, const value_type &x): vector<CBMapPt>(i, x)
	{

	}
};

typedef list<BMapPtArray_t> BMapPtArrayList_t;

struct CBMEnt {
	// 
	BMapPtArrayList_t alm;
	// 
	My3DMath::Mtx rotation;
	// 
	My3DMath::Vtx3 o;

};

typedef list<CBMEnt> BMEntList_t;

struct CStrCaseInsensitiveLess_t {
	// 
	bool operator ()(const CString &s1, const CString &s2)
	{
		return s1.CompareNoCase(s2) < 0;
	}
};
