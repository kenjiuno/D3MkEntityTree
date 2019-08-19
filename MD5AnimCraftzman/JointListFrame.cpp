// JointListFrame.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "MD5AnimCraftzman.h"
#include "JointListFrame.h"

// CJointListFrame

CJointListFrame::CJointListFrame()
{

}

CJointListFrame::~CJointListFrame()
{

}

BEGIN_MESSAGE_MAP(CJointListFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CJointListFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_vw.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST))
		return -1;

	return 0;
}

void CJointListFrame::PostNcDestroy()
{

}
