
#pragma once

#include "RCI.h"

#include <vector>
#include <algorithm>
#include "DM2RPtr.h"

class CToolContextSink
{
public:
	// 
	virtual void OnToolContextDispatch(UINT nHint, void *pv) = NULL;
};

class CToolContext : protected CRCI
{
	// 
	std::vector<CToolContextSink *> vec;

	// 
	CDocument *pDocSet;

public:
	// 
	void Add(CToolContextSink *p)
	{
		vec.push_back(p);
	}
	// 
	void Remove(CToolContextSink *p)
	{
		vec.erase(std::remove(vec.begin(), vec.end(), p), vec.end());
	}
	// 
	void Dispatch(UINT nHint, void *pv)
	{
		for (size_t i = 0; i < vec.size(); i++) vec[i]->OnToolContextDispatch(nHint, pv);
	}

	// 
	void SetDoc(CDocument *pDoc, bool fSet);

	// 
	virtual LONG AddRef()
	{
		return CRCI::AddRef();
	}
	// 
	virtual LONG Release()
	{
		return CRCI::Release();
	}

};

typedef CDM2SPtr<CToolContext> CToolContextSPtr;
