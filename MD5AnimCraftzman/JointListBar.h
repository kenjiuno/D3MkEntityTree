#pragma once

#include "JointListFrame.h"

// CJointListBar

class CJointListBar : public CSizingControlBarCF
{
public:
	// 
	CJointListFrame m_frm;

	// 
	CJointListBar();
	// 
	virtual ~CJointListBar();

protected:
	// 
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
