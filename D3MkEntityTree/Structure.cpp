
// +--------------------------------------------------
// |
// | Structure.cpp
// |
// | D3MkEntityTree : Copyright (c) 2004, 2005, kentaro-k.21
// |

#include "StdAfx.h"
#include "Structure.h"
#include "OSP.h"
#include "OutDeb.h"
#include "Sol.h"
#include "resource.h"

#pragma comment(lib, "vfw32")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

namespace
{

void _InheritMap(map<CString, CString> &m0, const map<CString, CString> &m1)
{
	map<CString, CString>::const_iterator
		iterPos = m1.begin(),
		iterEnd = m1.end();
	for (; iterPos != iterEnd; iterPos++) {
		if (m0.find(iterPos->first) == m0.end()) {
			m0.insert(*iterPos);
		}
	}
}

};

// ->
//  ->
//   ->
//    ->
// CSparseDef
//      ->
//       ->
//        ->
//         ->

bool CSparseDef::Parse()
{
	if (!Next()) return false;

	try {
		while (SkipAlWsComment()) {
			CString strToken;
			ForceReadTexto(strToken);
			CString strKey = strToken;
			strToken.MakeLower();
			CString strName;
			SkipWs();
			if (false);
			else if (strToken == "entitydef") {
				Parse_entityDef();
			}
			else if (strToken == "model") {
				Parse_model();
			}
			else if (strToken == "sound") {
				Parse_sound();
			}
			else if (strToken == "export") {
				Parse_export();
			}
			else if (strToken == "mapdef") {
				Parse_mapDef();
			}
			else if (strToken == "skin") {
				Parse_skin();
			}
			else if (strToken == "table") {
				Parse_table();
			}
			else if (strToken == "material") {
				ForceReadTexto(strName);
				Parse_material(strName);
			}
			else if (strToken == "particle") {
				Parse_any();
			}
			else if (strToken == "fx") {
				Parse_any();
			}
			else if (strToken == "articulatedfigure") {
				Parse_any();
			}
			else if (strToken == "pda") {
				Parse_any();
			}
			else if (strToken == "video") {
				Parse_any();
			}
			else if (strToken == "audio") {
				Parse_any();
			}
			else if (strToken == "email") {
				Parse_any();
			}
			else if (Q4() && strToken == "guide") {
				Parse_guide();
			}
			else if (Q4() && strToken == "camera") {
				Parse_camera();
			}
			else {
				Parse_material(strKey);
			}
		}

		return true;
	} catch (Error) {
		return false;
	}
}

bool CSparseDef::ReadText1(CString &strText, bool fQuotedCondOk)
{
	if (fQuotedCondOk) {
		if (Cur() == '\"') {
			ReadTextQuoted(strText);
			return true;
		}
	}

	strText.Empty();
	while (!IsEnd()) {
		int a = Cur();
		if (!isalnum(a) && a != '_')
			break;
		strText += (char)a;

		Next();
	}
	return strText.GetLength() != 0;
}

void CSparseDef::Parse_entityDef()
{
	SkipAlWsComment();

	CDoom3entityDef entityDef;
	if (!ReadText1(entityDef.strName))
		KillMe();
	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		CString strKey, strValue;
		SkipAlWsComment();
		ReadTexto(strKey);
		SkipAlWsComment();
		ReadTextTokens(strValue);

		// DO NOT overwrite
		entityDef.m.insert(make_pair(strKey, strValue));
		entityDef.o.push_back(strKey);
	}

	wk.entityDefMap[entityDef.strName] = entityDef;
}

void CSparseDef::Parse_model()
{
	SkipAlWsComment();

	CDoom3model model;
	if (!ReadText1(model.strName))
		KillMe();
	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		CString strToken;
		if (!ReadText1(strToken, true))
			KillMe();
		SkipWs();
		if (false);
		else if (strToken == "mesh" || strToken == "skin" || strToken == "inherit") {
			CString strValue;
			if (!ReadTexto(strValue))
				KillMe();
			model.m[strToken] = strValue;
		}
		else if (strToken == "offset") {
			ReadTextBracketInside(model.strOffset);
		}
		else if (strToken == "channel") {
			CString channel, channelValue;
			if (!ReadText1(channel))
				KillMe();
			SkipWs();
			ReadTextBracketInside(channelValue);
			model.channels[channel] = channelValue;
		}
		else if (strToken == "anim") {
			CString anim, animValue;
			if (!ReadTexto(anim))
				KillMe();
			while (true) {
				CString anim1;
				SkipWs();
				if (!ReadTexto(anim1)) // q4
					KillMe();
				if (animValue.IsEmpty())
					animValue = anim1;
				SkipWs();
				if (IsMatch(','))
					continue;
				break;
			};
			SkipAlWsComment();
			if (Cur() == '{') {
				Force1(SkipMidBracketContext());
			}
#if 0
			if (IsMatch('{')) {
				while (!IsMatchFinally('}')) {
					SkipMidBracketContext();
					SkipTokensUntilMidBracket();
#if 1
					SkipAlWsComment();
					CString strValue;
					if (!ReadTexto(strValue))
						KillMe();
#else
					SkipAlWsComment();
					CString frame, frameNo;
					if (!ReadText1(frame))
						KillMe();
					if (frame == "frame") {
						SkipWs();
						if (!ReadText3(frameNo))
							KillMe();
						CString co, coValue;
						SkipWs();
						if (!ReadText2(co))
							KillMe();
						SkipWs();
						ReadText2(coValue, true);
					} else if (frame == "ai_no_turn") {

					} else if (frame == "anim_turn") {

					} else if (frame == "prevent_idle_override") {

					} else if (frame == "random_cycle_start") {

					} else {
						KillMe();
					}
#endif
				}
			}
#endif
			model.anims[anim] = animValue;
		}
	}

	wk.modelMap[model.strName] = model;
}

bool CSparseDef::ReadText2(CString &strText, bool fQuotedCondOk)
{
	if (fQuotedCondOk) {
		if (Cur() == '\"') {
			ReadTextQuoted(strText);
			return true;
		}
	}

	strText.Empty();
	while (!IsEnd()) {
		int a = Cur();
		if (!isalnum(a) && a != '_' && a != '/' && a != '.' && a != ':' && a != '\\')
			break;
		strText += (char)a;

		Next();
	}
	return strText.GetLength() != 0;
}

void CSparseDef::ReadTextBracketInside(CString &strText)
{
	strText.Empty();

	ForceMatch('(');
	while (!IsMatchFinally(')', false)) {
		strText += (char)Cur();
		Next();
	}
}

bool CSparseDef::ReadText3(CString &strText)
{
	strText.Empty();
	while (!IsEnd()) {
		int a = Cur();
		if (!isdigit(a) && a != '-' && a != '.') // FIXME: Ahh
			break;
		strText += (char)a;

		Next();
	}
	return strText.GetLength() != 0;
}

void CSparseDef::Parse_sound()
{
	SkipAlWsComment();

	CString strName;
	if (!ReadText1(strName))
		KillMe();
	ForceMatch('{');
	CString strKw, strValue;
	while (!IsMatchFinally('}')) {
		CString strKey, strValue;
		SkipAlWsComment();
		if (!ReadText2(strKey))
			KillMe();
		SkipWs();
		if (false);
		else if (strKey == "minDistance") {
			if (!ReadText3(strValue))
				KillMe();
		}
		else if (strKey == "maxDistance") {
			if (!ReadText3(strValue))
				KillMe();
		}
		else if (strKey == "volume") {
			if (!ReadText3(strValue))
				KillMe();
		}
		else if (strKey == "no_dups") {

		}
		else {

		}
	}
}

void CSparseDef::Parse_export()
{
	SkipAlWsComment();
	CString strName;
	if (!ReadText1(strName))
		KillMe();

	Force1(SkipMidBracketContext());

#if 0
	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		CString strKey;
		if (!ReadText1(strKey))
			KillMe();
		if (false);
		else if (strKey == "options" || strKey == "anim" || strKey == "mesh" || strKey == "addoptions" || strKey == "camera") {
			CString strValue;
			while (true) {
				SkipWs();
				if (IsLineBReak())
					break;
				if (!ReadText4(strValue))
					KillMe();
			}
		}
	}
#endif
}

bool CSparseDef::ReadText4(CString &strText)
{
	strText.Empty();
	while (!IsEnd()) {
		int a = Cur();
		if (!isalnum(a) && a != '_' && a != '/' && a != '.' && a != '-')
			break;
		strText += (char)a;

		Next();
	}
	return strText.GetLength() != 0;
}

void CSparseDef::Parse_mapDef()
{
	SkipAlWsComment();
	CString strName;
	if (!ReadTexto(strName))
		KillMe();
	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		CString strKey, strValue;
		SkipAlWsComment();
		ReadTexto(strKey);
		SkipAlWsComment();
		ReadTexto(strValue);
	}
}

void CSparseDef::Parse_any()
{
	SkipAlWsComment();
	CString strName;
	if (!ReadTexto(strName))
		KillMe();
	CString strValue;
	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		if (IsMatch('{')) {
			while (!IsMatchFinally('}')) {
				if (!ReadTexto(strValue))
					KillMe();
			}
		} else {
			if (!ReadTexto(strValue))
				KillMe();
		}
	}
}

void CSparseDef::Parse_skin()
{
	SkipAlWsComment();

	CDoom3skin skin;
	if (!ReadTexto(skin.strName))
		KillMe();
	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		CString strKey, strValue;
		SkipAlWsComment();
		if (!ReadTexto(strKey))
			KillMe();
		SkipWs();
		if (!ReadTexto(strValue)) {
			strValue = strKey;
			strKey = "model";
		}

		if (false);
		else if (strKey == "model" || strKey == "_default") {
			skin.m[strKey] = strValue;
		}
		else {
			skin.materialArray.push_back(make_pair(strKey, strValue));
		}
	}
}

void CSparseDef::Parse_material(CString strName)
{
	strName.Replace('\\', '/');

	CDoom3material mtr;
	mtr.strName = strName;
	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		if (IsMatch('{')) {
			Parse_indent(mtr.v);
			continue;
		}
		CString strKey, strValue;
		SkipAlWsComment();
		if (!ReadTexto(strKey))
			KillMe();
		strKey.MakeLower();
		SkipWs();
		if (false);
		else if (false
			|| strKey == "diffusemap"
			|| strKey == "specularmap"
			|| strKey == "bumpmap"
			||(Q4() && strKey == "downsize")
		) {
			CDoom3materialIndent v;
			Parse_map(v.tex);

			if (false);
			else if (strKey == "diffusemap") v.bf1 = bfDiffuseMap;
			else if (strKey == "specularmap") v.bf1 = bfSpecularMap;
			else if (strKey == "bumpmap") v.bf1 = bfBumpMap;
			else if (strKey == "downsize") v.bf1 = bfDownSize;

			mtr.v.push_back(v);
		}
		else if (false
		) {
		}
		else {
			mtr.m[strKey];

			SkipTokensUntilLf();
		}
	}
	wk.materialMap[mtr.strName] = mtr;
}

void CSparseDef::Parse_indent(CDoom3materialIndentArray &v)
{
	CDoom3materialIndent mi;
	while (!IsMatchFinally('}')) {
		CString strKey, strValue;
		SkipAlWsComment();
		if (!ReadTexto(strKey))
			KillMe();
		strKey.MakeLower();
		SkipWs();
		if (false);
		else if (false
			|| strKey == "alphatest"
			|| strKey == "if"
		) {
			CSparseExpr e(GetLeader());
			SymList syms;
			if (!e.Parse(syms))
				KillMe();
			CExprBinder r(syms);
			if (r.Parse()) {
				if (false);
				else if (strKey == "alphatest") mi.soAlphaTest = r.so;
				else if (strKey == "if") mi.soIf = r.so;
			} else {
				printf("");
			}
		}
		else if (false
			|| strKey == "scale"
			|| strKey == "translate"
			|| strKey == "rotate"
		//	|| strKey == "centerscale"
		//	|| strKey == "rgb"
		//	|| strKey == "red"
		//	|| strKey == "green"
		//	|| strKey == "blue"
		) {
			CSparseExpr e(GetLeader());
			SymList syms;
			if (!e.Parse(syms))
				KillMe();
			CExprBinder r(syms);
			if (r.Parse()) {
				if (false);
				else if (strKey == "scale") mi.so_scale = r.so;
				else if (strKey == "translate") mi.so_translate = r.so;
				else if (strKey == "rotate") mi.so_rotate = r.so;
			} else {
				printf("");
			}
		}
		else if (false
			|| strKey == "map"
		) {
			Parse_map(mi.tex);
		}
		else if (strKey == "blend") {
			if (!ReadTexto(strValue))
				KillMe();
			strValue.MakeLower();
			if (false);
			else if (mi.Parse_blendf1(strValue, mi.bf1)) {
				printf("");
			}
			else if (mi.Parse_blendf2(strValue, mi.bf1)) {
				ForceMatch(',');
				SkipWs();
				if (!ReadTexto(strValue))
					KillMe();
				strValue.MakeLower();
				if (!mi.Parse_blendf2(strValue, mi.bf2))
					KillMe();
				printf("");
			}
			else KillMe();
		}
		else {
			SkipTokensUntilLf();
		}
	}
	v.push_back(mi);
}

void CSparseDef::Parse_table()
{
	CString strKey;
	if (!ReadTexto(strKey))
		KillMe();
	ForceMatch('{');
	SkipAlWsComment();
	CDoom3table t;
	CString strValue;
	while (ReadTexto(strValue)) {
		if (false);
		else if (strValue == "clamp")
			t.fClamp = true;
		else if (strValue == "snap")
			t.fSnap = true;
		else
			break;

		SkipAlWsComment();
	}
	int iLv = 0;
	if (IsMatch('{'))
		iLv++;
	if (!IsMatchFinally('}')) {
		for (; ; ) {
			SkipAlWsComment();
			float x;
			if (ReadDecimalo(strValue)) {
				Force1(ParseFloat(strValue, x));
				t.vec.push_back(x);
			}
			if (IsMatchFinally('}'))
				break;
			ForceMatch(',');
		}
	}
	if (iLv != 0) {
		ForceMatch('}');
	}

	t.m = (int)t.vec.size();

	wk.tableMap[strKey] = t;
}

void CSparseDef::Parse_map(CDoom3materialTexo &tex)
{
	CSparseMapExpr e(GetLeader(), tex);
	Force1(e.Parse());
}

void CSparseDef::Parse_camera()
{
	CString strAlias;

	SkipWs();
	ForceReadTexto(strAlias);

	Force1(SkipMidBracketContext());
}

void CSparseDef::Parse_guide()
{
	CString strAlias;

	SkipWs();
	ForceReadTexto(strAlias);

	CString strFunc;

	SkipWs();
	ForceReadTexto(strFunc);

	ForceMatch('(');
	while (true) {
		SkipAlWsComment();
		CString strParm;
		ForceReadTexto(strParm);
		if (IsMatch(')'))
			break;
		ForceMatch(',');
	}
}

