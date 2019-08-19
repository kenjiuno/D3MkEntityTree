
#pragma once

#include "JointListView.h"

class CJointListFrame : public CFrameWnd
{
public:
	// 
	CJointListView m_vw;

	// 
	CJointListFrame();
	// 
	virtual ~CJointListFrame();

protected:
	// 
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void PostNcDestroy();
};


