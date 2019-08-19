
// +--------------------------------------------------
// |
// | RCI.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#if !defined(AFX_RCI_H__3D3FF9C0_64A3_4939_AE18_114878D44681__INCLUDED_)
#define AFX_RCI_H__3D3FF9C0_64A3_4939_AE18_114878D44681__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IRCI
{
public:
	// 
	virtual LONG AddRef() = NULL;
	// 
	virtual LONG Release() = NULL;

};

class CRCI : public IRCI
{
	// 
	LONG nLocked;

public:
	// 
	CRCI(): nLocked(1) { }
	// 
	virtual ~CRCI()
	{
		ASSERT(nLocked == 0);
	}
	// 
	virtual LONG AddRef()
	{
		LONG r = LockAdd(+1);
		return r;
	}
	// 
	virtual LONG Release()
	{
		LONG r = LockAdd(-1);
		if (r == 0) delete this;
		return r;
	}

protected:
	// 
	LONG LockAdd(LONG n)
	{
		nLocked += n;
		return nLocked;
	}

};

#endif // !defined(AFX_RCI_H__3D3FF9C0_64A3_4939_AE18_114878D44681__INCLUDED_)