// ->
//  ->
//   ->
//    ->
// CDoom3Workpad
//      ->
//       ->
//        ->
//         ->

void CDoom3Workpad::CompleteInherit(CDoom3entityDef &entityDef)
{
	CString strName = entityDef.strName;
	while (true) {
		{
			InheritMap::iterator
				iterPos = inheritMap.find(strName),
				iterEnd = inheritMap.end();
			if (iterPos == iterEnd)
				break;
			strName = iterPos->second;
		}

		{
			CDoom3entityDefMap::iterator
				iterPos = entityDefMap.find(strName),
				iterEnd = entityDefMap.end();
			if (iterPos == iterEnd)
				break;
			CDoom3entityDef &r = iterPos->second;

			_InheritMap(entityDef.m, r.m);

			entityDef.o.insert(entityDef.o.end(), r.o.begin(), r.o.end());
		}
	};
}

void CDoom3Workpad::CompleteInherit(CDoom3model &model)
{
	CString strName = model.strName;
	while (true) {
		{
			InheritMap::iterator
				iterPos = inheritModelMap.find(strName),
				iterEnd = inheritModelMap.end();
			if (iterPos == iterEnd)
				break;
			strName = iterPos->second;
		}

		{
			CDoom3modelMap::iterator
				iterPos = modelMap.find(strName),
				iterEnd = modelMap.end();
			if (iterPos == iterEnd)
				break;
			CDoom3model &r = iterPos->second;

			_InheritMap(model.m, r.m);
			_InheritMap(model.anims, r.anims);
			_InheritMap(model.channels, r.channels);
		}
	}
}

// ->
//  ->
//   ->
//    ->
// CSparseBase
//      ->
//       ->
//        ->
//         ->

bool CSparseBase::SkipWs()
{
	while (!IsEnd()) {
		int a = Cur();
		if (a == ' ' || a == '\t' || a == 11) {
			Next();
			continue;
		}
		return true;
	}
	return false;
}

bool CSparseBase::IsWs()
{
	switch (Cur()) {
	case ' ':
	case '\t':
	case '\r':
	case '\n':
	case 11:
	case 0:
		return true;
	}
	return false;
}

bool CSparseBase::IsLineBReak()
{
	switch (Cur()) {
	case '\r':
	case '\n':
		return true;
	}
	return false;
}

bool CSparseBase::IsSPC()
{
	switch (Cur()) {
	case ' ':
	case '\t':
		return true;
	}
	return false;
}

bool CSparseBase::IsInlineComment()
{
	if (!IsMatch('/', false))
		return false;
	if (IsMatch('/', false)) {
		if (Next()) {
			while (true) {
				if (IsLineBReak())
					break;
				if (!Next())
					break;
			};
		}
		return true;
	}
	if (IsMatch('*', false)) {
		if (!Next())
			KillMe();
		while (!IsEnd()) {
			if (IsMatch('*', false)) {
				if (IsMatch('/', false)) {
					return true;
				}
			} else {
				Next();
			}
			continue;
		}
		KillMe();
	}
	return true;
}

void CSparseBase::ReadTextQuoted(CString &strText)
{
	strText.Empty();

	ForceMatch('\"');
	while (!IsMatchFinally('\"', false)) {
		strText += (char)Cur();
		Next();
	}
}

void CSparseBase::ForceMatch(char c, bool fSkipAlWs)
{
	if (fSkipAlWs)
		SkipAlWsComment();
	if (IsEnd() || Cur() != c)
		KillMe();
	Next();
}

bool CSparseBase::IsMatch(char c, bool fSkipAlWs)
{
	if (fSkipAlWs)
		SkipAlWsComment();
	if (IsEnd() || Cur() != c)
		return false;
	Next();
	return true;
}

bool CSparseBase::IsMatchFinally(char c, bool fSkipAlWs)
{
	if (fSkipAlWs)
		SkipAlWsComment();
	if (IsEnd())
		KillMe();
	if (Cur() != c)
		return false;
	Next();
	return true;
}

bool CSparseBase::SkipAlWsComment()
{
	while (!IsEnd()) {
		if (IsWs()) {
			Next();
			continue;
		}
		if (IsInlineComment()) {
			continue;
		}
		return true;
	}
	return false;
}

bool CSparseBase::ReadTexto(CString &strText)
{
	if (Cur() == '\"') {
		ReadTextQuoted(strText);
		return true;
	}

	strText.Empty();
	while (!IsEnd()) {
		int a = Cur();
		if (isspace(a) || a == '{' || a == '}' || a == ',' || a == '(' || a == ')')
			break;
		strText += (char)a;

		Next();
	}
	return strText.GetLength() != 0;
}

bool CSparseBase::ReadDecimalo(CString &strText)
{
	strText.Empty();
	while (!IsEnd()) {
		int a = Cur();
		if (!(a == '-') && !('0' <= a && a <= '9') && !(a == '.'))
			break;
		strText += (char)a;

		Next();
	}

	{
		int a = Cur();
		if (a == 'f' || a == 'F') // q4
			Next();
	}
	return strText.GetLength() != 0;
}

bool CSparseBase::SkipMidBracketContext()
{
	if (IsMatch('{')) {
		if (!SkipTokensUntilMidBracket())
			KillMe();
		while (!IsMatchFinally('}')) {
			if (!SkipTokensUntilMidBracket())
				KillMe();
			int a = Cur();
			if (a == '{') {
				if (!SkipMidBracketContext())
					KillMe();
			}
		}
		return true;
	}
	return false;
}

bool CSparseBase::SkipTokensUntilMidBracket()
{
	while (!IsEnd()) {
		int a = Cur();
		if (a == '{' || a == '}')
			return true;
		Next();
	}
	return false;
}

void CSparseBase::SkipTokensUntilLf()
{
	while (true) {
		int a = Cur();
		if (IsLineBReak() || a == '{' || a == '}')
			break;
		if (!Next())
			break;
	}
}

void CSparseBase::ReadInlineTexto(CString &strText)
{
	strText.Empty();
	while (!IsLineBReak()) {
		int a = Cur();
		if (a == '{' || a == '}') {
			break;
		}
		else
		if (a == '/') {
			if (Next()) {
				int a = Cur();
				if (false);
				else if (a == '/') {
					while (Next() && !IsLineBReak());
					break;
				}
				else if (a == '*') {
					while (true) {
						if (!Next())
							KillMe();
						{
							int a = Cur();
							if (a != '*')
								continue;
						}
						if (!Next())
							KillMe();
						{
							int a = Cur();
							if (a != '/')
								continue;
						}
						Next();
						break;
					}
					break;
				}
			}
		}
		strText += (char)a;
		if (!Next()) {
			break;
		}
	}
}

void CSparseBase::ForceReadTexto(CString &strText)
{
	if (!ReadTexto(strText))
		KillMe();
}

bool CSparseBase::ReadTextTokens(CString &strText)
{
	CString str;
	strText.Empty();

	ReadTexto(str); strText += str;

	for (; ; ) {
		if (!SkipWs() || Cur() != '\\')
			break;
		Next();
		if (!SkipAlWsComment())
			break;
		if (IsWs())
			break;

		ReadTexto(str); strText += str;
	}

	return true;
}

// ->
//  ->
//   ->
//    ->
// CSparseMD5Mesh
//      ->
//       ->
//        ->
//         ->

bool CSparseMD5Mesh::Parse()
{
	if (!Next()) return false;

	if (mesh.basejointAvail) {
		mesh.basejoint.m.RotationQuaternion(My3DMath::Quatern().Identify(mesh.basejoint.q));
	}


	iCurMesh = 0;

	try {
		while (SkipAlWsComment()) {
			CString strKey, strValue;
			int nValue;
			if (!ReadTexto(strKey))
				KillMe();
			strKey.MakeLower();
			SkipWs();
			if (false);
			else if (strKey == "md5version") {
				if (!ReadTexto(strValue))
					KillMe();
				if (strValue != "10")
					KillMe();
			}
			else if (strKey == "commandline") {
				if (!ReadTexto(strValue))
					KillMe();
			}
			else if (strKey == "numjoints") {
				if (!ReadTexto(strValue) || !ParseInt(strValue, nValue))
					KillMe();
				mesh.joints.resize(nValue);
			}
			else if (strKey == "nummeshes") {
				if (!ReadTexto(strValue) || !ParseInt(strValue, nValue))
					KillMe();
				mesh.meshes.resize(nValue);
			}
			else if (strKey == "joints") {
				Parse_joints();
			}
			else if (strKey == "mesh") {
				Parse_mesh();
			}
			else {
				KillMe();
			}
		}

		return true;
	} catch (Error) {
		return false;
	}
}

void CSparseMD5Mesh::Parse_joints()
{
	ForceMatch('{');
	int iIndex = 0;
	while (!IsMatchFinally('}')) {
		if (mesh.joints.size() <= (UINT)iIndex)
			KillMe();

		CMD5MeshJoint &j = mesh.joints[iIndex];
		CString strValue;

		if (!ReadTexto(j.strName))
			KillMe();
		SkipWs();
		if (!ReadTexto(strValue) || !ParseInt(strValue, j.iJointRef) || (j.iJointRef != -1 && mesh.joints.size() < (UINT)j.iJointRef))
			KillMe();
		ReadVtx3(j.ok.o);
		ReadVtx3(j.ok.q);

		j.ok.m.RotationQuaternion(My3DMath::Quatern().Identify(j.ok.q));

		if (mesh.basejointAvail) {
			j.ok.o += mesh.basejoint.o;
		//	j.ok.m *= mesh.basejoint.m;
		}

		iIndex++;
	}
}

void CSparseMD5Mesh::ReadVtx3(My3DMath::Vtx3 &v)
{
	ForceMatch('(');
	CString str[3];
	SkipWs();
	if (!ReadTexto(str[0]) || !ParseFloat(str[0], v.x))
		KillMe();
	SkipWs();
	if (!ReadTexto(str[1]) || !ParseFloat(str[1], v.y))
		KillMe();
	SkipWs();
	if (!ReadTexto(str[2]) || !ParseFloat(str[2], v.z))
		KillMe();
	ForceMatch(')');
}

void CSparseMD5Mesh::ReadVtx2(My3DMath::Vtx2 &v)
{
	ForceMatch('(');
	CString str[2];
	SkipWs();
	if (!ReadTexto(str[0]) || !ParseFloat(str[0], v.x))
		KillMe();
	SkipWs();
	if (!ReadTexto(str[1]) || !ParseFloat(str[1], v.y))
		KillMe();
	ForceMatch(')');
}

void CSparseMD5Mesh::Parse_mesh()
{
	if (mesh.meshes.size() <= (UINT)iCurMesh)
		KillMe();

	CMD5MeshMesh &meshmesh = mesh.meshes[iCurMesh];

	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		CString strKey, strValue;
		int nValue;

		if (!ReadTexto(strKey))
			KillMe();

		SkipWs();
		if (false);
		else if (strKey == "shader") {
			if (!ReadTexto(strValue))
				KillMe();
			meshmesh.m[strKey] = strValue;
		}
		else if (strKey == "numverts") {
			if (!ReadTexto(strValue) || !ParseInt(strValue, nValue))
				KillMe();
			meshmesh.verts.resize(nValue);
		}
		else if (strKey == "vert") {
			int iElem;
			if (!ReadTexto(strValue) || !ParseInt(strValue, iElem))
				KillMe();
			if (meshmesh.verts.size() <= (UINT)iElem)
				KillMe();
			CMD5MeshMeshVert &vert = meshmesh.verts[iElem];

			SkipWs();
			ReadVtx2(vert.o);
			SkipWs();
			if (!ReadTexto(strValue) || !ParseInt(strValue, vert.iWeight))
				KillMe();
			SkipWs();
			if (!ReadTexto(strValue) || !ParseInt(strValue, vert.nWeights))
				KillMe();
		}
		else if (strKey == "numtris") {
			if (!ReadTexto(strValue) || !ParseInt(strValue, nValue))
				KillMe();
			meshmesh.tris.resize(nValue);
		}
		else if (strKey == "tri") {
			int iElem;
			if (!ReadTexto(strValue) || !ParseInt(strValue, iElem))
				KillMe();
			if (meshmesh.tris.size() <= (UINT)iElem)
				KillMe();
			CMD5MeshMeshTri &tri = meshmesh.tris[iElem];

			SkipWs();
			if (!ReadTexto(strValue) || !ParseInt(strValue, tri.v[0]))
				KillMe();
			SkipWs();
			if (!ReadTexto(strValue) || !ParseInt(strValue, tri.v[1]))
				KillMe();
			SkipWs();
			if (!ReadTexto(strValue) || !ParseInt(strValue, tri.v[2]))
				KillMe();
		}
		else if (strKey == "numweights") {
			if (!ReadTexto(strValue) || !ParseInt(strValue, nValue))
				KillMe();
			meshmesh.weights.resize(nValue);
		}
		else if (strKey == "weight") {
			int iElem;
			if (!ReadTexto(strValue) || !ParseInt(strValue, iElem))
				KillMe();
			if (meshmesh.weights.size() <= (UINT)iElem)
				KillMe();
			CMD5MeshMeshWeight &weight = meshmesh.weights[iElem];

			SkipWs();
			if (!ReadTexto(strValue) || !ParseInt(strValue, weight.iJoint))
				KillMe();
			SkipWs();
			if (!ReadTexto(strValue) || !ParseFloat(strValue, weight.nMass))
				KillMe();
			ReadVtx3(weight.o);
		}
		else {
			KillMe();
		}
	}

	iCurMesh++;
}

// ->
//  ->
//   ->
//    ->
// CHtmlMkThumb2
//      ->
//       ->
//        ->
//         ->

void CHtmlMkThumb2::MkHead(bool fHead)
{
	if (fHead) {
		fprintf(f,
			HTML_HEADER_1
			);
		fprintf(f,
			"<table>\n"
			);
	} else {
		if (iCol != -1) {
			fprintf(f,
				"</tr>\n"
				);
			iCol = -1;
		}
		fprintf(f,
			"</table>\n"
			);
		fprintf(f,
			"</body>\n"
			"</html>\n"
			);
	}

	iTable = 0;
}

void CHtmlMkThumb2::MkTableHead(bool fHead, LPCTSTR pszName)
{
	if (fHead) {
		if (pszName != NULL) {
			fprintf(f,
				"<tr>\n"
				"<td colspan=\"4\"><h1>%s</h1></td>\n"
				"</tr>\n"
				, (LPCSTR)pszName
				);
			iCol = -1;
		}
	} else {
		if (iCol < 0) {

		} else {
			fprintf(f,
				"</tr>\n"
				);
			iCol = -1;
		}
	}
}

void CHtmlMkThumb2::AddThumb(CString strName, CString strHref, CString strHrefThere)
{
	if (iCol < 0) {
		fprintf(f,
			"<tr>\n"
			);
		iCol = 0;
	}
	if (iCol == 4) {
		iCol = 0;
		fprintf(f,
			"</tr>\n"
			"<tr>\n"
			);
	}
	fprintf(f,
		"<td>\n"
		"<p align=\"center\">\n"
		"<img src=\"%s\" width=\"120\" height=\"90\" />\n"
		"<br />\n"
		"<a href=\"%s\">%s</a>\n"
		"</p>\n"
		"</td>\n"
		, (LPCSTR)strHref
		, (LPCSTR)strHrefThere
		, (LPCSTR)strName
		);

	iCol++;
}

