
// +--------------------------------------------------
// |
// | Doom3RP.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

#include "SizeVuff.h"

#include <gl/gl.h>

struct CTformedMesh {
	// 
	GLshort *pIndex;
	// 
	GLsizei nIndexStride;
	// 
	GLfloat *pVertex;
	// 
	GLsizei nVertexStride;
	// 
	GLfloat *pTexCoord0;
	// 
	GLsizei nTexCoord0Stride;
	// 
	GLsizei nVerts;
	// 
	GLshort *pTexIndex;
	// 
	GLsizei nTexIndexStride;

	// 
	SizeBuff fvIndex, fvVertex, fvTexCoord0, fvTexIndex;

	// 
	CTformedMesh()
	{
		nIndexStride = 2; nVertexStride = 4 * 3; nTexCoord0Stride = 4 * 2; nTexIndexStride = 2;
	}
	// 
	bool Malloc(UINT nTri, UINT nVert)
	{
		if (false
			|| !fvIndex.Alloc(nIndexStride *3*nTri)
			|| !fvVertex.Alloc(nVertexStride *nVert)
			|| !fvTexCoord0.Alloc(nTexCoord0Stride *nVert)
			|| !fvTexIndex.Alloc(nTexIndexStride *nTri)
		) {
			return false;
		}
		nVerts = nTri;

		pIndex = (GLshort *)fvIndex.GetData();
		pVertex = (GLfloat *)fvVertex.GetData();
		pTexCoord0 = (GLfloat *)fvTexCoord0.GetData();
		pTexIndex = (GLshort *)fvTexIndex.GetData();
		return true;
	}
	// 
	CTformedMesh(const CTformedMesh &s)
	{
		*this = s;
	}
	// 
	const CTformedMesh &operator =(const CTformedMesh &s)
	{
		fvIndex = s.fvIndex;
		fvVertex = s.fvVertex;
		fvTexCoord0 = s.fvTexCoord0;
		fvTexIndex = s.fvTexIndex;
		nVerts = s.nVerts;
		nIndexStride = s.nIndexStride;
		nVertexStride = s.nVertexStride;
		nTexCoord0Stride = s.nTexCoord0Stride;
		nTexIndexStride = s.nTexIndexStride;

		pIndex = (GLshort *)fvIndex.GetData();
		pVertex = (GLfloat *)fvVertex.GetData();
		pTexCoord0 = (GLfloat *)fvTexCoord0.GetData();
		pTexIndex = (GLshort *)fvTexIndex.GetData();

		return s;
	}

};

typedef vector<CTformedMesh> TformedMeshArray;

struct CTformedMeshTab {
	// 
	CTformedMesh *p;
	// 
	CStringArray strTexs;

	// 
	CTformedMeshTab() { }
	// 
	CTformedMeshTab(const CTformedMeshTab &s)
	{
		*this = s;
	}
	// 
	const CTformedMeshTab &operator =(const CTformedMeshTab &s)
	{
		p = s.p;
		strTexs.Copy(s.strTexs);
		return s;
	}
};

typedef vector<CTformedMeshTab> TformedMeshTabArray;

typedef vector<CTformedMeshTab *> TformedMeshTabPtrArray;
