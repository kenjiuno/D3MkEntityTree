// JointListBar.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "MD5AnimCraftzman.h"
#include "JointListBar.h"

// CJointListBar

CJointListBar::CJointListBar()
{

}

CJointListBar::~CJointListBar()
{

}

BEGIN_MESSAGE_MAP(CJointListBar, CSizingControlBarCF)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CJointListBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSizingControlBarCF::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_frm.LoadFrame(IDR_JointListTYPE, 0 |WS_CHILD, this))
		return -1;
	m_frm.m_hMenuDefault = NULL;
	m_frm.SetDlgCtrlID(0x0200);
	m_frm.InitialUpdateFrame(NULL, true);

	return 0;
}

void CJointListBar::OnSize(UINT nType, int cx, int cy)
{
	CSizingControlBarCF::OnSize(nType, cx, cy);

	RepositionBars(0, 0xffff, 0x0200);
}