// ->
//  ->
//   ->
//    ->
// CSparseSkin
//      ->
//       ->
//        ->
//         ->

bool CSparseSkin::Parse()
{
	if (!Next()) return false;

	try {
		while (SkipAlWsComment()) {
			CString strToken;
			ReadTexto(strToken);
			strToken.MakeLower();
			if (false);
			else if (strToken == "skin") {
				Parse_skin();
			}
			else {
				KillMe();
			}
		}

		return true;
	} catch (Error) {
		return false;
	}
}

void CSparseSkin::Parse_skin()
{
	SkipAlWsComment();

	CDoom3skin skin;
	if (!ReadTexto(skin.strName))
		KillMe();
	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		CString strKey, strValue;
		SkipAlWsComment();
		if (!ReadTexto(strKey))
			KillMe();
		SkipWs();
		if (!ReadTexto(strValue)) {
			strValue = strKey;
			strKey = "model";
		}

		if (false);
		else if (strKey == "model" || strKey == "_default") {
			skin.m[strKey] = strValue;
		}
		else {
			skin.materialArray.push_back(make_pair(strKey, strValue));
		}
	}
}

// ->
//  ->
//   ->
//    ->
// CMD5MeshMesh
//      ->
//       ->
//        ->
//         ->

void CMD5MeshMesh::InterpolateVertex(MD5MeshJointArray &j, UINT iVert, My3DMath::Vtx3 &v)
{
	CMD5MeshMeshVert &vert = verts.at(iVert);

	UINT i = 0 +vert.iWeight;
	UINT m = i +vert.nWeights;

	BYTE x = 0;

	My3DMath::Vtx3 v0;
	v0.Empty();

	for (; i < m; i++, x++) {
		CMD5MeshMeshWeight &weight = weights.at(i);
		CMD5MeshJoint &j0 = j.at(weight.iJoint);

		My3DMath::Vtx3 v1;
		v1 = (weight.o * j0.ok.m + j0.ok.o).Mult(weight.nMass);

		v0 += v1;
	}

	v = v0;
}

// ->
//  ->
//   ->
//    ->
// CSparseMD5Anim
//      ->
//       ->
//        ->
//         ->

bool CSparseMD5Anim::Parse()
{
	if (!Next()) return false;

	try {
		while (SkipAlWsComment()) {
			CString strKey, strValue;
			int nValue;
			if (!ReadTexto(strKey))
				KillMe();
			strKey.MakeLower();
			SkipWs();
			if (false);
			else if (strKey == "md5version") {
				if (!ReadTexto(strValue))
					KillMe();
				if (strValue != "10")
					KillMe();
			}
			else if (strKey == "commandline") {
				if (!ReadTexto(strValue))
					KillMe();
			}
			else if (strKey == "numframes") {
				if (!ReadTexto(strValue) || !ParseInt(strValue, nValue))
					KillMe();
				anim.frames.resize(nValue);
			}
			else if (strKey == "numjoints") {
				if (!ReadTexto(strValue) || !ParseInt(strValue, nValue))
					KillMe();
				anim.joints.resize(nValue);
				anim.baseframe.resize(nValue);
			}
			else if (strKey == "framerate") {
				if (!ReadTexto(strValue) || !ParseInt(strValue, nValue))
					KillMe();
				anim.nFrameRate = nValue;
			}
			else if (strKey == "numanimatedcomponents") {
				if (!ReadTexto(strValue) || !ParseInt(strValue, nValue))
					KillMe();
				anim.nAnimatedComp = nValue;
			}
			else if (strKey == "hierarchy") {
				Parse_hierarchy();
			}
			else if (strKey == "bounds") {
				Parse_bounds();
			}
			else if (strKey == "baseframe") {
				Parse_baseframe();
			}
			else if (strKey == "frame") {
				Parse_frame();
			}
			else {
				KillMe();
			}
		}

		return true;
	} catch (Error) {
		return false;
	}
}

void CSparseMD5Anim::Parse_hierarchy()
{
	SkipAlWsComment();

	UINT iIndex = 0;

	// # bounds: frame count
	// # baseframe: joint count

	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		if (anim.joints.size() <= iIndex)
			KillMe();
		CString strKey;
		if (!ReadTexto(strKey))
			KillMe();
		CString str1, str2, str3;
		int iJointRef, nFlags, iVarIndex;
		SkipWs();
		if (!ReadTexto(str1) || !ParseInt(str1, iJointRef))
			KillMe();
		SkipWs();
		if (!ReadTexto(str2) || !ParseInt(str2, nFlags))
			KillMe();
		SkipWs();
		if (!ReadTexto(str3) || !ParseInt(str3, iVarIndex))
			KillMe();

		CMD5AnimJoint &j = anim.joints.at(iIndex);
		j.strName = strKey;
		j.iJointRef = iJointRef;
		j.nFlags = nFlags;
		j.iVarIndex = iVarIndex;

		iIndex++;
	}
}

void CSparseMD5Anim::Parse_bounds()
{
	SkipAlWsComment();

	UINT iIndex = 0;

	My3DMath::Vtx3 v;

	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		ReadVtx3(v);
		ReadVtx3(v);

		iIndex++;
	}
}

void CSparseMD5Anim::ReadVtx3(My3DMath::Vtx3 &v)
{
	ForceMatch('(');
	CString str[3];
	SkipWs();
	if (!ReadTexto(str[0]) || !ParseFloat(str[0], v.x))
		KillMe();
	SkipWs();
	if (!ReadTexto(str[1]) || !ParseFloat(str[1], v.y))
		KillMe();
	SkipWs();
	if (!ReadTexto(str[2]) || !ParseFloat(str[2], v.z))
		KillMe();
	ForceMatch(')');
}

void CSparseMD5Anim::Parse_baseframe()
{
	SkipAlWsComment();

	UINT iIndex = 0;

	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		if (anim.baseframe.size() <= iIndex || anim.joints.size() <= iIndex)
			KillMe();
		CMD5AnimFramePatch &frm = anim.baseframe.at(iIndex);
		ReadVtx3(frm.o);
		ReadVtx3(frm.q);

		iIndex++;
	}
}

void CSparseMD5Anim::Parse_frame()
{
	SkipAlWsComment();

	CString strValue;
	int iFrame;
	if (!ReadTexto(strValue) || !ParseInt(strValue, iFrame))
		KillMe();
	if (anim.frames.size() <= 0U+iFrame)
		KillMe();
	ForceMatch('{');
	UINT i;
	// 0x01 Tx
	// 0x02 Ty
	// 0x04 Tz
	// 0x08 Qx
	// 0x10 Qy
	// 0x20 Qz
	MD5AnimFramePatchArray &frmvec = anim.frames.at(iFrame);
	frmvec = anim.baseframe;
	for (i = 0; i < anim.joints.size(); i++) {
		CMD5AnimJoint &j = anim.joints.at(i);
		CMD5AnimFramePatch &frm = frmvec.at(i);
		for (int x = 0; x < 6; x++) {
			if (j.nFlags & (1 << x)) {
				SkipAlWsComment();
				float f;
				if (!ReadTexto(strValue) || !ParseFloat(strValue, f))
					KillMe();
				switch (x) {
				case 0: frm.o.x  = f; break;
				case 1: frm.o.y  = f; break;
				case 2: frm.o.z  = f; break;
				case 3: frm.q.x  = f; break;
				case 4: frm.q.y  = f; break;
				case 5: frm.q.z  = f; break;
				}
			}
		}

		frm.m.RotationQuaternion(My3DMath::Quatern().Identify(frm.q));

		if (j.iJointRef == 0 && anim.basejointAvail && 0U+iFrame < anim.basejoint.size()) {
			CMD5AnimFramePatch &frmref = anim.basejoint.at(iFrame);

			frm.o = frm.o * frmref.m + frmref.o;
			frm.m = frm.m * frmref.m;
		} else if (!(j.iJointRef < 0)) {
			CMD5AnimFramePatch &frmref = frmvec.at(j.iJointRef);

			frm.o = frm.o * frmref.m + frmref.o;
			frm.m = frm.m * frmref.m;
		}

		printf("");
	}
	ForceMatch('}');
}

// ->
//  ->
//   ->
//    ->
// CAviMk
//      ->
//       ->
//        ->
//         ->

bool CAviMk::Create(CString strAvi, int nFrameRate, CSize size)
{
	Close();

	cv.cbSize = sizeof(cv);
	cv.dwFlags = ICMF_COMPVARS_VALID;
	if (cv.hic == NULL) {
		cv.hic = ICOpen(
			cv.fccType = ICTYPE_VIDEO,
			cv.fccHandler = MAKEFOURCC('i','v','5','0'),
			ICMODE_COMPRESS
			);
	}
	if (cv.hic == NULL) {
		cv.hic = ICOpen(
			cv.fccType = ICTYPE_VIDEO,
			cv.fccHandler = MAKEFOURCC('i','v','4','1'),
			ICMODE_COMPRESS
			);
	}
	if (cv.hic == NULL) {
		cv.hic = ICOpen(
			cv.fccType = ICTYPE_VIDEO,
			cv.fccHandler = MAKEFOURCC('i','v','3','2'),
			ICMODE_COMPRESS
			);
	}
	if (cv.hic == NULL) {
		cv.hic = ICOpen(
			cv.fccType = ICTYPE_VIDEO,
			cv.fccHandler = MAKEFOURCC('c','v','i','d'),
			ICMODE_COMPRESS
			);
	}
	if (cv.hic == NULL) {
		cv.fccType = ICTYPE_VIDEO;
		cv.fccHandler = comptypeDIB;
	}
	if (cv.hic != NULL) {
		DWORD dwICValue = 0;
		cv.lDataRate = 1024*32;
		cv.lQ = __max(-1, __min(2000, (int)ICGetDefaultQuality(cv.hic)));
		cv.lKey = ICGetDefaultKeyFrameRate(cv.hic);
		DWORD nData = ICGetStateSize(cv.hic);
		void *pData = GlobalAlloc(GPTR, nData);
		if (ICGetState(cv.hic, pData, nData) != 0) {
			cv.lpState = pData;
			cv.cbState = nData;
		}
	}

	fUseComp = (cv.hic != NULL);
	fCont = false;

	HRESULT hr;
	try {
		CLSID clsid = CLSID_AVIFile;
		hr = AVIFileOpen(
			&pAvif,
			strAvif = strAvi,
			0 |OF_CREATE |OF_SHARE_DENY_WRITE |OF_READWRITE,
			&clsid
			);
		if (FAILED(hr)) { fprintf(ff.f, "ERR! AVIFileOpen hr=0x%08lX\n", (ULONG)hr); throw hr; }

		AVISTREAMINFO avis;
		ZeroMemory(&avis, sizeof(avis));
		avis.fccType = streamtypeVIDEO;
		avis.fccHandler = ICTYPE_VIDEO;
		avis.dwScale = 1;
		avis.dwRate = nFrameRate;
		avis.dwQuality = -1;
		SetRect(&avis.rcFrame, 0, 0, size.cx, size.cy);
		_tcscpy(avis.szName, "Vid");

		hr = AVIFileCreateStream(
			pAvif,
			&pAvisVid,
			&avis
			);
		if (FAILED(hr)) { fprintf(ff.f, "ERR! AVIFileCreateStream hr=0x%08lX\n", (ULONG)hr); throw hr; }

		BITMAPINFOHEADER &bih = bi.bmiHeader;
		ZeroMemory(&bi, sizeof(bi));
		bih.biSize = sizeof(bih);
		bih.biWidth = size.cx;
		bih.biHeight = size.cy;
		bih.biPlanes = 1;
		bih.biBitCount = 24;
		bih.biCompression = BI_RGB;
		bih.biSizeImage = nSizeImage = 3 * size.cx * size.cy;

		if (fUseComp) {
			if (!ICSeqCompressFrameStart(&cv, &bi)) {
				fprintf(ff.f, "ERR! ICSeqCompressFrameStart hr=0x%08lX\n", (ULONG)hr);
				throw hr = E_FAIL;
			}
			fCont = true;
		}

		iCurFrame = 0;
		return true;
	} catch (HRESULT) {

	}
	return false;
}

void CAviMk::Close(bool fOk)
{
	if (cv.hic) {
		if (fCont) {
			ICSeqCompressFrameEnd(&cv);
		}
		ICCompressorFree(&cv);
	}

	fCont = false;

	ZeroMemory(&cv, sizeof(cv));

	pAvisVid = NULL;
	pAvif = NULL;

	if (!fOk && !strAvif.IsEmpty())
		VERIFY(DeleteFile(strAvif));

	strAvif = "";
}

bool CAviMk::AddFrame(void *pvData)
{
	HRESULT hr;
	if (iCurFrame == 0) {
		void *pHeader = &bi;
		int nHeader = sizeof(BITMAPINFOHEADER);
		if (fUseComp) {
			pHeader = cv.lpbiOut;
			nHeader = cv.lpbiOut->bmiHeader.biSize;
		}
		hr = AVIStreamSetFormat(
			pAvisVid,
			iCurFrame,
			pHeader,
			nHeader
			);
		if (FAILED(hr)) {
			fprintf(ff.f, "ERR! AVIStreamSetFormat hr=0x%08lX\n", (ULONG)hr);
			return false;
		}
	}
	if (fUseComp) {
		BOOL fKey = false;
		LONG nRawData = 0;

		LPVOID pRawData = ICSeqCompressFrame(
			&cv,
			0,
			pvData,
			&fKey,
			&nRawData
			);
		if (pRawData == NULL) {
			fprintf(ff.f, "ERR! ICSeqCompressFrame\n", (ULONG)hr);
			return false;
		}

		LONG nSampWritten = 0;
		LONG nBytesWritten = 0;

		hr = AVIStreamWrite(
			pAvisVid,
			iCurFrame,
			1,
			pRawData,
			nRawData,
			0,
			&nSampWritten,
			&nBytesWritten
			);
		if (FAILED(hr)) {
			fprintf(ff.f, "ERR! AVIStreamWrite hr=0x%08lX\n", (ULONG)hr);
			return false;
		}
	} else {
		LONG nSampWritten = 0;
		LONG nBytesWritten = 0;

		hr = AVIStreamWrite(
			pAvisVid,
			iCurFrame,
			1,
			pvData,
			nSizeImage,
			0,
			&nSampWritten,
			&nBytesWritten
			);
		if (FAILED(hr)) {
			fprintf(ff.f, "ERR! AVIStreamWrite hr=0x%08lX\n", (ULONG)hr);
			return false;
		}
	}

	iCurFrame++;
	return true;
}

// ->
//  ->
//   ->
//    ->
// CSparseExpr
//      ->
//       ->
//        ->
//         ->

bool CSparseExpr::ParseToken()
{
	if (IsLineBReak())
		return false;
	if (!SkipWs())
		return false;

	sym.strName.Empty();

	int a = Cur();
	if (false); 
	else if (a == '.' || ('0' <= a && a <= '9')) {
		Parse_decimal();
		return true;
	}
	else if (a == '[') {
		Next();
		sym.sym = sym.symIndexo;
		return true;
	}
	else if (a == ']') {
		Next();
		sym.sym = sym.symIndexc;
		return true;
	}
	else if (a == '(') {
		Next();
		sym.sym = sym.symBracketo;
		return true;
	}
	else if (a == ')') {
		Next();
		sym.sym = sym.symBracketc;
		return true;
	}
	else if (a == '+') {
		Next();
		sym.sym = sym.symAdd;
		return true;
	}
	else if (a == '*') {
		Next();
		sym.sym = sym.symMul;
		return true;
	}
	else if (a == '-') {
		Next();
		sym.sym = sym.symSub;
		return true;
	}
	else if (a == '/') {
		if (Next()) {
			int a = Cur();
			if (a == '/') {
				while (Next() && !IsLineBReak());
				return false;
			}
			if (a == '*') {
				KillMe();
			}
		}
		sym.sym = sym.symDiv;
		return true;
	}
	else if (a == '%') {
		Next();
		sym.sym = sym.symMod;
		return true;
	}
	else if (a == '<') {
		if (Next()) {
			int a = Cur();
			if (a == '=') {
				Next();
				sym.sym = sym.symOpLEqual;
				return true;
			}
		}
		sym.sym = sym.symOpLess;
		return true;
	}
	else if (a == '>') {
		if (Next()) {
			int a = Cur();
			if (a == '=') {
				Next();
				sym.sym = sym.symOpGEqual;
				return true;
			}
		}
		sym.sym = sym.symOpGreater;
		return true;
	}
	else if (a == ',') {
		Next();
		sym.sym = sym.symComma;
		return true;
	}
	else if (a == '=') {
		if (Next()) {
			int a = Cur();
			if (a == '=') {
				Next();
				sym.sym = sym.symOpEqual;
				return true;
			}
		}
		KillMe();
	}
	else if (a == '!') {
		if (Next()) {
			int a = Cur();
			if (a == '=') {
				Next();
				sym.sym = sym.symOpNotEqual;
				return true;
			}
		}
		KillMe();
	}
	else if (a == '&') {
		if (Next()) {
			int a = Cur();
			if (a == '&') {
				Next();
				sym.sym = sym.symOpLAnd;
				return true;
			}
		}
		KillMe();
	}
	else if (a == '|') {
		if (Next()) {
			int a = Cur();
			if (a == '|') {
				Next();
				sym.sym = sym.symOpLOr;
				return true;
			}
		}
		KillMe();
	}
	else if (Is_Name(a)) {
		Parse_name();
		return true;
	}
	return false;
}

bool CSparseExpr::Parse(SymList &syms)
{
//	if (!Next()) return false;

	try {
		syms.clear();
		while (ParseToken()) {
			syms.push_back(sym);
		}
		return true;
	} catch (Error) {
		return false;
	}
}

void CSparseExpr::Parse_decimal()
{
	while (true) {
		int a = Cur();
		if ('0' <= a && a <= '9') {
			if (!Next())
				break;
			continue;
		}
		break;
	}
	int a = Cur();
	if (a == '.') {
		Next();
	}
	while (true) {
		int a = Cur();
		if ('0' <= a && a <= '9') {
			if (!Next())
				break;
			continue;
		}
		break;
	}

	sym.sym = sym.symDecimal;
}

bool CSparseExpr::Next(bool fAccum)
{
	if (fAccum) sym.strName += (char)Cur();

	return CSparseBase::Next();
}

void CSparseExpr::Parse_name()
{
	while (!IsEnd()) {
		int a = Cur();
		if (Is_Name(a)) {
			if (!Next())
				break;
			continue;
		}
		if (a == '\"') {
			if (!Next(false))
				KillMe();
			while (true) {
				int a = Cur();
				if (a == '\"')
					break;
				if (!Next())
					KillMe();
			}
			Next(false);
		}
		break;
	}

	sym.sym = sym.symName;
}

// ->
//  ->
//   ->
//    ->
// CExprBinder
//      ->
//       ->
//        ->
//         ->

bool CExprBinder::Parse()
{
	iterPos = syms.begin();
	iterEnd = syms.end();

	sa = &so.var;

	try {
		while (!IsEnd()) {
			Parse_1(CSym::symComma, true);
			sa->back()->symo = CSymo::symoNo;
		}
		return true;
	} catch (Error) {
		return false;
	}
}

void CExprBinder::Parse_1(CSym::Sym symEnd, bool fRoot)
{
	sa->push_back(new CSymo());

	for (; ; ) {
		Parse_value();
		if (IsEnd()) {
			if (symEnd != CSym::symNo && !fRoot)
				KillMe();
			break;
		}
		if (Cur().sym == symEnd) {
			Next();
			break;
		}
		Parse_op2();
	}
	EndSymo();
}

void CExprBinder::Parse_op1()
{
	switch (Cur().sym) {
	case CSym::symAdd:
		AddSymo_ops(CSymo::symoAdd1);
		break;
	case CSym::symSub:
		AddSymo_ops(CSymo::symoSub1);
		break;
	default:
		return;
	}
	if (!Next()) {
		KillMe();
	}
}

void CExprBinder::Parse_op2()
{
	switch (Cur().sym) {
	case CSym::symAdd:
		AddSymo_ops(CSymo::symoAdd2);
		break;
	case CSym::symSub:
		AddSymo_ops(CSymo::symoSub2);
		break;
	case CSym::symMul:
		AddSymo_ops(CSymo::symoMul2);
		break;
	case CSym::symDiv:
		AddSymo_ops(CSymo::symoDiv2);
		break;
	case CSym::symMod:
		AddSymo_ops(CSymo::symoMod2);
		break;
	case CSym::symOpLess:
		AddSymo_ops(CSymo::symoOpLess);
		break;
	case CSym::symOpLEqual:
		AddSymo_ops(CSymo::symoOpLEqual);
		break;
	case CSym::symOpGreater:
		AddSymo_ops(CSymo::symoOpGreater);
		break;
	case CSym::symOpGEqual:
		AddSymo_ops(CSymo::symoOpGEqual);
		break;
	case CSym::symOpEqual:
		AddSymo_ops(CSymo::symoOpEqual);
		break;
	case CSym::symOpNotEqual:
		AddSymo_ops(CSymo::symoOpNotEqual);
		break;
	case CSym::symOpLAnd:
		AddSymo_ops(CSymo::symoOpLAnd);
		break;
	case CSym::symOpLOr:
		AddSymo_ops(CSymo::symoOpLOr);
		break;
	default:
		KillMe();
	}
	if (!Next()) {
		KillMe();
	}
}

void CExprBinder::Parse_value()
{
	Parse_op1();

	switch (Cur().sym) {
	case CSym::symDecimal:
		{
			AddSymo_var(CSymo::symoDecimal);
			Next();
			return;
		}
	case CSym::symName:
		{
			CSym sym = Cur();
			if (!Next())
				KillMe();
			if (Cur().sym == CSym::symIndexo) {
				if (!Next())
					KillMe();
				Parse_1(CSym::symIndexc);
				AddSymo_indexo(sym);
			} else {
				AddSymo_var(CSymo::symoName, sym);
			}
			return;
		}
	case CSym::symBracketo:
		{
			if (!Next())
				KillMe();
			Parse_1(CSym::symBracketc);
			AddSymo_bracketo();
			return;
		}
	default:
		{
			KillMe();
		}
	}
}

void CExprBinder::Printo()
{
	Printo(so.var, 0);
}

void CExprBinder::Printo(SymoArray &sa, int f)
{
	if (sa.size() == 0) return;

	CString str(' ', f);
	CString str1(' ', f+1);

	printf(str); puts("{");
	UINT i;
	for (i = 0; i < sa.size(); i++) {
		CSymo *sa1 = sa.at(i);
		printf(str1); printf("%s, '%s'\n"
			, (LPCSTR)sa1->GetOpName()
			, (LPCSTR)sa1->strName
			);
		Printo(sa1->var, f +1);
	}
	printf(str); puts("}");
}

void CExprBinder::Close()
{

}

// ->
//  ->
//   ->
//    ->
// CEvalExpr
//      ->
//       ->
//        ->
//         ->

void CEvalExpr::eval(SymoArray &sa)
{
	UINT i;
	for (i = 0; i < sa.size(); i++) {
		CSymo *so = sa[i];
		switch (so->symo) {
		case CSymo::symoDecimal:
			{
				vals.push_back(strtod(so->strName, NULL));
				break;
			}
		case CSymo::symoName:
			{
				double r = evalNameo.EvalNameo(so->strName);
				vals.push_back(0);
				break;
			}
		case CSymo::symoIndexo:
			{
				CEvalExpr ee(evalNameo);
				if (ee.EvalIndexo(*so) && ee.vals.size() == 1) {
					double r = ee.vals[0];
					r = evalTable.EvalTable(so->strName, (float)r);
					vals.push_back(r);
				} else {
					vals.push_back(0);
				}
				break;
			}
		case CSymo::symoAdd2:
		case CSymo::symoMul2:
		case CSymo::symoSub2:
		case CSymo::symoDiv2:
		case CSymo::symoMod2:
		case CSymo::symoOpEqual:
		case CSymo::symoOpLess:
		case CSymo::symoOpLEqual:
		case CSymo::symoOpGreater:
		case CSymo::symoOpGEqual:
		case CSymo::symoOpNotEqual:
		case CSymo::symoOpLAnd:
		case CSymo::symoOpLOr:
			{
				if (vals.size() < 2)
					KillMe();
				double v2 = vals.back(); vals.pop_back();
				double v1 = vals.back(); vals.pop_back();
				double r;
				if (false);
				else if (so->symo == CSymo::symoAdd2) r = EvalOp::Add2(v1, v2);
				else if (so->symo == CSymo::symoMul2) r = EvalOp::Mul2(v1, v2);
				else if (so->symo == CSymo::symoSub2) r = EvalOp::Sub2(v1, v2);
				else if (so->symo == CSymo::symoDiv2) r = EvalOp::Div2(v1, v2);
				else if (so->symo == CSymo::symoMod2) r = EvalOp::Mod2(v1, v2);
				else if (so->symo == CSymo::symoOpEqual) r = EvalOp::OpEqual(v1, v2);
				else if (so->symo == CSymo::symoOpLess) r = EvalOp::OpLess(v1, v2);
				else if (so->symo == CSymo::symoOpLEqual) r = EvalOp::OpLEqual(v1, v2);
				else if (so->symo == CSymo::symoOpGreater) r = EvalOp::OpGreater(v1, v2);
				else if (so->symo == CSymo::symoOpGEqual) r = EvalOp::OpGEqual(v1, v2);
				else if (so->symo == CSymo::symoOpNotEqual) r = EvalOp::OpNotEqual(v1, v2);
				else if (so->symo == CSymo::symoOpLAnd) r = EvalOp::OpLAnd(v1, v2);
				else if (so->symo == CSymo::symoOpLOr) r = EvalOp::OpLOr(v1, v2);
				vals.push_back(r);
				break;
			}
		case CSymo::symoAdd1:
		case CSymo::symoSub1:
			{
				if (vals.size() < 1)
					KillMe();
				double v1 = vals.back(); vals.pop_back();
				double r;
				if (false);
				else if (so->symo == CSymo::symoAdd1) r = EvalOp::Add1(v1);
				else if (so->symo == CSymo::symoSub1) r = EvalOp::Sub1(v1);
				vals.push_back(r);
				break;
			}
		default:
			{
				KillMe();
			}
		}
	}
}

bool CEvalExpr::Eval(SymoArray &sa)
{
	try {
		eval(sa);
		return true;
	} catch (Error) {
		return false;
	}
}

bool CEvalExpr::Eval(CSymo &so, UINT iIdx)
{
	if (so.var.size() <= iIdx)
		return false;
	if (so.var[iIdx]->var.size() == 0)
		return false;
	return Eval(so.var[iIdx]->var);
}

bool CEvalExpr::EvalIndexo(CSymo &so)
{
	if (so.var.size() == 0)
		return false;
	return Eval(so.var);
}

// ->
//  ->
//   ->
//    ->
// CSparseLWO2
//      ->
//       ->
//        ->
//         ->

bool CSparseLWO2::Parse()
{
	try {
		Parse_1();
		return true;
	} catch (Error) {
		return false;
	}
}

void CSparseLWO2::Parse_1()
{
	//TRACE0("Parse_1\n");
	Sure4r(*(DWORD *)"FORM");
	Skip(4);
	Sure4r(*(DWORD *)"LWO2");
	while (!IsEnd()) {
		DWORD x, v;
		Read4r(x);
		Read4(v);

		if (0) {
			char c[5];
			*(DWORD *)c = x;
			c[4] = 0;
			TRACE1(" %s\n", c);
		}

		CSparseLWO2 mv(mesh);
		mv.pData = pData +iData;
		mv.iData = 0;
		mv.nData = v;
		mv.fLE = fLE;
		//
		if (false);
		else if (x == *(DWORD *)"TAGS") {
			mesh.at0.clear();

			CString strName;
			while (!mv.IsEnd()) {
				mv.ReadName(strName);
				mesh.at0.push_back(strName);
			}
		}
		else if (x == *(DWORD *)"PNTS") {
			mesh.av.clear();
			mesh.av.reserve(v / 12);

			while (!mv.IsEnd()) {
				CLWO2MeshVertex v;
				mv.ReadVtx3(v.v);
				mesh.av.push_back(v);
			}
		}
		else if (x == *(DWORD *)"PTAG") {
			DWORD x;
			mv.Read4r(x);
			if (x == *(DWORD *)"SURF") {
				while (!mv.IsEnd()) {
					DWORD poly;
					WORD texture;
					mv.ReadX(poly);
					mv.Read2(texture);

					if (mesh.at0.size() <= texture)
						KillMe();
					if (mesh.ap.size() <= poly)
						KillMe();
					mesh.ap[poly].texture = texture;
				}
			}
		}
		else if (x == *(DWORD *)"POLS") {
			mv.Sure4r(*(DWORD *)"FACE");
			mesh.ap.clear();
			mesh.ap.reserve((v - 4) / (2 * 2));

			while (!mv.IsEnd()) {
				WORD x;
				mv.Read2(x);
				x &= 1023;
				CLWO2MeshPoly mp;
				UINT i;
				for (i = 0; i < x; i++) {
					DWORD vert;
					mv.ReadX(vert);
					CLWO2MeshPolyMap mpm;
					mpm.v = mpm.vt0 = vert;
					mp.verts.push_back(mpm);
				}
				mesh.ap.push_back(mp);
			}
		}
		else if (x == *(DWORD *)"VMAP") {
			DWORD x;
			mv.Read4r(x);
			if (x == *(DWORD *)"TXUV") {
				mv.Sure2(2);
				mv.ReadName();
				DWORD vert = 0;
				while (!mv.IsEnd()) {
					mv.ReadX(vert);
					if (mesh.av.size() <= vert)
						KillMe();
					CLWO2MeshVertex &v = mesh.av[vert];
					mv.ReadVtx2(v.vt0);
				}
			}
		}
		else if (x == *(DWORD *)"VMAD") {
			DWORD x;
			mv.Read4r(x);
			if (x == *(DWORD *)"TXUV") {
				mv.Sure2(2);
				mv.ReadName();
				DWORD vert = 0, poly = 0;
				while (!mv.IsEnd()) {
					mv.ReadX(vert);
					if (mesh.av.size() <= vert)
						KillMe();
					mv.ReadX(poly);
					if (mesh.ap.size() <= poly)
						KillMe();
					CLWO2MeshVertex mvm;
					UINT vt0New = mesh.av.size();
					mv.ReadVtx2(mvm.vt0);
					mesh.av.push_back(mvm);

					CLWO2MeshPoly &mp = mesh.ap[poly];
					UINT t;
					for (t = 0; t < mp.verts.size(); t++) {
						if (mp.verts[t].vt0 == vert) {
							mp.verts[t].vt0 = vt0New;
						}
					}
				}
			}
		}
		//
		Skip(v + (v & 1));
	}
}

// ->
//  ->
//   ->
//    ->
// CSparseASE
//      ->
//       ->
//        ->
//         ->

bool CSparseASE::Parse()
{
	try {
		Parse_1();
		return true;
	} catch (Error) {
		return false;
	}
}

void CSparseASE::Parse_1()
{
	if (!Next())
		KillMe();
	while (SkipAlWsComment()) {
		CString str;
		ReadLeadTexto(str);
		SkipWs();

		CString strValue;
		if (false);
		else if (str == "3DSMAX_ASCIIEXPORT") {
			ForceReadTexto(strValue);
			if (strValue != "200")
				KillMe();
		}
		else if (str == "GEOMOBJECT") {
			Parse_GEOMOBJECT();
		}
		else if (str == "MATERIAL_LIST") {
			Parse_MATERIAL_LIST();
		}
		else {
			SkipAny();
		}
	}
}

void CSparseASE::Parse_GEOMOBJECT()
{
	mesh.mza.push_back(CAseMeshGEOM());

	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		CString strKey, strValue;
		ReadLeadTexto(strKey);
		SkipWs();

		if (false);
		else if (strKey == "MESH") {
			Parse_MESH();
		}
		else {
			SkipAny();
		}
	}
}

void CSparseASE::Parse_MESH()
{
	CAseMeshGEOM &mz = mesh.mza.back();

	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		CString strKey, strValue;
		ReadLeadTexto(strKey);
		SkipWs();
		int nValue;

		if (false);
		else if (strKey == "MESH_NUMVERTEX") {
			ForceReadTexto(strValue);
			if (!ParseInt(strValue, nValue))
				KillMe();
			mz.mva.resize(nValue);
		}
		else if (strKey == "MESH_NUMFACES") {
			ForceReadTexto(strValue);
			if (!ParseInt(strValue, nValue))
				KillMe();
			mz.mfa.resize(nValue);
		}
		else if (strKey == "MESH_VERTEX_LIST") {
			CString strKey2;
			ForceMatch('{');
			while (!IsMatchFinally('}')) {
				SkipAlWsComment();
				ReadLeadTexto(strKey2);
				if (strKey2 != "MESH_VERTEX")
					KillMe();
				int iIdx;
				SkipWs();
				if (!ReadTexto(strValue) || !ParseInt(strValue, iIdx) || mz.mva.size() <= 0U +iIdx)
					KillMe();
				CAseMeshVertex &mv = mz.mva[iIdx];
				SkipWs();
				if (!ReadTexto(strValue) || !ParseFloat(strValue, mv.v.x))
					KillMe();
				SkipWs();
				if (!ReadTexto(strValue) || !ParseFloat(strValue, mv.v.y))
					KillMe();
				SkipWs();
				if (!ReadTexto(strValue) || !ParseFloat(strValue, mv.v.z))
					KillMe();
			}
		}
		else if (strKey == "MESH_FACE_LIST") {
			CString strKey2;
			ForceMatch('{');
			while (!IsMatchFinally('}')) {
				SkipAlWsComment();
				ReadLeadTexto(strKey2);
				if (strKey2 != "MESH_FACE")
					KillMe();
				int iIdx;
				SkipWs();
				if (!ReadNameo(strValue) || !ParseInt(strValue, iIdx) || mz.mfa.size() <= 0U +iIdx)
					KillMe();
				CAseMeshFace &mf = mz.mfa[iIdx];
				ForceMatch(':');
				while (true) {
					SkipWs();
					if (!ReadNameo(strKey2))
						break;
					ForceMatch(':');
					if (false
						|| strKey2 == "A"
						|| strKey2 == "B"
						|| strKey2 == "C"
						|| strKey2 == "AB"
						|| strKey2 == "BC"
						|| strKey2 == "CA"
					) {
						SkipWs();
						if (!ReadNameo(strValue) || !ParseInt(strValue, nValue))
							KillMe();
						if (false);
						else if (strKey2 == "A") mf.v[0] = nValue;
						else if (strKey2 == "B") mf.v[1] = nValue;
						else if (strKey2 == "C") mf.v[2] = nValue;
					}
					else {
						KillMe();
					}
				}
				SkipWs();
				while (!IsLineBReak()) {
					SkipWs();
					ReadLeadTexto(strKey2);
					if (false
						|| strKey2 == "MESH_SMOOTHING"
						|| strKey2 == "MESH_MTLID"
					) {
						SkipWs();
						ReadNameo(strValue);
						int nValue2;
						if (strKey2 == "MESH_MTLID" && ParseInt(strValue, nValue2) && 0U +nValue2 < mesh.mma.size()) {
							mf.tex0 = nValue2;
						}
						SkipWs();
						while (IsMatch(',', false)) {
							SkipWs();
							ReadNameo(strValue);
							SkipWs();
						}
					}
					else {
						KillMe();
					}
				}
			}
		}
		else if (strKey == "MESH_NUMTVERTEX") {
			ForceReadTexto(strValue);
			if (!ParseInt(strValue, nValue))
				KillMe();
			mz.mtva.resize(nValue);
		}
		else if (strKey == "MESH_TVERTLIST") {
			CString strKey2;
			ForceMatch('{');
			while (!IsMatchFinally('}')) {
				SkipAlWsComment();
				ReadLeadTexto(strKey2);
				if (strKey2 != "MESH_TVERT")
					KillMe();
				int iIdx;
				SkipWs();
				if (!ReadTexto(strValue) || !ParseInt(strValue, iIdx) || mz.mtva.size() <= 0U +iIdx)
					KillMe();

				CAseMeshTVert &mtv = mz.mtva[iIdx];

				float fValue;

				SkipWs();
				if (!ReadTexto(strValue) || !ParseFloat(strValue, fValue))
					KillMe();
				mtv.vt0.x = fValue;

				SkipWs();
				if (!ReadTexto(strValue) || !ParseFloat(strValue, fValue))
					KillMe();
				mtv.vt0.y = fValue;

				SkipWs();
				if (!ReadTexto(strValue) || !ParseFloat(strValue, fValue))
					KillMe();

				printf("");
			}
		}
		else if (strKey == "MESH_NUMTVFACES") {
			ForceReadTexto(strValue);
			if (!ParseInt(strValue, nValue))
				KillMe();
			ASSERT(mz.mfa.size() == nValue);
		}
		else if (strKey == "MESH_TFACELIST") {
			CString strKey2;
			ForceMatch('{');
			while (!IsMatchFinally('}')) {
				SkipAlWsComment();
				ReadLeadTexto(strKey2);
				if (strKey2 != "MESH_TFACE")
					KillMe();
				int iIdx;
				SkipWs();
				if (!ReadTexto(strValue) || !ParseInt(strValue, iIdx) || mz.mfa.size() <= 0U +iIdx)
					KillMe();

				CAseMeshFace &mf = mz.mfa[iIdx];

				SkipWs();
				if (!ReadTexto(strValue) || !ParseInt(strValue, nValue))
					KillMe();
				mf.tf[0] = nValue;

				SkipWs();
				if (!ReadTexto(strValue) || !ParseInt(strValue, nValue))
					KillMe();
				mf.tf[1] = nValue;

				SkipWs();
				if (!ReadTexto(strValue) || !ParseInt(strValue, nValue))
					KillMe();
				mf.tf[2] = nValue;
			}
		}
		else {
			SkipAny();
		}
	};
}

void CSparseASE::Parse_MATERIAL_LIST()
{
	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		CString strKey, strValue;
		ReadLeadTexto(strKey);
		SkipWs();
		int nValue;

		if (false);
		else if (strKey == "MATERIAL_COUNT") {
			ForceReadTexto(strValue);
			if (!ParseInt(strValue, nValue))
				KillMe();
			mesh.mma.resize(nValue);
		}
		else if (strKey == "MATERIAL") {
			int iMaterial;
			ForceReadTexto(strValue);
			if (!ParseInt(strValue, iMaterial))
				KillMe();
			if (mesh.mma.size() <= 0U +iMaterial)
				KillMe();

			ForceMatch('{');
			while (!IsMatchFinally('}')) {
				CString strKey, strValue;
				ReadLeadTexto(strKey);
				SkipWs();

				if (false);
				else if (strKey == "MAP_DIFFUSE") {

					ForceMatch('{');
					while (!IsMatchFinally('}')) {
						CString strKey, strValue;
						ReadLeadTexto(strKey);
						SkipWs();

						if (false);
						else if (strKey == "BITMAP") {
							CString strTex;
							ForceReadTexto(strTex);
							mesh.mma[iMaterial].strTex = strTex;
						}
						else {
							SkipAny();
						}
					}
				}
				else {
					SkipAny();
				}
			}
		}
		else {
			SkipAny();
		}
	};
}

bool CSparseASE::ReadNameo(CString &strText)
{
	strText.Empty();
	while (!IsEnd()) {
		int a = Cur();
		if (!isalnum(a) && a != '_')
			break;
		strText += (char)a;

		Next();
	}
	return strText.GetLength() != 0;
}

void CSparseASE::SkipAny(int nLv)
{
	CString str;

	for (; ; ) {
		if (!SkipWs()) {
			if (nLv == 0)
				break;
			if (!Next())
				KillMe();
			continue;
		}
		if (IsLineBReak()) {
			if (nLv == 0)
				break;
			if (!Next())
				KillMe();
			continue;
		}
		int a = Cur();
		if (a == '{') {
			if (!Next())
				KillMe();
			SkipAny(nLv +1);
			continue;
		}
		if (a == '}') {
			if (nLv == 0)
				KillMe();
			if (!Next())
				KillMe();
			break;
		}
		if (a == '*') {
			ReadLeadTexto(str);
			continue;
		}
		if (!ReadTexto(str)) {
			KillMe();
		}
	}
}

namespace MSHPP
{

// ->
//  ->
//   ->
//    ->
// CHHKMk
//      ->
//       ->
//        ->
//         ->

void CHHKMk::MkHeader(bool fHeader)
{
	if (fHeader) {
		fprintf(f,
			"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\" \"\">"
			"<HTML>"
			"<BODY>"
			"<OBJECT type=\"text/site properties\">"
			"<param name=\"ImageType\" value=\"Folder\" />"
			"</OBJECT>"
			"<UL>\n"
			);
	} else {
		fprintf(f,
			"</UL>"
			"</BODY>"
			"</HTML>\n"
			);
	}
}

void CHHKMk::StartKw(LPCSTR pszDispName)
{
	fprintf(f,
		"<LI>"
		"<OBJECT type=\"text/sitemap\">"
		"<param name=\"Name\" value=\"%s\" />\n"
		, pszDispName
		);
}

void CHHKMk::WriteKwHref(LPCSTR pszDispAlt, LPCSTR pszHref)
{
	fprintf(f,
		"<param name=\"Name\" value=\"%s\" />"
		"<param name=\"Local\" value=\"%s\" />\n"
		, pszDispAlt
		, pszHref
		);
}

void CHHKMk::EndKw()
{
	fprintf(f,
		"</OBJECT>"
		"</LI>\n"
		);
}

// ->
//  ->
//   ->
//    ->
// CHHCMk
//      ->
//       ->
//        ->
//         ->

void CHHCMk::MkHeader(bool fHeader)
{
	if (fHeader) {
		fprintf(f,
			"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\" \"\">"
			"<HTML>"
			"<BODY>"
			"<OBJECT type=\"text/site properties\">"
			"<param name=\"ImageType\" value=\"Folder\" />\n"
			"</OBJECT>"
			"<UL>\n"
			);
	} else {
		fprintf(f,
			"</UL>"
			"</BODY>"
			"</HTML>\n"
			);
	}
}

void CHHCMk::StartDir(LPCSTR pszDispName, LPCSTR pszHref)
{
	fprintf(f,
		"<LI>"
		"<OBJECT type=\"text/sitemap\">"
		"<param name=\"Name\" value=\"%s\" />"
		"<param name=\"%s\" value=\"%s\" />\n"
		"</OBJECT>"
		"</LI>"
		"<UL>\n"
		, pszDispName
		,(pszHref != NULL) ? "Local" : "_Local_"
		, pszHref
		);
}

void CHHCMk::WriteItem(LPCSTR pszDispName, LPCSTR pszHref)
{
	fprintf(f,
		"<LI>"
		"<OBJECT type=\"text/sitemap\">"
		"<param name=\"Name\" value=\"%s\" />"
		"<param name=\"Local\" value=\"%s\" />\n"
		"</OBJECT>"
		"</LI>\n"
		, pszDispName
		, pszHref
		);
}

void CHHCMk::EndDir()
{
	fprintf(f,
		"</UL>\n"
		);
}

// ->
//  ->
//   ->
//    ->
// CHHPMk
//      ->
//       ->
//        ->
//         ->

bool CHHPMk::Mk(LPCSTR pszFiles)
{
	SizeBuff sb;
	if (!Sol::ExtractResource(MAKEINTRESOURCE(rhw.gethhpres()), "DATA", sb))
		return false;

	SetEnvironmentVariable("FILES_HERE", "\r\n"); // SetEnvironmentVariable("FILES_HERE", pszFiles);

	DWORD r1 = ExpandEnvironmentStrings((LPSTR)sb.GetData(), NULL, 0);
	if (r1 == 0)
		return false;
	SizeBuff sbNew(r1);
	DWORD r2 = ExpandEnvironmentStrings((LPSTR)sb.GetData(), (LPSTR)sbNew.GetData(), sbNew.GetSize());
	if (r2 == 0)
		return false;

	if (fwrite(sbNew.GetData(), r2 -1, 1, f) != 1)
		return false;
	return true;
}

// ->
//  ->
//   ->
//    ->
// CHHKTemplate
//      ->
//       ->
//        ->
//         ->

bool CHHKTemplate::Commit(CHHKMk &mk)
{
	KwMMap::iterator
		iterPos = kmm.begin(),
		iterEnd = kmm.end();
	for (; iterPos != iterEnd; ) {
		KwMMap::iterator
			iterHi = kmm.upper_bound(iterPos->first);
		mk.StartKw(iterPos->first);
		for (; iterPos != iterHi; iterPos++) {
			mk.WriteKwHref(iterPos->first, iterPos->second);
		}
		mk.EndKw();
	}
	return true;
}

// ->
//  ->
//   ->
//    ->
// CHHCTemplate
//      ->
//       ->
//        ->
//         ->

bool CHHCTemplate::Commit(CHHCMk &mk)
{
	mk.WriteItem("index.html", "index.html");

	if (km_entityDef.size() != 0) {
		mk.StartDir("entityDef", NULL);
		{
			KwMMap::iterator
				iterPos = km_entityDef.begin(),
				iterEnd = km_entityDef.end();
			for (; iterPos != iterEnd; iterPos++) {
				mk.WriteItem(iterPos->first, iterPos->second);
			}
		}
		mk.EndDir();
	}

	//	KwMMap::iterator
	//		iterPos = km_entityDef.begin(),
	//		iterEnd = km_entityDef.end();
	//	for (; iterPos != iterEnd; ) {
	//		KwMMap::iterator
	//			iterHi = km_entityDef.upper_bound(iterPos->first);
	//		mk.StartDir(iterPos->first, NULL);
	//		for (; iterPos != iterHi; iterPos++) {
	//			mk.WriteItem(iterPos->first, iterPos->second);
	//		}
	//		mk.EndDir();
	//	}

	if (km_Mapobject.size() != 0) {
		mk.StartDir("Mapobject", NULL);
		{
			KwMMap::iterator
				iterPos = km_Mapobject.begin(),
				iterEnd = km_Mapobject.end();
			for (; iterPos != iterEnd; iterPos++) {
				mk.WriteItem(iterPos->first, iterPos->second);
			}
		}
		mk.EndDir();
	}

	return true;
}

// ->
//  ->
//   ->
//    ->
// CHHPTemplate
//      ->
//       ->
//        ->
//         ->

bool CHHPTemplate::Commit(CHHPMk &mk)
{
	CString str;
	FileList::iterator
		iterPos = files.begin(),
		iterEnd = files.end();
	for (; iterPos != iterEnd; iterPos++) {
		str += *iterPos + "\r\n";
	}

	if (!mk.Mk(str))
		return false;
	return true;
}

}; // namespace MSHPP

// ->
//  ->
//   ->
//    ->
// CMaterialStats
//      ->
//       ->
//        ->
//         ->

void CMaterialStats::Run()
{
	DataMap m;

	FILE *f = fopen(OSP_JoinPath(OSP_GetDir(OSP_GetModuleFileName(NULL)), "MatUsed.txt"), "wt");

	{
		CDoom3materialMap::const_iterator
			iterPos = wk.materialMap.begin(),
			iterEnd = wk.materialMap.end();
		for (; iterPos != iterEnd; iterPos++) {
			const CDoom3materialIndentArray &mia = iterPos->second.v;
			DataArray v(mia.size());
			UINT i;
			for (i = 0; i < mia.size(); i++) {
				Data &r = v[i];
				const CDoom3materialIndent &mi = mia[i];
				r.bf1 = mi.bf1;
				r.bf2 = mi.bf2;
			}
			m[v].cx++;
			m[v].names.push_back(iterPos->first);
		}
	}

	DataRMMap rmm;

	{
		DataMap::iterator
			iterPos = m.begin(),
			iterEnd = m.end();
		for (; iterPos != iterEnd; iterPos++) rmm.insert(make_pair(iterPos->second, iterPos->first));
	}

	if (f) {
		fprintf(f,
			"<table cellpadding=\"5\" cellspacing=\"1\">\n"
			"<tr><th>1</th><th>2</th><th>3</th></tr>\n"
			);
		DataRMMap::iterator
			iterPos = rmm.begin(),
			iterEnd = rmm.end();
		for (; iterPos != iterEnd; iterPos++) {
			fprintf(f,
				"<tr><td valign=\"top\">\n"
				);
			DataArray &v = iterPos->second;
			UINT i;
			for (i = 0; i < v.size(); i++) {
				fprintf(f,
					(v[i].bf2 == bfNo) ? "%s" : "%s,%s"
					, GetBlendfName(v[i].bf1)
					, GetBlendfName(v[i].bf2)
					);
				if (i +1 != v.size()) fprintf(f, "<br>");
			}
			fprintf(f,
				"</td>\n"
				);

			MaterialDistributionRMMap mrmm;
			list<CString>::const_iterator
				iter2Pos = iterPos->first.names.begin(),
				iter2End = iterPos->first.names.end();
			for (; iter2Pos != iter2End; iter2Pos++) {
				MaterialDistributionMMap::const_iterator
					iter3Pos = mm.find(*iter2Pos),
					iter3End = mm.end();
				if (iter3Pos == iter3End) {
					mrmm.insert(make_pair(0, *iter2Pos));
				} else {
					mrmm.insert(make_pair(iter3Pos->second, iter3Pos->first));
				}
			}
			fprintf(f,
				"<td valign=\"top\">\n"
				);
			{
				UINT x = 0, cx = mrmm.size();
				MaterialDistributionRMMap::iterator
					iter4Pos = mrmm.begin(),
					iter4End = mrmm.end();
				for (; iter4Pos != iter4End && x < 10; iter4Pos++, x++) {
					fprintf(f,
						"%s%s\n"
						, (x != 0) ? "<br>" : ""
						, (LPCSTR)iter4Pos->second
						);
				}
			}
			fprintf(f,
				"</td>\n"
				);

			fprintf(f,
				"<td valign=\"top\">%d</td>\n"
				, 0U +iterPos->first.cx
				);
		}
		fprintf(f,
			"</table>\n"
			);
	}

	if (f) fclose(f);
}

// ->
//  ->
//   ->
//    ->
// CSparseV2Map
//      ->
//       ->
//        ->
//         ->

bool CSparseV2Map::Parse()
{
	if (!Next()) return false;

	try {
		Parse_1();
		return true;
	} catch (Error) {
		return false;
	}
}

void CSparseV2Map::Parse_1()
{
	CString strKey, strValue, strTex, strBind;

	ForceReadTexto(strKey);
	SkipWs();
	ForceReadTexto(strValue);
	if (strKey != "Version")
		KillMe();
	if (strValue == "2") {
		iVer = 2;
	}
	else if (fq4 && strValue == "3") {
		iVer = 3;
	}
	else {
		KillMe();
	}

	StrSet_t ss;

	while (SkipAlWsComment()) {
		CV2MapEntity &entity = *(mesh.ea.insert(mesh.ea.end(), CV2MapEntity()));
		ForceMatch('{');
		while (!IsMatchFinally('}')) {
			SkipAlWsComment();
			if (ReadTexto(strKey)) {
				SkipWs();
				ForceReadTexto(strValue);

				strKey.MakeLower();
				entity.m[*(ss.insert(strKey).first)] = *(ss.insert(strValue).first);
			} else {
				ForceMatch('{');
				SkipAlWsComment();
				ForceReadTexto(strKey);
				if (false);
				else if (strKey == "brushDef3") {
					V2MapBrushArray_t &va = entity.va;
					va.push_back(CV2MapBrush());
					CV2MapBrush &vv = va.back();
					ForceMatch('{');
					while (!IsMatchFinally('}')) {
						vv.pa.push_back(CV2MapPlane());
						CV2MapPlane &vp = vv.pa.back();

						ForceMatch('(');
						SkipWs(); Force1((ForceReadTexto(strValue), ParseFloat(strValue, vp.v.x)));
						SkipWs(); Force1((ForceReadTexto(strValue), ParseFloat(strValue, vp.v.y)));
						SkipWs(); Force1((ForceReadTexto(strValue), ParseFloat(strValue, vp.v.z)));
						SkipWs(); Force1((ForceReadTexto(strValue), ParseFloat(strValue, vp.fr)));
						ForceMatch(')');

						ForceMatch('(');
						ForceMatch('(');
						SkipWs(); Force1((ForceReadTexto(strValue), ParseFloat(strValue, vp.m32[0][0])));
						SkipWs(); Force1((ForceReadTexto(strValue), ParseFloat(strValue, vp.m32[0][1])));
						SkipWs(); Force1((ForceReadTexto(strValue), ParseFloat(strValue, vp.m32[0][2])));
						ForceMatch(')');
						ForceMatch('(');
						SkipWs(); Force1((ForceReadTexto(strValue), ParseFloat(strValue, vp.m32[1][0])));
						SkipWs(); Force1((ForceReadTexto(strValue), ParseFloat(strValue, vp.m32[1][1])));
						SkipWs(); Force1((ForceReadTexto(strValue), ParseFloat(strValue, vp.m32[1][2])));
						ForceMatch(')');
						ForceMatch(')');

						SkipWs(); ForceReadTexto(strValue); vp.strTex = *(ss.insert(strValue).first);
						if (iVer == 2) {
							SkipWs(); ForceReadTexto(strValue);
							SkipWs(); ForceReadTexto(strValue);
							SkipWs(); ForceReadTexto(strValue);
						}

						if (fStat) mm[strTex]++;
					}
				}
				else if (strKey == "patchDef3") {
					V2MapPatchDef3Array_t &p3a = entity.p3a;
					p3a.push_back(CV2MapPatchDef3());
					CV2MapPatchDef3 &p3 = p3a.back();
					p3.fIs3 = true;

					int cx;
					int cy;
					ForceMatch('{');
					SkipAlWsComment();
					ForceReadTexto(strTex);
					ForceMatch('(');
					SkipWs(); ForceReadTexto(strValue); Force1(ParseInt(strValue, cy));
					SkipWs(); ForceReadTexto(strValue); Force1(ParseInt(strValue, cx));
					SkipWs(); ForceReadTexto(strValue); Force1(ParseInt(strValue, p3.t[0]));
					SkipWs(); ForceReadTexto(strValue); Force1(ParseInt(strValue, p3.t[1]));
					SkipWs(); ForceReadTexto(strValue); Force1(ParseInt(strValue, p3.t[2]));
					SkipWs(); ForceReadTexto(strValue); Force1(ParseInt(strValue, p3.t[3]));
					SkipWs(); ForceReadTexto(strValue); Force1(ParseInt(strValue, p3.t[4]));
					ForceMatch(')');

					Force1(cx > 0 && cy > 0);

					p3.cx = cx;
					p3.cy = cy;
					p3.ca.resize(cx*cy);
					p3.strTex = strTex;

					ForceMatch('(');
					for (int y = 0; y < cy; y++) {
						ForceMatch('(');
						for (int x = 0; x < cx; x++) {
							CV2MapPatchDef3Coord &p3c = p3.ca[x +cx*y];
							ForceMatch('(');
							SkipWs(); ForceReadTexto(strValue); Force1(ParseFloat(strValue, p3c.v.x));
							SkipWs(); ForceReadTexto(strValue); Force1(ParseFloat(strValue, p3c.v.y));
							SkipWs(); ForceReadTexto(strValue); Force1(ParseFloat(strValue, p3c.v.z));
							SkipWs(); ForceReadTexto(strValue); Force1(ParseFloat(strValue, p3c.tv.x));
							SkipWs(); ForceReadTexto(strValue); Force1(ParseFloat(strValue, p3c.tv.y));
							ForceMatch(')');
						}
						ForceMatch(')');
					}
					ForceMatch(')');

					ForceMatch('}');

					if (fStat) mm[strTex]++;
				}
				else if (strKey == "patchDef2") {
					V2MapPatchDef3Array_t &p3a = entity.p3a;
					p3a.push_back(CV2MapPatchDef3());
					CV2MapPatchDef3 &p3 = p3a.back();
					p3.fIs3 = false;
					p3.t[3] = p3.t[4] = 0;

					int cx;
					int cy;
					ForceMatch('{');
					SkipAlWsComment();
					ForceReadTexto(strTex);
					ForceMatch('(');
					SkipWs(); ForceReadTexto(strValue); Force1(ParseInt(strValue, cy));
					SkipWs(); ForceReadTexto(strValue); Force1(ParseInt(strValue, cx));
					SkipWs(); ForceReadTexto(strValue); Force1(ParseInt(strValue, p3.t[0]));
					SkipWs(); ForceReadTexto(strValue); Force1(ParseInt(strValue, p3.t[1]));
					SkipWs(); ForceReadTexto(strValue); Force1(ParseInt(strValue, p3.t[2]));
					ForceMatch(')');

					Force1(cx > 0 && cy > 0);

					p3.cx = cx;
					p3.cy = cy;
					p3.ca.resize(cx*cy);
					p3.strTex = strTex;

					ForceMatch('(');
					for (int y = 0; y < cy; y++) {
						ForceMatch('(');
						for (int x = 0; x < cx; x++) {
							CV2MapPatchDef3Coord &p3c = p3.ca[x +cx*y];
							ForceMatch('(');
							SkipWs(); ForceReadTexto(strValue); Force1(ParseFloat(strValue, p3c.v.x));
							SkipWs(); ForceReadTexto(strValue); Force1(ParseFloat(strValue, p3c.v.y));
							SkipWs(); ForceReadTexto(strValue); Force1(ParseFloat(strValue, p3c.v.z));
							SkipWs(); ForceReadTexto(strValue); Force1(ParseFloat(strValue, p3c.tv.x));
							SkipWs(); ForceReadTexto(strValue); Force1(ParseFloat(strValue, p3c.tv.y));
							ForceMatch(')');
						}
						ForceMatch(')');
					}
					ForceMatch(')');

					ForceMatch('}');

					if (fStat) mm[strTex]++;
				}
				else {
					KillMe();
				}
				ForceMatch('}');
			}
		}

		entity.o.Empty();
		entity.rotation.Identity();
		entity.fHasBind = false;

		if (Sol::GetValue(entity.m, "name", strValue) && strValue.GetLength() > 0) {
			entity.strName = strValue;
			mesh.em[strValue] = &entity;

			if (Sol::GetValue(entity.m, "bind", strBind) && strBind.GetLength() > 0) {
				entity.fHasBind = true;
				mesh.vrmm.insert(make_pair(strBind, strValue));
				mesh.vs.insert(strBind);
			}
		}
		if (Sol::GetValue(entity.m, "origin", strValue)) {
			if (3 != sscanf(strValue, "%f %f %f", &entity.o.x, &entity.o.y, &entity.o.z)) {
				entity.o.Empty();
			}
		}
		float fAngle;
		if (Sol::GetValue(entity.m, "angle", strValue) && ParseFloat(strValue, fAngle)) {
			entity.rotation.RotateZ(entity.rotation, (180.f + fAngle) /180.f *3.14f);
		}
		if (Sol::GetValue(entity.m, "rotation", strValue)) {
			if (9 != sscanf(strValue, "%f %f %f %f %f %f %f %f %f"
				, &entity.rotation._11
				, &entity.rotation._12
				, &entity.rotation._13
				, &entity.rotation._21
				, &entity.rotation._22
				, &entity.rotation._23
				, &entity.rotation._31
				, &entity.rotation._32
				, &entity.rotation._33
				)
			) {
				entity.rotation.Identity();
			} else {
			//	My3DMath::Mtx m;
			//	m.RotateZ(m, 180);
			//	entity.rotation = entity.rotation * m;
			}
		}
	}
	printf("");
}

namespace NfMD5
{

// ->
//  ->
//   ->
//    ->
// ParseMD5Mesh_t
//      ->
//       ->
//        ->
//         ->

bool ParseMD5Mesh_t::Parse()
{
	if (!Next()) return false;

	iCurMesh = 0;

	try {
		while (SkipAlWsComment()) {
			CString strKey, strValue;
			int nValue;
			if (!ReadTexto(strKey))
				KillMe();
			strKey.MakeLower();
			SkipWs();
			if (false);
			else if (strKey == "md5version") {
				Force1((ForceReadTexto(strValue), strValue == "10"));
			}
			else if (strKey == "commandline") {
				Force1((ForceReadTexto(strValue), true));
			}
			else if (strKey == "numjoints") {
				Force1((ForceReadTexto(strValue), ParseInt(strValue, nValue)));
				mesh.aJ.resize(nValue);
			}
			else if (strKey == "nummeshes") {
				Force1((ForceReadTexto(strValue), ParseInt(strValue, nValue)));
				mesh.aM.resize(nValue);
			}
			else if (strKey == "joints") {
				Parse_joints();
			}
			else if (strKey == "mesh") {
				Parse_mesh();
			}
			else {
				KillMe();
			}
		}

		return true;
	} catch (Error) {
		return false;
	}
}

void ParseMD5Mesh_t::Parse_joints()
{
	ForceMatch('{');
	int iIndex = 0;
	while (!IsMatchFinally('}')) {
		if (mesh.aJ.size() <= (UINT)iIndex)
			KillMe();

		MD5MeshJoint_t &j = mesh.aJ[iIndex];
		CString strValue;

		ForceReadTexto(j.strName);
		SkipWs();
		Force1((ForceReadTexto(strValue), ParseInt(strValue, j.iJ) && (j.iJ == -1 || (UINT)j.iJ < mesh.aJ.size())));
		My3DMath::Vtx3 q;
		ReadVtx3(j.o);
		ReadVtx3(q);

		j.q.Identify(q);

		j.pRefJ = (j.iJ < 0)
			? NULL
			: &mesh.aJ.at(j.iJ)
			;

		iIndex++;
	}
}

void ParseMD5Mesh_t::ReadVtx3(My3DMath::Vtx3 &v)
{
	ForceMatch('(');
	CString str[3];
	SkipWs();
	if (!ReadTexto(str[0]) || !ParseFloat(str[0], v.x))
		KillMe();
	SkipWs();
	if (!ReadTexto(str[1]) || !ParseFloat(str[1], v.y))
		KillMe();
	SkipWs();
	if (!ReadTexto(str[2]) || !ParseFloat(str[2], v.z))
		KillMe();
	ForceMatch(')');
}

void ParseMD5Mesh_t::ReadVtx2(My3DMath::Vtx2 &v)
{
	ForceMatch('(');
	CString str[2];
	SkipWs();
	if (!ReadTexto(str[0]) || !ParseFloat(str[0], v.x))
		KillMe();
	SkipWs();
	if (!ReadTexto(str[1]) || !ParseFloat(str[1], v.y))
		KillMe();
	ForceMatch(')');
}

void ParseMD5Mesh_t::Parse_mesh()
{
	if (mesh.aM.size() <= (UINT)iCurMesh)
		KillMe();

	MD5MeshModel_t &mm = mesh.aM[iCurMesh];

	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		CString strKey, strValue;
		int nValue;

		ForceReadTexto(strKey);

		SkipWs();
		if (false);
		else if (strKey == "shader") {
			ForceReadTexto(mm.strShader);
		}
		else if (strKey == "numverts") {
			Force1((ForceReadTexto(strValue), ParseInt(strValue, nValue)));
			mm.aV.resize(nValue);
		}
		else if (strKey == "vert") {
			int iElem;
			Force1((ForceReadTexto(strValue), ParseInt(strValue, iElem)));
			Force1((UINT)iElem < mm.aV.size());
			MD5MeshVert_t &mv = mm.aV[iElem];

			SkipWs();
			ReadVtx2(mv.o);
			SkipWs();
			Force1(ReadTexto(strValue) && ParseInt(strValue, mv.iW));
			SkipWs();
			Force1(ReadTexto(strValue) && ParseInt(strValue, mv.nW));
		}
		else if (strKey == "numtris") {
			Force1((ForceReadTexto(strValue), ParseInt(strValue, nValue)));
			mm.aT.resize(nValue);
		}
		else if (strKey == "tri") {
			int iElem;
			Force1((ForceReadTexto(strValue), ParseInt(strValue, iElem)));
			Force1((UINT)iElem < mm.aT.size());
			MD5MeshTri_t &mt = mm.aT[iElem];

			SkipWs();
			Force1(ReadTexto(strValue) && ParseInt(strValue, mt.iV[0]));
			SkipWs();
			Force1(ReadTexto(strValue) && ParseInt(strValue, mt.iV[1]));
			SkipWs();
			Force1(ReadTexto(strValue) && ParseInt(strValue, mt.iV[2]));

			Force1((UINT)mt.iV[0] < mm.aV.size());
			Force1((UINT)mt.iV[1] < mm.aV.size());
			Force1((UINT)mt.iV[2] < mm.aV.size());

			mt.pV[0] = &mm.aV.at(mt.iV[0]);
			mt.pV[1] = &mm.aV.at(mt.iV[1]);
			mt.pV[2] = &mm.aV.at(mt.iV[2]);
		}
		else if (strKey == "numweights") {
			Force1((ForceReadTexto(strValue), ParseInt(strValue, nValue)));
			mm.aW.resize(nValue);
		}
		else if (strKey == "weight") {
			int iElem;
			Force1((ForceReadTexto(strValue), ParseInt(strValue, iElem)));
			Force1((UINT)iElem < mm.aW.size());
			MD5MeshWeight_t &mw = mm.aW[iElem];

			SkipWs();
			Force1(ReadTexto(strValue) && ParseInt(strValue, mw.iJ));
			SkipWs();
			Force1(ReadTexto(strValue) && ParseFloat(strValue, mw.nMass));

			ReadVtx3(mw.o);

			Force1((UINT)mw.iJ < mesh.aJ.size());

			mw.pJ = &mesh.aJ.at(mw.iJ);
		}
		else {
			KillMe();
		}
	}

	{
		size_t i;
		for (i = 0; i < mm.aV.size(); i++) {
			MD5MeshVert_t &mv = mm.aV[i];

			Force1((UINT)(mv.iW + mv.nW) <= mm.aW.size());

			size_t x, cx = (size_t)mv.nW;

			mv.ppW.resize(cx);

			for (x = 0; x < cx; x++) mv.ppW.at(x) = &mm.aW.at(mv.iW + x);
		}
	}

	iCurMesh++;
}

// ->
//  ->
//   ->
//    ->
// ParseMD5Anim_t
//      ->
//       ->
//        ->
//         ->

bool ParseMD5Anim_t::Parse()
{
	if (!Next()) return false;

	try {
		while (SkipAlWsComment()) {
			CString strKey, strValue;
			int nValue;
			ForceReadTexto(strKey);
			strKey.MakeLower();
			SkipWs();
			if (false);
			else if (strKey == "md5version") {
				Force1((ForceReadTexto(strValue), strValue == "10"));
			}
			else if (strKey == "commandline") {
				Force1((ForceReadTexto(strValue), true));
			}
			else if (strKey == "numframes") {
				Force1((ForceReadTexto(strValue), ParseInt(strValue, nValue)));
				anim.framea.resize(nValue);
				anim.bounda.resize(nValue);
			}
			else if (strKey == "numjoints") {
				Force1((ForceReadTexto(strValue), ParseInt(strValue, nValue)));
				anim.jointa.resize(nValue);
				anim.baseframe.resize(nValue);
			}
			else if (strKey == "framerate") {
				Force1((ForceReadTexto(strValue), ParseInt(strValue, nValue)));
				anim.nFrameRate = nValue;
			}
			else if (strKey == "numanimatedcomponents") {
				Force1((ForceReadTexto(strValue), ParseInt(strValue, nValue)));
				anim.nAnimatedComp = nValue;
			}
			else if (strKey == "hierarchy") {
				Parse_hierarchy();
			}
			else if (strKey == "bounds") {
				Parse_bounds();
			}
			else if (strKey == "baseframe") {
				Parse_baseframe();
			}
			else if (strKey == "frame") {
				Parse_frame();
			}
			else {
				KillMe();
			}
		}

		return true;
	} catch (Error) {
		return false;
	}
}

void ParseMD5Anim_t::Parse_hierarchy()
{
	SkipAlWsComment();

	UINT iIndex = 0;

	// # bounds: frame count
	// # baseframe: joint count

	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		Force1(iIndex < anim.jointa.size());
		CString strKey;
		ForceReadTexto(strKey);
		CString str1, str2, str3;
		int iJointRef, nFlags, iVarIndex;
		SkipWs();
		Force1((ForceReadTexto(str1), ParseInt(str1, iJointRef)));
		SkipWs();
		Force1((ForceReadTexto(str1), ParseInt(str1, nFlags)));
		SkipWs();
		Force1((ForceReadTexto(str1), ParseInt(str1, iVarIndex)));

		Force1(iJointRef < 0 || ((UINT)iJointRef < anim.jointa.size()));

		MD5AnimJoint_t &j = anim.jointa.at(iIndex);
		j.strName = strKey;
		j.iRefJ = iJointRef;
		j.f6 = nFlags;

		j.pRefJ = (iJointRef < 0)
			? NULL
			: &anim.jointa.at(iJointRef)
			;

		iIndex++;
	}
}

void ParseMD5Anim_t::Parse_bounds()
{
	SkipAlWsComment();

	UINT iIndex = 0;

	My3DMath::Vtx3 v;

	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		Force1(iIndex < anim.bounda.size());
		MD5AnimBound_t &r = anim.bounda[iIndex];

		ReadVtx3(r.vmin);
		ReadVtx3(r.vmax);

		iIndex++;
	}
}

void ParseMD5Anim_t::ReadVtx3(My3DMath::Vtx3 &v)
{
	ForceMatch('(');
	CString str[3];
	SkipWs();
	if (!ReadTexto(str[0]) || !ParseFloat(str[0], v.x))
		KillMe();
	SkipWs();
	if (!ReadTexto(str[1]) || !ParseFloat(str[1], v.y))
		KillMe();
	SkipWs();
	if (!ReadTexto(str[2]) || !ParseFloat(str[2], v.z))
		KillMe();
	ForceMatch(')');
}

void ParseMD5Anim_t::Parse_baseframe()
{
	SkipAlWsComment();

	UINT iIndex = 0;

	ForceMatch('{');
	while (!IsMatchFinally('}')) {
		Force1(iIndex < anim.baseframe.size() && iIndex < anim.jointa.size());
		MD5AnimPatch_t &e = anim.baseframe.at(iIndex);
		My3DMath::Vtx3 q;
		ReadVtx3(e.o);
		ReadVtx3(e.p);

		iIndex++;
	}
}

void ParseMD5Anim_t::Parse_frame()
{
	SkipAlWsComment();

	CString strValue;
	int iFrame;
	Force1((ForceReadTexto(strValue), ParseInt(strValue, iFrame)));
	Force1((UINT)iFrame < anim.framea.size());
	ForceMatch('{');
	UINT i;
	// 0x01 Tx
	// 0x02 Ty
	// 0x04 Tz
	// 0x08 Qx
	// 0x10 Qy
	// 0x20 Qz
	MD5AnimFrame_t &f = anim.framea.at(iFrame);
	f = anim.baseframe;
	for (i = 0; i < anim.jointa.size(); i++) {
		MD5AnimJoint_t &j = anim.jointa.at(i);
		MD5AnimPatch_t &e = f.at(i);
		for (int x = 0; x < 6; x++) {
			if (j.f6 & (1 << x)) {
				SkipAlWsComment();
				float f;
				Force1((ForceReadTexto(strValue), ParseFloat(strValue, f)));
				switch (x) {
				case 0: e.o.x  = f; break;
				case 1: e.o.y  = f; break;
				case 2: e.o.z  = f; break;
				case 3: e.p.x  = f; break;
				case 4: e.p.y  = f; break;
				case 5: e.p.z  = f; break;
				}
			}
		}

		e.q.Identify(e.p);
	}
	ForceMatch('}');
}

};

// ->
//  ->
//   ->
//    ->
// CSparseMapExpr
//      ->
//       ->
//        ->
//         ->

bool CSparseMapExpr::Parse()
{
	try {
		mt.iFirst = Parse_map();
		return true;
	} catch (Error) {
		return false;
	}
}

size_t CSparseMapExpr::Parse_map()
{
	SkipAlWsComment();
	CString strName;
	ForceReadTexto(strName);
	CString strKey = strName;
	strName.MakeLower();
	CDoom3materialTexoMap m1;
	CString strValue;
	if (false);
	else if (strName == "addnormals") {
		ForceMatch('(');
		m1.x0 = Parse_map();
		ForceMatch(',');
		m1.x1 = Parse_map();
		ForceMatch(')');

		m1.tt = texAn;
	}
	else if (strName == "heightmap") {
		ForceMatch('(');
		m1.x0 = Parse_map();
		ForceMatch(',');
		SkipAlWsComment();
		Force1(ReadTexto(strValue) && ParseFloat(strValue, m1.v0));
		ForceMatch(')');

		m1.tt = texHm;
	}
	else if (strName == "makealpha") {
		ForceMatch('(');
		m1.x0 = Parse_map();
		ForceMatch(')');

		m1.tt = texMa;
	}
	else if (strName == "makeintensity") {
		ForceMatch('(');
		m1.x0 = Parse_map();
		ForceMatch(')');

		m1.tt = texMi;
	}
	else if (strName == "smoothnormals") {
		ForceMatch('(');
		m1.x0 = Parse_map();
		ForceMatch(')');

		m1.tt = texSn;
	}
	else if (strName == "add") {
		ForceMatch('(');
		m1.x0 = Parse_map();
		ForceMatch(',');
		m1.x1 = Parse_map();
		ForceMatch(')');

		m1.tt = texAx;
	}
	else if (strName == "scale") {
		ForceMatch('(');
		m1.x0 = Parse_map();
		ForceMatch(',');
		SkipAlWsComment();
		Force1(ReadTexto(strValue) && ParseFloat(strValue, m1.v0));
		ForceMatch(',');
		SkipAlWsComment();
		Force1(ReadTexto(strValue) && ParseFloat(strValue, m1.v1));
		ForceMatch(',');
		SkipAlWsComment();
		Force1(ReadTexto(strValue) && ParseFloat(strValue, m1.v2));
		ForceMatch(',');
		SkipAlWsComment();
		Force1(ReadTexto(strValue) && ParseFloat(strValue, m1.v3));
		ForceMatch(')');

		m1.tt = texSx;
	}
	else if (strName == "invertalpha") {
		ForceMatch('(');
		m1.x0 = Parse_map();
		ForceMatch(')');

		m1.tt = texIa;
	}
	else if (strName == "downsize") {
		ForceMatch('(');
		m1.x0 = Parse_map();
		ForceMatch(',');
		SkipAlWsComment();
		Force1(ReadTexto(strValue) && ParseFloat(strValue, m1.v1));
		ForceMatch(')');

		m1.tt = texDs;
	}
	else {
		m1.tt = texFlat;
		m1.strName = strKey;
	}
	size_t r = mt.size();
	mt.push_back(m1);

	return r;
}

// ->
//  ->
//   ->
//    ->
// CSparseTarga
//      ->
//       ->
//        ->
//         ->

bool CSparseTarga::Parse()
{
	// http://www.openspc2.org/format/TGA/

#pragma pack(push, 1)
	struct Header {
		BYTE x1;
		BYTE fColorMapped;
		BYTE iFormat;
		WORD x2;
		WORD x3;
		BYTE x4;
		WORD x5;
		WORD x6;
		WORD cx;
		WORD cy;
		BYTE nBitCount;
		BYTE x7;
	};
#pragma pack(pop)

	Header v;
	if (!SurelyReadBytes(&v, sizeof(v)))
		return false;
	if (v.fColorMapped != 0 || v.x2 != 0 || v.x3 != 0 || (v.x7 & 0xD0) != 0)
		return false;
	if (false);
	else if (v.iFormat ==  2 && v.nBitCount == 24 && (v.x4 == 0 || v.x4 == 24));
	else if (v.iFormat ==  2 && v.nBitCount == 32 && (v.x4 == 0 || v.x4 == 32));
	else if (v.iFormat ==  3 && v.nBitCount == 8  && (v.x4 == 0 || v.x4 == 8 ));
	else if (v.iFormat == 10 && v.nBitCount == 24 && (v.x4 == 0 || v.x4 == 24));
	else if (v.iFormat == 10 && v.nBitCount == 32 && (v.x4 == 0 || v.x4 == 32));
	else {
		return false;
	}

	bool fUpdown = ((v.x7 & 0x20) ? true : false);
	bool fRLE = (v.iFormat == 10);

	if (!rIma.Create(v.cx, v.cy, v.nBitCount))
		return false;
	if (!SurelySkipBytes(v.x1))
		return false;
	if (!fRLE) {
		for (UINT y = 0; y < v.cy; y++) {
			BYTE *pData = rIma.GetVert(fUpdown
				? (y)
				: (v.cy -y -1)
				);
			if (!SurelyReadBytes(pData, rIma.nPitch))
				return false;
		}
	} else {
		CDoom3ImaPix iw(rIma, fUpdown);
		UINT iPix = 0, nPix = v.cy * v.cx;
		BYTE x1 = v.nBitCount / 8;
		char aPix[4];
		for (; iPix < nPix; ) {
			BYTE x;
			if (!ReadByte(x))
				return false;
			BYTE cx = (x & 0x7F) + 1;
			if (x & 0x80) {
				if (!SurelyReadBytes(aPix, x1))
					return false;
				if (!iw.WritePix(aPix, cx))
					return false;
			} else {
				for (BYTE i = 0; i < cx; i++) {
					if (!SurelyReadBytes(aPix, x1))
						return false;
					if (!iw.WritePix(aPix, 1))
						return false;
				}
			}
			iPix += cx;
		}
	}
	return true;
}

// ->
//  ->
//   ->
//    ->
// CDoom3FlatIma
//      ->
//       ->
//        ->
//         ->

const CDoom3FlatIma &CDoom3FlatIma::operator =(const CDoom3FlatIma &s)
{
	fv = s.fv;

	if (fv.GetData() == NULL) {
		cx = 0;
		cy = 0;
		nBitCount = 0;
		nPitch = 0;
		nDataLen = 0;
	} else {
		cx = s.cx;
		cy = s.cy;
		nBitCount = s.nBitCount;
		nPitch = s.nPitch;
		nDataLen = s.nDataLen;
	}

	pData = fv.GetData();

	return s;
}

bool CDoom3FlatIma::Create(const CDoom3FlatIma &s)
{
	return Create(s.cx, s.cy, s.nBitCount);
}

bool CDoom3FlatIma::Create(UINT cx, UINT cy, WORD nBitCount)
{
	for (; ; ) {
		if (nBitCount != 8 && nBitCount != 24 && nBitCount != 32)
			break;
		this->nPitch = nBitCount * cx / 8;
		this->nDataLen = this->nPitch * cy;
		if (!fv.Alloc(this->nDataLen))
			break;
		this->pData = (BYTE *)fv.GetData();
		this->cx = cx;
		this->cy = cy;
		this->nBitCount = nBitCount;

		return true;
	}
	Close();
	return false;
}

void CDoom3FlatIma::Close()
{
	cx = cy = 0;
	nBitCount = nPitch = 0;
	pData = NULL;

	fv.Free();
}

BYTE *CDoom3FlatIma::GetVert(UINT y)
{
	return fv.GetData() + nPitch * y;
}

namespace MaterialTexo {

// ->
//  ->
//   ->
//    ->
// TexoEval
//      ->
//       ->
//        ->
//         ->

bool TexoEval::Eval(CDoom3FlatIma &rIma)
{
	try {
		if (mt.empty())
			return false;
		if (!eval(mt.iFirst, rIma))
			return false;
		return true;
	} catch (Error) {
		return false;
	}
}

bool TexoEval::eval(size_t i, CDoom3FlatIma &rIma)
{
	CDoom3materialTexoMap &mtm = mt[i];

	CDoom3FlatIma aIma0, aIma1;

	switch (mtm.tt) {
	case texFlat:
		{
			if (!rTex.LoadIma(mtm.strName, rIma))
				return false;
			return true;
		}
	case texMa:
		{
			if (!eval(mtm.x0, aIma0))
				return false;
			switch (aIma0.nBitCount) {
			case 8:
				{
					rIma = aIma0;
					return true;
				}
			case 24:
				{
					UINT x, y, cx = aIma0.cx, cy = aIma0.cy;
					if (!rIma.Create(aIma0.cx, aIma0.cy, 8))
						return false;
					for (y = 0; y < cy; y++) {
						BYTE *pVertSrc = aIma0.GetVert(y);
						BYTE *pVertDst = rIma.GetVert(y);
						for (x = 0; x < cx; x++, pVertDst++, pVertSrc += 3) {
							*pVertDst = (0U +pVertSrc[0] +pVertSrc[1] +pVertSrc[2]) / 3;
						}
					}
					return true;
				}
			case 32:
				{
					UINT x, y, cx = aIma0.cx, cy = aIma0.cy;
					if (!rIma.Create(aIma0.cx, aIma0.cy, 8))
						return false;
					for (y = 0; y < cy; y++) {
						BYTE *pVertSrc = aIma0.GetVert(y);
						BYTE *pVertDst = rIma.GetVert(y);
						for (x = 0; x < cx; x++, pVertDst++, pVertSrc += 4) {
							*pVertDst = (0U +pVertSrc[0] +pVertSrc[1] +pVertSrc[2] +pVertSrc[3]) / 4;
						}
					}
					return true;
				}
			default:
				{
					ASSERT(false);
					return false;
				}
			}
		}
	case texAn:
	case texHm:
	case texMi:
	case texSn:
	case texAx:
	case texSx:
	case texIa:
		{
			if (!eval(mtm.x0, rIma))
				return false;

			return true;
		}
	default:
		{
			return false;
		}
	}
}

};

// ->
//  ->
//   ->
//    ->
// CDoom3table
//      ->
//       ->
//        ->
//         ->

float CDoom3table::Eval(float x)
{
	if (fSnap) {
		// snap
		int i = int(x * m + 0.5f);
		if (fClamp) {
			// clamp
			if (i < 0) i = 0;
			if (m <=i) i = m -1;
		} else {
			// repeat
			i = ((i % m) + m) % m;
		}
		return vec[i];
	} else {
		// linear interpolation (lerp?)
		int x1 = int(x * m   );
		int x2 = int(x * m +1);
		float f = x * m -x1;
		x1 = (x1 + m) % m;
		x2 = (x2 + m) % m;
		float r = 0
			+ vec[x1] * (1.0f -f)
			+ vec[x2] * (      f)
			;
		return r;
	}
}

// ->
//  ->
//   ->
//    ->
// CEaseWriter
//      ->
//       ->
//        ->
//         ->

bool CEaseWriter::Write()
{
	if (f == NULL) return false;

	fprintf(f, "%u\n", 0U +mesh.meshes.size());

	size_t i;
	for (i = 0; i < mesh.meshes.size(); i++) {
		CMD5MeshMesh &mm = mesh.meshes[i];
		size_t x;

		fprintf(f, "%u\n", 0U +mm.verts.size());
		for (x = 0; x < mm.verts.size(); x++) {
			CMD5MeshMeshVert &v0 = mm.verts[x];
			fprintf(f, "%f,%f", v0.o.x, v0.o.y);
			My3DMath::Vtx3 v3;
			mm.InterpolateVertex(mesh.joints, x, v3);
			fprintf(f, ",%f,%f,%f\n", v3.x, v3.y, v3.z);
		}

		fprintf(f, "%u\n", 0U +mm.tris.size());
		for (x = 0; x < mm.tris.size(); x++) {
			CMD5MeshMeshTri &v0 = mm.tris[x];
			fprintf(f, "%u,%u,%u\n", v0.v[0], v0.v[1], v0.v[2]);
		}
	}

	return true;
}

// ->
//  ->
//   ->
//    ->
// CV2MapDeck
//      ->
//       ->
//        ->
//         ->

void CV2MapDeck::SearchDep(CString strName, StrSet_t &ss) const
{
	if (ss.find(strName) != ss.end()) return;

	ss.insert(strName);

	V2MapBindRMMap_t::const_iterator
		iterPos = vrmm.lower_bound(strName),
		iterEnd = vrmm.upper_bound(strName);
	for (; iterPos != iterEnd; iterPos++) {
		SearchDep(iterPos->second, ss);
	}
}

CV2MapEntity *CV2MapDeck::FindEntity(CString strName) const
{
	V2MapEntityPtrMap_t::const_iterator
		iterPos = em.find(strName),
		iterEnd = em.end();
	if (iterPos != iterEnd)
		return iterPos->second;

	return NULL;
}

// ->
//  ->
//   ->
//    ->
// CV2MapWriter
//      ->
//       ->
//        ->
//         ->

bool CV2MapWriter::MkHeader()
{
	fprintf(f,
		"Version 2\n"
		);
	iEntity = 0;

	return true;
}

bool CV2MapWriter::MkEntityHeader()
{
	fprintf(f,
		"// entity %u\n"
		"{\n"
		, iEntity
		);
	iEntity++;
	iPrimitive = 0;

	return true;
}

bool CV2MapWriter::MkEntityFooter()
{
	fprintf(f,
		"}\n"
		);
	return true;
}

bool CV2MapWriter::MkEntity(CV2MapEntity &entity)
{
	VarMap::iterator
		iterPos = entity.m.begin(),
		iterEnd = entity.m.end();
	for (; iterPos != iterEnd; iterPos++) {
		fprintf(f,
			"\"%s\" \"%s\"\n"
			, (LPCSTR)iterPos->first
			, (LPCSTR)iterPos->second
			);
	}
	if (entity.va.size() != 0) {
		size_t i;
		for (i = 0; i < entity.va.size(); i++) {
			fprintf(f,
				"// primitive %u\n"
				"{\n"
				" brushDef3\n"
				" {\n"
				, iPrimitive
				);
			iPrimitive++;
			CV2MapBrush &v = entity.va[i];
			size_t x;
			for (x = 0; x < v.pa.size(); x++) {
				CV2MapPlane &vp = v.pa[x];
				fprintf(f,
					"  ( %s %s %s %s ) ( ( %s %s %s ) ( %s %s %s ) ) \"%s\" 0 0 0\n"
					, (LPCSTR)Sol::f2s(vp.v.x)
					, (LPCSTR)Sol::f2s(vp.v.y)
					, (LPCSTR)Sol::f2s(vp.v.z)
					, (LPCSTR)Sol::f2s(vp.fr)
					, (LPCSTR)Sol::f2s(vp.m32[0][0])
					, (LPCSTR)Sol::f2s(vp.m32[0][1])
					, (LPCSTR)Sol::f2s(vp.m32[0][2])
					, (LPCSTR)Sol::f2s(vp.m32[1][0])
					, (LPCSTR)Sol::f2s(vp.m32[1][1])
					, (LPCSTR)Sol::f2s(vp.m32[1][2])
					, (LPCSTR)vp.strTex
					);
			}
			fprintf(f,
				" }\n"
				"}\n"
				);
		}
		for (i = 0; i < entity.p3a.size(); i++) {
			CV2MapPatchDef3 &p3 = entity.p3a[i];
			fprintf(f,
				"// primitive %u\n"
				"{\n"
				" patchDef3\n"
				" {\n"
				"  \"%s\"\n"
				"  ( %d %d %d %d %d %d %d )\n"
				"  (\n"

				, iPrimitive
				, (LPCSTR)p3.strTex
				, p3.cy
				, p3.cx
				, p3.t[0]
				, p3.t[1]
				, p3.t[2]
				, p3.t[3]
				, p3.t[4]
				);
			iPrimitive++;
			size_t x, y, cx = p3.cx, cy = p3.cy;
			for (y = 0; y < cy; y++) {
				fprintf(f,
					"   ("
					);
				for (x = 0; x < cx; x++) {
					CV2MapPatchDef3Coord &p3c = p3.ca[x +cx*y];
					My3DMath::Vtx3 v = p3c.v + patchDef_forceo;
					fprintf(f,
						" ( %s %s %s %s %s )"
						, (LPCSTR)Sol::f2s(v.x)
						, (LPCSTR)Sol::f2s(v.y)
						, (LPCSTR)Sol::f2s(v.z)
						, (LPCSTR)Sol::f2s(p3c.tv.x)
						, (LPCSTR)Sol::f2s(p3c.tv.y)
						);
				}
				fprintf(f,
					" )\n"
					);
			}
			fprintf(f,
				"  )\n"
				" }\n"
				"}\n"
				);
		}
	}

	return true;
}

// ->
//  ->
//   ->
//    ->
// CHwsMan
//      ->
//       ->
//        ->
//         ->

UINT CHwsMan::getindexres() { return IDR_HTML_INDEX; }

UINT CHwsMan::gethhpres() { return IDR_DATA_D3HHP; }

// ->
//  ->
//   ->
//    ->
// CHwsManq4
//      ->
//       ->
//        ->
//         ->

UINT CHwsManq4::getindexres() { return IDR_HTML_INDEX_Q4; }

UINT CHwsManq4::gethhpres() { return IDR_DATA_Q4HHP; }
